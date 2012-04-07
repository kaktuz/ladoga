
//#include <linux/compiler.h>
//#include <linux/errno.h>
//#include <linux/if_arp.h>
//#include <linux/in6.h>
//#include <linux/in.h>
//#include <linux/ip.h>

  #include <linux/interrupt.h>
#include <linux/kernel.h>
#include <linux/module.h>
//#include <linux/netdevice.h>
#include <linux/proc_fs.h>
//#include <linux/skbuff.h>
//#include <linux/slab.h>
//#include <linux/tcp.h>
//#include <linux/types.h>
//#include <linux/wireless.h>
#include <linux/etherdevice.h>
//#include <asm/uaccess.h>
#include <net/net_namespace.h>
//#include <net/arp.h>

#include "ladoga.h"

#define DRV_DESCRIPTION "ladoga 802.11 data/management/control stack"
#define DRV_NAME        "ladoga"
#define DRV_VERSION     "0.1"
#define DRV_COPYRIGHT   "Copyright (C) 2012 kolobok <starodumov@gmail.com>"

MODULE_VERSION(DRV_VERSION);
MODULE_DESCRIPTION(DRV_DESCRIPTION);
MODULE_AUTHOR(DRV_COPYRIGHT);
MODULE_LICENSE("GPL");

static int ladoga_networks_allocate(struct ladoga_device *ladoga)
{
	if (ladoga->networks)
		return 0;

	ladoga->networks =
	    kzalloc(MAX_NETWORK_COUNT * sizeof(struct ladoga_network),
		    GFP_KERNEL);
	if (!ladoga->networks) {
		printk(KERN_WARNING "%s: Out of memory allocating beacons\n",
		       ladoga->dev->name);
		return -ENOMEM;
	}

	return 0;
}

//void libipw_network_reset(struct libipw_network *network)
//{
//	if (!network)
//		return;

//	if (network->ibss_dfs) {
//		kfree(network->ibss_dfs);
//		network->ibss_dfs = NULL;
//	}
//}

static inline void ladoga_networks_free(struct ladoga_device *ladoga)
{
	int i;

	if (!ladoga->networks)
		return;

	for (i = 0; i < MAX_NETWORK_COUNT; i++)
		if (ladoga->networks[i].ibss_dfs)
			kfree(ladoga->networks[i].ibss_dfs);

	kfree(ladoga->networks);
	ladoga->networks = NULL;
}

//void libipw_networks_age(struct libipw_device *ieee,
//                            unsigned long age_secs)
//{
//	struct libipw_network *network = NULL;
//	unsigned long flags;
//	unsigned long age_jiffies = msecs_to_jiffies(age_secs * MSEC_PER_SEC);

//	spin_lock_irqsave(&ieee->lock, flags);
//	list_for_each_entry(network, &ieee->network_list, list) {
//		network->last_scanned -= age_jiffies;
//	}
//	spin_unlock_irqrestore(&ieee->lock, flags);
//}
//EXPORT_SYMBOL(libipw_networks_age);

static void ladoga_networks_initialize(struct ladoga_device *ladoga)
{
	int i;

	INIT_LIST_HEAD(&ladoga->network_free_list);
	INIT_LIST_HEAD(&ladoga->network_list);
	for (i = 0; i < MAX_NETWORK_COUNT; i++)
		list_add_tail(&ladoga->networks[i].list,
			      &ladoga->network_free_list);
}

//int libipw_change_mtu(struct net_device *dev, int new_mtu)
//{
//	if ((new_mtu < 68) || (new_mtu > LIBIPW_DATA_LEN))
//		return -EINVAL;
//	dev->mtu = new_mtu;
//	return 0;
//}
//EXPORT_SYMBOL(libipw_change_mtu);

struct net_device *alloc_ladoga(int sizeof_priv)
{
	struct ladoga_device *ladoga;
	struct net_device *dev;
	int err;

	LADOGA_DEBUG_INFO("Initializing...\n");

	dev = alloc_etherdev(sizeof(struct ladoga_device) + sizeof_priv);
	if (!dev) {
		LADOGA_ERROR("Unable to allocate network device.\n");
		goto failed;
	}
	ladoga = netdev_priv(dev);
	ladoga->dev = dev;

	err = ladoga_networks_allocate(ladoga);
	if (err) {
		LADOGA_ERROR("Unable to allocate beacon storage: %d\n", err);
		goto failed_free_netdev;
	}
	ladoga_networks_initialize(ladoga);

