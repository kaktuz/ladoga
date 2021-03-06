//
//#include <linux/sched.h>
//#include "ladoga_drv.h"
//
///*
// * Rx theory of operation
// *
// * The host allocates 32 DMA target addresses and passes the host address
// * to the firmware at register IPW_RFDS_TABLE_LOWER + N * RFD_SIZE where N is
// * 0 to 31
// *
// * Rx Queue Indexes
// * The host/firmware share two index registers for managing the Rx buffers.
// *
// * The READ index maps to the first position that the firmware may be writing
// * to -- the driver can read up to (but not including) this position and get
// * good data.
// * The READ index is managed by the firmware once the card is enabled.
// *
// * The WRITE index maps to the last position the driver has read from -- the
// * position preceding WRITE is the last slot the firmware can place a packet.
// *
// * The queue is empty (no good data) if WRITE = READ - 1, and is full if
// * WRITE = READ.
// *
// * During initialization the host sets up the READ queue position to the first
// * INDEX position, and WRITE to the last (READ - 1 wrapped)
// *
// * When the firmware places a packet in a buffer it will advance the READ index
// * and fire the RX interrupt.  The driver can then query the READ index and
// * process as many packets as possible, moving the WRITE index forward as it
// * resets the Rx queue buffers with new memory.
// *
// * The management in the driver is as follows:
// * + A list of pre-allocated SKBs is stored in ipw->rxq->rx_free.  When
// *   ipw->rxq->free_count drops to or below RX_LOW_WATERMARK, work is scheduled
// *   to replensish the ipw->rxq->rx_free.
// * + In ipw_rx_queue_replenish (scheduled) if 'processed' != 'read' then the
// *   ipw->rxq is replenished and the READ INDEX is updated (updating the
// *   'processed' and 'read' driver indexes as well)
// * + A received packet is processed and handed to the kernel network stack,
// *   detached from the ipw->rxq.  The driver 'processed' index is updated.
// * + The Host/Firmware ipw->rxq is replenished at tasklet time from the rx_free
// *   list. If there are no allocated buffers in ipw->rxq->rx_free, the READ
// *   INDEX is not incremented and ipw->status(RX_STALLED) is set.  If there
// *   were enough free buffers and RX_STALLED is set it is cleared.
// *
// *
// * Driver sequence:
// *
// * ipw_rx_queue_alloc()       Allocates rx_free
// * ipw_rx_queue_replenish()   Replenishes rx_free list from rx_used, and calls
// *                            ipw_rx_queue_restock
// * ipw_rx_queue_restock()     Moves available buffers from rx_free into Rx
// *                            queue, updates firmware pointers, and updates
// *                            the WRITE index.  If insufficient rx_free buffers
// *                            are available, schedules ipw_rx_queue_replenish
// *
// * -- enable interrupts --
// * ISR - ipw_rx()             Detach ipw_rx_mem_buffers from pool up to the
// *                            READ INDEX, detaching the SKB from the pool.
// *                            Moves the packet buffer from queue to rx_used.
// *                            Calls ipw_rx_queue_restock to refill any empty
// *                            slots.
// * ...
// *
// */
//
///*
// * If there are slots in the RX queue that  need to be restocked,
// * and we have free pre-allocated buffers, fill the ranks as much
// * as we can pulling from rx_free.
// *
// * This moves the 'write' index forward to catch up with 'processed', and
// * also updates the memory address in the firmware to reference the new
// * target buffer.
// */
//static void ipw_rx_queue_restock(struct ipw_priv *priv)
//{
//	struct ipw_rx_queue *rxq = priv->rxq;
//	struct list_head *element;
//	struct ipw_rx_mem_buffer *rxb;
//	unsigned long flags;
//	int write;
//
//	spin_lock_irqsave(&rxq->lock, flags);
//	write = rxq->write;
//	while ((ipw_rx_queue_space(rxq) > 0) && (rxq->free_count)) {
//		element = rxq->rx_free.next;
//		rxb = list_entry(element, struct ipw_rx_mem_buffer, list);
//		list_del(element);
//
//		ipw_write32(priv, IPW_RFDS_TABLE_LOWER + rxq->write * RFD_SIZE,
//			    rxb->dma_addr);
//		rxq->queue[rxq->write] = rxb;
//		rxq->write = (rxq->write + 1) % RX_QUEUE_SIZE;
//		rxq->free_count--;
//	}
//	spin_unlock_irqrestore(&rxq->lock, flags);
//
//	/* If the pre-allocated buffer pool is dropping low, schedule to
//	 * refill it */
//	if (rxq->free_count <= RX_LOW_WATERMARK)
//		queue_work(priv->workqueue, &priv->rx_replenish);
//
//	/* If we've added more space for the firmware to place data, tell it */
//	if (write != rxq->write)
//		ipw_write32(priv, IPW_RX_WRITE_INDEX, rxq->write);
//}
//
///*
// * Move all used packet from rx_used to rx_free, allocating a new SKB for each.
// * Also restock the Rx queue via ipw_rx_queue_restock.
// *
// * This is called as a scheduled work item (except for during intialization)
// */
//static void ipw_rx_queue_replenish(void *data)
//{
//	struct ipw_priv *priv = data;
//	struct ipw_rx_queue *rxq = priv->rxq;
//	struct list_head *element;
//	struct ipw_rx_mem_buffer *rxb;
//	unsigned long flags;
//
//	spin_lock_irqsave(&rxq->lock, flags);
//	while (!list_empty(&rxq->rx_used)) {
//		element = rxq->rx_used.next;
//		rxb = list_entry(element, struct ipw_rx_mem_buffer, list);
//		rxb->skb = alloc_skb(IPW_RX_BUF_SIZE, GFP_ATOMIC);
//		if (!rxb->skb) {
//			printk(KERN_CRIT "%s: Can not allocate SKB buffers.\n",
//			       priv->net_dev->name);
//			/* We don't reschedule replenish work here -- we will
//			 * call the restock method and if it still needs
//			 * more buffers it will schedule replenish */
//			break;
//		}
//		list_del(element);
//
//		rxb->dma_addr =
//		    pci_map_single(priv->pci_dev, rxb->skb->data,
//				   IPW_RX_BUF_SIZE, PCI_DMA_FROMDEVICE);
//
//		list_add_tail(&rxb->list, &rxq->rx_free);
//		rxq->free_count++;
//	}
//	spin_unlock_irqrestore(&rxq->lock, flags);
//
//	ipw_rx_queue_restock(priv);
//}
//
//static void ipw_bg_rx_queue_replenish(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, rx_replenish);
//	mutex_lock(&priv->mutex);
//	ipw_rx_queue_replenish(priv);
//	mutex_unlock(&priv->mutex);
//}
//
///* Assumes that the skb field of the buffers in 'pool' is kept accurate.
// * If an SKB has been detached, the POOL needs to have its SKB set to NULL
// * This free routine walks the list of POOL entries and if SKB is set to
// * non NULL it is unmapped and freed
// */
//static void ipw_rx_queue_free(struct ipw_priv *priv, struct ipw_rx_queue *rxq)
//{
//	int i;
//
//	if (!rxq)
//		return;
//
//	for (i = 0; i < RX_QUEUE_SIZE + RX_FREE_BUFFERS; i++) {
//		if (rxq->pool[i].skb != NULL) {
//			pci_unmap_single(priv->pci_dev, rxq->pool[i].dma_addr,
//					 IPW_RX_BUF_SIZE, PCI_DMA_FROMDEVICE);
//			dev_kfree_skb(rxq->pool[i].skb);
//		}
//	}
//
//	kfree(rxq);
//}
//
//static struct ipw_rx_queue *ipw_rx_queue_alloc(struct ipw_priv *priv)
//{
//	struct ipw_rx_queue *rxq;
//	int i;
//
//	rxq = kzalloc(sizeof(*rxq), GFP_KERNEL);
//	if (unlikely(!rxq)) {
//		IPW_ERROR("memory allocation failed\n");
//		return NULL;
//	}
//	spin_lock_init(&rxq->lock);
//	INIT_LIST_HEAD(&rxq->rx_free);
//	INIT_LIST_HEAD(&rxq->rx_used);
//
//	/* Fill the rx_used queue with _all_ of the Rx buffers */
//	for (i = 0; i < RX_FREE_BUFFERS + RX_QUEUE_SIZE; i++)
//		list_add_tail(&rxq->pool[i].list, &rxq->rx_used);
//
//	/* Set us so that we have processed and used all buffers, but have
//	 * not restocked the Rx queue with fresh buffers */
//	rxq->read = rxq->write = 0;
//	rxq->free_count = 0;
//
//	return rxq;
//}
//
//static int ipw_is_rate_in_mask(struct ipw_priv *priv, int ieee_mode, u8 rate)
//{
//	rate &= ~LIBIPW_BASIC_RATE_MASK;
//	if (ieee_mode == IEEE_A) {
//		switch (rate) {
//		case LIBIPW_OFDM_RATE_6MB:
//			return priv->rates_mask & LIBIPW_OFDM_RATE_6MB_MASK ?
//			    1 : 0;
//		case LIBIPW_OFDM_RATE_9MB:
//			return priv->rates_mask & LIBIPW_OFDM_RATE_9MB_MASK ?
//			    1 : 0;
//		case LIBIPW_OFDM_RATE_12MB:
//			return priv->
//			    rates_mask & LIBIPW_OFDM_RATE_12MB_MASK ? 1 : 0;
//		case LIBIPW_OFDM_RATE_18MB:
//			return priv->
//			    rates_mask & LIBIPW_OFDM_RATE_18MB_MASK ? 1 : 0;
//		case LIBIPW_OFDM_RATE_24MB:
//			return priv->
//			    rates_mask & LIBIPW_OFDM_RATE_24MB_MASK ? 1 : 0;
//		case LIBIPW_OFDM_RATE_36MB:
//			return priv->
//			    rates_mask & LIBIPW_OFDM_RATE_36MB_MASK ? 1 : 0;
//		case LIBIPW_OFDM_RATE_48MB:
//			return priv->
//			    rates_mask & LIBIPW_OFDM_RATE_48MB_MASK ? 1 : 0;
//		case LIBIPW_OFDM_RATE_54MB:
//			return priv->
//			    rates_mask & LIBIPW_OFDM_RATE_54MB_MASK ? 1 : 0;
//		default:
//			return 0;
//		}
//	}
//
//	/* B and G mixed */
//	switch (rate) {
//	case LIBIPW_CCK_RATE_1MB:
//		return priv->rates_mask & LIBIPW_CCK_RATE_1MB_MASK ? 1 : 0;
//	case LIBIPW_CCK_RATE_2MB:
//		return priv->rates_mask & LIBIPW_CCK_RATE_2MB_MASK ? 1 : 0;
//	case LIBIPW_CCK_RATE_5MB:
//		return priv->rates_mask & LIBIPW_CCK_RATE_5MB_MASK ? 1 : 0;
//	case LIBIPW_CCK_RATE_11MB:
//		return priv->rates_mask & LIBIPW_CCK_RATE_11MB_MASK ? 1 : 0;
//	}
//
//	/* If we are limited to B modulations, bail at this point */
//	if (ieee_mode == IEEE_B)
//		return 0;
//
//	/* G */
//	switch (rate) {
//	case LIBIPW_OFDM_RATE_6MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_6MB_MASK ? 1 : 0;
//	case LIBIPW_OFDM_RATE_9MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_9MB_MASK ? 1 : 0;
//	case LIBIPW_OFDM_RATE_12MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_12MB_MASK ? 1 : 0;
//	case LIBIPW_OFDM_RATE_18MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_18MB_MASK ? 1 : 0;
//	case LIBIPW_OFDM_RATE_24MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_24MB_MASK ? 1 : 0;
//	case LIBIPW_OFDM_RATE_36MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_36MB_MASK ? 1 : 0;
//	case LIBIPW_OFDM_RATE_48MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_48MB_MASK ? 1 : 0;
//	case LIBIPW_OFDM_RATE_54MB:
//		return priv->rates_mask & LIBIPW_OFDM_RATE_54MB_MASK ? 1 : 0;
//	}
//
//	return 0;
//}
//
//static int ipw_compatible_rates(struct ipw_priv *priv,
//				const struct libipw_network *network,
//				struct ipw_supported_rates *rates)
//{
//	int num_rates, i;
//
//	memset(rates, 0, sizeof(*rates));
//	num_rates = min(network->rates_len, (u8) IPW_MAX_RATES);
//	rates->num_rates = 0;
//	for (i = 0; i < num_rates; i++) {
//		if (!ipw_is_rate_in_mask(priv, network->mode,
//					 network->rates[i])) {
//
//			if (network->rates[i] & LIBIPW_BASIC_RATE_MASK) {
//				IPW_DEBUG_SCAN("Adding masked mandatory "
//					       "rate %02X\n",
//					       network->rates[i]);
//				rates->supported_rates[rates->num_rates++] =
//				    network->rates[i];
//				continue;
//			}
//
//			IPW_DEBUG_SCAN("Rate %02X masked : 0x%08X\n",
//				       network->rates[i], priv->rates_mask);
//			continue;
//		}
//
//		rates->supported_rates[rates->num_rates++] = network->rates[i];
//	}
//
//	num_rates = min(network->rates_ex_len,
//			(u8) (IPW_MAX_RATES - num_rates));
//	for (i = 0; i < num_rates; i++) {
//		if (!ipw_is_rate_in_mask(priv, network->mode,
//					 network->rates_ex[i])) {
//			if (network->rates_ex[i] & LIBIPW_BASIC_RATE_MASK) {
//				IPW_DEBUG_SCAN("Adding masked mandatory "
//					       "rate %02X\n",
//					       network->rates_ex[i]);
//				rates->supported_rates[rates->num_rates++] =
//				    network->rates[i];
//				continue;
//			}
//
//			IPW_DEBUG_SCAN("Rate %02X masked : 0x%08X\n",
//				       network->rates_ex[i], priv->rates_mask);
//			continue;
//		}
//
//		rates->supported_rates[rates->num_rates++] =
//		    network->rates_ex[i];
//	}
//
//	return 1;
//}
//
//static void ipw_copy_rates(struct ipw_supported_rates *dest,
//				  const struct ipw_supported_rates *src)
//{
//	u8 i;
//	for (i = 0; i < src->num_rates; i++)
//		dest->supported_rates[i] = src->supported_rates[i];
//	dest->num_rates = src->num_rates;
//}
//
///* TODO: Look at sniffed packets in the air to determine if the basic rate
// * mask should ever be used -- right now all callers to add the scan rates are
// * set with the modulation = CCK, so BASIC_RATE_MASK is never set... */
//static void ipw_add_cck_scan_rates(struct ipw_supported_rates *rates,
//				   u8 modulation, u32 rate_mask)
//{
//	u8 basic_mask = (LIBIPW_OFDM_MODULATION == modulation) ?
//	    LIBIPW_BASIC_RATE_MASK : 0;
//
//	if (rate_mask & LIBIPW_CCK_RATE_1MB_MASK)
//		rates->supported_rates[rates->num_rates++] =
//		    LIBIPW_BASIC_RATE_MASK | LIBIPW_CCK_RATE_1MB;
//
//	if (rate_mask & LIBIPW_CCK_RATE_2MB_MASK)
//		rates->supported_rates[rates->num_rates++] =
//		    LIBIPW_BASIC_RATE_MASK | LIBIPW_CCK_RATE_2MB;
//
//	if (rate_mask & LIBIPW_CCK_RATE_5MB_MASK)
//		rates->supported_rates[rates->num_rates++] = basic_mask |
//		    LIBIPW_CCK_RATE_5MB;
//
//	if (rate_mask & LIBIPW_CCK_RATE_11MB_MASK)
//		rates->supported_rates[rates->num_rates++] = basic_mask |
//		    LIBIPW_CCK_RATE_11MB;
//}
//
//static void ipw_add_ofdm_scan_rates(struct ipw_supported_rates *rates,
//				    u8 modulation, u32 rate_mask)
//{
//	u8 basic_mask = (LIBIPW_OFDM_MODULATION == modulation) ?
//	    LIBIPW_BASIC_RATE_MASK : 0;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_6MB_MASK)
//		rates->supported_rates[rates->num_rates++] = basic_mask |
//		    LIBIPW_OFDM_RATE_6MB;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_9MB_MASK)
//		rates->supported_rates[rates->num_rates++] =
//		    LIBIPW_OFDM_RATE_9MB;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_12MB_MASK)
//		rates->supported_rates[rates->num_rates++] = basic_mask |
//		    LIBIPW_OFDM_RATE_12MB;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_18MB_MASK)
//		rates->supported_rates[rates->num_rates++] =
//		    LIBIPW_OFDM_RATE_18MB;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_24MB_MASK)
//		rates->supported_rates[rates->num_rates++] = basic_mask |
//		    LIBIPW_OFDM_RATE_24MB;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_36MB_MASK)
//		rates->supported_rates[rates->num_rates++] =
//		    LIBIPW_OFDM_RATE_36MB;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_48MB_MASK)
//		rates->supported_rates[rates->num_rates++] =
//		    LIBIPW_OFDM_RATE_48MB;
//
//	if (rate_mask & LIBIPW_OFDM_RATE_54MB_MASK)
//		rates->supported_rates[rates->num_rates++] =
//		    LIBIPW_OFDM_RATE_54MB;
//}
//
//struct ipw_network_match {
//	struct libipw_network *network;
//	struct ipw_supported_rates rates;
//};
//
//static int ipw_find_adhoc_network(struct ipw_priv *priv,
//				  struct ipw_network_match *match,
//				  struct libipw_network *network,
//				  int roaming)
//{
//	struct ipw_supported_rates rates;
//	DECLARE_SSID_BUF(ssid);
//
//	/* Verify that this network's capability is compatible with the
//	 * current mode (AdHoc or Infrastructure) */
//	if ((priv->ieee->iw_mode == IW_MODE_ADHOC &&
//	     !(network->capability & WLAN_CAPABILITY_IBSS))) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded due to "
//				"capability mismatch.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	if (unlikely(roaming)) {
//		/* If we are roaming, then ensure check if this is a valid
//		 * network to try and roam to */
//		if ((network->ssid_len != match->network->ssid_len) ||
//		    memcmp(network->ssid, match->network->ssid,
//			   network->ssid_len)) {
//			IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//					"because of non-network ESSID.\n",
//					print_ssid(ssid, network->ssid,
//						   network->ssid_len),
//					network->bssid);
//			return 0;
//		}
//	} else {
//		/* If an ESSID has been configured then compare the broadcast
//		 * ESSID to ours */
//		if ((priv->config & CFG_STATIC_ESSID) &&
//		    ((network->ssid_len != priv->essid_len) ||
//		     memcmp(network->ssid, priv->essid,
//			    min(network->ssid_len, priv->essid_len)))) {
//			char escaped[IW_ESSID_MAX_SIZE * 2 + 1];
//
//			strncpy(escaped,
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				sizeof(escaped));
//			IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//					"because of ESSID mismatch: '%s'.\n",
//					escaped, network->bssid,
//					print_ssid(ssid, priv->essid,
//						   priv->essid_len));
//			return 0;
//		}
//	}
//
//	/* If the old network rate is better than this one, don't bother
//	 * testing everything else. */
//
//	if (network->time_stamp[0] < match->network->time_stamp[0]) {
//		IPW_DEBUG_MERGE("Network '%s excluded because newer than "
//				"current network.\n",
//				print_ssid(ssid, match->network->ssid,
//					   match->network->ssid_len));
//		return 0;
//	} else if (network->time_stamp[1] < match->network->time_stamp[1]) {
//		IPW_DEBUG_MERGE("Network '%s excluded because newer than "
//				"current network.\n",
//				print_ssid(ssid, match->network->ssid,
//					   match->network->ssid_len));
//		return 0;
//	}
//
//	/* Now go through and see if the requested network is valid... */
//	if (priv->ieee->scan_age != 0 &&
//	    time_after(jiffies, network->last_scanned + priv->ieee->scan_age)) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//				"because of age: %ums.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid,
//				jiffies_to_msecs(jiffies -
//						 network->last_scanned));
//		return 0;
//	}
//
//	if ((priv->config & CFG_STATIC_CHANNEL) &&
//	    (network->channel != priv->channel)) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//				"because of channel mismatch: %d != %d.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid,
//				network->channel, priv->channel);
//		return 0;
//	}
//
//	/* Verify privacy compatability */
//	if (((priv->capability & CAP_PRIVACY_ON) ? 1 : 0) !=
//	    ((network->capability & WLAN_CAPABILITY_PRIVACY) ? 1 : 0)) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//				"because of privacy mismatch: %s != %s.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid,
//				priv->
//				capability & CAP_PRIVACY_ON ? "on" : "off",
//				network->
//				capability & WLAN_CAPABILITY_PRIVACY ? "on" :
//				"off");
//		return 0;
//	}
//
//	if (!memcmp(network->bssid, priv->bssid, ETH_ALEN)) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//				"because of the same BSSID match: %pM"
//				".\n", print_ssid(ssid, network->ssid,
//						  network->ssid_len),
//				network->bssid,
//				priv->bssid);
//		return 0;
//	}
//
//	/* Filter out any incompatible freq / mode combinations */
//	if (!libipw_is_valid_mode(priv->ieee, network->mode)) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//				"because of invalid frequency/mode "
//				"combination.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	/* Ensure that the rates supported by the driver are compatible with
//	 * this AP, including verification of basic rates (mandatory) */
//	if (!ipw_compatible_rates(priv, network, &rates)) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//				"because configured rate mask excludes "
//				"AP mandatory rate.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	if (rates.num_rates == 0) {
//		IPW_DEBUG_MERGE("Network '%s (%pM)' excluded "
//				"because of no compatible rates.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	/* TODO: Perform any further minimal comparititive tests.  We do not
//	 * want to put too much policy logic here; intelligent scan selection
//	 * should occur within a generic IEEE 802.11 user space tool.  */
//
//	/* Set up 'new' AP to this network */
//	ipw_copy_rates(&match->rates, &rates);
//	match->network = network;
//	IPW_DEBUG_MERGE("Network '%s (%pM)' is a viable match.\n",
//			print_ssid(ssid, network->ssid, network->ssid_len),
//			network->bssid);
//
//	return 1;
//}
//
//static void ipw_merge_adhoc_network(struct work_struct *work)
//{
//	DECLARE_SSID_BUF(ssid);
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, merge_networks);
//	struct libipw_network *network = NULL;
//	struct ipw_network_match match = {
//		.network = priv->assoc_network
//	};
//
//	if ((priv->status & STATUS_ASSOCIATED) &&
//	    (priv->ieee->iw_mode == IW_MODE_ADHOC)) {
//		/* First pass through ROAM process -- look for a better
//		 * network */
//		unsigned long flags;
//
//		spin_lock_irqsave(&priv->ieee->lock, flags);
//		list_for_each_entry(network, &priv->ieee->network_list, list) {
//			if (network != priv->assoc_network)
//				ipw_find_adhoc_network(priv, &match, network,
//						       1);
//		}
//		spin_unlock_irqrestore(&priv->ieee->lock, flags);
//
//		if (match.network == priv->assoc_network) {
//			IPW_DEBUG_MERGE("No better ADHOC in this network to "
//					"merge to.\n");
//			return;
//		}
//
//		mutex_lock(&priv->mutex);
//		if ((priv->ieee->iw_mode == IW_MODE_ADHOC)) {
//			IPW_DEBUG_MERGE("remove network %s\n",
//					print_ssid(ssid, priv->essid,
//						   priv->essid_len));
//			ipw_remove_current_network(priv);
//		}
//
//		ipw_disassociate(priv);
//		priv->assoc_network = match.network;
//		mutex_unlock(&priv->mutex);
//		return;
//	}
//}
//
//static int ipw_best_network(struct ipw_priv *priv,
//			    struct ipw_network_match *match,
//			    struct libipw_network *network, int roaming)
//{
//	struct ipw_supported_rates rates;
//	DECLARE_SSID_BUF(ssid);
//
//	/* Verify that this network's capability is compatible with the
//	 * current mode (AdHoc or Infrastructure) */
//	if ((priv->ieee->iw_mode == IW_MODE_INFRA &&
//	     !(network->capability & WLAN_CAPABILITY_ESS)) ||
//	    (priv->ieee->iw_mode == IW_MODE_ADHOC &&
//	     !(network->capability & WLAN_CAPABILITY_IBSS))) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded due to "
//				"capability mismatch.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	if (unlikely(roaming)) {
//		/* If we are roaming, then ensure check if this is a valid
//		 * network to try and roam to */
//		if ((network->ssid_len != match->network->ssid_len) ||
//		    memcmp(network->ssid, match->network->ssid,
//			   network->ssid_len)) {
//			IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//					"because of non-network ESSID.\n",
//					print_ssid(ssid, network->ssid,
//						   network->ssid_len),
//					network->bssid);
//			return 0;
//		}
//	} else {
//		/* If an ESSID has been configured then compare the broadcast
//		 * ESSID to ours */
//		if ((priv->config & CFG_STATIC_ESSID) &&
//		    ((network->ssid_len != priv->essid_len) ||
//		     memcmp(network->ssid, priv->essid,
//			    min(network->ssid_len, priv->essid_len)))) {
//			char escaped[IW_ESSID_MAX_SIZE * 2 + 1];
//			strncpy(escaped,
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				sizeof(escaped));
//			IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//					"because of ESSID mismatch: '%s'.\n",
//					escaped, network->bssid,
//					print_ssid(ssid, priv->essid,
//						   priv->essid_len));
//			return 0;
//		}
//	}
//
//	/* If the old network rate is better than this one, don't bother
//	 * testing everything else. */
//	if (match->network && match->network->stats.rssi > network->stats.rssi) {
//		char escaped[IW_ESSID_MAX_SIZE * 2 + 1];
//		strncpy(escaped,
//			print_ssid(ssid, network->ssid, network->ssid_len),
//			sizeof(escaped));
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded because "
//				"'%s (%pM)' has a stronger signal.\n",
//				escaped, network->bssid,
//				print_ssid(ssid, match->network->ssid,
//					   match->network->ssid_len),
//				match->network->bssid);
//		return 0;
//	}
//
//	/* If this network has already had an association attempt within the
//	 * last 3 seconds, do not try and associate again... */
//	if (network->last_associate &&
//	    time_after(network->last_associate + (HZ * 3UL), jiffies)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of storming (%ums since last "
//				"assoc attempt).\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid,
//				jiffies_to_msecs(jiffies -
//						 network->last_associate));
//		return 0;
//	}
//
//	/* Now go through and see if the requested network is valid... */
//	if (priv->ieee->scan_age != 0 &&
//	    time_after(jiffies, network->last_scanned + priv->ieee->scan_age)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of age: %ums.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid,
//				jiffies_to_msecs(jiffies -
//						 network->last_scanned));
//		return 0;
//	}
//
//	if ((priv->config & CFG_STATIC_CHANNEL) &&
//	    (network->channel != priv->channel)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of channel mismatch: %d != %d.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid,
//				network->channel, priv->channel);
//		return 0;
//	}
//
//	/* Verify privacy compatability */
//	if (((priv->capability & CAP_PRIVACY_ON) ? 1 : 0) !=
//	    ((network->capability & WLAN_CAPABILITY_PRIVACY) ? 1 : 0)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of privacy mismatch: %s != %s.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid,
//				priv->capability & CAP_PRIVACY_ON ? "on" :
//				"off",
//				network->capability &
//				WLAN_CAPABILITY_PRIVACY ? "on" : "off");
//		return 0;
//	}
//
//	if ((priv->config & CFG_STATIC_BSSID) &&
//	    memcmp(network->bssid, priv->bssid, ETH_ALEN)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of BSSID mismatch: %pM.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid, priv->bssid);
//		return 0;
//	}
//
//	/* Filter out any incompatible freq / mode combinations */
//	if (!libipw_is_valid_mode(priv->ieee, network->mode)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of invalid frequency/mode "
//				"combination.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	/* Filter out invalid channel in current GEO */
//	if (!libipw_is_valid_channel(priv->ieee, network->channel)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of invalid channel in current GEO\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	/* Ensure that the rates supported by the driver are compatible with
//	 * this AP, including verification of basic rates (mandatory) */
//	if (!ipw_compatible_rates(priv, network, &rates)) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because configured rate mask excludes "
//				"AP mandatory rate.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	if (rates.num_rates == 0) {
//		IPW_DEBUG_ASSOC("Network '%s (%pM)' excluded "
//				"because of no compatible rates.\n",
//				print_ssid(ssid, network->ssid,
//					   network->ssid_len),
//				network->bssid);
//		return 0;
//	}
//
//	/* TODO: Perform any further minimal comparititive tests.  We do not
//	 * want to put too much policy logic here; intelligent scan selection
//	 * should occur within a generic IEEE 802.11 user space tool.  */
//
//	/* Set up 'new' AP to this network */
//	ipw_copy_rates(&match->rates, &rates);
//	match->network = network;
//
//	IPW_DEBUG_ASSOC("Network '%s (%pM)' is a viable match.\n",
//			print_ssid(ssid, network->ssid, network->ssid_len),
//			network->bssid);
//
//	return 1;
//}
//
//static void ipw_adhoc_create(struct ipw_priv *priv,
//			     struct libipw_network *network)
//{
//	const struct libipw_geo *geo = libipw_get_geo(priv->ieee);
//	int i;
//
//	/*
//	 * For the purposes of scanning, we can set our wireless mode
//	 * to trigger scans across combinations of bands, but when it
//	 * comes to creating a new ad-hoc network, we have tell the FW
//	 * exactly which band to use.
//	 *
//	 * We also have the possibility of an invalid channel for the
//	 * chossen band.  Attempting to create a new ad-hoc network
//	 * with an invalid channel for wireless mode will trigger a
//	 * FW fatal error.
//	 *
//	 */
//	switch (libipw_is_valid_channel(priv->ieee, priv->channel)) {
//	case LIBIPW_52GHZ_BAND:
//		network->mode = IEEE_A;
//		i = libipw_channel_to_index(priv->ieee, priv->channel);
//		BUG_ON(i == -1);
//		if (geo->a[i].flags & LIBIPW_CH_PASSIVE_ONLY) {
//			IPW_WARNING("Overriding invalid channel\n");
//			priv->channel = geo->a[0].channel;
//		}
//		break;
//
//	case LIBIPW_24GHZ_BAND:
//		if (priv->ieee->mode & IEEE_G)
//			network->mode = IEEE_G;
//		else
//			network->mode = IEEE_B;
//		i = libipw_channel_to_index(priv->ieee, priv->channel);
//		BUG_ON(i == -1);
//		if (geo->bg[i].flags & LIBIPW_CH_PASSIVE_ONLY) {
//			IPW_WARNING("Overriding invalid channel\n");
//			priv->channel = geo->bg[0].channel;
//		}
//		break;
//
//	default:
//		IPW_WARNING("Overriding invalid channel\n");
//		if (priv->ieee->mode & IEEE_A) {
//			network->mode = IEEE_A;
//			priv->channel = geo->a[0].channel;
//		} else if (priv->ieee->mode & IEEE_G) {
//			network->mode = IEEE_G;
//			priv->channel = geo->bg[0].channel;
//		} else {
//			network->mode = IEEE_B;
//			priv->channel = geo->bg[0].channel;
//		}
//		break;
//	}
//
//	network->channel = priv->channel;
//	priv->config |= CFG_ADHOC_PERSIST;
//	ipw_create_bssid(priv, network->bssid);
//	network->ssid_len = priv->essid_len;
//	memcpy(network->ssid, priv->essid, priv->essid_len);
//	memset(&network->stats, 0, sizeof(network->stats));
//	network->capability = WLAN_CAPABILITY_IBSS;
//	if (!(priv->config & CFG_PREAMBLE_LONG))
//		network->capability |= WLAN_CAPABILITY_SHORT_PREAMBLE;
//	if (priv->capability & CAP_PRIVACY_ON)
//		network->capability |= WLAN_CAPABILITY_PRIVACY;
//	network->rates_len = min(priv->rates.num_rates, MAX_RATES_LENGTH);
//	memcpy(network->rates, priv->rates.supported_rates, network->rates_len);
//	network->rates_ex_len = priv->rates.num_rates - network->rates_len;
//	memcpy(network->rates_ex,
//	       &priv->rates.supported_rates[network->rates_len],
//	       network->rates_ex_len);
//	network->last_scanned = 0;
//	network->flags = 0;
//	network->last_associate = 0;
//	network->time_stamp[0] = 0;
//	network->time_stamp[1] = 0;
//	network->beacon_interval = 100;	/* Default */
//	network->listen_interval = 10;	/* Default */
//	network->atim_window = 0;	/* Default */
//	network->wpa_ie_len = 0;
//	network->rsn_ie_len = 0;
//}
//
//static void ipw_send_tgi_tx_key(struct ipw_priv *priv, int type, int index)
//{
//	struct ipw_tgi_tx_key key;
//
//	if (!(priv->ieee->sec.flags & (1 << index)))
//		return;
//
//	key.key_id = index;
//	memcpy(key.key, priv->ieee->sec.keys[index], SCM_TEMPORAL_KEY_LENGTH);
//	key.security_type = type;
//	key.station_index = 0;	/* always 0 for BSS */
//	key.flags = 0;
//	/* 0 for new key; previous value of counter (after fatal error) */
//	key.tx_counter[0] = cpu_to_le32(0);
//	key.tx_counter[1] = cpu_to_le32(0);
//
//	ipw_send_cmd_pdu(priv, IPW_CMD_TGI_TX_KEY, sizeof(key), &key);
//}
//
//static void ipw_send_wep_keys(struct ipw_priv *priv, int type)
//{
//	struct ipw_wep_key key;
//	int i;
//
//	key.cmd_id = DINO_CMD_WEP_KEY;
//	key.seq_num = 0;
//
//	/* Note: AES keys cannot be set for multiple times.
//	 * Only set it at the first time. */
//	for (i = 0; i < 4; i++) {
//		key.key_index = i | type;
//		if (!(priv->ieee->sec.flags & (1 << i))) {
//			key.key_size = 0;
//			continue;
//		}
//
//		key.key_size = priv->ieee->sec.key_sizes[i];
//		memcpy(key.key, priv->ieee->sec.keys[i], key.key_size);
//
//		ipw_send_cmd_pdu(priv, IPW_CMD_WEP_KEY, sizeof(key), &key);
//	}
//}
//
//static void ipw_set_hw_decrypt_unicast(struct ipw_priv *priv, int level)
//{
//	if (priv->ieee->host_encrypt)
//		return;
//
//	switch (level) {
//	case SEC_LEVEL_3:
//		priv->sys_config.disable_unicast_decryption = 0;
//		priv->ieee->host_decrypt = 0;
//		break;
//	case SEC_LEVEL_2:
//		priv->sys_config.disable_unicast_decryption = 1;
//		priv->ieee->host_decrypt = 1;
//		break;
//	case SEC_LEVEL_1:
//		priv->sys_config.disable_unicast_decryption = 0;
//		priv->ieee->host_decrypt = 0;
//		break;
//	case SEC_LEVEL_0:
//		priv->sys_config.disable_unicast_decryption = 1;
//		break;
//	default:
//		break;
//	}
//}
//
//static void ipw_set_hw_decrypt_multicast(struct ipw_priv *priv, int level)
//{
//	if (priv->ieee->host_encrypt)
//		return;
//
//	switch (level) {
//	case SEC_LEVEL_3:
//		priv->sys_config.disable_multicast_decryption = 0;
//		break;
//	case SEC_LEVEL_2:
//		priv->sys_config.disable_multicast_decryption = 1;
//		break;
//	case SEC_LEVEL_1:
//		priv->sys_config.disable_multicast_decryption = 0;
//		break;
//	case SEC_LEVEL_0:
//		priv->sys_config.disable_multicast_decryption = 1;
//		break;
//	default:
//		break;
//	}
//}
//
//static void ipw_set_hwcrypto_keys(struct ipw_priv *priv)
//{
//	switch (priv->ieee->sec.level) {
//	case SEC_LEVEL_3:
//		if (priv->ieee->sec.flags & SEC_ACTIVE_KEY)
//			ipw_send_tgi_tx_key(priv,
//					    DCT_FLAG_EXT_SECURITY_CCM,
//					    priv->ieee->sec.active_key);
//
//		if (!priv->ieee->host_mc_decrypt)
//			ipw_send_wep_keys(priv, DCW_WEP_KEY_SEC_TYPE_CCM);
//		break;
//	case SEC_LEVEL_2:
//		if (priv->ieee->sec.flags & SEC_ACTIVE_KEY)
//			ipw_send_tgi_tx_key(priv,
//					    DCT_FLAG_EXT_SECURITY_TKIP,
//					    priv->ieee->sec.active_key);
//		break;
//	case SEC_LEVEL_1:
//		ipw_send_wep_keys(priv, DCW_WEP_KEY_SEC_TYPE_WEP);
//		ipw_set_hw_decrypt_unicast(priv, priv->ieee->sec.level);
//		ipw_set_hw_decrypt_multicast(priv, priv->ieee->sec.level);
//		break;
//	case SEC_LEVEL_0:
//	default:
//		break;
//	}
//}
//
//static void ipw_adhoc_check(void *data)
//{
//	struct ipw_priv *priv = data;
//
//	if (priv->missed_adhoc_beacons++ > priv->disassociate_threshold &&
//	    !(priv->config & CFG_ADHOC_PERSIST)) {
//		IPW_DEBUG(IPW_DL_INFO | IPW_DL_NOTIF |
//			  IPW_DL_STATE | IPW_DL_ASSOC,
//			  "Missed beacon: %d - disassociate\n",
//			  priv->missed_adhoc_beacons);
//		ipw_remove_current_network(priv);
//		ipw_disassociate(priv);
//		return;
//	}
//
//	queue_delayed_work(priv->workqueue, &priv->adhoc_check,
//			   le16_to_cpu(priv->assoc_request.beacon_interval));
//}
//
//static void ipw_bg_adhoc_check(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, adhoc_check.work);
//	mutex_lock(&priv->mutex);
//	ipw_adhoc_check(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static void ipw_debug_config(struct ipw_priv *priv)
//{
//	DECLARE_SSID_BUF(ssid);
//	IPW_DEBUG_INFO("Scan completed, no valid APs matched "
//		       "[CFG 0x%08X]\n", priv->config);
//	if (priv->config & CFG_STATIC_CHANNEL)
//		IPW_DEBUG_INFO("Channel locked to %d\n", priv->channel);
//	else
//		IPW_DEBUG_INFO("Channel unlocked.\n");
//	if (priv->config & CFG_STATIC_ESSID)
//		IPW_DEBUG_INFO("ESSID locked to '%s'\n",
//			       print_ssid(ssid, priv->essid, priv->essid_len));
//	else
//		IPW_DEBUG_INFO("ESSID unlocked.\n");
//	if (priv->config & CFG_STATIC_BSSID)
//		IPW_DEBUG_INFO("BSSID locked to %pM\n", priv->bssid);
//	else
//		IPW_DEBUG_INFO("BSSID unlocked.\n");
//	if (priv->capability & CAP_PRIVACY_ON)
//		IPW_DEBUG_INFO("PRIVACY on\n");
//	else
//		IPW_DEBUG_INFO("PRIVACY off\n");
//	IPW_DEBUG_INFO("RATE MASK: 0x%08X\n", priv->rates_mask);
//}
//
//static void ipw_set_fixed_rate(struct ipw_priv *priv, int mode)
//{
//	/* TODO: Verify that this works... */
//	struct ipw_fixed_rate fr;
//	u32 reg;
//	u16 mask = 0;
//	u16 new_tx_rates = priv->rates_mask;
//
//	/* Identify 'current FW band' and match it with the fixed
//	 * Tx rates */
//
//	switch (priv->ieee->freq_band) {
//	case LIBIPW_52GHZ_BAND:	/* A only */
//		/* IEEE_A */
//		if (priv->rates_mask & ~LIBIPW_OFDM_RATES_MASK) {
//			/* Invalid fixed rate mask */
//			IPW_DEBUG_WX
//			    ("invalid fixed rate mask in ipw_set_fixed_rate\n");
//			new_tx_rates = 0;
//			break;
//		}
//
//		new_tx_rates >>= LIBIPW_OFDM_SHIFT_MASK_A;
//		break;
//
//	default:		/* 2.4Ghz or Mixed */
//		/* IEEE_B */
//		if (mode == IEEE_B) {
//			if (new_tx_rates & ~LIBIPW_CCK_RATES_MASK) {
//				/* Invalid fixed rate mask */
//				IPW_DEBUG_WX
//				    ("invalid fixed rate mask in ipw_set_fixed_rate\n");
//				new_tx_rates = 0;
//			}
//			break;
//		}
//
//		/* IEEE_G */
//		if (new_tx_rates & ~(LIBIPW_CCK_RATES_MASK |
//				    LIBIPW_OFDM_RATES_MASK)) {
//			/* Invalid fixed rate mask */
//			IPW_DEBUG_WX
//			    ("invalid fixed rate mask in ipw_set_fixed_rate\n");
//			new_tx_rates = 0;
//			break;
//		}
//
//		if (LIBIPW_OFDM_RATE_6MB_MASK & new_tx_rates) {
//			mask |= (LIBIPW_OFDM_RATE_6MB_MASK >> 1);
//			new_tx_rates &= ~LIBIPW_OFDM_RATE_6MB_MASK;
//		}
//
//		if (LIBIPW_OFDM_RATE_9MB_MASK & new_tx_rates) {
//			mask |= (LIBIPW_OFDM_RATE_9MB_MASK >> 1);
//			new_tx_rates &= ~LIBIPW_OFDM_RATE_9MB_MASK;
//		}
//
//		if (LIBIPW_OFDM_RATE_12MB_MASK & new_tx_rates) {
//			mask |= (LIBIPW_OFDM_RATE_12MB_MASK >> 1);
//			new_tx_rates &= ~LIBIPW_OFDM_RATE_12MB_MASK;
//		}
//
//		new_tx_rates |= mask;
//		break;
//	}
//
//	fr.tx_rates = cpu_to_le16(new_tx_rates);
//
//	reg = ipw_read32(priv, IPW_MEM_FIXED_OVERRIDE);
//	ipw_write_reg32(priv, reg, *(u32 *) & fr);
//}
//
//static void ipw_abort_scan(struct ipw_priv *priv)
//{
//	int err;
//
//	if (priv->status & STATUS_SCAN_ABORTING) {
//		IPW_DEBUG_HC("Ignoring concurrent scan abort request.\n");
//		return;
//	}
//	priv->status |= STATUS_SCAN_ABORTING;
//
//	err = ipw_send_scan_abort(priv);
//	if (err)
//		IPW_DEBUG_HC("Request to abort scan failed.\n");
//}
//
//static void ipw_add_scan_channels(struct ipw_priv *priv,
//				  struct ipw_scan_request_ext *scan,
//				  int scan_type)
//{
//	int channel_index = 0;
//	const struct libipw_geo *geo;
//	int i;
//
//	geo = libipw_get_geo(priv->ieee);
//
//	if (priv->ieee->freq_band & LIBIPW_52GHZ_BAND) {
//		int start = channel_index;
//		for (i = 0; i < geo->a_channels; i++) {
//			if ((priv->status & STATUS_ASSOCIATED) &&
//			    geo->a[i].channel == priv->channel)
//				continue;
//			channel_index++;
//			scan->channels_list[channel_index] = geo->a[i].channel;
//			ipw_set_scan_type(scan, channel_index,
//					  geo->a[i].
//					  flags & LIBIPW_CH_PASSIVE_ONLY ?
//					  IPW_SCAN_PASSIVE_FULL_DWELL_SCAN :
//					  scan_type);
//		}
//
//		if (start != channel_index) {
//			scan->channels_list[start] = (u8) (IPW_A_MODE << 6) |
//			    (channel_index - start);
//			channel_index++;
//		}
//	}
//
//	if (priv->ieee->freq_band & LIBIPW_24GHZ_BAND) {
//		int start = channel_index;
//		if (priv->config & CFG_SPEED_SCAN) {
//			int index;
//			u8 channels[LIBIPW_24GHZ_CHANNELS] = {
//				/* nop out the list */
//				[0] = 0
//			};
//
//			u8 channel;
//			while (channel_index < IPW_SCAN_CHANNELS - 1) {
//				channel =
//				    priv->speed_scan[priv->speed_scan_pos];
//				if (channel == 0) {
//					priv->speed_scan_pos = 0;
//					channel = priv->speed_scan[0];
//				}
//				if ((priv->status & STATUS_ASSOCIATED) &&
//				    channel == priv->channel) {
//					priv->speed_scan_pos++;
//					continue;
//				}
//
//				/* If this channel has already been
//				 * added in scan, break from loop
//				 * and this will be the first channel
//				 * in the next scan.
//				 */
//				if (channels[channel - 1] != 0)
//					break;
//
//				channels[channel - 1] = 1;
//				priv->speed_scan_pos++;
//				channel_index++;
//				scan->channels_list[channel_index] = channel;
//				index =
//				    libipw_channel_to_index(priv->ieee, channel);
//				ipw_set_scan_type(scan, channel_index,
//						  geo->bg[index].
//						  flags &
//						  LIBIPW_CH_PASSIVE_ONLY ?
//						  IPW_SCAN_PASSIVE_FULL_DWELL_SCAN
//						  : scan_type);
//			}
//		} else {
//			for (i = 0; i < geo->bg_channels; i++) {
//				if ((priv->status & STATUS_ASSOCIATED) &&
//				    geo->bg[i].channel == priv->channel)
//					continue;
//				channel_index++;
//				scan->channels_list[channel_index] =
//				    geo->bg[i].channel;
//				ipw_set_scan_type(scan, channel_index,
//						  geo->bg[i].
//						  flags &
//						  LIBIPW_CH_PASSIVE_ONLY ?
//						  IPW_SCAN_PASSIVE_FULL_DWELL_SCAN
//						  : scan_type);
//			}
//		}
//
//		if (start != channel_index) {
//			scan->channels_list[start] = (u8) (IPW_B_MODE << 6) |
//			    (channel_index - start);
//		}
//	}
//}
//
//static int ipw_passive_dwell_time(struct ipw_priv *priv)
//{
//	/* staying on passive channels longer than the DTIM interval during a
//	 * scan, while associated, causes the firmware to cancel the scan
//	 * without notification. Hence, don't stay on passive channels longer
//	 * than the beacon interval.
//	 */
//	if (priv->status & STATUS_ASSOCIATED
//	    && priv->assoc_network->beacon_interval > 10)
//		return priv->assoc_network->beacon_interval - 10;
//	else
//		return 120;
//}
//
//static int ipw_request_scan_helper(struct ipw_priv *priv, int type, int direct)
//{
//	struct ipw_scan_request_ext scan;
//	int err = 0, scan_type;
//
//	if (!(priv->status & STATUS_INIT) ||
//	    (priv->status & STATUS_EXIT_PENDING))
//		return 0;
//
//	mutex_lock(&priv->mutex);
//
//	if (direct && (priv->direct_scan_ssid_len == 0)) {
//		IPW_DEBUG_HC("Direct scan requested but no SSID to scan for\n");
//		priv->status &= ~STATUS_DIRECT_SCAN_PENDING;
//		goto done;
//	}
//
//	if (priv->status & STATUS_SCANNING) {
//		IPW_DEBUG_HC("Concurrent scan requested.  Queuing.\n");
//		priv->status |= direct ? STATUS_DIRECT_SCAN_PENDING :
//					STATUS_SCAN_PENDING;
//		goto done;
//	}
//
//	if (!(priv->status & STATUS_SCAN_FORCED) &&
//	    priv->status & STATUS_SCAN_ABORTING) {
//		IPW_DEBUG_HC("Scan request while abort pending.  Queuing.\n");
//		priv->status |= direct ? STATUS_DIRECT_SCAN_PENDING :
//					STATUS_SCAN_PENDING;
//		goto done;
//	}
//
//	if (priv->status & STATUS_RF_KILL_MASK) {
//		IPW_DEBUG_HC("Queuing scan due to RF Kill activation\n");
//		priv->status |= direct ? STATUS_DIRECT_SCAN_PENDING :
//					STATUS_SCAN_PENDING;
//		goto done;
//	}
//
//	memset(&scan, 0, sizeof(scan));
//	scan.full_scan_index = cpu_to_le32(libipw_get_scans(priv->ieee));
//
//	if (type == IW_SCAN_TYPE_PASSIVE) {
//		IPW_DEBUG_WX("use passive scanning\n");
//		scan_type = IPW_SCAN_PASSIVE_FULL_DWELL_SCAN;
//		scan.dwell_time[IPW_SCAN_PASSIVE_FULL_DWELL_SCAN] =
//			cpu_to_le16(ipw_passive_dwell_time(priv));
//		ipw_add_scan_channels(priv, &scan, scan_type);
//		goto send_request;
//	}
//
//	/* Use active scan by default. */
//	if (priv->config & CFG_SPEED_SCAN)
//		scan.dwell_time[IPW_SCAN_ACTIVE_BROADCAST_SCAN] =
//			cpu_to_le16(30);
//	else
//		scan.dwell_time[IPW_SCAN_ACTIVE_BROADCAST_SCAN] =
//			cpu_to_le16(20);
//
//	scan.dwell_time[IPW_SCAN_ACTIVE_BROADCAST_AND_DIRECT_SCAN] =
//		cpu_to_le16(20);
//
//	scan.dwell_time[IPW_SCAN_PASSIVE_FULL_DWELL_SCAN] =
//		cpu_to_le16(ipw_passive_dwell_time(priv));
//	scan.dwell_time[IPW_SCAN_ACTIVE_DIRECT_SCAN] = cpu_to_le16(20);
//
//#ifdef CONFIG_IPW2200_MONITOR
//	if (priv->ieee->iw_mode == IW_MODE_MONITOR) {
//		u8 channel;
//		u8 band = 0;
//
//		switch (libipw_is_valid_channel(priv->ieee, priv->channel)) {
//		case LIBIPW_52GHZ_BAND:
//			band = (u8) (IPW_A_MODE << 6) | 1;
//			channel = priv->channel;
//			break;
//
//		case LIBIPW_24GHZ_BAND:
//			band = (u8) (IPW_B_MODE << 6) | 1;
//			channel = priv->channel;
//			break;
//
//		default:
//			band = (u8) (IPW_B_MODE << 6) | 1;
//			channel = 9;
//			break;
//		}
//
//		scan.channels_list[0] = band;
//		scan.channels_list[1] = channel;
//		ipw_set_scan_type(&scan, 1, IPW_SCAN_PASSIVE_FULL_DWELL_SCAN);
//
//		/* NOTE:  The card will sit on this channel for this time
//		 * period.  Scan aborts are timing sensitive and frequently
//		 * result in firmware restarts.  As such, it is best to
//		 * set a small dwell_time here and just keep re-issuing
//		 * scans.  Otherwise fast channel hopping will not actually
//		 * hop channels.
//		 *
//		 * TODO: Move SPEED SCAN support to all modes and bands */
//		scan.dwell_time[IPW_SCAN_PASSIVE_FULL_DWELL_SCAN] =
//			cpu_to_le16(2000);
//	} else {
//#endif				/* CONFIG_IPW2200_MONITOR */
//		/* Honor direct scans first, otherwise if we are roaming make
//		 * this a direct scan for the current network.  Finally,
//		 * ensure that every other scan is a fast channel hop scan */
//		if (direct) {
//			err = ipw_send_ssid(priv, priv->direct_scan_ssid,
//			                    priv->direct_scan_ssid_len);
//			if (err) {
//				IPW_DEBUG_HC("Attempt to send SSID command  "
//					     "failed\n");
//				goto done;
//			}
//
//			scan_type = IPW_SCAN_ACTIVE_BROADCAST_AND_DIRECT_SCAN;
//		} else if ((priv->status & STATUS_ROAMING)
//			   || (!(priv->status & STATUS_ASSOCIATED)
//			       && (priv->config & CFG_STATIC_ESSID)
//			       && (le32_to_cpu(scan.full_scan_index) % 2))) {
//			err = ipw_send_ssid(priv, priv->essid, priv->essid_len);
//			if (err) {
//				IPW_DEBUG_HC("Attempt to send SSID command "
//					     "failed.\n");
//				goto done;
//			}
//
//			scan_type = IPW_SCAN_ACTIVE_BROADCAST_AND_DIRECT_SCAN;
//		} else
//			scan_type = IPW_SCAN_ACTIVE_BROADCAST_SCAN;
//
//		ipw_add_scan_channels(priv, &scan, scan_type);
//#ifdef CONFIG_IPW2200_MONITOR
//	}
//#endif
//
//send_request:
//	err = ipw_send_scan_request_ext(priv, &scan);
//	if (err) {
//		IPW_DEBUG_HC("Sending scan command failed: %08X\n", err);
//		goto done;
//	}
//
//	priv->status |= STATUS_SCANNING;
//	if (direct) {
//		priv->status &= ~STATUS_DIRECT_SCAN_PENDING;
//		priv->direct_scan_ssid_len = 0;
//	} else
//		priv->status &= ~STATUS_SCAN_PENDING;
//
//	queue_delayed_work(priv->workqueue, &priv->scan_check,
//			   IPW_SCAN_CHECK_WATCHDOG);
//done:
//	mutex_unlock(&priv->mutex);
//	return err;
//}
//
//static void ipw_request_passive_scan(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, request_passive_scan.work);
//	ipw_request_scan_helper(priv, IW_SCAN_TYPE_PASSIVE, 0);
//}
//
//static void ipw_request_scan(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, request_scan.work);
//	ipw_request_scan_helper(priv, IW_SCAN_TYPE_ACTIVE, 0);
//}
//
//static void ipw_request_direct_scan(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, request_direct_scan.work);
//	ipw_request_scan_helper(priv, IW_SCAN_TYPE_ACTIVE, 1);
//}
//
//static void ipw_bg_abort_scan(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, abort_scan);
//	mutex_lock(&priv->mutex);
//	ipw_abort_scan(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static int ipw_wpa_enable(struct ipw_priv *priv, int value)
//{
//	/* This is called when wpa_supplicant loads and closes the driver
//	 * interface. */
//	priv->ieee->wpa_enabled = value;
//	return 0;
//}
//
//static int ipw_wpa_set_auth_algs(struct ipw_priv *priv, int value)
//{
//	struct libipw_device *ieee = priv->ieee;
//	struct libipw_security sec = {
//		.flags = SEC_AUTH_MODE,
//	};
//	int ret = 0;
//
//	if (value & IW_AUTH_ALG_SHARED_KEY) {
//		sec.auth_mode = WLAN_AUTH_SHARED_KEY;
//		ieee->open_wep = 0;
//	} else if (value & IW_AUTH_ALG_OPEN_SYSTEM) {
//		sec.auth_mode = WLAN_AUTH_OPEN;
//		ieee->open_wep = 1;
//	} else if (value & IW_AUTH_ALG_LEAP) {
//		sec.auth_mode = WLAN_AUTH_LEAP;
//		ieee->open_wep = 1;
//	} else
//		return -EINVAL;
//
//	if (ieee->set_security)
//		ieee->set_security(ieee->dev, &sec);
//	else
//		ret = -EOPNOTSUPP;
//
//	return ret;
//}
//
//static void ipw_wpa_assoc_frame(struct ipw_priv *priv, char *wpa_ie,
//				int wpa_ie_len)
//{
//	/* make sure WPA is enabled */
//	ipw_wpa_enable(priv, 1);
//}
//
//static int ipw_set_rsn_capa(struct ipw_priv *priv,
//			    char *capabilities, int length)
//{
//	IPW_DEBUG_HC("HOST_CMD_RSN_CAPABILITIES\n");
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_RSN_CAPABILITIES, length,
//				capabilities);
//}
//
///*
// * WE-18 support
// */
//
///* SIOCSIWGENIE */
//static int ipw_wx_set_genie(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct libipw_device *ieee = priv->ieee;
//	u8 *buf;
//	int err = 0;
//
//	if (wrqu->data.length > MAX_WPA_IE_LEN ||
//	    (wrqu->data.length && extra == NULL))
//		return -EINVAL;
//
//	if (wrqu->data.length) {
//		buf = kmalloc(wrqu->data.length, GFP_KERNEL);
//		if (buf == NULL) {
//			err = -ENOMEM;
//			goto out;
//		}
//
//		memcpy(buf, extra, wrqu->data.length);
//		kfree(ieee->wpa_ie);
//		ieee->wpa_ie = buf;
//		ieee->wpa_ie_len = wrqu->data.length;
//	} else {
//		kfree(ieee->wpa_ie);
//		ieee->wpa_ie = NULL;
//		ieee->wpa_ie_len = 0;
//	}
//
//	ipw_wpa_assoc_frame(priv, ieee->wpa_ie, ieee->wpa_ie_len);
//      out:
//	return err;
//}
//
///* SIOCGIWGENIE */
//static int ipw_wx_get_genie(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct libipw_device *ieee = priv->ieee;
//	int err = 0;
//
//	if (ieee->wpa_ie_len == 0 || ieee->wpa_ie == NULL) {
//		wrqu->data.length = 0;
//		goto out;
//	}
//
//	if (wrqu->data.length < ieee->wpa_ie_len) {
//		err = -E2BIG;
//		goto out;
//	}
//
//	wrqu->data.length = ieee->wpa_ie_len;
//	memcpy(extra, ieee->wpa_ie, ieee->wpa_ie_len);
//
//      out:
//	return err;
//}
//
//static int wext_cipher2level(int cipher)
//{
//	switch (cipher) {
//	case IW_AUTH_CIPHER_NONE:
//		return SEC_LEVEL_0;
//	case IW_AUTH_CIPHER_WEP40:
//	case IW_AUTH_CIPHER_WEP104:
//		return SEC_LEVEL_1;
//	case IW_AUTH_CIPHER_TKIP:
//		return SEC_LEVEL_2;
//	case IW_AUTH_CIPHER_CCMP:
//		return SEC_LEVEL_3;
//	default:
//		return -1;
//	}
//}
//
///* SIOCSIWAUTH */
//static int ipw_wx_set_auth(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct libipw_device *ieee = priv->ieee;
//	struct iw_param *param = &wrqu->param;
//	struct lib80211_crypt_data *crypt;
//	unsigned long flags;
//	int ret = 0;
//
//	switch (param->flags & IW_AUTH_INDEX) {
//	case IW_AUTH_WPA_VERSION:
//		break;
//	case IW_AUTH_CIPHER_PAIRWISE:
//		ipw_set_hw_decrypt_unicast(priv,
//					   wext_cipher2level(param->value));
//		break;
//	case IW_AUTH_CIPHER_GROUP:
//		ipw_set_hw_decrypt_multicast(priv,
//					     wext_cipher2level(param->value));
//		break;
//	case IW_AUTH_KEY_MGMT:
//		/*
//		 * ipw2200 does not use these parameters
//		 */
//		break;
//
//	case IW_AUTH_TKIP_COUNTERMEASURES:
//		crypt = priv->ieee->crypt_info.crypt[priv->ieee->crypt_info.tx_keyidx];
//		if (!crypt || !crypt->ops->set_flags || !crypt->ops->get_flags)
//			break;
//
//		flags = crypt->ops->get_flags(crypt->priv);
//
//		if (param->value)
//			flags |= IEEE80211_CRYPTO_TKIP_COUNTERMEASURES;
//		else
//			flags &= ~IEEE80211_CRYPTO_TKIP_COUNTERMEASURES;
//
//		crypt->ops->set_flags(flags, crypt->priv);
//
//		break;
//
//	case IW_AUTH_DROP_UNENCRYPTED:{
//			/* HACK:
//			 *
//			 * wpa_supplicant calls set_wpa_enabled when the driver
//			 * is loaded and unloaded, regardless of if WPA is being
//			 * used.  No other calls are made which can be used to
//			 * determine if encryption will be used or not prior to
//			 * association being expected.  If encryption is not being
//			 * used, drop_unencrypted is set to false, else true -- we
//			 * can use this to determine if the CAP_PRIVACY_ON bit should
//			 * be set.
//			 */
//			struct libipw_security sec = {
//				.flags = SEC_ENABLED,
//				.enabled = param->value,
//			};
//			priv->ieee->drop_unencrypted = param->value;
//			/* We only change SEC_LEVEL for open mode. Others
//			 * are set by ipw_wpa_set_encryption.
//			 */
//			if (!param->value) {
//				sec.flags |= SEC_LEVEL;
//				sec.level = SEC_LEVEL_0;
//			} else {
//				sec.flags |= SEC_LEVEL;
//				sec.level = SEC_LEVEL_1;
//			}
//			if (priv->ieee->set_security)
//				priv->ieee->set_security(priv->ieee->dev, &sec);
//			break;
//		}
//
//	case IW_AUTH_80211_AUTH_ALG:
//		ret = ipw_wpa_set_auth_algs(priv, param->value);
//		break;
//
//	case IW_AUTH_WPA_ENABLED:
//		ret = ipw_wpa_enable(priv, param->value);
//		ipw_disassociate(priv);
//		break;
//
//	case IW_AUTH_RX_UNENCRYPTED_EAPOL:
//		ieee->ieee802_1x = param->value;
//		break;
//
//	case IW_AUTH_PRIVACY_INVOKED:
//		ieee->privacy_invoked = param->value;
//		break;
//
//	default:
//		return -EOPNOTSUPP;
//	}
//	return ret;
//}
//
///* SIOCGIWAUTH */
//static int ipw_wx_get_auth(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct libipw_device *ieee = priv->ieee;
//	struct lib80211_crypt_data *crypt;
//	struct iw_param *param = &wrqu->param;
//	int ret = 0;
//
//	switch (param->flags & IW_AUTH_INDEX) {
//	case IW_AUTH_WPA_VERSION:
//	case IW_AUTH_CIPHER_PAIRWISE:
//	case IW_AUTH_CIPHER_GROUP:
//	case IW_AUTH_KEY_MGMT:
//		/*
//		 * wpa_supplicant will control these internally
//		 */
//		ret = -EOPNOTSUPP;
//		break;
//
//	case IW_AUTH_TKIP_COUNTERMEASURES:
//		crypt = priv->ieee->crypt_info.crypt[priv->ieee->crypt_info.tx_keyidx];
//		if (!crypt || !crypt->ops->get_flags)
//			break;
//
//		param->value = (crypt->ops->get_flags(crypt->priv) &
//				IEEE80211_CRYPTO_TKIP_COUNTERMEASURES) ? 1 : 0;
//
//		break;
//
//	case IW_AUTH_DROP_UNENCRYPTED:
//		param->value = ieee->drop_unencrypted;
//		break;
//
//	case IW_AUTH_80211_AUTH_ALG:
//		param->value = ieee->sec.auth_mode;
//		break;
//
//	case IW_AUTH_WPA_ENABLED:
//		param->value = ieee->wpa_enabled;
//		break;
//
//	case IW_AUTH_RX_UNENCRYPTED_EAPOL:
//		param->value = ieee->ieee802_1x;
//		break;
//
//	case IW_AUTH_ROAMING_CONTROL:
//	case IW_AUTH_PRIVACY_INVOKED:
//		param->value = ieee->privacy_invoked;
//		break;
//
//	default:
//		return -EOPNOTSUPP;
//	}
//	return 0;
//}
//
///* SIOCSIWENCODEEXT */
//static int ipw_wx_set_encodeext(struct net_device *dev,
//				struct iw_request_info *info,
//				union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct iw_encode_ext *ext = (struct iw_encode_ext *)extra;
//
//	if (hwcrypto) {
//		if (ext->alg == IW_ENCODE_ALG_TKIP) {
//			/* IPW HW can't build TKIP MIC,
//			   host decryption still needed */
//			if (ext->ext_flags & IW_ENCODE_EXT_GROUP_KEY)
//				priv->ieee->host_mc_decrypt = 1;
//			else {
//				priv->ieee->host_encrypt = 0;
//				priv->ieee->host_encrypt_msdu = 1;
//				priv->ieee->host_decrypt = 1;
//			}
//		} else {
//			priv->ieee->host_encrypt = 0;
//			priv->ieee->host_encrypt_msdu = 0;
//			priv->ieee->host_decrypt = 0;
//			priv->ieee->host_mc_decrypt = 0;
//		}
//	}
//
//	return libipw_wx_set_encodeext(priv->ieee, info, wrqu, extra);
//}
//
///* SIOCGIWENCODEEXT */
//static int ipw_wx_get_encodeext(struct net_device *dev,
//				struct iw_request_info *info,
//				union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	return libipw_wx_get_encodeext(priv->ieee, info, wrqu, extra);
//}
//
///* SIOCSIWMLME */
//static int ipw_wx_set_mlme(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct iw_mlme *mlme = (struct iw_mlme *)extra;
//	__le16 reason;
//
//	reason = cpu_to_le16(mlme->reason_code);
//
//	switch (mlme->cmd) {
//	case IW_MLME_DEAUTH:
//		/* silently ignore */
//		break;
//
//	case IW_MLME_DISASSOC:
//		ipw_disassociate(priv);
//		break;
//
//	default:
//		return -EOPNOTSUPP;
//	}
//	return 0;
//}
//
//#ifdef CONFIG_IPW2200_QOS
//
///* QoS */
///*
//* get the modulation type of the current network or
//* the card current mode
//*/
//static u8 ipw_qos_current_mode(struct ipw_priv * priv)
//{
//	u8 mode = 0;
//
//	if (priv->status & STATUS_ASSOCIATED) {
//		unsigned long flags;
//
//		spin_lock_irqsave(&priv->ieee->lock, flags);
//		mode = priv->assoc_network->mode;
//		spin_unlock_irqrestore(&priv->ieee->lock, flags);
//	} else {
//		mode = priv->ieee->mode;
//	}
//	IPW_DEBUG_QOS("QoS network/card mode %d \n", mode);
//	return mode;
//}
//
///*
//* Handle management frame beacon and probe response
//*/
//static int ipw_qos_handle_probe_response(struct ipw_priv *priv,
//					 int active_network,
//					 struct libipw_network *network)
//{
//	u32 size = sizeof(struct libipw_qos_parameters);
//
//	if (network->capability & WLAN_CAPABILITY_IBSS)
//		network->qos_data.active = network->qos_data.supported;
//
//	if (network->flags & NETWORK_HAS_QOS_MASK) {
//		if (active_network &&
//		    (network->flags & NETWORK_HAS_QOS_PARAMETERS))
//			network->qos_data.active = network->qos_data.supported;
//
//		if ((network->qos_data.active == 1) && (active_network == 1) &&
//		    (network->flags & NETWORK_HAS_QOS_PARAMETERS) &&
//		    (network->qos_data.old_param_count !=
//		     network->qos_data.param_count)) {
//			network->qos_data.old_param_count =
//			    network->qos_data.param_count;
//			schedule_work(&priv->qos_activate);
//			IPW_DEBUG_QOS("QoS parameters change call "
//				      "qos_activate\n");
//		}
//	} else {
//		if ((priv->ieee->mode == IEEE_B) || (network->mode == IEEE_B))
//			memcpy(&network->qos_data.parameters,
//			       &def_parameters_CCK, size);
//		else
//			memcpy(&network->qos_data.parameters,
//			       &def_parameters_OFDM, size);
//
//		if ((network->qos_data.active == 1) && (active_network == 1)) {
//			IPW_DEBUG_QOS("QoS was disabled call qos_activate \n");
//			schedule_work(&priv->qos_activate);
//		}
//
//		network->qos_data.active = 0;
//		network->qos_data.supported = 0;
//	}
//	if ((priv->status & STATUS_ASSOCIATED) &&
//	    (priv->ieee->iw_mode == IW_MODE_ADHOC) && (active_network == 0)) {
//		if (memcmp(network->bssid, priv->bssid, ETH_ALEN))
//			if (network->capability & WLAN_CAPABILITY_IBSS)
//				if ((network->ssid_len ==
//				     priv->assoc_network->ssid_len) &&
//				    !memcmp(network->ssid,
//					    priv->assoc_network->ssid,
//					    network->ssid_len)) {
//					queue_work(priv->workqueue,
//						   &priv->merge_networks);
//				}
//	}
//
//	return 0;
//}
//
///*
//* This function set up the firmware to support QoS. It sends
//* IPW_CMD_QOS_PARAMETERS and IPW_CMD_WME_INFO
//*/
//static int ipw_qos_activate(struct ipw_priv *priv,
//			    struct libipw_qos_data *qos_network_data)
//{
//	int err;
//	struct libipw_qos_parameters qos_parameters[QOS_QOS_SETS];
//	struct libipw_qos_parameters *active_one = NULL;
//	u32 size = sizeof(struct libipw_qos_parameters);
//	u32 burst_duration;
//	int i;
//	u8 type;
//
//	type = ipw_qos_current_mode(priv);
//
//	active_one = &(qos_parameters[QOS_PARAM_SET_DEF_CCK]);
//	memcpy(active_one, priv->qos_data.def_qos_parm_CCK, size);
//	active_one = &(qos_parameters[QOS_PARAM_SET_DEF_OFDM]);
//	memcpy(active_one, priv->qos_data.def_qos_parm_OFDM, size);
//
//	if (qos_network_data == NULL) {
//		if (type == IEEE_B) {
//			IPW_DEBUG_QOS("QoS activate network mode %d\n", type);
//			active_one = &def_parameters_CCK;
//		} else
//			active_one = &def_parameters_OFDM;
//
//		memcpy(&qos_parameters[QOS_PARAM_SET_ACTIVE], active_one, size);
//		burst_duration = ipw_qos_get_burst_duration(priv);
//		for (i = 0; i < QOS_QUEUE_NUM; i++)
//			qos_parameters[QOS_PARAM_SET_ACTIVE].tx_op_limit[i] =
//			    cpu_to_le16(burst_duration);
//	} else if (priv->ieee->iw_mode == IW_MODE_ADHOC) {
//		if (type == IEEE_B) {
//			IPW_DEBUG_QOS("QoS activate IBSS nework mode %d\n",
//				      type);
//			if (priv->qos_data.qos_enable == 0)
//				active_one = &def_parameters_CCK;
//			else
//				active_one = priv->qos_data.def_qos_parm_CCK;
//		} else {
//			if (priv->qos_data.qos_enable == 0)
//				active_one = &def_parameters_OFDM;
//			else
//				active_one = priv->qos_data.def_qos_parm_OFDM;
//		}
//		memcpy(&qos_parameters[QOS_PARAM_SET_ACTIVE], active_one, size);
//	} else {
//		unsigned long flags;
//		int active;
//
//		spin_lock_irqsave(&priv->ieee->lock, flags);
//		active_one = &(qos_network_data->parameters);
//		qos_network_data->old_param_count =
//		    qos_network_data->param_count;
//		memcpy(&qos_parameters[QOS_PARAM_SET_ACTIVE], active_one, size);
//		active = qos_network_data->supported;
//		spin_unlock_irqrestore(&priv->ieee->lock, flags);
//
//		if (active == 0) {
//			burst_duration = ipw_qos_get_burst_duration(priv);
//			for (i = 0; i < QOS_QUEUE_NUM; i++)
//				qos_parameters[QOS_PARAM_SET_ACTIVE].
//				    tx_op_limit[i] = cpu_to_le16(burst_duration);
//		}
//	}
//
//	IPW_DEBUG_QOS("QoS sending IPW_CMD_QOS_PARAMETERS\n");
//	err = ipw_send_qos_params_command(priv,
//					  (struct libipw_qos_parameters *)
//					  &(qos_parameters[0]));
//	if (err)
//		IPW_DEBUG_QOS("QoS IPW_CMD_QOS_PARAMETERS failed\n");
//
//	return err;
//}
//
///*
//* send IPW_CMD_WME_INFO to the firmware
//*/
//static int ipw_qos_set_info_element(struct ipw_priv *priv)
//{
//	int ret = 0;
//	struct libipw_qos_information_element qos_info;
//
//	if (priv == NULL)
//		return -1;
//
//	qos_info.elementID = QOS_ELEMENT_ID;
//	qos_info.length = sizeof(struct libipw_qos_information_element) - 2;
//
//	qos_info.version = QOS_VERSION_1;
//	qos_info.ac_info = 0;
//
//	memcpy(qos_info.qui, qos_oui, QOS_OUI_LEN);
//	qos_info.qui_type = QOS_OUI_TYPE;
//	qos_info.qui_subtype = QOS_OUI_INFO_SUB_TYPE;
//
//	ret = ipw_send_qos_info_command(priv, &qos_info);
//	if (ret != 0) {
//		IPW_DEBUG_QOS("QoS error calling ipw_send_qos_info_command\n");
//	}
//	return ret;
//}
//
///*
//* Set the QoS parameter with the association request structure
//*/
//static int ipw_qos_association(struct ipw_priv *priv,
//			       struct libipw_network *network)
//{
//	int err = 0;
//	struct libipw_qos_data *qos_data = NULL;
//	struct libipw_qos_data ibss_data = {
//		.supported = 1,
//		.active = 1,
//	};
//
//	switch (priv->ieee->iw_mode) {
//	case IW_MODE_ADHOC:
//		BUG_ON(!(network->capability & WLAN_CAPABILITY_IBSS));
//
//		qos_data = &ibss_data;
//		break;
//
//	case IW_MODE_INFRA:
//		qos_data = &network->qos_data;
//		break;
//
//	default:
//		BUG();
//		break;
//	}
//
//	err = ipw_qos_activate(priv, qos_data);
//	if (err) {
//		priv->assoc_request.policy_support &= ~HC_QOS_SUPPORT_ASSOC;
//		return err;
//	}
//
//	if (priv->qos_data.qos_enable && qos_data->supported) {
//		IPW_DEBUG_QOS("QoS will be enabled for this association\n");
//		priv->assoc_request.policy_support |= HC_QOS_SUPPORT_ASSOC;
//		return ipw_qos_set_info_element(priv);
//	}
//
//	return 0;
//}
//
///*
//* handling the beaconing responses. if we get different QoS setting
//* off the network from the associated setting, adjust the QoS
//* setting
//*/
//static int ipw_qos_association_resp(struct ipw_priv *priv,
//				    struct libipw_network *network)
//{
//	int ret = 0;
//	unsigned long flags;
//	u32 size = sizeof(struct libipw_qos_parameters);
//	int set_qos_param = 0;
//
//	if ((priv == NULL) || (network == NULL) ||
//	    (priv->assoc_network == NULL))
//		return ret;
//
//	if (!(priv->status & STATUS_ASSOCIATED))
//		return ret;
//
//	if ((priv->ieee->iw_mode != IW_MODE_INFRA))
//		return ret;
//
//	spin_lock_irqsave(&priv->ieee->lock, flags);
//	if (network->flags & NETWORK_HAS_QOS_PARAMETERS) {
//		memcpy(&priv->assoc_network->qos_data, &network->qos_data,
//		       sizeof(struct libipw_qos_data));
//		priv->assoc_network->qos_data.active = 1;
//		if ((network->qos_data.old_param_count !=
//		     network->qos_data.param_count)) {
//			set_qos_param = 1;
//			network->qos_data.old_param_count =
//			    network->qos_data.param_count;
//		}
//
//	} else {
//		if ((network->mode == IEEE_B) || (priv->ieee->mode == IEEE_B))
//			memcpy(&priv->assoc_network->qos_data.parameters,
//			       &def_parameters_CCK, size);
//		else
//			memcpy(&priv->assoc_network->qos_data.parameters,
//			       &def_parameters_OFDM, size);
//		priv->assoc_network->qos_data.active = 0;
//		priv->assoc_network->qos_data.supported = 0;
//		set_qos_param = 1;
//	}
//
//	spin_unlock_irqrestore(&priv->ieee->lock, flags);
//
//	if (set_qos_param == 1)
//		schedule_work(&priv->qos_activate);
//
//	return ret;
//}
//
//static u32 ipw_qos_get_burst_duration(struct ipw_priv *priv)
//{
//	u32 ret = 0;
//
//	if ((priv == NULL))
//		return 0;
//
//	if (!(priv->ieee->modulation & LIBIPW_OFDM_MODULATION))
//		ret = priv->qos_data.burst_duration_CCK;
//	else
//		ret = priv->qos_data.burst_duration_OFDM;
//
//	return ret;
//}
//
///*
//* Initialize the setting of QoS global
//*/
//static void ipw_qos_init(struct ipw_priv *priv, int enable,
//			 int burst_enable, u32 burst_duration_CCK,
//			 u32 burst_duration_OFDM)
//{
//	priv->qos_data.qos_enable = enable;
//
//	if (priv->qos_data.qos_enable) {
//		priv->qos_data.def_qos_parm_CCK = &def_qos_parameters_CCK;
//		priv->qos_data.def_qos_parm_OFDM = &def_qos_parameters_OFDM;
//		IPW_DEBUG_QOS("QoS is enabled\n");
//	} else {
//		priv->qos_data.def_qos_parm_CCK = &def_parameters_CCK;
//		priv->qos_data.def_qos_parm_OFDM = &def_parameters_OFDM;
//		IPW_DEBUG_QOS("QoS is not enabled\n");
//	}
//
//	priv->qos_data.burst_enable = burst_enable;
//
//	if (burst_enable) {
//		priv->qos_data.burst_duration_CCK = burst_duration_CCK;
//		priv->qos_data.burst_duration_OFDM = burst_duration_OFDM;
//	} else {
//		priv->qos_data.burst_duration_CCK = 0;
//		priv->qos_data.burst_duration_OFDM = 0;
//	}
//}
//
///*
//* map the packet priority to the right TX Queue
//*/
//static int ipw_get_tx_queue_number(struct ipw_priv *priv, u16 priority)
//{
//	if (priority > 7 || !priv->qos_data.qos_enable)
//		priority = 0;
//
//	return from_priority_to_tx_queue[priority] - 1;
//}
//
//static int ipw_is_qos_active(struct net_device *dev,
//			     struct sk_buff *skb)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct libipw_qos_data *qos_data = NULL;
//	int active, supported;
//	u8 *daddr = skb->data + ETH_ALEN;
//	int unicast = !is_multicast_ether_addr(daddr);
//
//	if (!(priv->status & STATUS_ASSOCIATED))
//		return 0;
//
//	qos_data = &priv->assoc_network->qos_data;
//
//	if (priv->ieee->iw_mode == IW_MODE_ADHOC) {
//		if (unicast == 0)
//			qos_data->active = 0;
//		else
//			qos_data->active = qos_data->supported;
//	}
//	active = qos_data->active;
//	supported = qos_data->supported;
//	IPW_DEBUG_QOS("QoS  %d network is QoS active %d  supported %d  "
//		      "unicast %d\n",
//		      priv->qos_data.qos_enable, active, supported, unicast);
//	if (active && priv->qos_data.qos_enable)
//		return 1;
//
//	return 0;
//
//}
///*
//* add QoS parameter to the TX command
//*/
//static int ipw_qos_set_tx_queue_command(struct ipw_priv *priv,
//					u16 priority,
//					struct tfd_data *tfd)
//{
//	int tx_queue_id = 0;
//
//
//	tx_queue_id = from_priority_to_tx_queue[priority] - 1;
//	tfd->tx_flags_ext |= DCT_FLAG_EXT_QOS_ENABLED;
//
//	if (priv->qos_data.qos_no_ack_mask & (1UL << tx_queue_id)) {
//		tfd->tx_flags &= ~DCT_FLAG_ACK_REQD;
//		tfd->tfd.tfd_26.mchdr.qos_ctrl |= cpu_to_le16(CTRL_QOS_NO_ACK);
//	}
//	return 0;
//}
//
///*
//* background support to run QoS activate functionality
//*/
//static void ipw_bg_qos_activate(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, qos_activate);
//
//	mutex_lock(&priv->mutex);
//
//	if (priv->status & STATUS_ASSOCIATED)
//		ipw_qos_activate(priv, &(priv->assoc_network->qos_data));
//
//	mutex_unlock(&priv->mutex);
//}
//
//static int ipw_handle_probe_response(struct net_device *dev,
//				     struct libipw_probe_response *resp,
//				     struct libipw_network *network)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int active_network = ((priv->status & STATUS_ASSOCIATED) &&
//			      (network == priv->assoc_network));
//
//	ipw_qos_handle_probe_response(priv, active_network, network);
//
//	return 0;
//}
//
//static int ipw_handle_beacon(struct net_device *dev,
//			     struct libipw_beacon *resp,
//			     struct libipw_network *network)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int active_network = ((priv->status & STATUS_ASSOCIATED) &&
//			      (network == priv->assoc_network));
//
//	ipw_qos_handle_probe_response(priv, active_network, network);
//
//	return 0;
//}
//
//static int ipw_handle_assoc_response(struct net_device *dev,
//				     struct libipw_assoc_response *resp,
//				     struct libipw_network *network)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	ipw_qos_association_resp(priv, network);
//	return 0;
//}
//
//static int ipw_send_qos_params_command(struct ipw_priv *priv, struct libipw_qos_parameters
//				       *qos_param)
//{
//	return ipw_send_cmd_pdu(priv, IPW_CMD_QOS_PARAMETERS,
//				sizeof(*qos_param) * 3, qos_param);
//}
//
//static int ipw_send_qos_info_command(struct ipw_priv *priv, struct libipw_qos_information_element
//				     *qos_param)
//{
//	return ipw_send_cmd_pdu(priv, IPW_CMD_WME_INFO, sizeof(*qos_param),
//				qos_param);
//}
//
//#endif				/* CONFIG_IPW2200_QOS */
//
//static int ipw_associate_network(struct ipw_priv *priv,
//				 struct libipw_network *network,
//				 struct ipw_supported_rates *rates, int roaming)
//{
//	int err;
//	DECLARE_SSID_BUF(ssid);
//
//	if (priv->config & CFG_FIXED_RATE)
//		ipw_set_fixed_rate(priv, network->mode);
//
//	if (!(priv->config & CFG_STATIC_ESSID)) {
//		priv->essid_len = min(network->ssid_len,
//				      (u8) IW_ESSID_MAX_SIZE);
//		memcpy(priv->essid, network->ssid, priv->essid_len);
//	}
//
//	network->last_associate = jiffies;
//
//	memset(&priv->assoc_request, 0, sizeof(priv->assoc_request));
//	priv->assoc_request.channel = network->channel;
//	priv->assoc_request.auth_key = 0;
//
//	if ((priv->capability & CAP_PRIVACY_ON) &&
//	    (priv->ieee->sec.auth_mode == WLAN_AUTH_SHARED_KEY)) {
//		priv->assoc_request.auth_type = AUTH_SHARED_KEY;
//		priv->assoc_request.auth_key = priv->ieee->sec.active_key;
//
//		if (priv->ieee->sec.level == SEC_LEVEL_1)
//			ipw_send_wep_keys(priv, DCW_WEP_KEY_SEC_TYPE_WEP);
//
//	} else if ((priv->capability & CAP_PRIVACY_ON) &&
//		   (priv->ieee->sec.auth_mode == WLAN_AUTH_LEAP))
//		priv->assoc_request.auth_type = AUTH_LEAP;
//	else
//		priv->assoc_request.auth_type = AUTH_OPEN;
//
//	if (priv->ieee->wpa_ie_len) {
//		priv->assoc_request.policy_support = cpu_to_le16(0x02);	/* RSN active */
//		ipw_set_rsn_capa(priv, priv->ieee->wpa_ie,
//				 priv->ieee->wpa_ie_len);
//	}
//
//	/*
//	 * It is valid for our ieee device to support multiple modes, but
//	 * when it comes to associating to a given network we have to choose
//	 * just one mode.
//	 */
//	if (network->mode & priv->ieee->mode & IEEE_A)
//		priv->assoc_request.ieee_mode = IPW_A_MODE;
//	else if (network->mode & priv->ieee->mode & IEEE_G)
//		priv->assoc_request.ieee_mode = IPW_G_MODE;
//	else if (network->mode & priv->ieee->mode & IEEE_B)
//		priv->assoc_request.ieee_mode = IPW_B_MODE;
//
//	priv->assoc_request.capability = cpu_to_le16(network->capability);
//	if ((network->capability & WLAN_CAPABILITY_SHORT_PREAMBLE)
//	    && !(priv->config & CFG_PREAMBLE_LONG)) {
//		priv->assoc_request.preamble_length = DCT_FLAG_SHORT_PREAMBLE;
//	} else {
//		priv->assoc_request.preamble_length = DCT_FLAG_LONG_PREAMBLE;
//
//		/* Clear the short preamble if we won't be supporting it */
//		priv->assoc_request.capability &=
//		    ~cpu_to_le16(WLAN_CAPABILITY_SHORT_PREAMBLE);
//	}
//
//	/* Clear capability bits that aren't used in Ad Hoc */
//	if (priv->ieee->iw_mode == IW_MODE_ADHOC)
//		priv->assoc_request.capability &=
//		    ~cpu_to_le16(WLAN_CAPABILITY_SHORT_SLOT_TIME);
//
//	IPW_DEBUG_ASSOC("%sssocation attempt: '%s', channel %d, "
//			"802.11%c [%d], %s[:%s], enc=%s%s%s%c%c\n",
//			roaming ? "Rea" : "A",
//			print_ssid(ssid, priv->essid, priv->essid_len),
//			network->channel,
//			ipw_modes[priv->assoc_request.ieee_mode],
//			rates->num_rates,
//			(priv->assoc_request.preamble_length ==
//			 DCT_FLAG_LONG_PREAMBLE) ? "long" : "short",
//			network->capability &
//			WLAN_CAPABILITY_SHORT_PREAMBLE ? "short" : "long",
//			priv->capability & CAP_PRIVACY_ON ? "on " : "off",
//			priv->capability & CAP_PRIVACY_ON ?
//			(priv->capability & CAP_SHARED_KEY ? "(shared)" :
//			 "(open)") : "",
//			priv->capability & CAP_PRIVACY_ON ? " key=" : "",
//			priv->capability & CAP_PRIVACY_ON ?
//			'1' + priv->ieee->sec.active_key : '.',
//			priv->capability & CAP_PRIVACY_ON ? '.' : ' ');
//
//	priv->assoc_request.beacon_interval = cpu_to_le16(network->beacon_interval);
//	if ((priv->ieee->iw_mode == IW_MODE_ADHOC) &&
//	    (network->time_stamp[0] == 0) && (network->time_stamp[1] == 0)) {
//		priv->assoc_request.assoc_type = HC_IBSS_START;
//		priv->assoc_request.assoc_tsf_msw = 0;
//		priv->assoc_request.assoc_tsf_lsw = 0;
//	} else {
//		if (unlikely(roaming))
//			priv->assoc_request.assoc_type = HC_REASSOCIATE;
//		else
//			priv->assoc_request.assoc_type = HC_ASSOCIATE;
//		priv->assoc_request.assoc_tsf_msw = cpu_to_le32(network->time_stamp[1]);
//		priv->assoc_request.assoc_tsf_lsw = cpu_to_le32(network->time_stamp[0]);
//	}
//
//	memcpy(priv->assoc_request.bssid, network->bssid, ETH_ALEN);
//
//	if (priv->ieee->iw_mode == IW_MODE_ADHOC) {
//		memset(&priv->assoc_request.dest, 0xFF, ETH_ALEN);
//		priv->assoc_request.atim_window = cpu_to_le16(network->atim_window);
//	} else {
//		memcpy(priv->assoc_request.dest, network->bssid, ETH_ALEN);
//		priv->assoc_request.atim_window = 0;
//	}
//
//	priv->assoc_request.listen_interval = cpu_to_le16(network->listen_interval);
//
//	err = ipw_send_ssid(priv, priv->essid, priv->essid_len);
//	if (err) {
//		IPW_DEBUG_HC("Attempt to send SSID command failed.\n");
//		return err;
//	}
//
//	rates->ieee_mode = priv->assoc_request.ieee_mode;
//	rates->purpose = IPW_RATE_CONNECT;
//	ipw_send_supported_rates(priv, rates);
//
//	if (priv->assoc_request.ieee_mode == IPW_G_MODE)
//		priv->sys_config.dot11g_auto_detection = 1;
//	else
//		priv->sys_config.dot11g_auto_detection = 0;
//
//	if (priv->ieee->iw_mode == IW_MODE_ADHOC)
//		priv->sys_config.answer_broadcast_ssid_probe = 1;
//	else
//		priv->sys_config.answer_broadcast_ssid_probe = 0;
//
//	err = ipw_send_system_config(priv);
//	if (err) {
//		IPW_DEBUG_HC("Attempt to send sys config command failed.\n");
//		return err;
//	}
//
//	IPW_DEBUG_ASSOC("Association sensitivity: %d\n", network->stats.rssi);
//	err = ipw_set_sensitivity(priv, network->stats.rssi + IPW_RSSI_TO_DBM);
//	if (err) {
//		IPW_DEBUG_HC("Attempt to send associate command failed.\n");
//		return err;
//	}
//
//	/*
//	 * If preemption is enabled, it is possible for the association
//	 * to complete before we return from ipw_send_associate.  Therefore
//	 * we have to be sure and update our priviate data first.
//	 */
//	priv->channel = network->channel;
//	memcpy(priv->bssid, network->bssid, ETH_ALEN);
//	priv->status |= STATUS_ASSOCIATING;
//	priv->status &= ~STATUS_SECURITY_UPDATED;
//
//	priv->assoc_network = network;
//
//#ifdef CONFIG_IPW2200_QOS
//	ipw_qos_association(priv, network);
//#endif
//
//	err = ipw_send_associate(priv, &priv->assoc_request);
//	if (err) {
//		IPW_DEBUG_HC("Attempt to send associate command failed.\n");
//		return err;
//	}
//
//	IPW_DEBUG(IPW_DL_STATE, "associating: '%s' %pM \n",
//		  print_ssid(ssid, priv->essid, priv->essid_len),
//		  priv->bssid);
//
//	return 0;
//}
//
//static void ipw_roam(void *data)
//{
//	struct ipw_priv *priv = data;
//	struct libipw_network *network = NULL;
//	struct ipw_network_match match = {
//		.network = priv->assoc_network
//	};
//
//	/* The roaming process is as follows:
//	 *
//	 * 1.  Missed beacon threshold triggers the roaming process by
//	 *     setting the status ROAM bit and requesting a scan.
//	 * 2.  When the scan completes, it schedules the ROAM work
//	 * 3.  The ROAM work looks at all of the known networks for one that
//	 *     is a better network than the currently associated.  If none
//	 *     found, the ROAM process is over (ROAM bit cleared)
//	 * 4.  If a better network is found, a disassociation request is
//	 *     sent.
//	 * 5.  When the disassociation completes, the roam work is again
//	 *     scheduled.  The second time through, the driver is no longer
//	 *     associated, and the newly selected network is sent an
//	 *     association request.
//	 * 6.  At this point ,the roaming process is complete and the ROAM
//	 *     status bit is cleared.
//	 */
//
//	/* If we are no longer associated, and the roaming bit is no longer
//	 * set, then we are not actively roaming, so just return */
//	if (!(priv->status & (STATUS_ASSOCIATED | STATUS_ROAMING)))
//		return;
//
//	if (priv->status & STATUS_ASSOCIATED) {
//		/* First pass through ROAM process -- look for a better
//		 * network */
//		unsigned long flags;
//		u8 rssi = priv->assoc_network->stats.rssi;
//		priv->assoc_network->stats.rssi = -128;
//		spin_lock_irqsave(&priv->ieee->lock, flags);
//		list_for_each_entry(network, &priv->ieee->network_list, list) {
//			if (network != priv->assoc_network)
//				ipw_best_network(priv, &match, network, 1);
//		}
//		spin_unlock_irqrestore(&priv->ieee->lock, flags);
//		priv->assoc_network->stats.rssi = rssi;
//
//		if (match.network == priv->assoc_network) {
//			IPW_DEBUG_ASSOC("No better APs in this network to "
//					"roam to.\n");
//			priv->status &= ~STATUS_ROAMING;
//			ipw_debug_config(priv);
//			return;
//		}
//
//		ipw_send_disassociate(priv, 1);
//		priv->assoc_network = match.network;
//
//		return;
//	}
//
//	/* Second pass through ROAM process -- request association */
//	ipw_compatible_rates(priv, priv->assoc_network, &match.rates);
//	ipw_associate_network(priv, priv->assoc_network, &match.rates, 1);
//	priv->status &= ~STATUS_ROAMING;
//}
//
//static void ipw_bg_roam(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, roam);
//	mutex_lock(&priv->mutex);
//	ipw_roam(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static int ipw_associate(void *data)
//{
//	struct ipw_priv *priv = data;
//
//	struct libipw_network *network = NULL;
//	struct ipw_network_match match = {
//		.network = NULL
//	};
//	struct ipw_supported_rates *rates;
//	struct list_head *element;
//	unsigned long flags;
//	DECLARE_SSID_BUF(ssid);
//
//	if (priv->ieee->iw_mode == IW_MODE_MONITOR) {
//		IPW_DEBUG_ASSOC("Not attempting association (monitor mode)\n");
//		return 0;
//	}
//
//	if (priv->status & (STATUS_ASSOCIATED | STATUS_ASSOCIATING)) {
//		IPW_DEBUG_ASSOC("Not attempting association (already in "
//				"progress)\n");
//		return 0;
//	}
//
//	if (priv->status & STATUS_DISASSOCIATING) {
//		IPW_DEBUG_ASSOC("Not attempting association (in "
//				"disassociating)\n ");
//		queue_work(priv->workqueue, &priv->associate);
//		return 0;
//	}
//
//	if (!ipw_is_init(priv) || (priv->status & STATUS_SCANNING)) {
//		IPW_DEBUG_ASSOC("Not attempting association (scanning or not "
//				"initialized)\n");
//		return 0;
//	}
//
//	if (!(priv->config & CFG_ASSOCIATE) &&
//	    !(priv->config & (CFG_STATIC_ESSID | CFG_STATIC_BSSID))) {
//		IPW_DEBUG_ASSOC("Not attempting association (associate=0)\n");
//		return 0;
//	}
//
//	/* Protect our use of the network_list */
//	spin_lock_irqsave(&priv->ieee->lock, flags);
//	list_for_each_entry(network, &priv->ieee->network_list, list)
//	    ipw_best_network(priv, &match, network, 0);
//
//	network = match.network;
//	rates = &match.rates;
//
//	if (network == NULL &&
//	    priv->ieee->iw_mode == IW_MODE_ADHOC &&
//	    priv->config & CFG_ADHOC_CREATE &&
//	    priv->config & CFG_STATIC_ESSID &&
//	    priv->config & CFG_STATIC_CHANNEL) {
//		/* Use oldest network if the free list is empty */
//		if (list_empty(&priv->ieee->network_free_list)) {
//			struct libipw_network *oldest = NULL;
//			struct libipw_network *target;
//
//			list_for_each_entry(target, &priv->ieee->network_list, list) {
//				if ((oldest == NULL) ||
//				    (target->last_scanned < oldest->last_scanned))
//					oldest = target;
//			}
//
//			/* If there are no more slots, expire the oldest */
//			list_del(&oldest->list);
//			target = oldest;
//			IPW_DEBUG_ASSOC("Expired '%s' (%pM) from "
//					"network list.\n",
//					print_ssid(ssid, target->ssid,
//						   target->ssid_len),
//					target->bssid);
//			list_add_tail(&target->list,
//				      &priv->ieee->network_free_list);
//		}
//
//		element = priv->ieee->network_free_list.next;
//		network = list_entry(element, struct libipw_network, list);
//		ipw_adhoc_create(priv, network);
//		rates = &priv->rates;
//		list_del(element);
//		list_add_tail(&network->list, &priv->ieee->network_list);
//	}
//	spin_unlock_irqrestore(&priv->ieee->lock, flags);
//
//	/* If we reached the end of the list, then we don't have any valid
//	 * matching APs */
//	if (!network) {
//		ipw_debug_config(priv);
//
//		if (!(priv->status & STATUS_SCANNING)) {
//			if (!(priv->config & CFG_SPEED_SCAN))
//				queue_delayed_work(priv->workqueue,
//						   &priv->request_scan,
//						   SCAN_INTERVAL);
//			else
//				queue_delayed_work(priv->workqueue,
//						   &priv->request_scan, 0);
//		}
//
//		return 0;
//	}
//
//	ipw_associate_network(priv, network, rates, 0);
//
//	return 1;
//}
//
//static void ipw_bg_associate(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, associate);
//	mutex_lock(&priv->mutex);
//	ipw_associate(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static void ipw_rebuild_decrypted_skb(struct ipw_priv *priv,
//				      struct sk_buff *skb)
//{
//	struct ieee80211_hdr *hdr;
//	u16 fc;
//
//	hdr = (struct ieee80211_hdr *)skb->data;
//	fc = le16_to_cpu(hdr->frame_control);
//	if (!(fc & IEEE80211_FCTL_PROTECTED))
//		return;
//
//	fc &= ~IEEE80211_FCTL_PROTECTED;
//	hdr->frame_control = cpu_to_le16(fc);
//	switch (priv->ieee->sec.level) {
//	case SEC_LEVEL_3:
//		/* Remove CCMP HDR */
//		memmove(skb->data + LIBIPW_3ADDR_LEN,
//			skb->data + LIBIPW_3ADDR_LEN + 8,
//			skb->len - LIBIPW_3ADDR_LEN - 8);
//		skb_trim(skb, skb->len - 16);	/* CCMP_HDR_LEN + CCMP_MIC_LEN */
//		break;
//	case SEC_LEVEL_2:
//		break;
//	case SEC_LEVEL_1:
//		/* Remove IV */
//		memmove(skb->data + LIBIPW_3ADDR_LEN,
//			skb->data + LIBIPW_3ADDR_LEN + 4,
//			skb->len - LIBIPW_3ADDR_LEN - 4);
//		skb_trim(skb, skb->len - 8);	/* IV + ICV */
//		break;
//	case SEC_LEVEL_0:
//		break;
//	default:
//		printk(KERN_ERR "Unknow security level %d\n",
//		       priv->ieee->sec.level);
//		break;
//	}
//}
//
//static void ipw_handle_data_packet(struct ipw_priv *priv,
//				   struct ipw_rx_mem_buffer *rxb,
//				   struct libipw_rx_stats *stats)
//{
//	struct net_device *dev = priv->net_dev;
//	struct libipw_hdr_4addr *hdr;
//	struct ipw_rx_packet *pkt = (struct ipw_rx_packet *)rxb->skb->data;
//
//	/* We received data from the HW, so stop the watchdog */
//	dev->trans_start = jiffies;
//
//	/* We only process data packets if the
//	 * interface is open */
//	if (unlikely((le16_to_cpu(pkt->u.frame.length) + IPW_RX_FRAME_SIZE) >
//		     skb_tailroom(rxb->skb))) {
//		dev->stats.rx_errors++;
//		priv->wstats.discard.misc++;
//		IPW_DEBUG_DROP("Corruption detected! Oh no!\n");
//		return;
//	} else if (unlikely(!netif_running(priv->net_dev))) {
//		dev->stats.rx_dropped++;
//		priv->wstats.discard.misc++;
//		IPW_DEBUG_DROP("Dropping packet while interface is not up.\n");
//		return;
//	}
//
//	/* Advance skb->data to the start of the actual payload */
//	skb_reserve(rxb->skb, offsetof(struct ipw_rx_packet, u.frame.data));
//
//	/* Set the size of the skb to the size of the frame */
//	skb_put(rxb->skb, le16_to_cpu(pkt->u.frame.length));
//
//	IPW_DEBUG_RX("Rx packet of %d bytes.\n", rxb->skb->len);
//
//	/* HW decrypt will not clear the WEP bit, MIC, PN, etc. */
//	hdr = (struct libipw_hdr_4addr *)rxb->skb->data;
//	if (priv->ieee->iw_mode != IW_MODE_MONITOR &&
//	    (is_multicast_ether_addr(hdr->addr1) ?
//	     !priv->ieee->host_mc_decrypt : !priv->ieee->host_decrypt))
//		ipw_rebuild_decrypted_skb(priv, rxb->skb);
//
//	if (!libipw_rx(priv->ieee, rxb->skb, stats))
//		dev->stats.rx_errors++;
//	else {			/* libipw_rx succeeded, so it now owns the SKB */
//		rxb->skb = NULL;
//		__ipw_led_activity_on(priv);
//	}
//}
//
//#ifdef CONFIG_IPW2200_RADIOTAP
//static void ipw_handle_data_packet_monitor(struct ipw_priv *priv,
//					   struct ipw_rx_mem_buffer *rxb,
//					   struct libipw_rx_stats *stats)
//{
//	struct net_device *dev = priv->net_dev;
//	struct ipw_rx_packet *pkt = (struct ipw_rx_packet *)rxb->skb->data;
//	struct ipw_rx_frame *frame = &pkt->u.frame;
//
//	/* initial pull of some data */
//	u16 received_channel = frame->received_channel;
//	u8 antennaAndPhy = frame->antennaAndPhy;
//	s8 antsignal = frame->rssi_dbm - IPW_RSSI_TO_DBM;	/* call it signed anyhow */
//	u16 pktrate = frame->rate;
//
//	/* Magic struct that slots into the radiotap header -- no reason
//	 * to build this manually element by element, we can write it much
//	 * more efficiently than we can parse it. ORDER MATTERS HERE */
//	struct ipw_rt_hdr *ipw_rt;
//
//	short len = le16_to_cpu(pkt->u.frame.length);
//
//	/* We received data from the HW, so stop the watchdog */
//	dev->trans_start = jiffies;
//
//	/* We only process data packets if the
//	 * interface is open */
//	if (unlikely((le16_to_cpu(pkt->u.frame.length) + IPW_RX_FRAME_SIZE) >
//		     skb_tailroom(rxb->skb))) {
//		dev->stats.rx_errors++;
//		priv->wstats.discard.misc++;
//		IPW_DEBUG_DROP("Corruption detected! Oh no!\n");
//		return;
//	} else if (unlikely(!netif_running(priv->net_dev))) {
//		dev->stats.rx_dropped++;
//		priv->wstats.discard.misc++;
//		IPW_DEBUG_DROP("Dropping packet while interface is not up.\n");
//		return;
//	}
//
//	/* Libpcap 0.9.3+ can handle variable length radiotap, so we'll use
//	 * that now */
//	if (len > IPW_RX_BUF_SIZE - sizeof(struct ipw_rt_hdr)) {
//		/* FIXME: Should alloc bigger skb instead */
//		dev->stats.rx_dropped++;
//		priv->wstats.discard.misc++;
//		IPW_DEBUG_DROP("Dropping too large packet in monitor\n");
//		return;
//	}
//
//	/* copy the frame itself */
//	memmove(rxb->skb->data + sizeof(struct ipw_rt_hdr),
//		rxb->skb->data + IPW_RX_FRAME_SIZE, len);
//
//	ipw_rt = (struct ipw_rt_hdr *)rxb->skb->data;
//
//	ipw_rt->rt_hdr.it_version = PKTHDR_RADIOTAP_VERSION;
//	ipw_rt->rt_hdr.it_pad = 0;	/* always good to zero */
//	ipw_rt->rt_hdr.it_len = cpu_to_le16(sizeof(struct ipw_rt_hdr));	/* total header+data */
//
//	/* Big bitfield of all the fields we provide in radiotap */
//	ipw_rt->rt_hdr.it_present = cpu_to_le32(
//	     (1 << IEEE80211_RADIOTAP_TSFT) |
//	     (1 << IEEE80211_RADIOTAP_FLAGS) |
//	     (1 << IEEE80211_RADIOTAP_RATE) |
//	     (1 << IEEE80211_RADIOTAP_CHANNEL) |
//	     (1 << IEEE80211_RADIOTAP_DBM_ANTSIGNAL) |
//	     (1 << IEEE80211_RADIOTAP_DBM_ANTNOISE) |
//	     (1 << IEEE80211_RADIOTAP_ANTENNA));
//
//	/* Zero the flags, we'll add to them as we go */
//	ipw_rt->rt_flags = 0;
//	ipw_rt->rt_tsf = (u64)(frame->parent_tsf[3] << 24 |
//			       frame->parent_tsf[2] << 16 |
//			       frame->parent_tsf[1] << 8  |
//			       frame->parent_tsf[0]);
//
//	/* Convert signal to DBM */
//	ipw_rt->rt_dbmsignal = antsignal;
//	ipw_rt->rt_dbmnoise = (s8) le16_to_cpu(frame->noise);
//
//	/* Convert the channel data and set the flags */
//	ipw_rt->rt_channel = cpu_to_le16(ieee80211chan2mhz(received_channel));
//	if (received_channel > 14) {	/* 802.11a */
//		ipw_rt->rt_chbitmask =
//		    cpu_to_le16((IEEE80211_CHAN_OFDM | IEEE80211_CHAN_5GHZ));
//	} else if (antennaAndPhy & 32) {	/* 802.11b */
//		ipw_rt->rt_chbitmask =
//		    cpu_to_le16((IEEE80211_CHAN_CCK | IEEE80211_CHAN_2GHZ));
//	} else {		/* 802.11g */
//		ipw_rt->rt_chbitmask =
//		    cpu_to_le16(IEEE80211_CHAN_OFDM | IEEE80211_CHAN_2GHZ);
//	}
//
//	/* set the rate in multiples of 500k/s */
//	switch (pktrate) {
//	case IPW_TX_RATE_1MB:
//		ipw_rt->rt_rate = 2;
//		break;
//	case IPW_TX_RATE_2MB:
//		ipw_rt->rt_rate = 4;
//		break;
//	case IPW_TX_RATE_5MB:
//		ipw_rt->rt_rate = 10;
//		break;
//	case IPW_TX_RATE_6MB:
//		ipw_rt->rt_rate = 12;
//		break;
//	case IPW_TX_RATE_9MB:
//		ipw_rt->rt_rate = 18;
//		break;
//	case IPW_TX_RATE_11MB:
//		ipw_rt->rt_rate = 22;
//		break;
//	case IPW_TX_RATE_12MB:
//		ipw_rt->rt_rate = 24;
//		break;
//	case IPW_TX_RATE_18MB:
//		ipw_rt->rt_rate = 36;
//		break;
//	case IPW_TX_RATE_24MB:
//		ipw_rt->rt_rate = 48;
//		break;
//	case IPW_TX_RATE_36MB:
//		ipw_rt->rt_rate = 72;
//		break;
//	case IPW_TX_RATE_48MB:
//		ipw_rt->rt_rate = 96;
//		break;
//	case IPW_TX_RATE_54MB:
//		ipw_rt->rt_rate = 108;
//		break;
//	default:
//		ipw_rt->rt_rate = 0;
//		break;
//	}
//
//	/* antenna number */
//	ipw_rt->rt_antenna = (antennaAndPhy & 3);	/* Is this right? */
//
//	/* set the preamble flag if we have it */
//	if ((antennaAndPhy & 64))
//		ipw_rt->rt_flags |= IEEE80211_RADIOTAP_F_SHORTPRE;
//
//	/* Set the size of the skb to the size of the frame */
//	skb_put(rxb->skb, len + sizeof(struct ipw_rt_hdr));
//
//	IPW_DEBUG_RX("Rx packet of %d bytes.\n", rxb->skb->len);
//
//	if (!libipw_rx(priv->ieee, rxb->skb, stats))
//		dev->stats.rx_errors++;
//	else {			/* libipw_rx succeeded, so it now owns the SKB */
//		rxb->skb = NULL;
//		/* no LED during capture */
//	}
//}
//#endif
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//#define libipw_is_probe_response(fc) \
//   ((fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_MGMT && \
//    (fc & IEEE80211_FCTL_STYPE) == IEEE80211_STYPE_PROBE_RESP )
//
//#define libipw_is_management(fc) \
//   ((fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_MGMT)
//
//#define libipw_is_control(fc) \
//   ((fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_CTL)
//
//#define libipw_is_data(fc) \
//   ((fc & IEEE80211_FCTL_FTYPE) == IEEE80211_FTYPE_DATA)
//
//#define libipw_is_assoc_request(fc) \
//   ((fc & IEEE80211_FCTL_STYPE) == IEEE80211_STYPE_ASSOC_REQ)
//
//#define libipw_is_reassoc_request(fc) \
//   ((fc & IEEE80211_FCTL_STYPE) == IEEE80211_STYPE_REASSOC_REQ)
//
//static void ipw_handle_promiscuous_rx(struct ipw_priv *priv,
//				      struct ipw_rx_mem_buffer *rxb,
//				      struct libipw_rx_stats *stats)
//{
//	struct net_device *dev = priv->prom_net_dev;
//	struct ipw_rx_packet *pkt = (struct ipw_rx_packet *)rxb->skb->data;
//	struct ipw_rx_frame *frame = &pkt->u.frame;
//	struct ipw_rt_hdr *ipw_rt;
//
//	/* First cache any information we need before we overwrite
//	 * the information provided in the skb from the hardware */
//	struct ieee80211_hdr *hdr;
//	u16 channel = frame->received_channel;
//	u8 phy_flags = frame->antennaAndPhy;
//	s8 signal = frame->rssi_dbm - IPW_RSSI_TO_DBM;
//	s8 noise = (s8) le16_to_cpu(frame->noise);
//	u8 rate = frame->rate;
//	short len = le16_to_cpu(pkt->u.frame.length);
//	struct sk_buff *skb;
//	int hdr_only = 0;
//	u16 filter = priv->prom_priv->filter;
//
//	/* If the filter is set to not include Rx frames then return */
//	if (filter & IPW_PROM_NO_RX)
//		return;
//
//	/* We received data from the HW, so stop the watchdog */
//	dev->trans_start = jiffies;
//
//	if (unlikely((len + IPW_RX_FRAME_SIZE) > skb_tailroom(rxb->skb))) {
//		dev->stats.rx_errors++;
//		IPW_DEBUG_DROP("Corruption detected! Oh no!\n");
//		return;
//	}
//
//	/* We only process data packets if the interface is open */
//	if (unlikely(!netif_running(dev))) {
//		dev->stats.rx_dropped++;
//		IPW_DEBUG_DROP("Dropping packet while interface is not up.\n");
//		return;
//	}
//
//	/* Libpcap 0.9.3+ can handle variable length radiotap, so we'll use
//	 * that now */
//	if (len > IPW_RX_BUF_SIZE - sizeof(struct ipw_rt_hdr)) {
//		/* FIXME: Should alloc bigger skb instead */
//		dev->stats.rx_dropped++;
//		IPW_DEBUG_DROP("Dropping too large packet in monitor\n");
//		return;
//	}
//
//	hdr = (void *)rxb->skb->data + IPW_RX_FRAME_SIZE;
//	if (libipw_is_management(le16_to_cpu(hdr->frame_control))) {
//		if (filter & IPW_PROM_NO_MGMT)
//			return;
//		if (filter & IPW_PROM_MGMT_HEADER_ONLY)
//			hdr_only = 1;
//	} else if (libipw_is_control(le16_to_cpu(hdr->frame_control))) {
//		if (filter & IPW_PROM_NO_CTL)
//			return;
//		if (filter & IPW_PROM_CTL_HEADER_ONLY)
//			hdr_only = 1;
//	} else if (libipw_is_data(le16_to_cpu(hdr->frame_control))) {
//		if (filter & IPW_PROM_NO_DATA)
//			return;
//		if (filter & IPW_PROM_DATA_HEADER_ONLY)
//			hdr_only = 1;
//	}
//
//	/* Copy the SKB since this is for the promiscuous side */
//	skb = skb_copy(rxb->skb, GFP_ATOMIC);
//	if (skb == NULL) {
//		IPW_ERROR("skb_clone failed for promiscuous copy.\n");
//		return;
//	}
//
//	/* copy the frame data to write after where the radiotap header goes */
//	ipw_rt = (void *)skb->data;
//
//	if (hdr_only)
//		len = libipw_get_hdrlen(le16_to_cpu(hdr->frame_control));
//
//	memcpy(ipw_rt->payload, hdr, len);
//
//	ipw_rt->rt_hdr.it_version = PKTHDR_RADIOTAP_VERSION;
//	ipw_rt->rt_hdr.it_pad = 0;	/* always good to zero */
//	ipw_rt->rt_hdr.it_len = cpu_to_le16(sizeof(*ipw_rt));	/* total header+data */
//
//	/* Set the size of the skb to the size of the frame */
//	skb_put(skb, sizeof(*ipw_rt) + len);
//
//	/* Big bitfield of all the fields we provide in radiotap */
//	ipw_rt->rt_hdr.it_present = cpu_to_le32(
//	     (1 << IEEE80211_RADIOTAP_TSFT) |
//	     (1 << IEEE80211_RADIOTAP_FLAGS) |
//	     (1 << IEEE80211_RADIOTAP_RATE) |
//	     (1 << IEEE80211_RADIOTAP_CHANNEL) |
//	     (1 << IEEE80211_RADIOTAP_DBM_ANTSIGNAL) |
//	     (1 << IEEE80211_RADIOTAP_DBM_ANTNOISE) |
//	     (1 << IEEE80211_RADIOTAP_ANTENNA));
//
//	/* Zero the flags, we'll add to them as we go */
//	ipw_rt->rt_flags = 0;
//	ipw_rt->rt_tsf = (u64)(frame->parent_tsf[3] << 24 |
//			       frame->parent_tsf[2] << 16 |
//			       frame->parent_tsf[1] << 8  |
//			       frame->parent_tsf[0]);
//
//	/* Convert to DBM */
//	ipw_rt->rt_dbmsignal = signal;
//	ipw_rt->rt_dbmnoise = noise;
//
//	/* Convert the channel data and set the flags */
//	ipw_rt->rt_channel = cpu_to_le16(ieee80211chan2mhz(channel));
//	if (channel > 14) {	/* 802.11a */
//		ipw_rt->rt_chbitmask =
//		    cpu_to_le16((IEEE80211_CHAN_OFDM | IEEE80211_CHAN_5GHZ));
//	} else if (phy_flags & (1 << 5)) {	/* 802.11b */
//		ipw_rt->rt_chbitmask =
//		    cpu_to_le16((IEEE80211_CHAN_CCK | IEEE80211_CHAN_2GHZ));
//	} else {		/* 802.11g */
//		ipw_rt->rt_chbitmask =
//		    cpu_to_le16(IEEE80211_CHAN_OFDM | IEEE80211_CHAN_2GHZ);
//	}
//
//	/* set the rate in multiples of 500k/s */
//	switch (rate) {
//	case IPW_TX_RATE_1MB:
//		ipw_rt->rt_rate = 2;
//		break;
//	case IPW_TX_RATE_2MB:
//		ipw_rt->rt_rate = 4;
//		break;
//	case IPW_TX_RATE_5MB:
//		ipw_rt->rt_rate = 10;
//		break;
//	case IPW_TX_RATE_6MB:
//		ipw_rt->rt_rate = 12;
//		break;
//	case IPW_TX_RATE_9MB:
//		ipw_rt->rt_rate = 18;
//		break;
//	case IPW_TX_RATE_11MB:
//		ipw_rt->rt_rate = 22;
//		break;
//	case IPW_TX_RATE_12MB:
//		ipw_rt->rt_rate = 24;
//		break;
//	case IPW_TX_RATE_18MB:
//		ipw_rt->rt_rate = 36;
//		break;
//	case IPW_TX_RATE_24MB:
//		ipw_rt->rt_rate = 48;
//		break;
//	case IPW_TX_RATE_36MB:
//		ipw_rt->rt_rate = 72;
//		break;
//	case IPW_TX_RATE_48MB:
//		ipw_rt->rt_rate = 96;
//		break;
//	case IPW_TX_RATE_54MB:
//		ipw_rt->rt_rate = 108;
//		break;
//	default:
//		ipw_rt->rt_rate = 0;
//		break;
//	}
//
//	/* antenna number */
//	ipw_rt->rt_antenna = (phy_flags & 3);
//
//	/* set the preamble flag if we have it */
//	if (phy_flags & (1 << 6))
//		ipw_rt->rt_flags |= IEEE80211_RADIOTAP_F_SHORTPRE;
//
//	IPW_DEBUG_RX("Rx packet of %d bytes.\n", skb->len);
//
//	if (!libipw_rx(priv->prom_priv->ieee, skb, stats)) {
//		dev->stats.rx_errors++;
//		dev_kfree_skb_any(skb);
//	}
//}
//#endif
//
//static int is_network_packet(struct ipw_priv *priv,
//				    struct libipw_hdr_4addr *header)
//{
//	/* Filter incoming packets to determine if they are targetted toward
//	 * this network, discarding packets coming from ourselves */
//	switch (priv->ieee->iw_mode) {
//	case IW_MODE_ADHOC:	/* Header: Dest. | Source    | BSSID */
//		/* packets from our adapter are dropped (echo) */
//		if (!memcmp(header->addr2, priv->net_dev->dev_addr, ETH_ALEN))
//			return 0;
//
//		/* {broad,multi}cast packets to our BSSID go through */
//		if (is_multicast_ether_addr(header->addr1))
//			return !memcmp(header->addr3, priv->bssid, ETH_ALEN);
//
//		/* packets to our adapter go through */
//		return !memcmp(header->addr1, priv->net_dev->dev_addr,
//			       ETH_ALEN);
//
//	case IW_MODE_INFRA:	/* Header: Dest. | BSSID | Source */
//		/* packets from our adapter are dropped (echo) */
//		if (!memcmp(header->addr3, priv->net_dev->dev_addr, ETH_ALEN))
//			return 0;
//
//		/* {broad,multi}cast packets to our BSS go through */
//		if (is_multicast_ether_addr(header->addr1))
//			return !memcmp(header->addr2, priv->bssid, ETH_ALEN);
//
//		/* packets to our adapter go through */
//		return !memcmp(header->addr1, priv->net_dev->dev_addr,
//			       ETH_ALEN);
//	}
//
//	return 1;
//}
//
//#define IPW_PACKET_RETRY_TIME HZ
//
//static  int is_duplicate_packet(struct ipw_priv *priv,
//				      struct libipw_hdr_4addr *header)
//{
//	u16 sc = le16_to_cpu(header->seq_ctl);
//	u16 seq = WLAN_GET_SEQ_SEQ(sc);
//	u16 frag = WLAN_GET_SEQ_FRAG(sc);
//	u16 *last_seq, *last_frag;
//	unsigned long *last_time;
//
//	switch (priv->ieee->iw_mode) {
//	case IW_MODE_ADHOC:
//		{
//			struct list_head *p;
//			struct ipw_ibss_seq *entry = NULL;
//			u8 *mac = header->addr2;
//			int index = mac[5] % IPW_IBSS_MAC_HASH_SIZE;
//
//			__list_for_each(p, &priv->ibss_mac_hash[index]) {
//				entry =
//				    list_entry(p, struct ipw_ibss_seq, list);
//				if (!memcmp(entry->mac, mac, ETH_ALEN))
//					break;
//			}
//			if (p == &priv->ibss_mac_hash[index]) {
//				entry = kmalloc(sizeof(*entry), GFP_ATOMIC);
//				if (!entry) {
//					IPW_ERROR
//					    ("Cannot malloc new mac entry\n");
//					return 0;
//				}
//				memcpy(entry->mac, mac, ETH_ALEN);
//				entry->seq_num = seq;
//				entry->frag_num = frag;
//				entry->packet_time = jiffies;
//				list_add(&entry->list,
//					 &priv->ibss_mac_hash[index]);
//				return 0;
//			}
//			last_seq = &entry->seq_num;
//			last_frag = &entry->frag_num;
//			last_time = &entry->packet_time;
//			break;
//		}
//	case IW_MODE_INFRA:
//		last_seq = &priv->last_seq_num;
//		last_frag = &priv->last_frag_num;
//		last_time = &priv->last_packet_time;
//		break;
//	default:
//		return 0;
//	}
//	if ((*last_seq == seq) &&
//	    time_after(*last_time + IPW_PACKET_RETRY_TIME, jiffies)) {
//		if (*last_frag == frag)
//			goto drop;
//		if (*last_frag + 1 != frag)
//			/* out-of-order fragment */
//			goto drop;
//	} else
//		*last_seq = seq;
//
//	*last_frag = frag;
//	*last_time = jiffies;
//	return 0;
//
//      drop:
//	/* Comment this line now since we observed the card receives
//	 * duplicate packets but the FCTL_RETRY bit is not set in the
//	 * IBSS mode with fragmentation enabled.
//	 BUG_ON(!(le16_to_cpu(header->frame_control) & IEEE80211_FCTL_RETRY)); */
//	return 1;
//}
//
//static void ipw_handle_mgmt_packet(struct ipw_priv *priv,
//				   struct ipw_rx_mem_buffer *rxb,
//				   struct libipw_rx_stats *stats)
//{
//	struct sk_buff *skb = rxb->skb;
//	struct ipw_rx_packet *pkt = (struct ipw_rx_packet *)skb->data;
//	struct libipw_hdr_4addr *header = (struct libipw_hdr_4addr *)
//	    (skb->data + IPW_RX_FRAME_SIZE);
//
//	libipw_rx_mgt(priv->ieee, header, stats);
//
//	if (priv->ieee->iw_mode == IW_MODE_ADHOC &&
//	    ((WLAN_FC_GET_STYPE(le16_to_cpu(header->frame_ctl)) ==
//	      IEEE80211_STYPE_PROBE_RESP) ||
//	     (WLAN_FC_GET_STYPE(le16_to_cpu(header->frame_ctl)) ==
//	      IEEE80211_STYPE_BEACON))) {
//		if (!memcmp(header->addr3, priv->bssid, ETH_ALEN))
//			ipw_add_station(priv, header->addr2);
//	}
//
//	if (priv->config & CFG_NET_STATS) {
//		IPW_DEBUG_HC("sending stat packet\n");
//
//		/* Set the size of the skb to the size of the full
//		 * ipw header and 802.11 frame */
//		skb_put(skb, le16_to_cpu(pkt->u.frame.length) +
//			IPW_RX_FRAME_SIZE);
//
//		/* Advance past the ipw packet header to the 802.11 frame */
//		skb_pull(skb, IPW_RX_FRAME_SIZE);
//
//		/* Push the libipw_rx_stats before the 802.11 frame */
//		memcpy(skb_push(skb, sizeof(*stats)), stats, sizeof(*stats));
//
//		skb->dev = priv->ieee->dev;
//
//		/* Point raw at the libipw_stats */
//		skb_reset_mac_header(skb);
//
//		skb->pkt_type = PACKET_OTHERHOST;
//		skb->protocol = cpu_to_be16(ETH_P_80211_STATS);
//		memset(skb->cb, 0, sizeof(rxb->skb->cb));
//		netif_rx(skb);
//		rxb->skb = NULL;
//	}
//}
//
///*
// * Main entry function for recieving a packet with 80211 headers.  This
// * should be called when ever the FW has notified us that there is a new
// * skb in the recieve queue.
// */
//static void ipw_rx(struct ipw_priv *priv)
//{
//	struct ipw_rx_mem_buffer *rxb;
//	struct ipw_rx_packet *pkt;
//	struct libipw_hdr_4addr *header;
//	u32 r, w, i;
//	u8 network_packet;
//	u8 fill_rx = 0;
//
//	r = ipw_read32(priv, IPW_RX_READ_INDEX);
//	w = ipw_read32(priv, IPW_RX_WRITE_INDEX);
//	i = priv->rxq->read;
//
//	if (ipw_rx_queue_space (priv->rxq) > (RX_QUEUE_SIZE / 2))
//		fill_rx = 1;
//
//	while (i != r) {
//		rxb = priv->rxq->queue[i];
//		if (unlikely(rxb == NULL)) {
//			printk(KERN_CRIT "Queue not allocated!\n");
//			break;
//		}
//		priv->rxq->queue[i] = NULL;
//
//		pci_dma_sync_single_for_cpu(priv->pci_dev, rxb->dma_addr,
//					    IPW_RX_BUF_SIZE,
//					    PCI_DMA_FROMDEVICE);
//
//		pkt = (struct ipw_rx_packet *)rxb->skb->data;
//		IPW_DEBUG_RX("Packet: type=%02X seq=%02X bits=%02X\n",
//			     pkt->header.message_type,
//			     pkt->header.rx_seq_num, pkt->header.control_bits);
//
//		switch (pkt->header.message_type) {
//		case RX_FRAME_TYPE:	/* 802.11 frame */  {
//				struct libipw_rx_stats stats = {
//					.rssi = pkt->u.frame.rssi_dbm -
//					    IPW_RSSI_TO_DBM,
//					.signal =
//					    pkt->u.frame.rssi_dbm -
//					    IPW_RSSI_TO_DBM + 0x100,
//					.noise =
//					    le16_to_cpu(pkt->u.frame.noise),
//					.rate = pkt->u.frame.rate,
//					.mac_time = jiffies,
//					.received_channel =
//					    pkt->u.frame.received_channel,
//					.freq =
//					    (pkt->u.frame.
//					     control & (1 << 0)) ?
//					    LIBIPW_24GHZ_BAND :
//					    LIBIPW_52GHZ_BAND,
//					.len = le16_to_cpu(pkt->u.frame.length),
//				};
//
//				if (stats.rssi != 0)
//					stats.mask |= LIBIPW_STATMASK_RSSI;
//				if (stats.signal != 0)
//					stats.mask |= LIBIPW_STATMASK_SIGNAL;
//				if (stats.noise != 0)
//					stats.mask |= LIBIPW_STATMASK_NOISE;
//				if (stats.rate != 0)
//					stats.mask |= LIBIPW_STATMASK_RATE;
//
//				priv->rx_packets++;
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//	if (priv->prom_net_dev && netif_running(priv->prom_net_dev))
//		ipw_handle_promiscuous_rx(priv, rxb, &stats);
//#endif
//
//#ifdef CONFIG_IPW2200_MONITOR
//				if (priv->ieee->iw_mode == IW_MODE_MONITOR) {
//#ifdef CONFIG_IPW2200_RADIOTAP
//
//                ipw_handle_data_packet_monitor(priv,
//					       rxb,
//					       &stats);
//#else
//		ipw_handle_data_packet(priv, rxb,
//				       &stats);
//#endif
//					break;
//				}
//#endif
//
//				header =
//				    (struct libipw_hdr_4addr *)(rxb->skb->
//								   data +
//								   IPW_RX_FRAME_SIZE);
//				/* TODO: Check Ad-Hoc dest/source and make sure
//				 * that we are actually parsing these packets
//				 * correctly -- we should probably use the
//				 * frame control of the packet and disregard
//				 * the current iw_mode */
//
//				network_packet =
//				    is_network_packet(priv, header);
//				if (network_packet && priv->assoc_network) {
//					priv->assoc_network->stats.rssi =
//					    stats.rssi;
//					priv->exp_avg_rssi =
//					    exponential_average(priv->exp_avg_rssi,
//					    stats.rssi, DEPTH_RSSI);
//				}
//
//				IPW_DEBUG_RX("Frame: len=%u\n",
//					     le16_to_cpu(pkt->u.frame.length));
//
//				if (le16_to_cpu(pkt->u.frame.length) <
//				    libipw_get_hdrlen(le16_to_cpu(
//						    header->frame_ctl))) {
//					IPW_DEBUG_DROP
//					    ("Received packet is too small. "
//					     "Dropping.\n");
//					priv->net_dev->stats.rx_errors++;
//					priv->wstats.discard.misc++;
//					break;
//				}
//
//				switch (WLAN_FC_GET_TYPE
//					(le16_to_cpu(header->frame_ctl))) {
//
//				case IEEE80211_FTYPE_MGMT:
//					ipw_handle_mgmt_packet(priv, rxb,
//							       &stats);
//					break;
//
//				case IEEE80211_FTYPE_CTL:
//					break;
//
//				case IEEE80211_FTYPE_DATA:
//					if (unlikely(!network_packet ||
//						     is_duplicate_packet(priv,
//									 header)))
//					{
//						IPW_DEBUG_DROP("Dropping: "
//							       "%pM, "
//							       "%pM, "
//							       "%pM\n",
//							       header->addr1,
//							       header->addr2,
//							       header->addr3);
//						break;
//					}
//
//					ipw_handle_data_packet(priv, rxb,
//							       &stats);
//
//					break;
//				}
//				break;
//			}
//
//		case RX_HOST_NOTIFICATION_TYPE:{
//				IPW_DEBUG_RX
//				    ("Notification: subtype=%02X flags=%02X size=%d\n",
//				     pkt->u.notification.subtype,
//				     pkt->u.notification.flags,
//				     le16_to_cpu(pkt->u.notification.size));
//				ipw_rx_notification(priv, &pkt->u.notification);
//				break;
//			}
//
//		default:
//			IPW_DEBUG_RX("Bad Rx packet of type %d\n",
//				     pkt->header.message_type);
//			break;
//		}
//
//		/* For now we just don't re-use anything.  We can tweak this
//		 * later to try and re-use notification packets and SKBs that
//		 * fail to Rx correctly */
//		if (rxb->skb != NULL) {
//			dev_kfree_skb_any(rxb->skb);
//			rxb->skb = NULL;
//		}
//
//		pci_unmap_single(priv->pci_dev, rxb->dma_addr,
//				 IPW_RX_BUF_SIZE, PCI_DMA_FROMDEVICE);
//		list_add_tail(&rxb->list, &priv->rxq->rx_used);
//
//		i = (i + 1) % RX_QUEUE_SIZE;
//
//		/* If there are a lot of unsued frames, restock the Rx queue
//		 * so the ucode won't assert */
//		if (fill_rx) {
//			priv->rxq->read = i;
//			ipw_rx_queue_replenish(priv);
//		}
//	}
//
//	/* Backtrack one entry */
//	priv->rxq->read = i;
//	ipw_rx_queue_restock(priv);
//}
//
//#define DEFAULT_RTS_THRESHOLD     2304U
//#define MIN_RTS_THRESHOLD         1U
//#define MAX_RTS_THRESHOLD         2304U
//#define DEFAULT_BEACON_INTERVAL   100U
//#define	DEFAULT_SHORT_RETRY_LIMIT 7U
//#define	DEFAULT_LONG_RETRY_LIMIT  4U
//
///**
// * ipw_sw_reset
// * @option: options to control different reset behaviour
// * 	    0 = reset everything except the 'disable' module_param
// * 	    1 = reset everything and print out driver info (for probe only)
// * 	    2 = reset everything
// */
//static int ipw_sw_reset(struct ipw_priv *priv, int option)
//{
//	int band, modulation;
//	int old_mode = priv->ieee->iw_mode;
//
//	/* Initialize module parameter values here */
//	priv->config = 0;
//
//	/* We default to disabling the LED code as right now it causes
//	 * too many systems to lock up... */
//	if (!led_support)
//		priv->config |= CFG_NO_LED;
//
//	if (associate)
//		priv->config |= CFG_ASSOCIATE;
//	else
//		IPW_DEBUG_INFO("Auto associate disabled.\n");
//
//	if (auto_create)
//		priv->config |= CFG_ADHOC_CREATE;
//	else
//		IPW_DEBUG_INFO("Auto adhoc creation disabled.\n");
//
//	priv->config &= ~CFG_STATIC_ESSID;
//	priv->essid_len = 0;
//	memset(priv->essid, 0, IW_ESSID_MAX_SIZE);
//
//	if (disable && option) {
//		priv->status |= STATUS_RF_KILL_SW;
//		IPW_DEBUG_INFO("Radio disabled.\n");
//	}
//
//	if (default_channel != 0) {
//		priv->config |= CFG_STATIC_CHANNEL;
//		priv->channel = default_channel;
//		IPW_DEBUG_INFO("Bind to static channel %d\n", default_channel);
//		/* TODO: Validate that provided channel is in range */
//	}
//#ifdef CONFIG_IPW2200_QOS
//	ipw_qos_init(priv, qos_enable, qos_burst_enable,
//		     burst_duration_CCK, burst_duration_OFDM);
//#endif				/* CONFIG_IPW2200_QOS */
//
//	switch (network_mode) {
//	case 1:
//		priv->ieee->iw_mode = IW_MODE_ADHOC;
//		priv->net_dev->type = ARPHRD_ETHER;
//
//		break;
//#ifdef CONFIG_IPW2200_MONITOR
//	case 2:
//		priv->ieee->iw_mode = IW_MODE_MONITOR;
//#ifdef CONFIG_IPW2200_RADIOTAP
//		priv->net_dev->type = ARPHRD_IEEE80211_RADIOTAP;
//#else
//		priv->net_dev->type = ARPHRD_IEEE80211;
//#endif
//		break;
//#endif
//	default:
//	case 0:
//		priv->net_dev->type = ARPHRD_ETHER;
//		priv->ieee->iw_mode = IW_MODE_INFRA;
//		break;
//	}
//
//	if (hwcrypto) {
//		priv->ieee->host_encrypt = 0;
//		priv->ieee->host_encrypt_msdu = 0;
//		priv->ieee->host_decrypt = 0;
//		priv->ieee->host_mc_decrypt = 0;
//	}
//	IPW_DEBUG_INFO("Hardware crypto [%s]\n", hwcrypto ? "on" : "off");
//
//	/* IPW2200/2915 is abled to do hardware fragmentation. */
//	priv->ieee->host_open_frag = 0;
//
//	if ((priv->pci_dev->device == 0x4223) ||
//	    (priv->pci_dev->device == 0x4224)) {
//		if (option == 1)
//			printk(KERN_INFO DRV_NAME
//			       ": Detected Intel PRO/Wireless 2915ABG Network "
//			       "Connection\n");
//		priv->ieee->abg_true = 1;
//		band = LIBIPW_52GHZ_BAND | LIBIPW_24GHZ_BAND;
//		modulation = LIBIPW_OFDM_MODULATION |
//		    LIBIPW_CCK_MODULATION;
//		priv->adapter = IPW_2915ABG;
//		priv->ieee->mode = IEEE_A | IEEE_G | IEEE_B;
//	} else {
//		if (option == 1)
//			printk(KERN_INFO DRV_NAME
//			       ": Detected Intel PRO/Wireless 2200BG Network "
//			       "Connection\n");
//
//		priv->ieee->abg_true = 0;
//		band = LIBIPW_24GHZ_BAND;
//		modulation = LIBIPW_OFDM_MODULATION |
//		    LIBIPW_CCK_MODULATION;
//		priv->adapter = IPW_2200BG;
//		priv->ieee->mode = IEEE_G | IEEE_B;
//	}
//
//	priv->ieee->freq_band = band;
//	priv->ieee->modulation = modulation;
//
//	priv->rates_mask = LIBIPW_DEFAULT_RATES_MASK;
//
//	priv->disassociate_threshold = IPW_MB_DISASSOCIATE_THRESHOLD_DEFAULT;
//	priv->roaming_threshold = IPW_MB_ROAMING_THRESHOLD_DEFAULT;
//
//	priv->rts_threshold = DEFAULT_RTS_THRESHOLD;
//	priv->short_retry_limit = DEFAULT_SHORT_RETRY_LIMIT;
//	priv->long_retry_limit = DEFAULT_LONG_RETRY_LIMIT;
//
//	/* If power management is turned on, default to AC mode */
//	priv->power_mode = IPW_POWER_AC;
//	priv->tx_power = IPW_TX_POWER_DEFAULT;
//
//	return old_mode == priv->ieee->iw_mode;
//}
//
///*
// * This file defines the Wireless Extension handlers.  It does not
// * define any methods of hardware manipulation and relies on the
// * functions defined in ipw_main to provide the HW interaction.
// *
// * The exception to this is the use of the ipw_get_ordinal()
// * function used to poll the hardware vs. making unecessary calls.
// *
// */
//
//static int ipw_wx_get_name(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	if (priv->status & STATUS_RF_KILL_MASK)
//		strcpy(wrqu->name, "radio off");
//	else if (!(priv->status & STATUS_ASSOCIATED))
//		strcpy(wrqu->name, "unassociated");
//	else
//		snprintf(wrqu->name, IFNAMSIZ, "IEEE 802.11%c",
//			 ipw_modes[priv->assoc_request.ieee_mode]);
//	IPW_DEBUG_WX("Name: %s\n", wrqu->name);
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_set_channel(struct ipw_priv *priv, u8 channel)
//{
//	if (channel == 0) {
//		IPW_DEBUG_INFO("Setting channel to ANY (0)\n");
//		priv->config &= ~CFG_STATIC_CHANNEL;
//		IPW_DEBUG_ASSOC("Attempting to associate with new "
//				"parameters.\n");
//		ipw_associate(priv);
//		return 0;
//	}
//
//	priv->config |= CFG_STATIC_CHANNEL;
//
//	if (priv->channel == channel) {
//		IPW_DEBUG_INFO("Request to set channel to current value (%d)\n",
//			       channel);
//		return 0;
//	}
//
//	IPW_DEBUG_INFO("Setting channel to %i\n", (int)channel);
//	priv->channel = channel;
//
//#ifdef CONFIG_IPW2200_MONITOR
//	if (priv->ieee->iw_mode == IW_MODE_MONITOR) {
//		int i;
//		if (priv->status & STATUS_SCANNING) {
//			IPW_DEBUG_SCAN("Scan abort triggered due to "
//				       "channel change.\n");
//			ipw_abort_scan(priv);
//		}
//
//		for (i = 1000; i && (priv->status & STATUS_SCANNING); i--)
//			udelay(10);
//
//		if (priv->status & STATUS_SCANNING)
//			IPW_DEBUG_SCAN("Still scanning...\n");
//		else
//			IPW_DEBUG_SCAN("Took %dms to abort current scan\n",
//				       1000 - i);
//
//		return 0;
//	}
//#endif				/* CONFIG_IPW2200_MONITOR */
//
//	/* Network configuration changed -- force [re]association */
//	IPW_DEBUG_ASSOC("[re]association triggered due to channel change.\n");
//	if (!ipw_disassociate(priv))
//		ipw_associate(priv);
//
//	return 0;
//}
//
//static int ipw_wx_set_freq(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	const struct libipw_geo *geo = libipw_get_geo(priv->ieee);
//	struct iw_freq *fwrq = &wrqu->freq;
//	int ret = 0, i;
//	u8 channel, flags;
//	int band;
//
//	if (fwrq->m == 0) {
//		IPW_DEBUG_WX("SET Freq/Channel -> any\n");
//		mutex_lock(&priv->mutex);
//		ret = ipw_set_channel(priv, 0);
//		mutex_unlock(&priv->mutex);
//		return ret;
//	}
//	/* if setting by freq convert to channel */
//	if (fwrq->e == 1) {
//		channel = libipw_freq_to_channel(priv->ieee, fwrq->m);
//		if (channel == 0)
//			return -EINVAL;
//	} else
//		channel = fwrq->m;
//
//	if (!(band = libipw_is_valid_channel(priv->ieee, channel)))
//		return -EINVAL;
//
//	if (priv->ieee->iw_mode == IW_MODE_ADHOC) {
//		i = libipw_channel_to_index(priv->ieee, channel);
//		if (i == -1)
//			return -EINVAL;
//
//		flags = (band == LIBIPW_24GHZ_BAND) ?
//		    geo->bg[i].flags : geo->a[i].flags;
//		if (flags & LIBIPW_CH_PASSIVE_ONLY) {
//			IPW_DEBUG_WX("Invalid Ad-Hoc channel for 802.11a\n");
//			return -EINVAL;
//		}
//	}
//
//	IPW_DEBUG_WX("SET Freq/Channel -> %d \n", fwrq->m);
//	mutex_lock(&priv->mutex);
//	ret = ipw_set_channel(priv, channel);
//	mutex_unlock(&priv->mutex);
//	return ret;
//}
//
//static int ipw_wx_get_freq(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//
//	wrqu->freq.e = 0;
//
//	/* If we are associated, trying to associate, or have a statically
//	 * configured CHANNEL then return that; otherwise return ANY */
//	mutex_lock(&priv->mutex);
//	if (priv->config & CFG_STATIC_CHANNEL ||
//	    priv->status & (STATUS_ASSOCIATING | STATUS_ASSOCIATED)) {
//		int i;
//
//		i = libipw_channel_to_index(priv->ieee, priv->channel);
//		BUG_ON(i == -1);
//		wrqu->freq.e = 1;
//
//		switch (libipw_is_valid_channel(priv->ieee, priv->channel)) {
//		case LIBIPW_52GHZ_BAND:
//			wrqu->freq.m = priv->ieee->geo.a[i].freq * 100000;
//			break;
//
//		case LIBIPW_24GHZ_BAND:
//			wrqu->freq.m = priv->ieee->geo.bg[i].freq * 100000;
//			break;
//
//		default:
//			BUG();
//		}
//	} else
//		wrqu->freq.m = 0;
//
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("GET Freq/Channel -> %d \n", priv->channel);
//	return 0;
//}
//
//static int ipw_wx_set_mode(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int err = 0;
//
//	IPW_DEBUG_WX("Set MODE: %d\n", wrqu->mode);
//
//	switch (wrqu->mode) {
//#ifdef CONFIG_IPW2200_MONITOR
//	case IW_MODE_MONITOR:
//#endif
//	case IW_MODE_ADHOC:
//	case IW_MODE_INFRA:
//		break;
//	case IW_MODE_AUTO:
//		wrqu->mode = IW_MODE_INFRA;
//		break;
//	default:
//		return -EINVAL;
//	}
//	if (wrqu->mode == priv->ieee->iw_mode)
//		return 0;
//
//	mutex_lock(&priv->mutex);
//
//	ipw_sw_reset(priv, 0);
//
//#ifdef CONFIG_IPW2200_MONITOR
//	if (priv->ieee->iw_mode == IW_MODE_MONITOR)
//		priv->net_dev->type = ARPHRD_ETHER;
//
//	if (wrqu->mode == IW_MODE_MONITOR)
//#ifdef CONFIG_IPW2200_RADIOTAP
//		priv->net_dev->type = ARPHRD_IEEE80211_RADIOTAP;
//#else
//		priv->net_dev->type = ARPHRD_IEEE80211;
//#endif
//#endif				/* CONFIG_IPW2200_MONITOR */
//
//	/* Free the existing firmware and reset the fw_loaded
//	 * flag so ipw_load() will bring in the new firmware */
//	free_firmware();
//
//	priv->ieee->iw_mode = wrqu->mode;
//
//	queue_work(priv->workqueue, &priv->adapter_restart);
//	mutex_unlock(&priv->mutex);
//	return err;
//}
//
//static int ipw_wx_get_mode(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	wrqu->mode = priv->ieee->iw_mode;
//	IPW_DEBUG_WX("Get MODE -> %d\n", wrqu->mode);
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
///* Values are in microsecond */
//static const s32 timeout_duration[] = {
//	350000,
//	250000,
//	75000,
//	37000,
//	25000,
//};
//
//static const s32 period_duration[] = {
//	400000,
//	700000,
//	1000000,
//	1000000,
//	1000000
//};
//
//static int ipw_wx_get_range(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct iw_range *range = (struct iw_range *)extra;
//	const struct libipw_geo *geo = libipw_get_geo(priv->ieee);
//	int i = 0, j;
//
//	wrqu->data.length = sizeof(*range);
//	memset(range, 0, sizeof(*range));
//
//	/* 54Mbs == ~27 Mb/s real (802.11g) */
//	range->throughput = 27 * 1000 * 1000;
//
//	range->max_qual.qual = 100;
//	/* TODO: Find real max RSSI and stick here */
//	range->max_qual.level = 0;
//	range->max_qual.noise = 0;
//	range->max_qual.updated = 7;	/* Updated all three */
//
//	range->avg_qual.qual = 70;
//	/* TODO: Find real 'good' to 'bad' threshol value for RSSI */
//	range->avg_qual.level = 0;	/* FIXME to real average level */
//	range->avg_qual.noise = 0;
//	range->avg_qual.updated = 7;	/* Updated all three */
//	mutex_lock(&priv->mutex);
//	range->num_bitrates = min(priv->rates.num_rates, (u8) IW_MAX_BITRATES);
//
//	for (i = 0; i < range->num_bitrates; i++)
//		range->bitrate[i] = (priv->rates.supported_rates[i] & 0x7F) *
//		    500000;
//
//	range->max_rts = DEFAULT_RTS_THRESHOLD;
//	range->min_frag = MIN_FRAG_THRESHOLD;
//	range->max_frag = MAX_FRAG_THRESHOLD;
//
//	range->encoding_size[0] = 5;
//	range->encoding_size[1] = 13;
//	range->num_encoding_sizes = 2;
//	range->max_encoding_tokens = WEP_KEYS;
//
//	/* Set the Wireless Extension versions */
//	range->we_version_compiled = WIRELESS_EXT;
//	range->we_version_source = 18;
//
//	i = 0;
//	if (priv->ieee->mode & (IEEE_B | IEEE_G)) {
//		for (j = 0; j < geo->bg_channels && i < IW_MAX_FREQUENCIES; j++) {
//			if ((priv->ieee->iw_mode == IW_MODE_ADHOC) &&
//			    (geo->bg[j].flags & LIBIPW_CH_PASSIVE_ONLY))
//				continue;
//
//			range->freq[i].i = geo->bg[j].channel;
//			range->freq[i].m = geo->bg[j].freq * 100000;
//			range->freq[i].e = 1;
//			i++;
//		}
//	}
//
//	if (priv->ieee->mode & IEEE_A) {
//		for (j = 0; j < geo->a_channels && i < IW_MAX_FREQUENCIES; j++) {
//			if ((priv->ieee->iw_mode == IW_MODE_ADHOC) &&
//			    (geo->a[j].flags & LIBIPW_CH_PASSIVE_ONLY))
//				continue;
//
//			range->freq[i].i = geo->a[j].channel;
//			range->freq[i].m = geo->a[j].freq * 100000;
//			range->freq[i].e = 1;
//			i++;
//		}
//	}
//
//	range->num_channels = i;
//	range->num_frequency = i;
//
//	mutex_unlock(&priv->mutex);
//
//	/* Event capability (kernel + driver) */
//	range->event_capa[0] = (IW_EVENT_CAPA_K_0 |
//				IW_EVENT_CAPA_MASK(SIOCGIWTHRSPY) |
//				IW_EVENT_CAPA_MASK(SIOCGIWAP) |
//				IW_EVENT_CAPA_MASK(SIOCGIWSCAN));
//	range->event_capa[1] = IW_EVENT_CAPA_K_1;
//
//	range->enc_capa = IW_ENC_CAPA_WPA | IW_ENC_CAPA_WPA2 |
//		IW_ENC_CAPA_CIPHER_TKIP | IW_ENC_CAPA_CIPHER_CCMP;
//
//	range->scan_capa = IW_SCAN_CAPA_ESSID | IW_SCAN_CAPA_TYPE;
//
//	IPW_DEBUG_WX("GET Range\n");
//	return 0;
//}
//
//static int ipw_wx_set_wap(struct net_device *dev,
//			  struct iw_request_info *info,
//			  union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//
//	static const unsigned char any[] = {
//		0xff, 0xff, 0xff, 0xff, 0xff, 0xff
//	};
//	static const unsigned char off[] = {
//		0x00, 0x00, 0x00, 0x00, 0x00, 0x00
//	};
//
//	if (wrqu->ap_addr.sa_family != ARPHRD_ETHER)
//		return -EINVAL;
//	mutex_lock(&priv->mutex);
//	if (!memcmp(any, wrqu->ap_addr.sa_data, ETH_ALEN) ||
//	    !memcmp(off, wrqu->ap_addr.sa_data, ETH_ALEN)) {
//		/* we disable mandatory BSSID association */
//		IPW_DEBUG_WX("Setting AP BSSID to ANY\n");
//		priv->config &= ~CFG_STATIC_BSSID;
//		IPW_DEBUG_ASSOC("Attempting to associate with new "
//				"parameters.\n");
//		ipw_associate(priv);
//		mutex_unlock(&priv->mutex);
//		return 0;
//	}
//
//	priv->config |= CFG_STATIC_BSSID;
//	if (!memcmp(priv->bssid, wrqu->ap_addr.sa_data, ETH_ALEN)) {
//		IPW_DEBUG_WX("BSSID set to current BSSID.\n");
//		mutex_unlock(&priv->mutex);
//		return 0;
//	}
//
//	IPW_DEBUG_WX("Setting mandatory BSSID to %pM\n",
//		     wrqu->ap_addr.sa_data);
//
//	memcpy(priv->bssid, wrqu->ap_addr.sa_data, ETH_ALEN);
//
//	/* Network configuration changed -- force [re]association */
//	IPW_DEBUG_ASSOC("[re]association triggered due to BSSID change.\n");
//	if (!ipw_disassociate(priv))
//		ipw_associate(priv);
//
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_get_wap(struct net_device *dev,
//			  struct iw_request_info *info,
//			  union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//
//	/* If we are associated, trying to associate, or have a statically
//	 * configured BSSID then return that; otherwise return ANY */
//	mutex_lock(&priv->mutex);
//	if (priv->config & CFG_STATIC_BSSID ||
//	    priv->status & (STATUS_ASSOCIATED | STATUS_ASSOCIATING)) {
//		wrqu->ap_addr.sa_family = ARPHRD_ETHER;
//		memcpy(wrqu->ap_addr.sa_data, priv->bssid, ETH_ALEN);
//	} else
//		memset(wrqu->ap_addr.sa_data, 0, ETH_ALEN);
//
//	IPW_DEBUG_WX("Getting WAP BSSID: %pM\n",
//		     wrqu->ap_addr.sa_data);
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_set_essid(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//        int length;
//	DECLARE_SSID_BUF(ssid);
//
//        mutex_lock(&priv->mutex);
//
//        if (!wrqu->essid.flags)
//        {
//                IPW_DEBUG_WX("Setting ESSID to ANY\n");
//                ipw_disassociate(priv);
//                priv->config &= ~CFG_STATIC_ESSID;
//                ipw_associate(priv);
//                mutex_unlock(&priv->mutex);
//                return 0;
//        }
//
//	length = min((int)wrqu->essid.length, IW_ESSID_MAX_SIZE);
//
//	priv->config |= CFG_STATIC_ESSID;
//
//	if (priv->essid_len == length && !memcmp(priv->essid, extra, length)
//	    && (priv->status & (STATUS_ASSOCIATED | STATUS_ASSOCIATING))) {
//		IPW_DEBUG_WX("ESSID set to current ESSID.\n");
//		mutex_unlock(&priv->mutex);
//		return 0;
//	}
//
//	IPW_DEBUG_WX("Setting ESSID: '%s' (%d)\n",
//		     print_ssid(ssid, extra, length), length);
//
//	priv->essid_len = length;
//	memcpy(priv->essid, extra, priv->essid_len);
//
//	/* Network configuration changed -- force [re]association */
//	IPW_DEBUG_ASSOC("[re]association triggered due to ESSID change.\n");
//	if (!ipw_disassociate(priv))
//		ipw_associate(priv);
//
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_get_essid(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	DECLARE_SSID_BUF(ssid);
//
//	/* If we are associated, trying to associate, or have a statically
//	 * configured ESSID then return that; otherwise return ANY */
//	mutex_lock(&priv->mutex);
//	if (priv->config & CFG_STATIC_ESSID ||
//	    priv->status & (STATUS_ASSOCIATED | STATUS_ASSOCIATING)) {
//		IPW_DEBUG_WX("Getting essid: '%s'\n",
//			     print_ssid(ssid, priv->essid, priv->essid_len));
//		memcpy(extra, priv->essid, priv->essid_len);
//		wrqu->essid.length = priv->essid_len;
//		wrqu->essid.flags = 1;	/* active */
//	} else {
//		IPW_DEBUG_WX("Getting essid: ANY\n");
//		wrqu->essid.length = 0;
//		wrqu->essid.flags = 0;	/* active */
//	}
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_set_nick(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//
//	IPW_DEBUG_WX("Setting nick to '%s'\n", extra);
//	if (wrqu->data.length > IW_ESSID_MAX_SIZE)
//		return -E2BIG;
//	mutex_lock(&priv->mutex);
//	wrqu->data.length = min((size_t) wrqu->data.length, sizeof(priv->nick));
//	memset(priv->nick, 0, sizeof(priv->nick));
//	memcpy(priv->nick, extra, wrqu->data.length);
//	IPW_DEBUG_TRACE("<<\n");
//	mutex_unlock(&priv->mutex);
//	return 0;
//
//}
//
//static int ipw_wx_get_nick(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	IPW_DEBUG_WX("Getting nick\n");
//	mutex_lock(&priv->mutex);
//	wrqu->data.length = strlen(priv->nick);
//	memcpy(extra, priv->nick, wrqu->data.length);
//	wrqu->data.flags = 1;	/* active */
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_set_sens(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int err = 0;
//
//	IPW_DEBUG_WX("Setting roaming threshold to %d\n", wrqu->sens.value);
//	IPW_DEBUG_WX("Setting disassociate threshold to %d\n", 3*wrqu->sens.value);
//	mutex_lock(&priv->mutex);
//
//	if (wrqu->sens.fixed == 0)
//	{
//		priv->roaming_threshold = IPW_MB_ROAMING_THRESHOLD_DEFAULT;
//		priv->disassociate_threshold = IPW_MB_DISASSOCIATE_THRESHOLD_DEFAULT;
//		goto out;
//	}
//	if ((wrqu->sens.value > IPW_MB_ROAMING_THRESHOLD_MAX) ||
//	    (wrqu->sens.value < IPW_MB_ROAMING_THRESHOLD_MIN)) {
//		err = -EINVAL;
//		goto out;
//	}
//
//	priv->roaming_threshold = wrqu->sens.value;
//	priv->disassociate_threshold = 3*wrqu->sens.value;
//      out:
//	mutex_unlock(&priv->mutex);
//	return err;
//}
//
//static int ipw_wx_get_sens(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	wrqu->sens.fixed = 1;
//	wrqu->sens.value = priv->roaming_threshold;
//	mutex_unlock(&priv->mutex);
//
//	IPW_DEBUG_WX("GET roaming threshold -> %s %d \n",
//		     wrqu->power.disabled ? "OFF" : "ON", wrqu->power.value);
//
//	return 0;
//}
//
//static int ipw_wx_set_rate(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	/* TODO: We should use semaphores or locks for access to priv */
//	struct ipw_priv *priv = libipw_priv(dev);
//	u32 target_rate = wrqu->bitrate.value;
//	u32 fixed, mask;
//
//	/* value = -1, fixed = 0 means auto only, so we should use all rates offered by AP */
//	/* value = X, fixed = 1 means only rate X */
//	/* value = X, fixed = 0 means all rates lower equal X */
//
//	if (target_rate == -1) {
//		fixed = 0;
//		mask = LIBIPW_DEFAULT_RATES_MASK;
//		/* Now we should reassociate */
//		goto apply;
//	}
//
//	mask = 0;
//	fixed = wrqu->bitrate.fixed;
//
//	if (target_rate == 1000000 || !fixed)
//		mask |= LIBIPW_CCK_RATE_1MB_MASK;
//	if (target_rate == 1000000)
//		goto apply;
//
//	if (target_rate == 2000000 || !fixed)
//		mask |= LIBIPW_CCK_RATE_2MB_MASK;
//	if (target_rate == 2000000)
//		goto apply;
//
//	if (target_rate == 5500000 || !fixed)
//		mask |= LIBIPW_CCK_RATE_5MB_MASK;
//	if (target_rate == 5500000)
//		goto apply;
//
//	if (target_rate == 6000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_6MB_MASK;
//	if (target_rate == 6000000)
//		goto apply;
//
//	if (target_rate == 9000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_9MB_MASK;
//	if (target_rate == 9000000)
//		goto apply;
//
//	if (target_rate == 11000000 || !fixed)
//		mask |= LIBIPW_CCK_RATE_11MB_MASK;
//	if (target_rate == 11000000)
//		goto apply;
//
//	if (target_rate == 12000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_12MB_MASK;
//	if (target_rate == 12000000)
//		goto apply;
//
//	if (target_rate == 18000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_18MB_MASK;
//	if (target_rate == 18000000)
//		goto apply;
//
//	if (target_rate == 24000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_24MB_MASK;
//	if (target_rate == 24000000)
//		goto apply;
//
//	if (target_rate == 36000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_36MB_MASK;
//	if (target_rate == 36000000)
//		goto apply;
//
//	if (target_rate == 48000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_48MB_MASK;
//	if (target_rate == 48000000)
//		goto apply;
//
//	if (target_rate == 54000000 || !fixed)
//		mask |= LIBIPW_OFDM_RATE_54MB_MASK;
//	if (target_rate == 54000000)
//		goto apply;
//
//	IPW_DEBUG_WX("invalid rate specified, returning error\n");
//	return -EINVAL;
//
//      apply:
//	IPW_DEBUG_WX("Setting rate mask to 0x%08X [%s]\n",
//		     mask, fixed ? "fixed" : "sub-rates");
//	mutex_lock(&priv->mutex);
//	if (mask == LIBIPW_DEFAULT_RATES_MASK) {
//		priv->config &= ~CFG_FIXED_RATE;
//		ipw_set_fixed_rate(priv, priv->ieee->mode);
//	} else
//		priv->config |= CFG_FIXED_RATE;
//
//	if (priv->rates_mask == mask) {
//		IPW_DEBUG_WX("Mask set to current mask.\n");
//		mutex_unlock(&priv->mutex);
//		return 0;
//	}
//
//	priv->rates_mask = mask;
//
//	/* Network configuration changed -- force [re]association */
//	IPW_DEBUG_ASSOC("[re]association triggered due to rates change.\n");
//	if (!ipw_disassociate(priv))
//		ipw_associate(priv);
//
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_get_rate(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	wrqu->bitrate.value = priv->last_rate;
//	wrqu->bitrate.fixed = (priv->config & CFG_FIXED_RATE) ? 1 : 0;
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("GET Rate -> %d \n", wrqu->bitrate.value);
//	return 0;
//}
//
//static int ipw_wx_set_rts(struct net_device *dev,
//			  struct iw_request_info *info,
//			  union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	if (wrqu->rts.disabled || !wrqu->rts.fixed)
//		priv->rts_threshold = DEFAULT_RTS_THRESHOLD;
//	else {
//		if (wrqu->rts.value < MIN_RTS_THRESHOLD ||
//		    wrqu->rts.value > MAX_RTS_THRESHOLD) {
//			mutex_unlock(&priv->mutex);
//			return -EINVAL;
//		}
//		priv->rts_threshold = wrqu->rts.value;
//	}
//
//	ipw_send_rts_threshold(priv, priv->rts_threshold);
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("SET RTS Threshold -> %d \n", priv->rts_threshold);
//	return 0;
//}
//
//static int ipw_wx_get_rts(struct net_device *dev,
//			  struct iw_request_info *info,
//			  union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	wrqu->rts.value = priv->rts_threshold;
//	wrqu->rts.fixed = 0;	/* no auto select */
//	wrqu->rts.disabled = (wrqu->rts.value == DEFAULT_RTS_THRESHOLD);
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("GET RTS Threshold -> %d \n", wrqu->rts.value);
//	return 0;
//}
//
//static int ipw_wx_set_txpow(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int err = 0;
//
//	mutex_lock(&priv->mutex);
//	if (ipw_radio_kill_sw(priv, wrqu->power.disabled)) {
//		err = -EINPROGRESS;
//		goto out;
//	}
//
//	if (!wrqu->power.fixed)
//		wrqu->power.value = IPW_TX_POWER_DEFAULT;
//
//	if (wrqu->power.flags != IW_TXPOW_DBM) {
//		err = -EINVAL;
//		goto out;
//	}
//
//	if ((wrqu->power.value > IPW_TX_POWER_MAX) ||
//	    (wrqu->power.value < IPW_TX_POWER_MIN)) {
//		err = -EINVAL;
//		goto out;
//	}
//
//	priv->tx_power = wrqu->power.value;
//	err = ipw_set_tx_power(priv);
//      out:
//	mutex_unlock(&priv->mutex);
//	return err;
//}
//
//static int ipw_wx_get_txpow(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	wrqu->power.value = priv->tx_power;
//	wrqu->power.fixed = 1;
//	wrqu->power.flags = IW_TXPOW_DBM;
//	wrqu->power.disabled = (priv->status & STATUS_RF_KILL_MASK) ? 1 : 0;
//	mutex_unlock(&priv->mutex);
//
//	IPW_DEBUG_WX("GET TX Power -> %s %d \n",
//		     wrqu->power.disabled ? "OFF" : "ON", wrqu->power.value);
//
//	return 0;
//}
//
//static int ipw_wx_set_frag(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	if (wrqu->frag.disabled || !wrqu->frag.fixed)
//		priv->ieee->fts = DEFAULT_FTS;
//	else {
//		if (wrqu->frag.value < MIN_FRAG_THRESHOLD ||
//		    wrqu->frag.value > MAX_FRAG_THRESHOLD) {
//			mutex_unlock(&priv->mutex);
//			return -EINVAL;
//		}
//
//		priv->ieee->fts = wrqu->frag.value & ~0x1;
//	}
//
//	ipw_send_frag_threshold(priv, wrqu->frag.value);
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("SET Frag Threshold -> %d \n", wrqu->frag.value);
//	return 0;
//}
//
//static int ipw_wx_get_frag(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	wrqu->frag.value = priv->ieee->fts;
//	wrqu->frag.fixed = 0;	/* no auto select */
//	wrqu->frag.disabled = (wrqu->frag.value == DEFAULT_FTS);
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("GET Frag Threshold -> %d \n", wrqu->frag.value);
//
//	return 0;
//}
//
//static int ipw_wx_set_retry(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//
//	if (wrqu->retry.flags & IW_RETRY_LIFETIME || wrqu->retry.disabled)
//		return -EINVAL;
//
//	if (!(wrqu->retry.flags & IW_RETRY_LIMIT))
//		return 0;
//
//	if (wrqu->retry.value < 0 || wrqu->retry.value >= 255)
//		return -EINVAL;
//
//	mutex_lock(&priv->mutex);
//	if (wrqu->retry.flags & IW_RETRY_SHORT)
//		priv->short_retry_limit = (u8) wrqu->retry.value;
//	else if (wrqu->retry.flags & IW_RETRY_LONG)
//		priv->long_retry_limit = (u8) wrqu->retry.value;
//	else {
//		priv->short_retry_limit = (u8) wrqu->retry.value;
//		priv->long_retry_limit = (u8) wrqu->retry.value;
//	}
//
//	ipw_send_retry_limit(priv, priv->short_retry_limit,
//			     priv->long_retry_limit);
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("SET retry limit -> short:%d long:%d\n",
//		     priv->short_retry_limit, priv->long_retry_limit);
//	return 0;
//}
//
//static int ipw_wx_get_retry(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//
//	mutex_lock(&priv->mutex);
//	wrqu->retry.disabled = 0;
//
//	if ((wrqu->retry.flags & IW_RETRY_TYPE) == IW_RETRY_LIFETIME) {
//		mutex_unlock(&priv->mutex);
//		return -EINVAL;
//	}
//
//	if (wrqu->retry.flags & IW_RETRY_LONG) {
//		wrqu->retry.flags = IW_RETRY_LIMIT | IW_RETRY_LONG;
//		wrqu->retry.value = priv->long_retry_limit;
//	} else if (wrqu->retry.flags & IW_RETRY_SHORT) {
//		wrqu->retry.flags = IW_RETRY_LIMIT | IW_RETRY_SHORT;
//		wrqu->retry.value = priv->short_retry_limit;
//	} else {
//		wrqu->retry.flags = IW_RETRY_LIMIT;
//		wrqu->retry.value = priv->short_retry_limit;
//	}
//	mutex_unlock(&priv->mutex);
//
//	IPW_DEBUG_WX("GET retry -> %d \n", wrqu->retry.value);
//
//	return 0;
//}
//
//static int ipw_wx_set_scan(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct iw_scan_req *req = (struct iw_scan_req *)extra;
//	struct delayed_work *work = NULL;
//
//	mutex_lock(&priv->mutex);
//
//	priv->user_requested_scan = 1;
//
//	if (wrqu->data.length == sizeof(struct iw_scan_req)) {
//		if (wrqu->data.flags & IW_SCAN_THIS_ESSID) {
//			int len = min((int)req->essid_len,
//			              (int)sizeof(priv->direct_scan_ssid));
//			memcpy(priv->direct_scan_ssid, req->essid, len);
//			priv->direct_scan_ssid_len = len;
//			work = &priv->request_direct_scan;
//		} else if (req->scan_type == IW_SCAN_TYPE_PASSIVE) {
//			work = &priv->request_passive_scan;
//		}
//	} else {
//		/* Normal active broadcast scan */
//		work = &priv->request_scan;
//	}
//
//	mutex_unlock(&priv->mutex);
//
//	IPW_DEBUG_WX("Start scan\n");
//
//	queue_delayed_work(priv->workqueue, work, 0);
//
//	return 0;
//}
//
//static int ipw_wx_get_scan(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	return libipw_wx_get_scan(priv->ieee, info, wrqu, extra);
//}
//
//static int ipw_wx_set_encode(struct net_device *dev,
//			     struct iw_request_info *info,
//			     union iwreq_data *wrqu, char *key)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int ret;
//	u32 cap = priv->capability;
//
//	mutex_lock(&priv->mutex);
//	ret = libipw_wx_set_encode(priv->ieee, info, wrqu, key);
//
//	/* In IBSS mode, we need to notify the firmware to update
//	 * the beacon info after we changed the capability. */
//	if (cap != priv->capability &&
//	    priv->ieee->iw_mode == IW_MODE_ADHOC &&
//	    priv->status & STATUS_ASSOCIATED)
//		ipw_disassociate(priv);
//
//	mutex_unlock(&priv->mutex);
//	return ret;
//}
//
//static int ipw_wx_get_encode(struct net_device *dev,
//			     struct iw_request_info *info,
//			     union iwreq_data *wrqu, char *key)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	return libipw_wx_get_encode(priv->ieee, info, wrqu, key);
//}
//
//static int ipw_wx_set_power(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int err;
//	mutex_lock(&priv->mutex);
//	if (wrqu->power.disabled) {
//		priv->power_mode = IPW_POWER_LEVEL(priv->power_mode);
//		err = ipw_send_power_mode(priv, IPW_POWER_MODE_CAM);
//		if (err) {
//			IPW_DEBUG_WX("failed setting power mode.\n");
//			mutex_unlock(&priv->mutex);
//			return err;
//		}
//		IPW_DEBUG_WX("SET Power Management Mode -> off\n");
//		mutex_unlock(&priv->mutex);
//		return 0;
//	}
//
//	switch (wrqu->power.flags & IW_POWER_MODE) {
//	case IW_POWER_ON:	/* If not specified */
//	case IW_POWER_MODE:	/* If set all mask */
//	case IW_POWER_ALL_R:	/* If explicitly state all */
//		break;
//	default:		/* Otherwise we don't support it */
//		IPW_DEBUG_WX("SET PM Mode: %X not supported.\n",
//			     wrqu->power.flags);
//		mutex_unlock(&priv->mutex);
//		return -EOPNOTSUPP;
//	}
//
//	/* If the user hasn't specified a power management mode yet, default
//	 * to BATTERY */
//	if (IPW_POWER_LEVEL(priv->power_mode) == IPW_POWER_AC)
//		priv->power_mode = IPW_POWER_ENABLED | IPW_POWER_BATTERY;
//	else
//		priv->power_mode = IPW_POWER_ENABLED | priv->power_mode;
//
//	err = ipw_send_power_mode(priv, IPW_POWER_LEVEL(priv->power_mode));
//	if (err) {
//		IPW_DEBUG_WX("failed setting power mode.\n");
//		mutex_unlock(&priv->mutex);
//		return err;
//	}
//
//	IPW_DEBUG_WX("SET Power Management Mode -> 0x%02X\n", priv->power_mode);
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_get_power(struct net_device *dev,
//			    struct iw_request_info *info,
//			    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	if (!(priv->power_mode & IPW_POWER_ENABLED))
//		wrqu->power.disabled = 1;
//	else
//		wrqu->power.disabled = 0;
//
//	mutex_unlock(&priv->mutex);
//	IPW_DEBUG_WX("GET Power Management Mode -> %02X\n", priv->power_mode);
//
//	return 0;
//}
//
//static int ipw_wx_set_powermode(struct net_device *dev,
//				struct iw_request_info *info,
//				union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int mode = *(int *)extra;
//	int err;
//
//	mutex_lock(&priv->mutex);
//	if ((mode < 1) || (mode > IPW_POWER_LIMIT))
//		mode = IPW_POWER_AC;
//
//	if (IPW_POWER_LEVEL(priv->power_mode) != mode) {
//		err = ipw_send_power_mode(priv, mode);
//		if (err) {
//			IPW_DEBUG_WX("failed setting power mode.\n");
//			mutex_unlock(&priv->mutex);
//			return err;
//		}
//		priv->power_mode = IPW_POWER_ENABLED | mode;
//	}
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//#define MAX_WX_STRING 80
//static int ipw_wx_get_powermode(struct net_device *dev,
//				struct iw_request_info *info,
//				union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int level = IPW_POWER_LEVEL(priv->power_mode);
//	char *p = extra;
//
//	p += snprintf(p, MAX_WX_STRING, "Power save level: %d ", level);
//
//	switch (level) {
//	case IPW_POWER_AC:
//		p += snprintf(p, MAX_WX_STRING - (p - extra), "(AC)");
//		break;
//	case IPW_POWER_BATTERY:
//		p += snprintf(p, MAX_WX_STRING - (p - extra), "(BATTERY)");
//		break;
//	default:
//		p += snprintf(p, MAX_WX_STRING - (p - extra),
//			      "(Timeout %dms, Period %dms)",
//			      timeout_duration[level - 1] / 1000,
//			      period_duration[level - 1] / 1000);
//	}
//
//	if (!(priv->power_mode & IPW_POWER_ENABLED))
//		p += snprintf(p, MAX_WX_STRING - (p - extra), " OFF");
//
//	wrqu->data.length = p - extra + 1;
//
//	return 0;
//}
//
//static int ipw_wx_set_wireless_mode(struct net_device *dev,
//				    struct iw_request_info *info,
//				    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int mode = *(int *)extra;
//	u8 band = 0, modulation = 0;
//
//	if (mode == 0 || mode & ~IEEE_MODE_MASK) {
//		IPW_WARNING("Attempt to set invalid wireless mode: %d\n", mode);
//		return -EINVAL;
//	}
//	mutex_lock(&priv->mutex);
//	if (priv->adapter == IPW_2915ABG) {
//		priv->ieee->abg_true = 1;
//		if (mode & IEEE_A) {
//			band |= LIBIPW_52GHZ_BAND;
//			modulation |= LIBIPW_OFDM_MODULATION;
//		} else
//			priv->ieee->abg_true = 0;
//	} else {
//		if (mode & IEEE_A) {
//			IPW_WARNING("Attempt to set 2200BG into "
//				    "802.11a mode\n");
//			mutex_unlock(&priv->mutex);
//			return -EINVAL;
//		}
//
//		priv->ieee->abg_true = 0;
//	}
//
//	if (mode & IEEE_B) {
//		band |= LIBIPW_24GHZ_BAND;
//		modulation |= LIBIPW_CCK_MODULATION;
//	} else
//		priv->ieee->abg_true = 0;
//
//	if (mode & IEEE_G) {
//		band |= LIBIPW_24GHZ_BAND;
//		modulation |= LIBIPW_OFDM_MODULATION;
//	} else
//		priv->ieee->abg_true = 0;
//
//	priv->ieee->mode = mode;
//	priv->ieee->freq_band = band;
//	priv->ieee->modulation = modulation;
//	init_supported_rates(priv, &priv->rates);
//
//	/* Network configuration changed -- force [re]association */
//	IPW_DEBUG_ASSOC("[re]association triggered due to mode change.\n");
//	if (!ipw_disassociate(priv)) {
//		ipw_send_supported_rates(priv, &priv->rates);
//		ipw_associate(priv);
//	}
//
//	/* Update the band LEDs */
//	ipw_led_band_on(priv);
//
//	IPW_DEBUG_WX("PRIV SET MODE: %c%c%c\n",
//		     mode & IEEE_A ? 'a' : '.',
//		     mode & IEEE_B ? 'b' : '.', mode & IEEE_G ? 'g' : '.');
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_get_wireless_mode(struct net_device *dev,
//				    struct iw_request_info *info,
//				    union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	switch (priv->ieee->mode) {
//	case IEEE_A:
//		strncpy(extra, "802.11a (1)", MAX_WX_STRING);
//		break;
//	case IEEE_B:
//		strncpy(extra, "802.11b (2)", MAX_WX_STRING);
//		break;
//	case IEEE_A | IEEE_B:
//		strncpy(extra, "802.11ab (3)", MAX_WX_STRING);
//		break;
//	case IEEE_G:
//		strncpy(extra, "802.11g (4)", MAX_WX_STRING);
//		break;
//	case IEEE_A | IEEE_G:
//		strncpy(extra, "802.11ag (5)", MAX_WX_STRING);
//		break;
//	case IEEE_B | IEEE_G:
//		strncpy(extra, "802.11bg (6)", MAX_WX_STRING);
//		break;
//	case IEEE_A | IEEE_B | IEEE_G:
//		strncpy(extra, "802.11abg (7)", MAX_WX_STRING);
//		break;
//	default:
//		strncpy(extra, "unknown", MAX_WX_STRING);
//		break;
//	}
//
//	IPW_DEBUG_WX("PRIV GET MODE: %s\n", extra);
//
//	wrqu->data.length = strlen(extra) + 1;
//	mutex_unlock(&priv->mutex);
//
//	return 0;
//}
//
//static int ipw_wx_set_preamble(struct net_device *dev,
//			       struct iw_request_info *info,
//			       union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int mode = *(int *)extra;
//	mutex_lock(&priv->mutex);
//	/* Switching from SHORT -> LONG requires a disassociation */
//	if (mode == 1) {
//		if (!(priv->config & CFG_PREAMBLE_LONG)) {
//			priv->config |= CFG_PREAMBLE_LONG;
//
//			/* Network configuration changed -- force [re]association */
//			IPW_DEBUG_ASSOC
//			    ("[re]association triggered due to preamble change.\n");
//			if (!ipw_disassociate(priv))
//				ipw_associate(priv);
//		}
//		goto done;
//	}
//
//	if (mode == 0) {
//		priv->config &= ~CFG_PREAMBLE_LONG;
//		goto done;
//	}
//	mutex_unlock(&priv->mutex);
//	return -EINVAL;
//
//      done:
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static int ipw_wx_get_preamble(struct net_device *dev,
//			       struct iw_request_info *info,
//			       union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//	if (priv->config & CFG_PREAMBLE_LONG)
//		snprintf(wrqu->name, IFNAMSIZ, "long (1)");
//	else
//		snprintf(wrqu->name, IFNAMSIZ, "auto (0)");
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//#ifdef CONFIG_IPW2200_MONITOR
//static int ipw_wx_set_monitor(struct net_device *dev,
//			      struct iw_request_info *info,
//			      union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int *parms = (int *)extra;
//	int enable = (parms[0] > 0);
//	mutex_lock(&priv->mutex);
//	IPW_DEBUG_WX("SET MONITOR: %d %d\n", enable, parms[1]);
//	if (enable) {
//		if (priv->ieee->iw_mode != IW_MODE_MONITOR) {
//#ifdef CONFIG_IPW2200_RADIOTAP
//			priv->net_dev->type = ARPHRD_IEEE80211_RADIOTAP;
//#else
//			priv->net_dev->type = ARPHRD_IEEE80211;
//#endif
//			queue_work(priv->workqueue, &priv->adapter_restart);
//		}
//
//		ipw_set_channel(priv, parms[1]);
//	} else {
//		if (priv->ieee->iw_mode != IW_MODE_MONITOR) {
//			mutex_unlock(&priv->mutex);
//			return 0;
//		}
//		priv->net_dev->type = ARPHRD_ETHER;
//		queue_work(priv->workqueue, &priv->adapter_restart);
//	}
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//#endif				/* CONFIG_IPW2200_MONITOR */
//
//static int ipw_wx_reset(struct net_device *dev,
//			struct iw_request_info *info,
//			union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	IPW_DEBUG_WX("RESET\n");
//	queue_work(priv->workqueue, &priv->adapter_restart);
//	return 0;
//}
//
//static int ipw_wx_sw_reset(struct net_device *dev,
//			   struct iw_request_info *info,
//			   union iwreq_data *wrqu, char *extra)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	union iwreq_data wrqu_sec = {
//		.encoding = {
//			     .flags = IW_ENCODE_DISABLED,
//			     },
//	};
//	int ret;
//
//	IPW_DEBUG_WX("SW_RESET\n");
//
//	mutex_lock(&priv->mutex);
//
//	ret = ipw_sw_reset(priv, 2);
//	if (!ret) {
//		free_firmware();
//		ipw_adapter_restart(priv);
//	}
//
//	/* The SW reset bit might have been toggled on by the 'disable'
//	 * module parameter, so take appropriate action */
//	ipw_radio_kill_sw(priv, priv->status & STATUS_RF_KILL_SW);
//
//	mutex_unlock(&priv->mutex);
//	libipw_wx_set_encode(priv->ieee, info, &wrqu_sec, NULL);
//	mutex_lock(&priv->mutex);
//
//	if (!(priv->status & STATUS_RF_KILL_MASK)) {
//		/* Configuration likely changed -- force [re]association */
//		IPW_DEBUG_ASSOC("[re]association triggered due to sw "
//				"reset.\n");
//		if (!ipw_disassociate(priv))
//			ipw_associate(priv);
//	}
//
//	mutex_unlock(&priv->mutex);
//
//	return 0;
//}
//
///* Rebase the WE IOCTLs to zero for the handler array */
//#define IW_IOCTL(x) [(x)-SIOCSIWCOMMIT]
//static iw_handler ipw_wx_handlers[] = {
//	IW_IOCTL(SIOCGIWNAME) = ipw_wx_get_name,
//	IW_IOCTL(SIOCSIWFREQ) = ipw_wx_set_freq,
//	IW_IOCTL(SIOCGIWFREQ) = ipw_wx_get_freq,
//	IW_IOCTL(SIOCSIWMODE) = ipw_wx_set_mode,
//	IW_IOCTL(SIOCGIWMODE) = ipw_wx_get_mode,
//	IW_IOCTL(SIOCSIWSENS) = ipw_wx_set_sens,
//	IW_IOCTL(SIOCGIWSENS) = ipw_wx_get_sens,
//	IW_IOCTL(SIOCGIWRANGE) = ipw_wx_get_range,
//	IW_IOCTL(SIOCSIWAP) = ipw_wx_set_wap,
//	IW_IOCTL(SIOCGIWAP) = ipw_wx_get_wap,
//	IW_IOCTL(SIOCSIWSCAN) = ipw_wx_set_scan,
//	IW_IOCTL(SIOCGIWSCAN) = ipw_wx_get_scan,
//	IW_IOCTL(SIOCSIWESSID) = ipw_wx_set_essid,
//	IW_IOCTL(SIOCGIWESSID) = ipw_wx_get_essid,
//	IW_IOCTL(SIOCSIWNICKN) = ipw_wx_set_nick,
//	IW_IOCTL(SIOCGIWNICKN) = ipw_wx_get_nick,
//	IW_IOCTL(SIOCSIWRATE) = ipw_wx_set_rate,
//	IW_IOCTL(SIOCGIWRATE) = ipw_wx_get_rate,
//	IW_IOCTL(SIOCSIWRTS) = ipw_wx_set_rts,
//	IW_IOCTL(SIOCGIWRTS) = ipw_wx_get_rts,
//	IW_IOCTL(SIOCSIWFRAG) = ipw_wx_set_frag,
//	IW_IOCTL(SIOCGIWFRAG) = ipw_wx_get_frag,
//	IW_IOCTL(SIOCSIWTXPOW) = ipw_wx_set_txpow,
//	IW_IOCTL(SIOCGIWTXPOW) = ipw_wx_get_txpow,
//	IW_IOCTL(SIOCSIWRETRY) = ipw_wx_set_retry,
//	IW_IOCTL(SIOCGIWRETRY) = ipw_wx_get_retry,
//	IW_IOCTL(SIOCSIWENCODE) = ipw_wx_set_encode,
//	IW_IOCTL(SIOCGIWENCODE) = ipw_wx_get_encode,
//	IW_IOCTL(SIOCSIWPOWER) = ipw_wx_set_power,
//	IW_IOCTL(SIOCGIWPOWER) = ipw_wx_get_power,
//	IW_IOCTL(SIOCSIWSPY) = iw_handler_set_spy,
//	IW_IOCTL(SIOCGIWSPY) = iw_handler_get_spy,
//	IW_IOCTL(SIOCSIWTHRSPY) = iw_handler_set_thrspy,
//	IW_IOCTL(SIOCGIWTHRSPY) = iw_handler_get_thrspy,
//	IW_IOCTL(SIOCSIWGENIE) = ipw_wx_set_genie,
//	IW_IOCTL(SIOCGIWGENIE) = ipw_wx_get_genie,
//	IW_IOCTL(SIOCSIWMLME) = ipw_wx_set_mlme,
//	IW_IOCTL(SIOCSIWAUTH) = ipw_wx_set_auth,
//	IW_IOCTL(SIOCGIWAUTH) = ipw_wx_get_auth,
//	IW_IOCTL(SIOCSIWENCODEEXT) = ipw_wx_set_encodeext,
//	IW_IOCTL(SIOCGIWENCODEEXT) = ipw_wx_get_encodeext,
//};
//
//enum {
//	IPW_PRIV_SET_POWER = SIOCIWFIRSTPRIV,
//	IPW_PRIV_GET_POWER,
//	IPW_PRIV_SET_MODE,
//	IPW_PRIV_GET_MODE,
//	IPW_PRIV_SET_PREAMBLE,
//	IPW_PRIV_GET_PREAMBLE,
//	IPW_PRIV_RESET,
//	IPW_PRIV_SW_RESET,
//#ifdef CONFIG_IPW2200_MONITOR
//	IPW_PRIV_SET_MONITOR,
//#endif
//};
//
//static struct iw_priv_args ipw_priv_args[] = {
//	{
//	 .cmd = IPW_PRIV_SET_POWER,
//	 .set_args = IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,
//	 .name = "set_power"},
//	{
//	 .cmd = IPW_PRIV_GET_POWER,
//	 .get_args = IW_PRIV_TYPE_CHAR | IW_PRIV_SIZE_FIXED | MAX_WX_STRING,
//	 .name = "get_power"},
//	{
//	 .cmd = IPW_PRIV_SET_MODE,
//	 .set_args = IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,
//	 .name = "set_mode"},
//	{
//	 .cmd = IPW_PRIV_GET_MODE,
//	 .get_args = IW_PRIV_TYPE_CHAR | IW_PRIV_SIZE_FIXED | MAX_WX_STRING,
//	 .name = "get_mode"},
//	{
//	 .cmd = IPW_PRIV_SET_PREAMBLE,
//	 .set_args = IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 1,
//	 .name = "set_preamble"},
//	{
//	 .cmd = IPW_PRIV_GET_PREAMBLE,
//	 .get_args = IW_PRIV_TYPE_CHAR | IW_PRIV_SIZE_FIXED | IFNAMSIZ,
//	 .name = "get_preamble"},
//	{
//	 IPW_PRIV_RESET,
//	 IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 0, 0, "reset"},
//	{
//	 IPW_PRIV_SW_RESET,
//	 IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 0, 0, "sw_reset"},
//#ifdef CONFIG_IPW2200_MONITOR
//	{
//	 IPW_PRIV_SET_MONITOR,
//	 IW_PRIV_TYPE_INT | IW_PRIV_SIZE_FIXED | 2, 0, "monitor"},
//#endif				/* CONFIG_IPW2200_MONITOR */
//};
//
//static iw_handler ipw_priv_handler[] = {
//	ipw_wx_set_powermode,
//	ipw_wx_get_powermode,
//	ipw_wx_set_wireless_mode,
//	ipw_wx_get_wireless_mode,
//	ipw_wx_set_preamble,
//	ipw_wx_get_preamble,
//	ipw_wx_reset,
//	ipw_wx_sw_reset,
//#ifdef CONFIG_IPW2200_MONITOR
//	ipw_wx_set_monitor,
//#endif
//};
//
//static struct iw_handler_def ipw_wx_handler_def = {
//	.standard = ipw_wx_handlers,
//	.num_standard = ARRAY_SIZE(ipw_wx_handlers),
//	.num_private = ARRAY_SIZE(ipw_priv_handler),
//	.num_private_args = ARRAY_SIZE(ipw_priv_args),
//	.private = ipw_priv_handler,
//	.private_args = ipw_priv_args,
//	.get_wireless_stats = ipw_get_wireless_stats,
//};
//
///*
// * Get wireless statistics.
// * Called by /proc/net/wireless
// * Also called by SIOCGIWSTATS
// */
//static struct iw_statistics *ipw_get_wireless_stats(struct net_device *dev)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct iw_statistics *wstats;
//
//	wstats = &priv->wstats;
//
//	/* if hw is disabled, then ipw_get_ordinal() can't be called.
//	 * netdev->get_wireless_stats seems to be called before fw is
//	 * initialized.  STATUS_ASSOCIATED will only be set if the hw is up
//	 * and associated; if not associcated, the values are all meaningless
//	 * anyway, so set them all to NULL and INVALID */
//	if (!(priv->status & STATUS_ASSOCIATED)) {
//		wstats->miss.beacon = 0;
//		wstats->discard.retries = 0;
//		wstats->qual.qual = 0;
//		wstats->qual.level = 0;
//		wstats->qual.noise = 0;
//		wstats->qual.updated = 7;
//		wstats->qual.updated |= IW_QUAL_NOISE_INVALID |
//		    IW_QUAL_QUAL_INVALID | IW_QUAL_LEVEL_INVALID;
//		return wstats;
//	}
//
//	wstats->qual.qual = priv->quality;
//	wstats->qual.level = priv->exp_avg_rssi;
//	wstats->qual.noise = priv->exp_avg_noise;
//	wstats->qual.updated = IW_QUAL_QUAL_UPDATED | IW_QUAL_LEVEL_UPDATED |
//	    IW_QUAL_NOISE_UPDATED | IW_QUAL_DBM;
//
//	wstats->miss.beacon = average_value(&priv->average_missed_beacons);
//	wstats->discard.retries = priv->last_tx_failures;
//	wstats->discard.code = priv->ieee->ieee_stats.rx_discards_undecryptable;
//
///*	if (ipw_get_ordinal(priv, IPW_ORD_STAT_TX_RETRY, &tx_retry, &len))
//	goto fail_get_ordinal;
//	wstats->discard.retries += tx_retry; */
//
//	return wstats;
//}
//
///* net device stuff */
//
//static  void init_sys_config(struct ipw_sys_config *sys_config)
//{
//	memset(sys_config, 0, sizeof(struct ipw_sys_config));
//	sys_config->bt_coexistence = 0;
//	sys_config->answer_broadcast_ssid_probe = 0;
//	sys_config->accept_all_data_frames = 0;
//	sys_config->accept_non_directed_frames = 1;
//	sys_config->exclude_unicast_unencrypted = 0;
//	sys_config->disable_unicast_decryption = 1;
//	sys_config->exclude_multicast_unencrypted = 0;
//	sys_config->disable_multicast_decryption = 1;
//	if (antenna < CFG_SYS_ANTENNA_BOTH || antenna > CFG_SYS_ANTENNA_B)
//		antenna = CFG_SYS_ANTENNA_BOTH;
//	sys_config->antenna_diversity = antenna;
//	sys_config->pass_crc_to_host = 0;	/* TODO: See if 1 gives us FCS */
//	sys_config->dot11g_auto_detection = 0;
//	sys_config->enable_cts_to_self = 0;
//	sys_config->bt_coexist_collision_thr = 0;
//	sys_config->pass_noise_stats_to_host = 1;	/* 1 -- fix for 256 */
//	sys_config->silence_threshold = 0x1e;
//}
//
//static int ipw_net_open(struct net_device *dev)
//{
//	IPW_DEBUG_INFO("dev->open\n");
//	netif_start_queue(dev);
//	return 0;
//}
//
//static int ipw_net_stop(struct net_device *dev)
//{
//	IPW_DEBUG_INFO("dev->close\n");
//	netif_stop_queue(dev);
//	return 0;
//}
//
///*
//todo:
//
//modify to send one tfd per fragment instead of using chunking.  otherwise
//we need to heavily modify the libipw_skb_to_txb.
//*/
//
//static int ipw_tx_skb(struct ipw_priv *priv, struct libipw_txb *txb,
//			     int pri)
//{
//	struct libipw_hdr_3addrqos *hdr = (struct libipw_hdr_3addrqos *)
//	    txb->fragments[0]->data;
//	int i = 0;
//	struct tfd_frame *tfd;
//#ifdef CONFIG_IPW2200_QOS
//	int tx_id = ipw_get_tx_queue_number(priv, pri);
//	struct clx2_tx_queue *txq = &priv->txq[tx_id];
//#else
//	struct clx2_tx_queue *txq = &priv->txq[0];
//#endif
//	struct clx2_queue *q = &txq->q;
//	u8 id, hdr_len, unicast;
//	int fc;
//
//	if (!(priv->status & STATUS_ASSOCIATED))
//		goto drop;
//
//	hdr_len = libipw_get_hdrlen(le16_to_cpu(hdr->frame_ctl));
//	switch (priv->ieee->iw_mode) {
//	case IW_MODE_ADHOC:
//		unicast = !is_multicast_ether_addr(hdr->addr1);
//		id = ipw_find_station(priv, hdr->addr1);
//		if (id == IPW_INVALID_STATION) {
//			id = ipw_add_station(priv, hdr->addr1);
//			if (id == IPW_INVALID_STATION) {
//				IPW_WARNING("Attempt to send data to "
//					    "invalid cell: %pM\n",
//					    hdr->addr1);
//				goto drop;
//			}
//		}
//		break;
//
//	case IW_MODE_INFRA:
//	default:
//		unicast = !is_multicast_ether_addr(hdr->addr3);
//		id = 0;
//		break;
//	}
//
//	tfd = &txq->bd[q->first_empty];
//	txq->txb[q->first_empty] = txb;
//	memset(tfd, 0, sizeof(*tfd));
//	tfd->u.data.station_number = id;
//
//	tfd->control_flags.message_type = TX_FRAME_TYPE;
//	tfd->control_flags.control_bits = TFD_NEED_IRQ_MASK;
//
//	tfd->u.data.cmd_id = DINO_CMD_TX;
//	tfd->u.data.len = cpu_to_le16(txb->payload_size);
//
//	if (priv->assoc_request.ieee_mode == IPW_B_MODE)
//		tfd->u.data.tx_flags_ext |= DCT_FLAG_EXT_MODE_CCK;
//	else
//		tfd->u.data.tx_flags_ext |= DCT_FLAG_EXT_MODE_OFDM;
//
//	if (priv->assoc_request.preamble_length == DCT_FLAG_SHORT_PREAMBLE)
//		tfd->u.data.tx_flags |= DCT_FLAG_SHORT_PREAMBLE;
//
//	fc = le16_to_cpu(hdr->frame_ctl);
//	hdr->frame_ctl = cpu_to_le16(fc & ~IEEE80211_FCTL_MOREFRAGS);
//
//	memcpy(&tfd->u.data.tfd.tfd_24.mchdr, hdr, hdr_len);
//
//	if (likely(unicast))
//		tfd->u.data.tx_flags |= DCT_FLAG_ACK_REQD;
//
//	if (txb->encrypted && !priv->ieee->host_encrypt) {
//		switch (priv->ieee->sec.level) {
//		case SEC_LEVEL_3:
//			tfd->u.data.tfd.tfd_24.mchdr.frame_ctl |=
//			    cpu_to_le16(IEEE80211_FCTL_PROTECTED);
//			/* XXX: ACK flag must be set for CCMP even if it
//			 * is a multicast/broadcast packet, because CCMP
//			 * group communication encrypted by GTK is
//			 * actually done by the AP. */
//			if (!unicast)
//				tfd->u.data.tx_flags |= DCT_FLAG_ACK_REQD;
//
//			tfd->u.data.tx_flags &= ~DCT_FLAG_NO_WEP;
//			tfd->u.data.tx_flags_ext |= DCT_FLAG_EXT_SECURITY_CCM;
//			tfd->u.data.key_index = 0;
//			tfd->u.data.key_index |= DCT_WEP_INDEX_USE_IMMEDIATE;
//			break;
//		case SEC_LEVEL_2:
//			tfd->u.data.tfd.tfd_24.mchdr.frame_ctl |=
//			    cpu_to_le16(IEEE80211_FCTL_PROTECTED);
//			tfd->u.data.tx_flags &= ~DCT_FLAG_NO_WEP;
//			tfd->u.data.tx_flags_ext |= DCT_FLAG_EXT_SECURITY_TKIP;
//			tfd->u.data.key_index = DCT_WEP_INDEX_USE_IMMEDIATE;
//			break;
//		case SEC_LEVEL_1:
//			tfd->u.data.tfd.tfd_24.mchdr.frame_ctl |=
//			    cpu_to_le16(IEEE80211_FCTL_PROTECTED);
//			tfd->u.data.key_index = priv->ieee->crypt_info.tx_keyidx;
//			if (priv->ieee->sec.key_sizes[priv->ieee->crypt_info.tx_keyidx] <=
//			    40)
//				tfd->u.data.key_index |= DCT_WEP_KEY_64Bit;
//			else
//				tfd->u.data.key_index |= DCT_WEP_KEY_128Bit;
//			break;
//		case SEC_LEVEL_0:
//			break;
//		default:
//			printk(KERN_ERR "Unknow security level %d\n",
//			       priv->ieee->sec.level);
//			break;
//		}
//	} else
//		/* No hardware encryption */
//		tfd->u.data.tx_flags |= DCT_FLAG_NO_WEP;
//
//#ifdef CONFIG_IPW2200_QOS
//	if (fc & IEEE80211_STYPE_QOS_DATA)
//		ipw_qos_set_tx_queue_command(priv, pri, &(tfd->u.data));
//#endif				/* CONFIG_IPW2200_QOS */
//
//	/* payload */
//	tfd->u.data.num_chunks = cpu_to_le32(min((u8) (NUM_TFD_CHUNKS - 2),
//						 txb->nr_frags));
//	IPW_DEBUG_FRAG("%i fragments being sent as %i chunks.\n",
//		       txb->nr_frags, le32_to_cpu(tfd->u.data.num_chunks));
//	for (i = 0; i < le32_to_cpu(tfd->u.data.num_chunks); i++) {
//		IPW_DEBUG_FRAG("Adding fragment %i of %i (%d bytes).\n",
//			       i, le32_to_cpu(tfd->u.data.num_chunks),
//			       txb->fragments[i]->len - hdr_len);
//		IPW_DEBUG_TX("Dumping TX packet frag %i of %i (%d bytes):\n",
//			     i, tfd->u.data.num_chunks,
//			     txb->fragments[i]->len - hdr_len);
//		printk_buf(IPW_DL_TX, txb->fragments[i]->data + hdr_len,
//			   txb->fragments[i]->len - hdr_len);
//
//		tfd->u.data.chunk_ptr[i] =
//		    cpu_to_le32(pci_map_single
//				(priv->pci_dev,
//				 txb->fragments[i]->data + hdr_len,
//				 txb->fragments[i]->len - hdr_len,
//				 PCI_DMA_TODEVICE));
//		tfd->u.data.chunk_len[i] =
//		    cpu_to_le16(txb->fragments[i]->len - hdr_len);
//	}
//
//	if (i != txb->nr_frags) {
//		struct sk_buff *skb;
//		u16 remaining_bytes = 0;
//		int j;
//
//		for (j = i; j < txb->nr_frags; j++)
//			remaining_bytes += txb->fragments[j]->len - hdr_len;
//
//		printk(KERN_INFO "Trying to reallocate for %d bytes\n",
//		       remaining_bytes);
//		skb = alloc_skb(remaining_bytes, GFP_ATOMIC);
//		if (skb != NULL) {
//			tfd->u.data.chunk_len[i] = cpu_to_le16(remaining_bytes);
//			for (j = i; j < txb->nr_frags; j++) {
//				int size = txb->fragments[j]->len - hdr_len;
//
//				printk(KERN_INFO "Adding frag %d %d...\n",
//				       j, size);
//				memcpy(skb_put(skb, size),
//				       txb->fragments[j]->data + hdr_len, size);
//			}
//			dev_kfree_skb_any(txb->fragments[i]);
//			txb->fragments[i] = skb;
//			tfd->u.data.chunk_ptr[i] =
//			    cpu_to_le32(pci_map_single
//					(priv->pci_dev, skb->data,
//					 remaining_bytes,
//					 PCI_DMA_TODEVICE));
//
//			le32_add_cpu(&tfd->u.data.num_chunks, 1);
//		}
//	}
//
//	/* kick DMA */
//	q->first_empty = ipw_queue_inc_wrap(q->first_empty, q->n_bd);
//	ipw_write32(priv, q->reg_w, q->first_empty);
//
//	if (ipw_tx_queue_space(q) < q->high_mark)
//		netif_stop_queue(priv->net_dev);
//
//	return NETDEV_TX_OK;
//
//      drop:
//	IPW_DEBUG_DROP("Silently dropping Tx packet.\n");
//	libipw_txb_free(txb);
//	return NETDEV_TX_OK;
//}
//
//static int ipw_net_is_queue_full(struct net_device *dev, int pri)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//#ifdef CONFIG_IPW2200_QOS
//	int tx_id = ipw_get_tx_queue_number(priv, pri);
//	struct clx2_tx_queue *txq = &priv->txq[tx_id];
//#else
//	struct clx2_tx_queue *txq = &priv->txq[0];
//#endif				/* CONFIG_IPW2200_QOS */
//
//	if (ipw_tx_queue_space(&txq->q) < txq->q.high_mark)
//		return 1;
//
//	return 0;
//}
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//static void ipw_handle_promiscuous_tx(struct ipw_priv *priv,
//				      struct libipw_txb *txb)
//{
//	struct libipw_rx_stats dummystats;
//	struct ieee80211_hdr *hdr;
//	u8 n;
//	u16 filter = priv->prom_priv->filter;
//	int hdr_only = 0;
//
//	if (filter & IPW_PROM_NO_TX)
//		return;
//
//	memset(&dummystats, 0, sizeof(dummystats));
//
//	/* Filtering of fragment chains is done agains the first fragment */
//	hdr = (void *)txb->fragments[0]->data;
//	if (libipw_is_management(le16_to_cpu(hdr->frame_control))) {
//		if (filter & IPW_PROM_NO_MGMT)
//			return;
//		if (filter & IPW_PROM_MGMT_HEADER_ONLY)
//			hdr_only = 1;
//	} else if (libipw_is_control(le16_to_cpu(hdr->frame_control))) {
//		if (filter & IPW_PROM_NO_CTL)
//			return;
//		if (filter & IPW_PROM_CTL_HEADER_ONLY)
//			hdr_only = 1;
//	} else if (libipw_is_data(le16_to_cpu(hdr->frame_control))) {
//		if (filter & IPW_PROM_NO_DATA)
//			return;
//		if (filter & IPW_PROM_DATA_HEADER_ONLY)
//			hdr_only = 1;
//	}
//
//	for(n=0; n<txb->nr_frags; ++n) {
//		struct sk_buff *src = txb->fragments[n];
//		struct sk_buff *dst;
//		struct ieee80211_radiotap_header *rt_hdr;
//		int len;
//
//		if (hdr_only) {
//			hdr = (void *)src->data;
//			len = libipw_get_hdrlen(le16_to_cpu(hdr->frame_control));
//		} else
//			len = src->len;
//
//		dst = alloc_skb(len + sizeof(*rt_hdr), GFP_ATOMIC);
//		if (!dst)
//			continue;
//
//		rt_hdr = (void *)skb_put(dst, sizeof(*rt_hdr));
//
//		rt_hdr->it_version = PKTHDR_RADIOTAP_VERSION;
//		rt_hdr->it_pad = 0;
//		rt_hdr->it_present = 0; /* after all, it's just an idea */
//		rt_hdr->it_present |=  cpu_to_le32(1 << IEEE80211_RADIOTAP_CHANNEL);
//
//		*(__le16*)skb_put(dst, sizeof(u16)) = cpu_to_le16(
//			ieee80211chan2mhz(priv->channel));
//		if (priv->channel > 14) 	/* 802.11a */
//			*(__le16*)skb_put(dst, sizeof(u16)) =
//				cpu_to_le16(IEEE80211_CHAN_OFDM |
//					     IEEE80211_CHAN_5GHZ);
//		else if (priv->ieee->mode == IEEE_B) /* 802.11b */
//			*(__le16*)skb_put(dst, sizeof(u16)) =
//				cpu_to_le16(IEEE80211_CHAN_CCK |
//					     IEEE80211_CHAN_2GHZ);
//		else 		/* 802.11g */
//			*(__le16*)skb_put(dst, sizeof(u16)) =
//				cpu_to_le16(IEEE80211_CHAN_OFDM |
//				 IEEE80211_CHAN_2GHZ);
//
//		rt_hdr->it_len = cpu_to_le16(dst->len);
//
//		skb_copy_from_linear_data(src, skb_put(dst, len), len);
//
//		if (!libipw_rx(priv->prom_priv->ieee, dst, &dummystats))
//			dev_kfree_skb_any(dst);
//	}
//}
//#endif
//
//static netdev_tx_t ipw_net_hard_start_xmit(struct libipw_txb *txb,
//					   struct net_device *dev, int pri)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	unsigned long flags;
//	netdev_tx_t ret;
//
//	IPW_DEBUG_TX("dev->xmit(%d bytes)\n", txb->payload_size);
//	spin_lock_irqsave(&priv->lock, flags);
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//	if (rtap_iface && netif_running(priv->prom_net_dev))
//		ipw_handle_promiscuous_tx(priv, txb);
//#endif
//
//	ret = ipw_tx_skb(priv, txb, pri);
//	if (ret == NETDEV_TX_OK)
//		__ipw_led_activity_on(priv);
//	spin_unlock_irqrestore(&priv->lock, flags);
//
//	return ret;
//}
//
//static void ipw_net_set_multicast_list(struct net_device *dev)
//{
//
//}
//
//static int ipw_net_set_mac_address(struct net_device *dev, void *p)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	struct sockaddr *addr = p;
//
//	if (!is_valid_ether_addr(addr->sa_data))
//		return -EADDRNOTAVAIL;
//	mutex_lock(&priv->mutex);
//	priv->config |= CFG_CUSTOM_MAC;
//	memcpy(priv->mac_addr, addr->sa_data, ETH_ALEN);
//	printk(KERN_INFO "%s: Setting MAC to %pM\n",
//	       priv->net_dev->name, priv->mac_addr);
//	queue_work(priv->workqueue, &priv->adapter_restart);
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
//static void ipw_ethtool_get_drvinfo(struct net_device *dev,
//				    struct ethtool_drvinfo *info)
//{
//	struct ipw_priv *p = libipw_priv(dev);
//	char vers[64];
//	char date[32];
//	u32 len;
//
//	strcpy(info->driver, DRV_NAME);
//	strcpy(info->version, DRV_VERSION);
//
//	len = sizeof(vers);
//	ipw_get_ordinal(p, IPW_ORD_STAT_FW_VERSION, vers, &len);
//	len = sizeof(date);
//	ipw_get_ordinal(p, IPW_ORD_STAT_FW_DATE, date, &len);
//
//	snprintf(info->fw_version, sizeof(info->fw_version), "%s (%s)",
//		 vers, date);
//	strcpy(info->bus_info, pci_name(p->pci_dev));
//	info->eedump_len = IPW_EEPROM_IMAGE_SIZE;
//}
//
//static u32 ipw_ethtool_get_link(struct net_device *dev)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	return (priv->status & STATUS_ASSOCIATED) != 0;
//}
//
//static int ipw_ethtool_get_eeprom_len(struct net_device *dev)
//{
//	return IPW_EEPROM_IMAGE_SIZE;
//}
//
//static int ipw_ethtool_get_eeprom(struct net_device *dev,
//				  struct ethtool_eeprom *eeprom, u8 * bytes)
//{
//	struct ipw_priv *p = libipw_priv(dev);
//
//	if (eeprom->offset + eeprom->len > IPW_EEPROM_IMAGE_SIZE)
//		return -EINVAL;
//	mutex_lock(&p->mutex);
//	memcpy(bytes, &p->eeprom[eeprom->offset], eeprom->len);
//	mutex_unlock(&p->mutex);
//	return 0;
//}
//
//static int ipw_ethtool_set_eeprom(struct net_device *dev,
//				  struct ethtool_eeprom *eeprom, u8 * bytes)
//{
//	struct ipw_priv *p = libipw_priv(dev);
//	int i;
//
//	if (eeprom->offset + eeprom->len > IPW_EEPROM_IMAGE_SIZE)
//		return -EINVAL;
//	mutex_lock(&p->mutex);
//	memcpy(&p->eeprom[eeprom->offset], bytes, eeprom->len);
//	for (i = 0; i < IPW_EEPROM_IMAGE_SIZE; i++)
//		ipw_write8(p, i + IPW_EEPROM_DATA, p->eeprom[i]);
//	mutex_unlock(&p->mutex);
//	return 0;
//}
//
//static const struct ethtool_ops ipw_ethtool_ops = {
//	.get_link = ipw_ethtool_get_link,
//	.get_drvinfo = ipw_ethtool_get_drvinfo,
//	.get_eeprom_len = ipw_ethtool_get_eeprom_len,
//	.get_eeprom = ipw_ethtool_get_eeprom,
//	.set_eeprom = ipw_ethtool_set_eeprom,
//};
//
//static irqreturn_t ipw_isr(int irq, void *data)
//{
//	struct ipw_priv *priv = data;
//	u32 inta, inta_mask;
//
//	if (!priv)
//		return IRQ_NONE;
//
//	spin_lock(&priv->irq_lock);
//
//	if (!(priv->status & STATUS_INT_ENABLED)) {
//		/* IRQ is disabled */
//		goto none;
//	}
//
//	inta = ipw_read32(priv, IPW_INTA_RW);
//	inta_mask = ipw_read32(priv, IPW_INTA_MASK_R);
//
//	if (inta == 0xFFFFFFFF) {
//		/* Hardware disappeared */
//		IPW_WARNING("IRQ INTA == 0xFFFFFFFF\n");
//		goto none;
//	}
//
//	if (!(inta & (IPW_INTA_MASK_ALL & inta_mask))) {
//		/* Shared interrupt */
//		goto none;
//	}
//
//	/* tell the device to stop sending interrupts */
//	__ipw_disable_interrupts(priv);
//
//	/* ack current interrupts */
//	inta &= (IPW_INTA_MASK_ALL & inta_mask);
//	ipw_write32(priv, IPW_INTA_RW, inta);
//
//	/* Cache INTA value for our tasklet */
//	priv->isr_inta = inta;
//
//	tasklet_schedule(&priv->irq_tasklet);
//
//	spin_unlock(&priv->irq_lock);
//
//	return IRQ_HANDLED;
//      none:
//	spin_unlock(&priv->irq_lock);
//	return IRQ_NONE;
//}
//
//static void ipw_rf_kill(void *adapter)
//{
//	struct ipw_priv *priv = adapter;
//	unsigned long flags;
//
//	spin_lock_irqsave(&priv->lock, flags);
//
//	if (rf_kill_active(priv)) {
//		IPW_DEBUG_RF_KILL("RF Kill active, rescheduling GPIO check\n");
//		if (priv->workqueue)
//			queue_delayed_work(priv->workqueue,
//					   &priv->rf_kill, 2 * HZ);
//		goto exit_unlock;
//	}
//
//	/* RF Kill is now disabled, so bring the device back up */
//
//	if (!(priv->status & STATUS_RF_KILL_MASK)) {
//		IPW_DEBUG_RF_KILL("HW RF Kill no longer active, restarting "
//				  "device\n");
//
//		/* we can not do an adapter restart while inside an irq lock */
//		queue_work(priv->workqueue, &priv->adapter_restart);
//	} else
//		IPW_DEBUG_RF_KILL("HW RF Kill deactivated.  SW RF Kill still "
//				  "enabled\n");
//
//      exit_unlock:
//	spin_unlock_irqrestore(&priv->lock, flags);
//}
//
//static void ipw_bg_rf_kill(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, rf_kill.work);
//	mutex_lock(&priv->mutex);
//	ipw_rf_kill(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static void ipw_link_up(struct ipw_priv *priv)
//{
//	priv->last_seq_num = -1;
//	priv->last_frag_num = -1;
//	priv->last_packet_time = 0;
//
//	netif_carrier_on(priv->net_dev);
//
//	cancel_delayed_work(&priv->request_scan);
//	cancel_delayed_work(&priv->request_direct_scan);
//	cancel_delayed_work(&priv->request_passive_scan);
//	cancel_delayed_work(&priv->scan_event);
//	ipw_reset_stats(priv);
//	/* Ensure the rate is updated immediately */
//	priv->last_rate = ipw_get_current_rate(priv);
//	ipw_gather_stats(priv);
//	ipw_led_link_up(priv);
//	notify_wx_assoc_event(priv);
//
//	if (priv->config & CFG_BACKGROUND_SCAN)
//		queue_delayed_work(priv->workqueue, &priv->request_scan, HZ);
//}
//
//static void ipw_bg_link_up(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, link_up);
//	mutex_lock(&priv->mutex);
//	ipw_link_up(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static void ipw_link_down(struct ipw_priv *priv)
//{
//	ipw_led_link_down(priv);
//	netif_carrier_off(priv->net_dev);
//	notify_wx_assoc_event(priv);
//
//	/* Cancel any queued work ... */
//	cancel_delayed_work(&priv->request_scan);
//	cancel_delayed_work(&priv->request_direct_scan);
//	cancel_delayed_work(&priv->request_passive_scan);
//	cancel_delayed_work(&priv->adhoc_check);
//	cancel_delayed_work(&priv->gather_stats);
//
//	ipw_reset_stats(priv);
//
//	if (!(priv->status & STATUS_EXIT_PENDING)) {
//		/* Queue up another scan... */
//		queue_delayed_work(priv->workqueue, &priv->request_scan, 0);
//	} else
//		cancel_delayed_work(&priv->scan_event);
//}
//
//static void ipw_bg_link_down(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, link_down);
//	mutex_lock(&priv->mutex);
//	ipw_link_down(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static int __devinit ipw_setup_deferred_work(struct ipw_priv *priv)
//{
//	int ret = 0;
//
//	priv->workqueue = create_workqueue(DRV_NAME);
//	init_waitqueue_head(&priv->wait_command_queue);
//	init_waitqueue_head(&priv->wait_state);
//
//	INIT_DELAYED_WORK(&priv->adhoc_check, ipw_bg_adhoc_check);
//	INIT_WORK(&priv->associate, ipw_bg_associate);
//	INIT_WORK(&priv->disassociate, ipw_bg_disassociate);
//	INIT_WORK(&priv->system_config, ipw_system_config);
//	INIT_WORK(&priv->rx_replenish, ipw_bg_rx_queue_replenish);
//	INIT_WORK(&priv->adapter_restart, ipw_bg_adapter_restart);
//	INIT_DELAYED_WORK(&priv->rf_kill, ipw_bg_rf_kill);
//	INIT_WORK(&priv->up, ipw_bg_up);
//	INIT_WORK(&priv->down, ipw_bg_down);
//	INIT_DELAYED_WORK(&priv->request_scan, ipw_request_scan);
//	INIT_DELAYED_WORK(&priv->request_direct_scan, ipw_request_direct_scan);
//	INIT_DELAYED_WORK(&priv->request_passive_scan, ipw_request_passive_scan);
//	INIT_DELAYED_WORK(&priv->scan_event, ipw_scan_event);
//	INIT_DELAYED_WORK(&priv->gather_stats, ipw_bg_gather_stats);
//	INIT_WORK(&priv->abort_scan, ipw_bg_abort_scan);
//	INIT_WORK(&priv->roam, ipw_bg_roam);
//	INIT_DELAYED_WORK(&priv->scan_check, ipw_bg_scan_check);
//	INIT_WORK(&priv->link_up, ipw_bg_link_up);
//	INIT_WORK(&priv->link_down, ipw_bg_link_down);
//	INIT_DELAYED_WORK(&priv->led_link_on, ipw_bg_led_link_on);
//	INIT_DELAYED_WORK(&priv->led_link_off, ipw_bg_led_link_off);
//	INIT_DELAYED_WORK(&priv->led_act_off, ipw_bg_led_activity_off);
//	INIT_WORK(&priv->merge_networks, ipw_merge_adhoc_network);
//
//#ifdef CONFIG_IPW2200_QOS
//	INIT_WORK(&priv->qos_activate, ipw_bg_qos_activate);
//#endif				/* CONFIG_IPW2200_QOS */
//
//	tasklet_init(&priv->irq_tasklet, (void (*)(unsigned long))
//		     ipw_irq_tasklet, (unsigned long)priv);
//
//	return ret;
//}
//
//static void shim__set_security(struct net_device *dev,
//			       struct libipw_security *sec)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	int i;
//	for (i = 0; i < 4; i++) {
//		if (sec->flags & (1 << i)) {
//			priv->ieee->sec.encode_alg[i] = sec->encode_alg[i];
//			priv->ieee->sec.key_sizes[i] = sec->key_sizes[i];
//			if (sec->key_sizes[i] == 0)
//				priv->ieee->sec.flags &= ~(1 << i);
//			else {
//				memcpy(priv->ieee->sec.keys[i], sec->keys[i],
//				       sec->key_sizes[i]);
//				priv->ieee->sec.flags |= (1 << i);
//			}
//			priv->status |= STATUS_SECURITY_UPDATED;
//		} else if (sec->level != SEC_LEVEL_1)
//			priv->ieee->sec.flags &= ~(1 << i);
//	}
//
//	if (sec->flags & SEC_ACTIVE_KEY) {
//		if (sec->active_key <= 3) {
//			priv->ieee->sec.active_key = sec->active_key;
//			priv->ieee->sec.flags |= SEC_ACTIVE_KEY;
//		} else
//			priv->ieee->sec.flags &= ~SEC_ACTIVE_KEY;
//		priv->status |= STATUS_SECURITY_UPDATED;
//	} else
//		priv->ieee->sec.flags &= ~SEC_ACTIVE_KEY;
//
//	if ((sec->flags & SEC_AUTH_MODE) &&
//	    (priv->ieee->sec.auth_mode != sec->auth_mode)) {
//		priv->ieee->sec.auth_mode = sec->auth_mode;
//		priv->ieee->sec.flags |= SEC_AUTH_MODE;
//		if (sec->auth_mode == WLAN_AUTH_SHARED_KEY)
//			priv->capability |= CAP_SHARED_KEY;
//		else
//			priv->capability &= ~CAP_SHARED_KEY;
//		priv->status |= STATUS_SECURITY_UPDATED;
//	}
//
//	if (sec->flags & SEC_ENABLED && priv->ieee->sec.enabled != sec->enabled) {
//		priv->ieee->sec.flags |= SEC_ENABLED;
//		priv->ieee->sec.enabled = sec->enabled;
//		priv->status |= STATUS_SECURITY_UPDATED;
//		if (sec->enabled)
//			priv->capability |= CAP_PRIVACY_ON;
//		else
//			priv->capability &= ~CAP_PRIVACY_ON;
//	}
//
//	if (sec->flags & SEC_ENCRYPT)
//		priv->ieee->sec.encrypt = sec->encrypt;
//
//	if (sec->flags & SEC_LEVEL && priv->ieee->sec.level != sec->level) {
//		priv->ieee->sec.level = sec->level;
//		priv->ieee->sec.flags |= SEC_LEVEL;
//		priv->status |= STATUS_SECURITY_UPDATED;
//	}
//
//	if (!priv->ieee->host_encrypt && (sec->flags & SEC_ENCRYPT))
//		ipw_set_hwcrypto_keys(priv);
//
//	/* To match current functionality of ipw2100 (which works well w/
//	 * various supplicants, we don't force a disassociate if the
//	 * privacy capability changes ... */
//#if 0
//	if ((priv->status & (STATUS_ASSOCIATED | STATUS_ASSOCIATING)) &&
//	    (((priv->assoc_request.capability &
//	       cpu_to_le16(WLAN_CAPABILITY_PRIVACY)) && !sec->enabled) ||
//	     (!(priv->assoc_request.capability &
//		cpu_to_le16(WLAN_CAPABILITY_PRIVACY)) && sec->enabled))) {
//		IPW_DEBUG_ASSOC("Disassociating due to capability "
//				"change.\n");
//		ipw_disassociate(priv);
//	}
//#endif
//}
//
//static int init_supported_rates(struct ipw_priv *priv,
//				struct ipw_supported_rates *rates)
//{
//	/* TODO: Mask out rates based on priv->rates_mask */
//
//	memset(rates, 0, sizeof(*rates));
//	/* configure supported rates */
//	switch (priv->ieee->freq_band) {
//	case LIBIPW_52GHZ_BAND:
//		rates->ieee_mode = IPW_A_MODE;
//		rates->purpose = IPW_RATE_CAPABILITIES;
//		ipw_add_ofdm_scan_rates(rates, LIBIPW_CCK_MODULATION,
//					LIBIPW_OFDM_DEFAULT_RATES_MASK);
//		break;
//
//	default:		/* Mixed or 2.4Ghz */
//		rates->ieee_mode = IPW_G_MODE;
//		rates->purpose = IPW_RATE_CAPABILITIES;
//		ipw_add_cck_scan_rates(rates, LIBIPW_CCK_MODULATION,
//				       LIBIPW_CCK_DEFAULT_RATES_MASK);
//		if (priv->ieee->modulation & LIBIPW_OFDM_MODULATION) {
//			ipw_add_ofdm_scan_rates(rates, LIBIPW_CCK_MODULATION,
//						LIBIPW_OFDM_DEFAULT_RATES_MASK);
//		}
//		break;
//	}
//
//	return 0;
//}
//
//static int ipw_config(struct ipw_priv *priv)
//{
//	/* This is only called from ipw_up, which resets/reloads the firmware
//	   so, we don't need to first disable the card before we configure
//	   it */
//	if (ipw_set_tx_power(priv))
//		goto error;
//
//	/* initialize adapter address */
//	if (ipw_send_adapter_address(priv, priv->net_dev->dev_addr))
//		goto error;
//
//	/* set basic system config settings */
//	init_sys_config(&priv->sys_config);
//
//	/* Support Bluetooth if we have BT h/w on board, and user wants to.
//	 * Does not support BT priority yet (don't abort or defer our Tx) */
//	if (bt_coexist) {
//		unsigned char bt_caps = priv->eeprom[EEPROM_SKU_CAPABILITY];
//
//		if (bt_caps & EEPROM_SKU_CAP_BT_CHANNEL_SIG)
//			priv->sys_config.bt_coexistence
//			    |= CFG_BT_COEXISTENCE_SIGNAL_CHNL;
//		if (bt_caps & EEPROM_SKU_CAP_BT_OOB)
//			priv->sys_config.bt_coexistence
//			    |= CFG_BT_COEXISTENCE_OOB;
//	}
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//	if (priv->prom_net_dev && netif_running(priv->prom_net_dev)) {
//		priv->sys_config.accept_all_data_frames = 1;
//		priv->sys_config.accept_non_directed_frames = 1;
//		priv->sys_config.accept_all_mgmt_bcpr = 1;
//		priv->sys_config.accept_all_mgmt_frames = 1;
//	}
//#endif
//
//	if (priv->ieee->iw_mode == IW_MODE_ADHOC)
//		priv->sys_config.answer_broadcast_ssid_probe = 1;
//	else
//		priv->sys_config.answer_broadcast_ssid_probe = 0;
//
//	if (ipw_send_system_config(priv))
//		goto error;
//
//	init_supported_rates(priv, &priv->rates);
//	if (ipw_send_supported_rates(priv, &priv->rates))
//		goto error;
//
//	/* Set request-to-send threshold */
//	if (priv->rts_threshold) {
//		if (ipw_send_rts_threshold(priv, priv->rts_threshold))
//			goto error;
//	}
//#ifdef CONFIG_IPW2200_QOS
//	IPW_DEBUG_QOS("QoS: call ipw_qos_activate\n");
//	ipw_qos_activate(priv, NULL);
//#endif				/* CONFIG_IPW2200_QOS */
//
//	if (ipw_set_random_seed(priv))
//		goto error;
//
//	/* final state transition to the RUN state */
//	if (ipw_send_host_complete(priv))
//		goto error;
//
//	priv->status |= STATUS_INIT;
//
//	ipw_led_init(priv);
//	ipw_led_radio_on(priv);
//	priv->notif_missed_beacons = 0;
//
//	/* Set hardware WEP key if it is configured. */
//	if ((priv->capability & CAP_PRIVACY_ON) &&
//	    (priv->ieee->sec.level == SEC_LEVEL_1) &&
//	    !(priv->ieee->host_encrypt || priv->ieee->host_decrypt))
//		ipw_set_hwcrypto_keys(priv);
//
//	return 0;
//
//      error:
//	return -EIO;
//}
//
///*
// * NOTE:
// *
// * These tables have been tested in conjunction with the
// * Intel PRO/Wireless 2200BG and 2915ABG Network Connection Adapters.
// *
// * Altering this values, using it on other hardware, or in geographies
// * not intended for resale of the above mentioned Intel adapters has
// * not been tested.
// *
// * Remember to update the table in README.ipw2200 when changing this
// * table.
// *
// */
//static const struct libipw_geo ipw_geos[] = {
//	{			/* Restricted */
//	 "---",
//	 .bg_channels = 11,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}},
//	 },
//
//	{			/* Custom US/Canada */
//	 "ZZF",
//	 .bg_channels = 11,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}},
//	 .a_channels = 8,
//	 .a = {{5180, 36},
//	       {5200, 40},
//	       {5220, 44},
//	       {5240, 48},
//	       {5260, 52, LIBIPW_CH_PASSIVE_ONLY},
//	       {5280, 56, LIBIPW_CH_PASSIVE_ONLY},
//	       {5300, 60, LIBIPW_CH_PASSIVE_ONLY},
//	       {5320, 64, LIBIPW_CH_PASSIVE_ONLY}},
//	 },
//
//	{			/* Rest of World */
//	 "ZZD",
//	 .bg_channels = 13,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}, {2467, 12},
//		{2472, 13}},
//	 },
//
//	{			/* Custom USA & Europe & High */
//	 "ZZA",
//	 .bg_channels = 11,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}},
//	 .a_channels = 13,
//	 .a = {{5180, 36},
//	       {5200, 40},
//	       {5220, 44},
//	       {5240, 48},
//	       {5260, 52, LIBIPW_CH_PASSIVE_ONLY},
//	       {5280, 56, LIBIPW_CH_PASSIVE_ONLY},
//	       {5300, 60, LIBIPW_CH_PASSIVE_ONLY},
//	       {5320, 64, LIBIPW_CH_PASSIVE_ONLY},
//	       {5745, 149},
//	       {5765, 153},
//	       {5785, 157},
//	       {5805, 161},
//	       {5825, 165}},
//	 },
//
//	{			/* Custom NA & Europe */
//	 "ZZB",
//	 .bg_channels = 11,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}},
//	 .a_channels = 13,
//	 .a = {{5180, 36},
//	       {5200, 40},
//	       {5220, 44},
//	       {5240, 48},
//	       {5260, 52, LIBIPW_CH_PASSIVE_ONLY},
//	       {5280, 56, LIBIPW_CH_PASSIVE_ONLY},
//	       {5300, 60, LIBIPW_CH_PASSIVE_ONLY},
//	       {5320, 64, LIBIPW_CH_PASSIVE_ONLY},
//	       {5745, 149, LIBIPW_CH_PASSIVE_ONLY},
//	       {5765, 153, LIBIPW_CH_PASSIVE_ONLY},
//	       {5785, 157, LIBIPW_CH_PASSIVE_ONLY},
//	       {5805, 161, LIBIPW_CH_PASSIVE_ONLY},
//	       {5825, 165, LIBIPW_CH_PASSIVE_ONLY}},
//	 },
//
//	{			/* Custom Japan */
//	 "ZZC",
//	 .bg_channels = 11,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}},
//	 .a_channels = 4,
//	 .a = {{5170, 34}, {5190, 38},
//	       {5210, 42}, {5230, 46}},
//	 },
//
//	{			/* Custom */
//	 "ZZM",
//	 .bg_channels = 11,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}},
//	 },
//
//	{			/* Europe */
//	 "ZZE",
//	 .bg_channels = 13,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}, {2467, 12},
//		{2472, 13}},
//	 .a_channels = 19,
//	 .a = {{5180, 36},
//	       {5200, 40},
//	       {5220, 44},
//	       {5240, 48},
//	       {5260, 52, LIBIPW_CH_PASSIVE_ONLY},
//	       {5280, 56, LIBIPW_CH_PASSIVE_ONLY},
//	       {5300, 60, LIBIPW_CH_PASSIVE_ONLY},
//	       {5320, 64, LIBIPW_CH_PASSIVE_ONLY},
//	       {5500, 100, LIBIPW_CH_PASSIVE_ONLY},
//	       {5520, 104, LIBIPW_CH_PASSIVE_ONLY},
//	       {5540, 108, LIBIPW_CH_PASSIVE_ONLY},
//	       {5560, 112, LIBIPW_CH_PASSIVE_ONLY},
//	       {5580, 116, LIBIPW_CH_PASSIVE_ONLY},
//	       {5600, 120, LIBIPW_CH_PASSIVE_ONLY},
//	       {5620, 124, LIBIPW_CH_PASSIVE_ONLY},
//	       {5640, 128, LIBIPW_CH_PASSIVE_ONLY},
//	       {5660, 132, LIBIPW_CH_PASSIVE_ONLY},
//	       {5680, 136, LIBIPW_CH_PASSIVE_ONLY},
//	       {5700, 140, LIBIPW_CH_PASSIVE_ONLY}},
//	 },
//
//	{			/* Custom Japan */
//	 "ZZJ",
//	 .bg_channels = 14,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}, {2467, 12},
//		{2472, 13}, {2484, 14, LIBIPW_CH_B_ONLY}},
//	 .a_channels = 4,
//	 .a = {{5170, 34}, {5190, 38},
//	       {5210, 42}, {5230, 46}},
//	 },
//
//	{			/* Rest of World */
//	 "ZZR",
//	 .bg_channels = 14,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}, {2467, 12},
//		{2472, 13}, {2484, 14, LIBIPW_CH_B_ONLY |
//			     LIBIPW_CH_PASSIVE_ONLY}},
//	 },
//
//	{			/* High Band */
//	 "ZZH",
//	 .bg_channels = 13,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11},
//		{2467, 12, LIBIPW_CH_PASSIVE_ONLY},
//		{2472, 13, LIBIPW_CH_PASSIVE_ONLY}},
//	 .a_channels = 4,
//	 .a = {{5745, 149}, {5765, 153},
//	       {5785, 157}, {5805, 161}},
//	 },
//
//	{			/* Custom Europe */
//	 "ZZG",
//	 .bg_channels = 13,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11},
//		{2467, 12}, {2472, 13}},
//	 .a_channels = 4,
//	 .a = {{5180, 36}, {5200, 40},
//	       {5220, 44}, {5240, 48}},
//	 },
//
//	{			/* Europe */
//	 "ZZK",
//	 .bg_channels = 13,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11},
//		{2467, 12, LIBIPW_CH_PASSIVE_ONLY},
//		{2472, 13, LIBIPW_CH_PASSIVE_ONLY}},
//	 .a_channels = 24,
//	 .a = {{5180, 36, LIBIPW_CH_PASSIVE_ONLY},
//	       {5200, 40, LIBIPW_CH_PASSIVE_ONLY},
//	       {5220, 44, LIBIPW_CH_PASSIVE_ONLY},
//	       {5240, 48, LIBIPW_CH_PASSIVE_ONLY},
//	       {5260, 52, LIBIPW_CH_PASSIVE_ONLY},
//	       {5280, 56, LIBIPW_CH_PASSIVE_ONLY},
//	       {5300, 60, LIBIPW_CH_PASSIVE_ONLY},
//	       {5320, 64, LIBIPW_CH_PASSIVE_ONLY},
//	       {5500, 100, LIBIPW_CH_PASSIVE_ONLY},
//	       {5520, 104, LIBIPW_CH_PASSIVE_ONLY},
//	       {5540, 108, LIBIPW_CH_PASSIVE_ONLY},
//	       {5560, 112, LIBIPW_CH_PASSIVE_ONLY},
//	       {5580, 116, LIBIPW_CH_PASSIVE_ONLY},
//	       {5600, 120, LIBIPW_CH_PASSIVE_ONLY},
//	       {5620, 124, LIBIPW_CH_PASSIVE_ONLY},
//	       {5640, 128, LIBIPW_CH_PASSIVE_ONLY},
//	       {5660, 132, LIBIPW_CH_PASSIVE_ONLY},
//	       {5680, 136, LIBIPW_CH_PASSIVE_ONLY},
//	       {5700, 140, LIBIPW_CH_PASSIVE_ONLY},
//	       {5745, 149, LIBIPW_CH_PASSIVE_ONLY},
//	       {5765, 153, LIBIPW_CH_PASSIVE_ONLY},
//	       {5785, 157, LIBIPW_CH_PASSIVE_ONLY},
//	       {5805, 161, LIBIPW_CH_PASSIVE_ONLY},
//	       {5825, 165, LIBIPW_CH_PASSIVE_ONLY}},
//	 },
//
//	{			/* Europe */
//	 "ZZL",
//	 .bg_channels = 11,
//	 .bg = {{2412, 1}, {2417, 2}, {2422, 3},
//		{2427, 4}, {2432, 5}, {2437, 6},
//		{2442, 7}, {2447, 8}, {2452, 9},
//		{2457, 10}, {2462, 11}},
//	 .a_channels = 13,
//	 .a = {{5180, 36, LIBIPW_CH_PASSIVE_ONLY},
//	       {5200, 40, LIBIPW_CH_PASSIVE_ONLY},
//	       {5220, 44, LIBIPW_CH_PASSIVE_ONLY},
//	       {5240, 48, LIBIPW_CH_PASSIVE_ONLY},
//	       {5260, 52, LIBIPW_CH_PASSIVE_ONLY},
//	       {5280, 56, LIBIPW_CH_PASSIVE_ONLY},
//	       {5300, 60, LIBIPW_CH_PASSIVE_ONLY},
//	       {5320, 64, LIBIPW_CH_PASSIVE_ONLY},
//	       {5745, 149, LIBIPW_CH_PASSIVE_ONLY},
//	       {5765, 153, LIBIPW_CH_PASSIVE_ONLY},
//	       {5785, 157, LIBIPW_CH_PASSIVE_ONLY},
//	       {5805, 161, LIBIPW_CH_PASSIVE_ONLY},
//	       {5825, 165, LIBIPW_CH_PASSIVE_ONLY}},
//	 }
//};
//
//#define MAX_HW_RESTARTS 5
//static int ipw_up(struct ipw_priv *priv)
//{
//	int rc, i, j;
//
//	/* Age scan list entries found before suspend */
//	if (priv->suspend_time) {
//		libipw_networks_age(priv->ieee, priv->suspend_time);
//		priv->suspend_time = 0;
//	}
//
//	if (priv->status & STATUS_EXIT_PENDING)
//		return -EIO;
//
//	if (cmdlog && !priv->cmdlog) {
//		priv->cmdlog = kcalloc(cmdlog, sizeof(*priv->cmdlog),
//				       GFP_KERNEL);
//		if (priv->cmdlog == NULL) {
//			IPW_ERROR("Error allocating %d command log entries.\n",
//				  cmdlog);
//			return -ENOMEM;
//		} else {
//			priv->cmdlog_len = cmdlog;
//		}
//	}
//
//	for (i = 0; i < MAX_HW_RESTARTS; i++) {
//		/* Load the microcode, firmware, and eeprom.
//		 * Also start the clocks. */
//		rc = ipw_load(priv);
//		if (rc) {
//			IPW_ERROR("Unable to load firmware: %d\n", rc);
//			return rc;
//		}
//
//		ipw_init_ordinals(priv);
//		if (!(priv->config & CFG_CUSTOM_MAC))
//			eeprom_parse_mac(priv, priv->mac_addr);
//		memcpy(priv->net_dev->dev_addr, priv->mac_addr, ETH_ALEN);
//
//		for (j = 0; j < ARRAY_SIZE(ipw_geos); j++) {
//			if (!memcmp(&priv->eeprom[EEPROM_COUNTRY_CODE],
//				    ipw_geos[j].name, 3))
//				break;
//		}
//		if (j == ARRAY_SIZE(ipw_geos)) {
//			IPW_WARNING("SKU [%c%c%c] not recognized.\n",
//				    priv->eeprom[EEPROM_COUNTRY_CODE + 0],
//				    priv->eeprom[EEPROM_COUNTRY_CODE + 1],
//				    priv->eeprom[EEPROM_COUNTRY_CODE + 2]);
//			j = 0;
//		}
//		if (libipw_set_geo(priv->ieee, &ipw_geos[j])) {
//			IPW_WARNING("Could not set geography.");
//			return 0;
//		}
//
//		if (priv->status & STATUS_RF_KILL_SW) {
//			IPW_WARNING("Radio disabled by module parameter.\n");
//			return 0;
//		} else if (rf_kill_active(priv)) {
//			IPW_WARNING("Radio Frequency Kill Switch is On:\n"
//				    "Kill switch must be turned off for "
//				    "wireless networking to work.\n");
//			queue_delayed_work(priv->workqueue, &priv->rf_kill,
//					   2 * HZ);
//			return 0;
//		}
//
//		rc = ipw_config(priv);
//		if (!rc) {
//			IPW_DEBUG_INFO("Configured device on count %i\n", i);
//
//			/* If configure to try and auto-associate, kick
//			 * off a scan. */
//			queue_delayed_work(priv->workqueue,
//					   &priv->request_scan, 0);
//
//			return 0;
//		}
//
//		IPW_DEBUG_INFO("Device configuration failed: 0x%08X\n", rc);
//		IPW_DEBUG_INFO("Failed to config device on retry %d of %d\n",
//			       i, MAX_HW_RESTARTS);
//
//		/* We had an error bringing up the hardware, so take it
//		 * all the way back down so we can try again */
//		ipw_down(priv);
//	}
//
//	/* tried to restart and config the device for as long as our
//	 * patience could withstand */
//	IPW_ERROR("Unable to initialize device after %d attempts.\n", i);
//
//	return -EIO;
//}
//
//static void ipw_bg_up(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, up);
//	mutex_lock(&priv->mutex);
//	ipw_up(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static void ipw_deinit(struct ipw_priv *priv)
//{
//	int i;
//
//	if (priv->status & STATUS_SCANNING) {
//		IPW_DEBUG_INFO("Aborting scan during shutdown.\n");
//		ipw_abort_scan(priv);
//	}
//
//	if (priv->status & STATUS_ASSOCIATED) {
//		IPW_DEBUG_INFO("Disassociating during shutdown.\n");
//		ipw_disassociate(priv);
//	}
//
//	ipw_led_shutdown(priv);
//
//	/* Wait up to 1s for status to change to not scanning and not
//	 * associated (disassociation can take a while for a ful 802.11
//	 * exchange */
//	for (i = 1000; i && (priv->status &
//			     (STATUS_DISASSOCIATING |
//			      STATUS_ASSOCIATED | STATUS_SCANNING)); i--)
//		udelay(10);
//
//	if (priv->status & (STATUS_DISASSOCIATING |
//			    STATUS_ASSOCIATED | STATUS_SCANNING))
//		IPW_DEBUG_INFO("Still associated or scanning...\n");
//	else
//		IPW_DEBUG_INFO("Took %dms to de-init\n", 1000 - i);
//
//	/* Attempt to disable the card */
//	ipw_send_card_disable(priv, 0);
//
//	priv->status &= ~STATUS_INIT;
//}
//
//static void ipw_down(struct ipw_priv *priv)
//{
//	int exit_pending = priv->status & STATUS_EXIT_PENDING;
//
//	priv->status |= STATUS_EXIT_PENDING;
//
//	if (ipw_is_init(priv))
//		ipw_deinit(priv);
//
//	/* Wipe out the EXIT_PENDING status bit if we are not actually
//	 * exiting the module */
//	if (!exit_pending)
//		priv->status &= ~STATUS_EXIT_PENDING;
//
//	/* tell the device to stop sending interrupts */
//	ipw_disable_interrupts(priv);
//
//	/* Clear all bits but the RF Kill */
//	priv->status &= STATUS_RF_KILL_MASK | STATUS_EXIT_PENDING;
//	netif_carrier_off(priv->net_dev);
//
//	ipw_stop_nic(priv);
//
//	ipw_led_radio_off(priv);
//}
//
//static void ipw_bg_down(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, down);
//	mutex_lock(&priv->mutex);
//	ipw_down(priv);
//	mutex_unlock(&priv->mutex);
//}
//
///* Called by register_netdev() */
//static int ipw_net_init(struct net_device *dev)
//{
//	struct ipw_priv *priv = libipw_priv(dev);
//	mutex_lock(&priv->mutex);
//
//	if (ipw_up(priv)) {
//		mutex_unlock(&priv->mutex);
//		return -EIO;
//	}
//
//	mutex_unlock(&priv->mutex);
//	return 0;
//}
//
///* PCI driver stuff */
//static struct pci_device_id card_ids[] = {
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2701, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2702, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2711, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2712, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2721, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2722, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2731, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2732, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2741, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x103c, 0x2741, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2742, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2751, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2752, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2753, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2754, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2761, 0, 0, 0},
//	{PCI_VENDOR_ID_INTEL, 0x1043, 0x8086, 0x2762, 0, 0, 0},
//	{PCI_VDEVICE(INTEL, 0x104f), 0},
//	{PCI_VDEVICE(INTEL, 0x4220), 0},	/* BG */
//	{PCI_VDEVICE(INTEL, 0x4221), 0},	/* BG */
//	{PCI_VDEVICE(INTEL, 0x4223), 0},	/* ABG */
//	{PCI_VDEVICE(INTEL, 0x4224), 0},	/* ABG */
//
//	/* required last entry */
//	{0,}
//};
//
//MODULE_DEVICE_TABLE(pci, card_ids);
//
//static struct attribute *ipw_sysfs_entries[] = {
//	&dev_attr_rf_kill.attr,
//	&dev_attr_direct_dword.attr,
//	&dev_attr_indirect_byte.attr,
//	&dev_attr_indirect_dword.attr,
//	&dev_attr_mem_gpio_reg.attr,
//	&dev_attr_command_event_reg.attr,
//	&dev_attr_nic_type.attr,
//	&dev_attr_status.attr,
//	&dev_attr_cfg.attr,
//	&dev_attr_error.attr,
//	&dev_attr_event_log.attr,
//	&dev_attr_cmd_log.attr,
//	&dev_attr_eeprom_delay.attr,
//	&dev_attr_ucode_version.attr,
//	&dev_attr_rtc.attr,
//	&dev_attr_scan_age.attr,
//	&dev_attr_led.attr,
//	&dev_attr_speed_scan.attr,
//	&dev_attr_net_stats.attr,
//	&dev_attr_channels.attr,
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//	&dev_attr_rtap_iface.attr,
//	&dev_attr_rtap_filter.attr,
//#endif
//	NULL
//};
//
//static struct attribute_group ipw_attribute_group = {
//	.name = NULL,		/* put in device directory */
//	.attrs = ipw_sysfs_entries,
//};
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//static int ipw_prom_open(struct net_device *dev)
//{
//	struct ipw_prom_priv *prom_priv = libipw_priv(dev);
//	struct ipw_priv *priv = prom_priv->priv;
//
//	IPW_DEBUG_INFO("prom dev->open\n");
//	netif_carrier_off(dev);
//
//	if (priv->ieee->iw_mode != IW_MODE_MONITOR) {
//		priv->sys_config.accept_all_data_frames = 1;
//		priv->sys_config.accept_non_directed_frames = 1;
//		priv->sys_config.accept_all_mgmt_bcpr = 1;
//		priv->sys_config.accept_all_mgmt_frames = 1;
//
//		ipw_send_system_config(priv);
//	}
//
//	return 0;
//}
//
//static int ipw_prom_stop(struct net_device *dev)
//{
//	struct ipw_prom_priv *prom_priv = libipw_priv(dev);
//	struct ipw_priv *priv = prom_priv->priv;
//
//	IPW_DEBUG_INFO("prom dev->stop\n");
//
//	if (priv->ieee->iw_mode != IW_MODE_MONITOR) {
//		priv->sys_config.accept_all_data_frames = 0;
//		priv->sys_config.accept_non_directed_frames = 0;
//		priv->sys_config.accept_all_mgmt_bcpr = 0;
//		priv->sys_config.accept_all_mgmt_frames = 0;
//
//		ipw_send_system_config(priv);
//	}
//
//	return 0;
//}
//
//static netdev_tx_t ipw_prom_hard_start_xmit(struct sk_buff *skb,
//					    struct net_device *dev)
//{
//	IPW_DEBUG_INFO("prom dev->xmit\n");
//	dev_kfree_skb(skb);
//	return NETDEV_TX_OK;
//}
//
//static const struct net_device_ops ipw_prom_netdev_ops = {
//	.ndo_open 		= ipw_prom_open,
//	.ndo_stop		= ipw_prom_stop,
//	.ndo_start_xmit		= ipw_prom_hard_start_xmit,
//	.ndo_change_mtu		= libipw_change_mtu,
//	.ndo_set_mac_address 	= eth_mac_addr,
//	.ndo_validate_addr	= eth_validate_addr,
//};
//
//static int ipw_prom_alloc(struct ipw_priv *priv)
//{
//	int rc = 0;
//
//	if (priv->prom_net_dev)
//		return -EPERM;
//
//	priv->prom_net_dev = alloc_ieee80211(sizeof(struct ipw_prom_priv));
//	if (priv->prom_net_dev == NULL)
//		return -ENOMEM;
//
//	priv->prom_priv = libipw_priv(priv->prom_net_dev);
//	priv->prom_priv->ieee = netdev_priv(priv->prom_net_dev);
//	priv->prom_priv->priv = priv;
//
//	strcpy(priv->prom_net_dev->name, "rtap%d");
//	memcpy(priv->prom_net_dev->dev_addr, priv->mac_addr, ETH_ALEN);
//
//	priv->prom_net_dev->type = ARPHRD_IEEE80211_RADIOTAP;
//	priv->prom_net_dev->netdev_ops = &ipw_prom_netdev_ops;
//
//	priv->prom_priv->ieee->iw_mode = IW_MODE_MONITOR;
//	SET_NETDEV_DEV(priv->prom_net_dev, &priv->pci_dev->dev);
//
//	rc = register_netdev(priv->prom_net_dev);
//	if (rc) {
//		free_ieee80211(priv->prom_net_dev);
//		priv->prom_net_dev = NULL;
//		return rc;
//	}
//
//	return 0;
//}
//
//static void ipw_prom_free(struct ipw_priv *priv)
//{
//	if (!priv->prom_net_dev)
//		return;
//
//	unregister_netdev(priv->prom_net_dev);
//	free_ieee80211(priv->prom_net_dev);
//
//	priv->prom_net_dev = NULL;
//}
//
//#endif
//
//static const struct net_device_ops ipw_netdev_ops = {
//	.ndo_init		= ipw_net_init,
//	.ndo_open		= ipw_net_open,
//	.ndo_stop		= ipw_net_stop,
//	.ndo_set_multicast_list	= ipw_net_set_multicast_list,
//	.ndo_set_mac_address	= ipw_net_set_mac_address,
//	.ndo_start_xmit		= libipw_xmit,
//	.ndo_change_mtu		= libipw_change_mtu,
//	.ndo_validate_addr	= eth_validate_addr,
//};
//
//static int __devinit ipw_pci_probe(struct pci_dev *pdev,
//				   const struct pci_device_id *ent)
//{
//	int err = 0;
//	struct net_device *net_dev;
//	void __iomem *base;
//	u32 length, val;
//	struct ipw_priv *priv;
//	int i;
//
//	net_dev = alloc_ieee80211(sizeof(struct ipw_priv));
//	if (net_dev == NULL) {
//		err = -ENOMEM;
//		goto out;
//	}
//
//	priv = libipw_priv(net_dev);
//	priv->ieee = netdev_priv(net_dev);
//
//	priv->net_dev = net_dev;
//	priv->pci_dev = pdev;
//	ipw_debug_level = debug;
//	spin_lock_init(&priv->irq_lock);
//	spin_lock_init(&priv->lock);
//	for (i = 0; i < IPW_IBSS_MAC_HASH_SIZE; i++)
//		INIT_LIST_HEAD(&priv->ibss_mac_hash[i]);
//
//	mutex_init(&priv->mutex);
//	if (pci_enable_device(pdev)) {
//		err = -ENODEV;
//		goto out_free_ieee80211;
//	}
//
//	pci_set_master(pdev);
//
//	err = pci_set_dma_mask(pdev, DMA_BIT_MASK(32));
//	if (!err)
//		err = pci_set_consistent_dma_mask(pdev, DMA_BIT_MASK(32));
//	if (err) {
//		printk(KERN_WARNING DRV_NAME ": No suitable DMA available.\n");
//		goto out_pci_disable_device;
//	}
//
//	pci_set_drvdata(pdev, priv);
//
//	err = pci_request_regions(pdev, DRV_NAME);
//	if (err)
//		goto out_pci_disable_device;
//
//	/* We disable the RETRY_TIMEOUT register (0x41) to keep
//	 * PCI Tx retries from interfering with C3 CPU state */
//	pci_read_config_dword(pdev, 0x40, &val);
//	if ((val & 0x0000ff00) != 0)
//		pci_write_config_dword(pdev, 0x40, val & 0xffff00ff);
//
//	length = pci_resource_len(pdev, 0);
//	priv->hw_len = length;
//
//	base = pci_ioremap_bar(pdev, 0);
//	if (!base) {
//		err = -ENODEV;
//		goto out_pci_release_regions;
//	}
//
//	priv->hw_base = base;
//	IPW_DEBUG_INFO("pci_resource_len = 0x%08x\n", length);
//	IPW_DEBUG_INFO("pci_resource_base = %p\n", base);
//
//	err = ipw_setup_deferred_work(priv);
//	if (err) {
//		IPW_ERROR("Unable to setup deferred work\n");
//		goto out_iounmap;
//	}
//
//	ipw_sw_reset(priv, 1);
//
//	err = request_irq(pdev->irq, ipw_isr, IRQF_SHARED, DRV_NAME, priv);
//	if (err) {
//		IPW_ERROR("Error allocating IRQ %d\n", pdev->irq);
//		goto out_destroy_workqueue;
//	}
//
//	SET_NETDEV_DEV(net_dev, &pdev->dev);
//
//	mutex_lock(&priv->mutex);
//
//	priv->ieee->hard_start_xmit = ipw_net_hard_start_xmit;
//	priv->ieee->set_security = shim__set_security;
//	priv->ieee->is_queue_full = ipw_net_is_queue_full;
//
//#ifdef CONFIG_IPW2200_QOS
//	priv->ieee->is_qos_active = ipw_is_qos_active;
//	priv->ieee->handle_probe_response = ipw_handle_beacon;
//	priv->ieee->handle_beacon = ipw_handle_probe_response;
//	priv->ieee->handle_assoc_response = ipw_handle_assoc_response;
//#endif				/* CONFIG_IPW2200_QOS */
//
//	priv->ieee->perfect_rssi = -20;
//	priv->ieee->worst_rssi = -85;
//
//	net_dev->netdev_ops = &ipw_netdev_ops;
//	priv->wireless_data.spy_data = &priv->ieee->spy_data;
//	net_dev->wireless_data = &priv->wireless_data;
//	net_dev->wireless_handlers = &ipw_wx_handler_def;
//	net_dev->ethtool_ops = &ipw_ethtool_ops;
//	net_dev->irq = pdev->irq;
//	net_dev->base_addr = (unsigned long)priv->hw_base;
//	net_dev->mem_start = pci_resource_start(pdev, 0);
//	net_dev->mem_end = net_dev->mem_start + pci_resource_len(pdev, 0) - 1;
//
//	err = sysfs_create_group(&pdev->dev.kobj, &ipw_attribute_group);
//	if (err) {
//		IPW_ERROR("failed to create sysfs device attributes\n");
//		mutex_unlock(&priv->mutex);
//		goto out_release_irq;
//	}
//
//	mutex_unlock(&priv->mutex);
//	err = register_netdev(net_dev);
//	if (err) {
//		IPW_ERROR("failed to register network device\n");
//		goto out_remove_sysfs;
//	}
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//	if (rtap_iface) {
//	        err = ipw_prom_alloc(priv);
//		if (err) {
//			IPW_ERROR("Failed to register promiscuous network "
//				  "device (error %d).\n", err);
//			unregister_netdev(priv->net_dev);
//			goto out_remove_sysfs;
//		}
//	}
//#endif
//
//	printk(KERN_INFO DRV_NAME ": Detected geography %s (%d 802.11bg "
//	       "channels, %d 802.11a channels)\n",
//	       priv->ieee->geo.name, priv->ieee->geo.bg_channels,
//	       priv->ieee->geo.a_channels);
//
//	return 0;
//
//      out_remove_sysfs:
//	sysfs_remove_group(&pdev->dev.kobj, &ipw_attribute_group);
//      out_release_irq:
//	free_irq(pdev->irq, priv);
//      out_destroy_workqueue:
//	destroy_workqueue(priv->workqueue);
//	priv->workqueue = NULL;
//      out_iounmap:
//	iounmap(priv->hw_base);
//      out_pci_release_regions:
//	pci_release_regions(pdev);
//      out_pci_disable_device:
//	pci_disable_device(pdev);
//	pci_set_drvdata(pdev, NULL);
//      out_free_ieee80211:
//	free_ieee80211(priv->net_dev);
//      out:
//	return err;
//}
//
//static void __devexit ipw_pci_remove(struct pci_dev *pdev)
//{
//	struct ipw_priv *priv = pci_get_drvdata(pdev);
//	struct list_head *p, *q;
//	int i;
//
//	if (!priv)
//		return;
//
//	mutex_lock(&priv->mutex);
//
//	priv->status |= STATUS_EXIT_PENDING;
//	ipw_down(priv);
//	sysfs_remove_group(&pdev->dev.kobj, &ipw_attribute_group);
//
//	mutex_unlock(&priv->mutex);
//
//	unregister_netdev(priv->net_dev);
//
//	if (priv->rxq) {
//		ipw_rx_queue_free(priv, priv->rxq);
//		priv->rxq = NULL;
//	}
//	ipw_tx_queue_free(priv);
//
//	if (priv->cmdlog) {
//		kfree(priv->cmdlog);
//		priv->cmdlog = NULL;
//	}
//	/* ipw_down will ensure that there is no more pending work
//	 * in the workqueue's, so we can safely remove them now. */
//	cancel_delayed_work(&priv->adhoc_check);
//	cancel_delayed_work(&priv->gather_stats);
//	cancel_delayed_work(&priv->request_scan);
//	cancel_delayed_work(&priv->request_direct_scan);
//	cancel_delayed_work(&priv->request_passive_scan);
//	cancel_delayed_work(&priv->scan_event);
//	cancel_delayed_work(&priv->rf_kill);
//	cancel_delayed_work(&priv->scan_check);
//	destroy_workqueue(priv->workqueue);
//	priv->workqueue = NULL;
//
//	/* Free MAC hash list for ADHOC */
//	for (i = 0; i < IPW_IBSS_MAC_HASH_SIZE; i++) {
//		list_for_each_safe(p, q, &priv->ibss_mac_hash[i]) {
//			list_del(p);
//			kfree(list_entry(p, struct ipw_ibss_seq, list));
//		}
//	}
//
//	kfree(priv->error);
//	priv->error = NULL;
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//	ipw_prom_free(priv);
//#endif
//
//	free_irq(pdev->irq, priv);
//	iounmap(priv->hw_base);
//	pci_release_regions(pdev);
//	pci_disable_device(pdev);
//	pci_set_drvdata(pdev, NULL);
//	free_ieee80211(priv->net_dev);
//	free_firmware();
//}
//
//#ifdef CONFIG_PM
//static int ipw_pci_suspend(struct pci_dev *pdev, pm_message_t state)
//{
//	struct ipw_priv *priv = pci_get_drvdata(pdev);
//	struct net_device *dev = priv->net_dev;
//
//	printk(KERN_INFO "%s: Going into suspend...\n", dev->name);
//
//	/* Take down the device; powers it off, etc. */
//	ipw_down(priv);
//
//	/* Remove the PRESENT state of the device */
//	netif_device_detach(dev);
//
//	pci_save_state(pdev);
//	pci_disable_device(pdev);
//	pci_set_power_state(pdev, pci_choose_state(pdev, state));
//
//	priv->suspend_at = get_seconds();
//
//	return 0;
//}
//
//static int ipw_pci_resume(struct pci_dev *pdev)
//{
//	struct ipw_priv *priv = pci_get_drvdata(pdev);
//	struct net_device *dev = priv->net_dev;
//	int err;
//	u32 val;
//
//	printk(KERN_INFO "%s: Coming out of suspend...\n", dev->name);
//
//	pci_set_power_state(pdev, PCI_D0);
//	err = pci_enable_device(pdev);
//	if (err) {
//		printk(KERN_ERR "%s: pci_enable_device failed on resume\n",
//		       dev->name);
//		return err;
//	}
//	pci_restore_state(pdev);
//
//	/*
//	 * Suspend/Resume resets the PCI configuration space, so we have to
//	 * re-disable the RETRY_TIMEOUT register (0x41) to keep PCI Tx retries
//	 * from interfering with C3 CPU state. pci_restore_state won't help
//	 * here since it only restores the first 64 bytes pci config header.
//	 */
//	pci_read_config_dword(pdev, 0x40, &val);
//	if ((val & 0x0000ff00) != 0)
//		pci_write_config_dword(pdev, 0x40, val & 0xffff00ff);
//
//	/* Set the device back into the PRESENT state; this will also wake
//	 * the queue of needed */
//	netif_device_attach(dev);
//
//	priv->suspend_time = get_seconds() - priv->suspend_at;
//
//	/* Bring the device back up */
//	queue_work(priv->workqueue, &priv->up);
//
//	return 0;
//}
//#endif
//
//static void ipw_pci_shutdown(struct pci_dev *pdev)
//{
//	struct ipw_priv *priv = pci_get_drvdata(pdev);
//
//	/* Take down the device; powers it off, etc. */
//	ipw_down(priv);
//
//	pci_disable_device(pdev);
//}
//
///* driver initialization stuff */
//static struct pci_driver ipw_driver = {
//	.name = DRV_NAME,
//	.id_table = card_ids,
//	.probe = ipw_pci_probe,
//	.remove = __devexit_p(ipw_pci_remove),
//#ifdef CONFIG_PM
//	.suspend = ipw_pci_suspend,
//	.resume = ipw_pci_resume,
//#endif
//	.shutdown = ipw_pci_shutdown,
//};
//
//static int __init ipw_init(void)
//{
//	int ret;
//
//	printk(KERN_INFO DRV_NAME ": " DRV_DESCRIPTION ", " DRV_VERSION "\n");
//	printk(KERN_INFO DRV_NAME ": " DRV_COPYRIGHT "\n");
//
//	ret = pci_register_driver(&ipw_driver);
//	if (ret) {
//		IPW_ERROR("Unable to initialize PCI module\n");
//		return ret;
//	}
//
//	ret = driver_create_file(&ipw_driver.driver, &driver_attr_debug_level);
//	if (ret) {
//		IPW_ERROR("Unable to create driver sysfs file\n");
//		pci_unregister_driver(&ipw_driver);
//		return ret;
//	}
//
//	return ret;
//}
//
//static void __exit ipw_exit(void)
//{
//	driver_remove_file(&ipw_driver.driver, &driver_attr_debug_level);
//	pci_unregister_driver(&ipw_driver);
//}
//
//module_param(disable, int, 0444);
//MODULE_PARM_DESC(disable, "manually disable the radio (default 0 [radio on])");
//
//module_param(associate, int, 0444);
//MODULE_PARM_DESC(associate, "auto associate when scanning (default off)");
//
//module_param(auto_create, int, 0444);
//MODULE_PARM_DESC(auto_create, "auto create adhoc network (default on)");
//
//module_param_named(led, led_support, int, 0444);
//MODULE_PARM_DESC(led, "enable led control on some systems (default 0 off)");
//
//module_param(debug, int, 0444);
//MODULE_PARM_DESC(debug, "debug output mask");
//
//module_param_named(channel, default_channel, int, 0444);
//MODULE_PARM_DESC(channel, "channel to limit associate to (default 0 [ANY])");
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//module_param(rtap_iface, int, 0444);
//MODULE_PARM_DESC(rtap_iface, "create the rtap interface (1 - create, default 0)");
//#endif
//
//#ifdef CONFIG_IPW2200_QOS
//module_param(qos_enable, int, 0444);
//MODULE_PARM_DESC(qos_enable, "enable all QoS functionalitis");
//
//module_param(qos_burst_enable, int, 0444);
//MODULE_PARM_DESC(qos_burst_enable, "enable QoS burst mode");
//
//module_param(qos_no_ack_mask, int, 0444);
//MODULE_PARM_DESC(qos_no_ack_mask, "mask Tx_Queue to no ack");
//
//module_param(burst_duration_CCK, int, 0444);
//MODULE_PARM_DESC(burst_duration_CCK, "set CCK burst value");
//
//module_param(burst_duration_OFDM, int, 0444);
//MODULE_PARM_DESC(burst_duration_OFDM, "set OFDM burst value");
//#endif				/* CONFIG_IPW2200_QOS */
//
//#ifdef CONFIG_IPW2200_MONITOR
//module_param_named(mode, network_mode, int, 0444);
//MODULE_PARM_DESC(mode, "network mode (0=BSS,1=IBSS,2=Monitor)");
//#else
//module_param_named(mode, network_mode, int, 0444);
//MODULE_PARM_DESC(mode, "network mode (0=BSS,1=IBSS)");
//#endif
//
//module_param(bt_coexist, int, 0444);
//MODULE_PARM_DESC(bt_coexist, "enable bluetooth coexistence (default off)");
//
//module_param(hwcrypto, int, 0444);
//MODULE_PARM_DESC(hwcrypto, "enable hardware crypto (default off)");
//
//module_param(cmdlog, int, 0444);
//MODULE_PARM_DESC(cmdlog,
//		 "allocate a ring buffer for logging firmware commands");
//
//module_param(roaming, int, 0444);
//MODULE_PARM_DESC(roaming, "enable roaming support (default on)");
//
//module_param(antenna, int, 0444);
//MODULE_PARM_DESC(antenna, "select antenna 1=Main, 3=Aux, default 0 [both], 2=slow_diversity (choose the one with lower background noise)");
//
//module_exit(ipw_exit);
//module_init(ipw_init);
//