	/* Default fragmentation threshold is maximum payload size */
	ladoga->fts = DEFAULT_FTS;
	ladoga->rts = DEFAULT_FTS;
	ladoga->scan_age = DEFAULT_MAX_SCAN_AGE;
	ladoga->open_wep = 1;

	/* Default to enabling full open WEP with host based encrypt/decrypt */
	ladoga->host_encrypt = 1;
	ladoga->host_decrypt = 1;
	ladoga->host_mc_decrypt = 1;

	/* Host fragementation in Open mode. Default is enabled.
	 * Note: host fragmentation is always enabled if host encryption
	 * is enabled. For cards can do hardware encryption, they must do
	 * hardware fragmentation as well. So we don't need a variable
	 * like host_enc_frag. */
	ladoga->host_open_frag = 1;
	ladoga->ieee802_1x = 1;	/* Default to supporting 802.1x */

	spin_lock_init(&ladoga->lock);

	lib80211_crypt_info_init(&ladoga->crypt_info, dev->name, &ladoga->lock);

	ladoga->wpa_enabled = 0;
	ladoga->drop_unencrypted = 0;
	ladoga->privacy_invoked = 0;

	return dev;

failed_free_netdev:
	free_netdev(dev);
failed:
	return NULL;
}

void free_ladoga(struct net_device *dev)
{
	struct ladoga_device *ladoga = netdev_priv(dev);

   lib80211_crypt_info_free(&ladoga->crypt_info);

   ladoga_networks_free(ladoga);
	free_netdev(dev);
}

#ifdef CONFIG_LADOGA_DEBUG
static int debug = 0;
u32 ladoga_debug_level = 0;
EXPORT_SYMBOL_GPL(ladoga_debug_level);
static struct proc_dir_entry *ladoga_proc = NULL;

static int show_debug_level(char *page, char **start, off_t offset,
			    int count, int *eof, void *data)
{
	return snprintf(page, count, "0x%08X\n", ladoga_debug_level);
}

static int store_debug_level(struct file *file, const char __user * buffer,
			     unsigned long count, void *data)
{
	char buf[] = "0x00000000\n";
	unsigned long len = min((unsigned long)sizeof(buf) - 1, count);
	unsigned long val;

	if (copy_from_user(buf, buffer, len))
		return count;
	buf[len] = 0;
	if (sscanf(buf, "%li", &val) != 1)
		printk(KERN_INFO DRV_NAME
		       ": %s is not in hex or decimal form.\n", buf);
	else
		ladoga_debug_level = val;

	return strnlen(buf, len);
}
#endif				/* CONFIG_LADOGA_DEBUG */

static int __init ladoga_init(void)
{
#ifdef CONFIG_LADOGA_DEBUG
	struct proc_dir_entry *e;

	ladoga_debug_level = debug;
	ladoga_proc = proc_mkdir(DRV_NAME, init_net.proc_net);
	if (ladoga_proc == NULL) {
		LADOGA_ERROR("Unable to create " DRV_NAME	" proc directory\n");
		return -EIO;
	}
	e = create_proc_entry("debug_level", S_IFREG | S_IRUGO | S_IWUSR,
			      ladoga_proc);
	if (!e) {
		remove_proc_entry(DRV_NAME, init_net.proc_net);
		ladoga_proc = NULL;
		return -EIO;
	}
	e->read_proc = show_debug_level;
	e->write_proc = store_debug_level;
	e->data = NULL;
#endif

//	printk(KERN_INFO DRV_NAME ": " DRV_DESCRIPTION ", " DRV_VERSION "\n");
//	printk(KERN_INFO DRV_NAME ": " DRV_COPYRIGHT "\n");

	return 0;
}

static void __exit ladoga_exit(void)
{
#ifdef CONFIG_LADOGA_DEBUG
	if (ladoga_proc) {
		remove_proc_entry("debug_level", ladoga_proc);
		remove_proc_entry(DRV_NAME, init_net.proc_net);
		ladoga_proc = NULL;
	}
#endif
}

#ifdef CONFIG_LADOGA_DEBUG
#include <linux/moduleparam.h>
module_param(debug, int, 0444);
MODULE_PARM_DESC(debug, "debug output mask");
#endif

module_exit(ladoga_exit);
module_init(ladoga_init);

EXPORT_SYMBOL(alloc_ladoga);
EXPORT_SYMBOL(free_ladoga);
