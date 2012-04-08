
#include <linux/sched.h>
#include "ladoga_drv.h"

///*
//#ifndef KBUILD_EXTMOD
//#define VK "k"
//#else
//#define VK
//#endif
//
//#ifdef CONFIG_IPW2200_DEBUG
//#define VD "d"
//#else
//#define VD
//#endif
//
//#ifdef CONFIG_IPW2200_MONITOR
//#define VM "m"
//#else
//#define VM
//#endif
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//#define VP "p"
//#else
//#define VP
//#endif
//
//#ifdef CONFIG_IPW2200_RADIOTAP
//#define VR "r"
//#else
//#define VR
//#endif
//
//#ifdef CONFIG_IPW2200_QOS
//#define VQ "q"
//#else
//#define VQ
//#endif
//
//#define IPW2200_VERSION "1.2.2" VK VD VM VP VR VQ
//#define DRV_DESCRIPTION	"Intel(R) PRO/Wireless 2200/2915 Network Driver"
//#define DRV_COPYRIGHT	"Copyright(c) 2003-2006 Intel Corporation"
//#define DRV_VERSION     IPW2200_VERSION
//
//#define ETH_P_80211_STATS (ETH_P_80211_RAW + 1)
//
//MODULE_DESCRIPTION(DRV_DESCRIPTION);
//MODULE_VERSION(DRV_VERSION);
//MODULE_AUTHOR(DRV_COPYRIGHT);
//MODULE_LICENSE("GPL");
//
//static int cmdlog = 0;
//static int debug = 0;
//static int default_channel = 0;
//static int network_mode = 0;
//
//static u32 ipw_debug_level;
//static int associate;
//static int auto_create = 1;
//static int led_support = 0;
//static int disable = 0;
//static int bt_coexist = 0;
//static int hwcrypto = 0;
//static int roaming = 1;
//static const char ipw_modes[] = {
//	'a', 'b', 'g', '?'
//};
//static int antenna = CFG_SYS_ANTENNA_BOTH;
//*/
////#ifdef CONFIG_IPW2200_PROMISCUOUS
////static int rtap_iface = 0;     /* def: 0 -- do not create rtap interface */
////#endif
//
///*
//#ifdef CONFIG_IPW2200_QOS
//static int qos_enable = 0;
//static int qos_burst_enable = 0;
//static int qos_no_ack_mask = 0;
//static int burst_duration_CCK = 0;
//static int burst_duration_OFDM = 0;
//
//static struct libipw_qos_parameters def_qos_parameters_OFDM = {
//	{QOS_TX0_CW_MIN_OFDM, QOS_TX1_CW_MIN_OFDM, QOS_TX2_CW_MIN_OFDM,
//	 QOS_TX3_CW_MIN_OFDM},
//	{QOS_TX0_CW_MAX_OFDM, QOS_TX1_CW_MAX_OFDM, QOS_TX2_CW_MAX_OFDM,
//	 QOS_TX3_CW_MAX_OFDM},
//	{QOS_TX0_AIFS, QOS_TX1_AIFS, QOS_TX2_AIFS, QOS_TX3_AIFS},
//	{QOS_TX0_ACM, QOS_TX1_ACM, QOS_TX2_ACM, QOS_TX3_ACM},
//	{QOS_TX0_TXOP_LIMIT_OFDM, QOS_TX1_TXOP_LIMIT_OFDM,
//	 QOS_TX2_TXOP_LIMIT_OFDM, QOS_TX3_TXOP_LIMIT_OFDM}
//};
//
//static struct libipw_qos_parameters def_qos_parameters_CCK = {
//	{QOS_TX0_CW_MIN_CCK, QOS_TX1_CW_MIN_CCK, QOS_TX2_CW_MIN_CCK,
//	 QOS_TX3_CW_MIN_CCK},
//	{QOS_TX0_CW_MAX_CCK, QOS_TX1_CW_MAX_CCK, QOS_TX2_CW_MAX_CCK,
//	 QOS_TX3_CW_MAX_CCK},
//	{QOS_TX0_AIFS, QOS_TX1_AIFS, QOS_TX2_AIFS, QOS_TX3_AIFS},
//	{QOS_TX0_ACM, QOS_TX1_ACM, QOS_TX2_ACM, QOS_TX3_ACM},
//	{QOS_TX0_TXOP_LIMIT_CCK, QOS_TX1_TXOP_LIMIT_CCK, QOS_TX2_TXOP_LIMIT_CCK,
//	 QOS_TX3_TXOP_LIMIT_CCK}
//};
//
//static struct libipw_qos_parameters def_parameters_OFDM = {
//	{DEF_TX0_CW_MIN_OFDM, DEF_TX1_CW_MIN_OFDM, DEF_TX2_CW_MIN_OFDM,
//	 DEF_TX3_CW_MIN_OFDM},
//	{DEF_TX0_CW_MAX_OFDM, DEF_TX1_CW_MAX_OFDM, DEF_TX2_CW_MAX_OFDM,
//	 DEF_TX3_CW_MAX_OFDM},
//	{DEF_TX0_AIFS, DEF_TX1_AIFS, DEF_TX2_AIFS, DEF_TX3_AIFS},
//	{DEF_TX0_ACM, DEF_TX1_ACM, DEF_TX2_ACM, DEF_TX3_ACM},
//	{DEF_TX0_TXOP_LIMIT_OFDM, DEF_TX1_TXOP_LIMIT_OFDM,
//	 DEF_TX2_TXOP_LIMIT_OFDM, DEF_TX3_TXOP_LIMIT_OFDM}
//};
//
//static struct libipw_qos_parameters def_parameters_CCK = {
//	{DEF_TX0_CW_MIN_CCK, DEF_TX1_CW_MIN_CCK, DEF_TX2_CW_MIN_CCK,
//	 DEF_TX3_CW_MIN_CCK},
//	{DEF_TX0_CW_MAX_CCK, DEF_TX1_CW_MAX_CCK, DEF_TX2_CW_MAX_CCK,
//	 DEF_TX3_CW_MAX_CCK},
//	{DEF_TX0_AIFS, DEF_TX1_AIFS, DEF_TX2_AIFS, DEF_TX3_AIFS},
//	{DEF_TX0_ACM, DEF_TX1_ACM, DEF_TX2_ACM, DEF_TX3_ACM},
//	{DEF_TX0_TXOP_LIMIT_CCK, DEF_TX1_TXOP_LIMIT_CCK, DEF_TX2_TXOP_LIMIT_CCK,
//	 DEF_TX3_TXOP_LIMIT_CCK}
//};
//
//static u8 qos_oui[QOS_OUI_LEN] = { 0x00, 0x50, 0xF2 };
//
//static int from_priority_to_tx_queue[] = {
//	IPW_TX_QUEUE_1, IPW_TX_QUEUE_2, IPW_TX_QUEUE_2, IPW_TX_QUEUE_1,
//	IPW_TX_QUEUE_3, IPW_TX_QUEUE_3, IPW_TX_QUEUE_4, IPW_TX_QUEUE_4
//};
//*/
///*
//static u32 ipw_qos_get_burst_duration(struct ipw_priv *priv);
//
//static int ipw_send_qos_params_command(struct ipw_priv *priv, struct libipw_qos_parameters
//				       *qos_param);
//static int ipw_send_qos_info_command(struct ipw_priv *priv, struct libipw_qos_information_element
//				     *qos_param);
//#endif*/				/* CONFIG_IPW2200_QOS */
///*
//static struct iw_statistics *ipw_get_wireless_stats(struct net_device *dev);
//static void ipw_remove_current_network(struct ipw_priv *priv);
//static void ipw_rx(struct ipw_priv *priv);
//static int ipw_queue_tx_reclaim(struct ipw_priv *priv,
//				struct clx2_tx_queue *txq, int qindex);
//static int ipw_queue_reset(struct ipw_priv *priv);
//
//static int ipw_queue_tx_hcmd(struct ipw_priv *priv, int hcmd, void *buf,
//			     int len, int sync);
//
//static void ipw_tx_queue_free(struct ipw_priv *);
//
//static struct ipw_rx_queue *ipw_rx_queue_alloc(struct ipw_priv *);
//static void ipw_rx_queue_free(struct ipw_priv *, struct ipw_rx_queue *);
//static void ipw_rx_queue_replenish(void *);
//static int ipw_up(struct ipw_priv *);
//static void ipw_bg_up(struct work_struct *work);
//static void ipw_down(struct ipw_priv *);
//static void ipw_bg_down(struct work_struct *work);
//static int ipw_config(struct ipw_priv *);
//static int init_supported_rates(struct ipw_priv *priv,
//				struct ipw_supported_rates *prates);
//static void ipw_set_hwcrypto_keys(struct ipw_priv *);
//static void ipw_send_wep_keys(struct ipw_priv *, int);
//
//static int snprint_line(char *buf, size_t count,
//			const u8 * data, u32 len, u32 ofs)
//{
//	int out, i, j, l;
//	char c;
//
//	out = snprintf(buf, count, "%08X", ofs);
//
//	for (l = 0, i = 0; i < 2; i++) {
//		out += snprintf(buf + out, count - out, " ");
//		for (j = 0; j < 8 && l < len; j++, l++)
//			out += snprintf(buf + out, count - out, "%02X ",
//					data[(i * 8 + j)]);
//		for (; j < 8; j++)
//			out += snprintf(buf + out, count - out, "   ");
//	}
//
//	out += snprintf(buf + out, count - out, " ");
//	for (l = 0, i = 0; i < 2; i++) {
//		out += snprintf(buf + out, count - out, " ");
//		for (j = 0; j < 8 && l < len; j++, l++) {
//			c = data[(i * 8 + j)];
//			if (!isascii(c) || !isprint(c))
//				c = '.';
//
//			out += snprintf(buf + out, count - out, "%c", c);
//		}
//
//		for (; j < 8; j++)
//			out += snprintf(buf + out, count - out, " ");
//	}
//
//	return out;
//}
//
//static void printk_buf(int level, const u8 * data, u32 len)
//{
//	char line[81];
//	u32 ofs = 0;
//	if (!(ipw_debug_level & level))
//		return;
//
//	while (len) {
//		snprint_line(line, sizeof(line), &data[ofs],
//			     min(len, 16U), ofs);
//		printk(KERN_DEBUG "%s\n", line);
//		ofs += 16;
//		len -= min(len, 16U);
//	}
//}
//
//static int snprintk_buf(u8 * output, size_t size, const u8 * data, size_t len)
//{
//	size_t out = size;
//	u32 ofs = 0;
//	int total = 0;
//
//	while (size && len) {
//		out = snprint_line(output, size, &data[ofs],
//				   min_t(size_t, len, 16U), ofs);
//
//		ofs += 16;
//		output += out;
//		size -= out;
//		len -= min_t(size_t, len, 16U);
//		total += out;
//	}
//	return total;
//}
//*/
///* alias for 32-bit indirect read (for SRAM/reg above 4K), with debug wrapper */
////static u32 _ipw_read_reg32(struct ipw_priv *priv, u32 reg);
////#define ipw_read_reg32(a, b) _ipw_read_reg32(a, b)
//
///* alias for 8-bit indirect read (for SRAM/reg above 4K), with debug wrapper */
////static u8 _ipw_read_reg8(struct ipw_priv *ipw, u32 reg);
////#define ipw_read_reg8(a, b) _ipw_read_reg8(a, b)
//
///* 8-bit indirect write (for SRAM/reg above 4K), with debug wrapper */
////static void _ipw_write_reg8(struct ipw_priv *priv, u32 reg, u8 value);
////static inline void ipw_write_reg8(struct ipw_priv *a, u32 b, u8 c)
////{
////	IPW_DEBUG_IO("%s %d: write_indirect8(0x%08X, 0x%08X)\n", __FILE__,
////		     __LINE__, (u32) (b), (u32) (c));
////	_ipw_write_reg8(a, b, c);
////}
//
///* 16-bit indirect write (for SRAM/reg above 4K), with debug wrapper */
////static void _ipw_write_reg16(struct ipw_priv *priv, u32 reg, u16 value);
////static inline void ipw_write_reg16(struct ipw_priv *a, u32 b, u16 c)
////{
////	IPW_DEBUG_IO("%s %d: write_indirect16(0x%08X, 0x%08X)\n", __FILE__,
////		     __LINE__, (u32) (b), (u32) (c));
////	_ipw_write_reg16(a, b, c);
////}
//
///* 32-bit indirect write (for SRAM/reg above 4K), with debug wrapper */
////static void _ipw_write_reg32(struct ipw_priv *priv, u32 reg, u32 value);
////static inline void ipw_write_reg32(struct ipw_priv *a, u32 b, u32 c)
////{
////	IPW_DEBUG_IO("%s %d: write_indirect32(0x%08X, 0x%08X)\n", __FILE__,
////		     __LINE__, (u32) (b), (u32) (c));
////	_ipw_write_reg32(a, b, c);
////}
//
///* 8-bit direct write (low 4K) */
////static inline void _ipw_write8(struct ipw_priv *ipw, unsigned long ofs,
////		u8 val)
////{
////	writeb(val, ipw->hw_base + ofs);
////}
//
///* 8-bit direct write (for low 4K of SRAM/regs), with debug wrapper */
////#define ipw_write8(ipw, ofs, val) do { \
////	IPW_DEBUG_IO("%s %d: write_direct8(0x%08X, 0x%08X)\n", __FILE__, \
////			__LINE__, (u32)(ofs), (u32)(val)); \
////	_ipw_write8(ipw, ofs, val); \
////} while (0)
//
///* 16-bit direct write (low 4K) */
////static inline void _ipw_write16(struct ipw_priv *ipw, unsigned long ofs,
////		u16 val)
////{
////	writew(val, ipw->hw_base + ofs);
////}
//
///* 16-bit direct write (for low 4K of SRAM/regs), with debug wrapper */
////#define ipw_write16(ipw, ofs, val) do { \
////	IPW_DEBUG_IO("%s %d: write_direct16(0x%08X, 0x%08X)\n", __FILE__, \
////			__LINE__, (u32)(ofs), (u32)(val)); \
////	_ipw_write16(ipw, ofs, val); \
////} while (0)
//
///* 32-bit direct write (low 4K) */
////static inline void _ipw_write32(struct ipw_priv *ipw, unsigned long ofs,
////		u32 val)
////{
////	writel(val, ipw->hw_base + ofs);
////}
//
///* 32-bit direct write (for low 4K of SRAM/regs), with debug wrapper */
////#define ipw_write32(ipw, ofs, val) do { \
////	IPW_DEBUG_IO("%s %d: write_direct32(0x%08X, 0x%08X)\n", __FILE__, \
////			__LINE__, (u32)(ofs), (u32)(val)); \
////	_ipw_write32(ipw, ofs, val); \
////} while (0)
//
///* 8-bit direct read (low 4K) */
////static inline u8 _ipw_read8(struct ipw_priv *ipw, unsigned long ofs)
////{
////	return readb(ipw->hw_base + ofs);
////}
//
///* alias to 8-bit direct read (low 4K of SRAM/regs), with debug wrapper */
////#define ipw_read8(ipw, ofs) ({ \
////	IPW_DEBUG_IO("%s %d: read_direct8(0x%08X)\n", __FILE__, __LINE__, \
////			(u32)(ofs)); \
////	_ipw_read8(ipw, ofs); \
////})
//
///* 16-bit direct read (low 4K) */
////static inline u16 _ipw_read16(struct ipw_priv *ipw, unsigned long ofs)
////{
////	return readw(ipw->hw_base + ofs);
////}
//
///* alias to 16-bit direct read (low 4K of SRAM/regs), with debug wrapper */
////#define ipw_read16(ipw, ofs) ({ \
////	IPW_DEBUG_IO("%s %d: read_direct16(0x%08X)\n", __FILE__, __LINE__, \
////			(u32)(ofs)); \
////	_ipw_read16(ipw, ofs); \
////})
//
///* 32-bit direct read (low 4K) */
////static inline u32 _ipw_read32(struct ipw_priv *ipw, unsigned long ofs)
////{
////	return readl(ipw->hw_base + ofs);
////}
//
///* alias to 32-bit direct read (low 4K of SRAM/regs), with debug wrapper */
////#define ipw_read32(ipw, ofs) ({ \
////	IPW_DEBUG_IO("%s %d: read_direct32(0x%08X)\n", __FILE__, __LINE__, \
////			(u32)(ofs)); \
////	_ipw_read32(ipw, ofs); \
////})
//
////static void _ipw_read_indirect(struct ipw_priv *, u32, u8 *, int);
///* alias to multi-byte read (SRAM/regs above 4K), with debug wrapper */
////#define ipw_read_indirect(a, b, c, d) ({ \
////	IPW_DEBUG_IO("%s %d: read_indirect(0x%08X) %u bytes\n", __FILE__, \
////			__LINE__, (u32)(b), (u32)(d)); \
////	_ipw_read_indirect(a, b, c, d); \
////})
//
///* alias to multi-byte read (SRAM/regs above 4K), with debug wrapper */
////static void _ipw_write_indirect(struct ipw_priv *priv, u32 addr, u8 * data,
////				int num);
////#define ipw_write_indirect(a, b, c, d) do { \
////	IPW_DEBUG_IO("%s %d: write_indirect(0x%08X) %u bytes\n", __FILE__, \
////			__LINE__, (u32)(b), (u32)(d)); \
////	_ipw_write_indirect(a, b, c, d); \
////} while (0)
//
///* 32-bit indirect write (above 4K) */
////static void _ipw_write_reg32(struct ipw_priv *priv, u32 reg, u32 value)
////{
////	IPW_DEBUG_IO(" %p : reg = 0x%8X : value = 0x%8X\n", priv, reg, value);
////	_ipw_write32(priv, IPW_INDIRECT_ADDR, reg);
////	_ipw_write32(priv, IPW_INDIRECT_DATA, value);
////}
//
///* 8-bit indirect write (above 4K) */
////static void _ipw_write_reg8(struct ipw_priv *priv, u32 reg, u8 value)
////{
////	u32 aligned_addr = reg & IPW_INDIRECT_ADDR_MASK;	/* dword align */
////	u32 dif_len = reg - aligned_addr;
//
////	IPW_DEBUG_IO(" reg = 0x%8X : value = 0x%8X\n", reg, value);
////	_ipw_write32(priv, IPW_INDIRECT_ADDR, aligned_addr);
////	_ipw_write8(priv, IPW_INDIRECT_DATA + dif_len, value);
////}
//
///* 16-bit indirect write (above 4K) */
////static void _ipw_write_reg16(struct ipw_priv *priv, u32 reg, u16 value)
////{
////	u32 aligned_addr = reg & IPW_INDIRECT_ADDR_MASK;	/* dword align */
////	u32 dif_len = (reg - aligned_addr) & (~0x1ul);
//
////	IPW_DEBUG_IO(" reg = 0x%8X : value = 0x%8X\n", reg, value);
////	_ipw_write32(priv, IPW_INDIRECT_ADDR, aligned_addr);
////	_ipw_write16(priv, IPW_INDIRECT_DATA + dif_len, value);
////}
//
///* 8-bit indirect read (above 4K) */
////static u8 _ipw_read_reg8(struct ipw_priv *priv, u32 reg)
////{
////	u32 word;
////	_ipw_write32(priv, IPW_INDIRECT_ADDR, reg & IPW_INDIRECT_ADDR_MASK);
////	IPW_DEBUG_IO(" reg = 0x%8X : \n", reg);
////	word = _ipw_read32(priv, IPW_INDIRECT_DATA);
////	return (word >> ((reg & 0x3) * 8)) & 0xff;
////}
//
///* 32-bit indirect read (above 4K) */
////static u32 _ipw_read_reg32(struct ipw_priv *priv, u32 reg)
////{
////	u32 value;
//
////	IPW_DEBUG_IO("%p : reg = 0x%08x\n", priv, reg);
//
////	_ipw_write32(priv, IPW_INDIRECT_ADDR, reg);
////	value = _ipw_read32(priv, IPW_INDIRECT_DATA);
////	IPW_DEBUG_IO(" reg = 0x%4X : value = 0x%4x \n", reg, value);
////	return value;
////}
//
///* General purpose, no alignment requirement, iterative (multi-byte) read, */
///*    for area above 1st 4K of SRAM/reg space */
////static void _ipw_read_indirect(struct ipw_priv *priv, u32 addr, u8 * buf,
////			       int num)
////{
////	u32 aligned_addr = addr & IPW_INDIRECT_ADDR_MASK;	/* dword align */
////	u32 dif_len = addr - aligned_addr;
////	u32 i;
//
////	IPW_DEBUG_IO("addr = %i, buf = %p, num = %i\n", addr, buf, num);
//
////	if (num <= 0) {
////		return;
////	}
//
//	/* Read the first dword (or portion) byte by byte */
////	if (unlikely(dif_len)) {
////		_ipw_write32(priv, IPW_INDIRECT_ADDR, aligned_addr);
//		/* Start reading at aligned_addr + dif_len */
////		for (i = dif_len; ((i < 4) && (num > 0)); i++, num--)
////			*buf++ = _ipw_read8(priv, IPW_INDIRECT_DATA + i);
////		aligned_addr += 4;
////	}
//
//	/* Read all of the middle dwords as dwords, with auto-increment */
////	_ipw_write32(priv, IPW_AUTOINC_ADDR, aligned_addr);
////	for (; num >= 4; buf += 4, aligned_addr += 4, num -= 4)
////		*(u32 *) buf = _ipw_read32(priv, IPW_AUTOINC_DATA);
//
//	/* Read the last dword (or portion) byte by byte */
////	if (unlikely(num)) {
////		_ipw_write32(priv, IPW_INDIRECT_ADDR, aligned_addr);
////		for (i = 0; num > 0; i++, num--)
////			*buf++ = ipw_read8(priv, IPW_INDIRECT_DATA + i);
////	}
////}
//
///* General purpose, no alignment requirement, iterative (multi-byte) write, */
///*    for area above 1st 4K of SRAM/reg space */
////static void _ipw_write_indirect(struct ipw_priv *priv, u32 addr, u8 * buf,
////				int num)
////{
////	u32 aligned_addr = addr & IPW_INDIRECT_ADDR_MASK;	/* dword align */
////	u32 dif_len = addr - aligned_addr;
////	u32 i;
//
////	IPW_DEBUG_IO("addr = %i, buf = %p, num = %i\n", addr, buf, num);
//
////	if (num <= 0) {
////		return;
////	}
//
//	/* Write the first dword (or portion) byte by byte */
////	if (unlikely(dif_len)) {
////		_ipw_write32(priv, IPW_INDIRECT_ADDR, aligned_addr);
////		/* Start writing at aligned_addr + dif_len */
////		for (i = dif_len; ((i < 4) && (num > 0)); i++, num--, buf++)
////			_ipw_write8(priv, IPW_INDIRECT_DATA + i, *buf);
////		aligned_addr += 4;
////	}
//
//	/* Write all of the middle dwords as dwords, with auto-increment */
////	_ipw_write32(priv, IPW_AUTOINC_ADDR, aligned_addr);
////	for (; num >= 4; buf += 4, aligned_addr += 4, num -= 4)
////		_ipw_write32(priv, IPW_AUTOINC_DATA, *(u32 *) buf);
//
//	/* Write the last dword (or portion) byte by byte */
////	if (unlikely(num)) {
////		_ipw_write32(priv, IPW_INDIRECT_ADDR, aligned_addr);
////		for (i = 0; num > 0; i++, num--, buf++)
////			_ipw_write8(priv, IPW_INDIRECT_DATA + i, *buf);
////	}
////}
//
///* General purpose, no alignment requirement, iterative (multi-byte) write, */
///*    for 1st 4K of SRAM/regs space */
////static void ipw_write_direct(struct ipw_priv *priv, u32 addr, void *buf,
////			     int num)
////{
////	memcpy_toio((priv->hw_base + addr), buf, num);
////}
//
///* Set bit(s) in low 4K of SRAM/regs */
////static inline void ipw_set_bit(struct ipw_priv *priv, u32 reg, u32 mask)
////{
////	ipw_write32(priv, reg, ipw_read32(priv, reg) | mask);
////}
//
///* Clear bit(s) in low 4K of SRAM/regs */
////static inline void ipw_clear_bit(struct ipw_priv *priv, u32 reg, u32 mask)
////{
////	ipw_write32(priv, reg, ipw_read32(priv, reg) & ~mask);
////}
//
////static inline void __ipw_enable_interrupts(struct ipw_priv *priv)
////{
////	if (priv->status & STATUS_INT_ENABLED)
////		return;
////	priv->status |= STATUS_INT_ENABLED;
////	ipw_write32(priv, IPW_INTA_MASK_R, IPW_INTA_MASK_ALL);
////}
///*
//static inline void __ipw_disable_interrupts(struct ipw_priv *priv)
//{
//	if (!(priv->status & STATUS_INT_ENABLED))
//		return;
//	priv->status &= ~STATUS_INT_ENABLED;
//	ipw_write32(priv, IPW_INTA_MASK_R, ~IPW_INTA_MASK_ALL);
//}
//
//static inline void ipw_enable_interrupts(struct ipw_priv *priv)
//{
//	unsigned long flags;
//
//	spin_lock_irqsave(&priv->irq_lock, flags);
//	__ipw_enable_interrupts(priv);
//	spin_unlock_irqrestore(&priv->irq_lock, flags);
//}
//
//static inline void ipw_disable_interrupts(struct ipw_priv *priv)
//{
//	unsigned long flags;
//
//	spin_lock_irqsave(&priv->irq_lock, flags);
//	__ipw_disable_interrupts(priv);
//	spin_unlock_irqrestore(&priv->irq_lock, flags);
//}
//
//static char *ipw_error_desc(u32 val)
//{
//	switch (val) {
//	case IPW_FW_ERROR_OK:
//		return "ERROR_OK";
//	case IPW_FW_ERROR_FAIL:
//		return "ERROR_FAIL";
//	case IPW_FW_ERROR_MEMORY_UNDERFLOW:
//		return "MEMORY_UNDERFLOW";
//	case IPW_FW_ERROR_MEMORY_OVERFLOW:
//		return "MEMORY_OVERFLOW";
//	case IPW_FW_ERROR_BAD_PARAM:
//		return "BAD_PARAM";
//	case IPW_FW_ERROR_BAD_CHECKSUM:
//		return "BAD_CHECKSUM";
//	case IPW_FW_ERROR_NMI_INTERRUPT:
//		return "NMI_INTERRUPT";
//	case IPW_FW_ERROR_BAD_DATABASE:
//		return "BAD_DATABASE";
//	case IPW_FW_ERROR_ALLOC_FAIL:
//		return "ALLOC_FAIL";
//	case IPW_FW_ERROR_DMA_UNDERRUN:
//		return "DMA_UNDERRUN";
//	case IPW_FW_ERROR_DMA_STATUS:
//		return "DMA_STATUS";
//	case IPW_FW_ERROR_DINO_ERROR:
//		return "DINO_ERROR";
//	case IPW_FW_ERROR_EEPROM_ERROR:
//		return "EEPROM_ERROR";
//	case IPW_FW_ERROR_SYSASSERT:
//		return "SYSASSERT";
//	case IPW_FW_ERROR_FATAL_ERROR:
//		return "FATAL_ERROR";
//	default:
//		return "UNKNOWN_ERROR";
//	}
//}
//
//static void ipw_dump_error_log(struct ipw_priv *priv,
//			       struct ipw_fw_error *error)
//{
//	u32 i;
//
//	if (!error) {
//		IPW_ERROR("Error allocating and capturing error log.  "
//			  "Nothing to dump.\n");
//		return;
//	}
//
//	IPW_ERROR("Start IPW Error Log Dump:\n");
//	IPW_ERROR("Status: 0x%08X, Config: %08X\n",
//		  error->status, error->config);
//
//	for (i = 0; i < error->elem_len; i++)
//		IPW_ERROR("%s %i 0x%08x  0x%08x  0x%08x  0x%08x  0x%08x\n",
//			  ipw_error_desc(error->elem[i].desc),
//			  error->elem[i].time,
//			  error->elem[i].blink1,
//			  error->elem[i].blink2,
//			  error->elem[i].link1,
//			  error->elem[i].link2, error->elem[i].data);
//	for (i = 0; i < error->log_len; i++)
//		IPW_ERROR("%i\t0x%08x\t%i\n",
//			  error->log[i].time,
//			  error->log[i].data, error->log[i].event);
//}
//
//static inline int ipw_is_init(struct ipw_priv *priv)
//{
//	return (priv->status & STATUS_INIT) ? 1 : 0;
//}
//
//static int ipw_get_ordinal(struct ipw_priv *priv, u32 ord, void *val, u32 * len)
//{
//	u32 addr, field_info, field_len, field_count, total_len;
//
//	IPW_DEBUG_ORD("ordinal = %i\n", ord);
//
//	if (!priv || !val || !len) {
//		IPW_DEBUG_ORD("Invalid argument\n");
//		return -EINVAL;
//	}
//*/
//	/* verify device ordinal tables have been initialized */
////	if (!priv->table0_addr || !priv->table1_addr || !priv->table2_addr) {
////		IPW_DEBUG_ORD("Access ordinals before initialization\n");
////		return -EINVAL;
////	}
//
////	switch (IPW_ORD_TABLE_ID_MASK & ord) {
////	case IPW_ORD_TABLE_0_MASK:
//		/*
//		 * TABLE 0: Direct access to a table of 32 bit values
//		 *
//		 * This is a very simple table with the data directly
//		 * read from the table
//		 */
//
//		/* remove the table id from the ordinal */
////		ord &= IPW_ORD_TABLE_VALUE_MASK;
//
//		/* boundary check */
////		if (ord > priv->table0_len) {
////			IPW_DEBUG_ORD("ordinal value (%i) longer then "
////				      "max (%i)\n", ord, priv->table0_len);
////			return -EINVAL;
////		}
//
//		/* verify we have enough room to store the value */
////		if (*len < sizeof(u32)) {
////			IPW_DEBUG_ORD("ordinal buffer length too small, "
////				      "need %zd\n", sizeof(u32));
////			return -EINVAL;
////		}
//
////		IPW_DEBUG_ORD("Reading TABLE0[%i] from offset 0x%08x\n",
////			      ord, priv->table0_addr + (ord << 2));
//
////		*len = sizeof(u32);
////		ord <<= 2;
////		*((u32 *) val) = ipw_read32(priv, priv->table0_addr + ord);
////		break;
//
////	case IPW_ORD_TABLE_1_MASK:
//		/*
//		 * TABLE 1: Indirect access to a table of 32 bit values
//		 *
//		 * This is a fairly large table of u32 values each
//		 * representing starting addr for the data (which is
//		 * also a u32)
//		 */
//
//		/* remove the table id from the ordinal */
////		ord &= IPW_ORD_TABLE_VALUE_MASK;
//
//		/* boundary check */
////		if (ord > priv->table1_len) {
////			IPW_DEBUG_ORD("ordinal value too long\n");
////			return -EINVAL;
////		}
//
//		/* verify we have enough room to store the value */
////		if (*len < sizeof(u32)) {
////			IPW_DEBUG_ORD("ordinal buffer length too small, "
////				      "need %zd\n", sizeof(u32));
////			return -EINVAL;
////		}
//
////		*((u32 *) val) =
////		    ipw_read_reg32(priv, (priv->table1_addr + (ord << 2)));
////		*len = sizeof(u32);
////		break;
//
////	case IPW_ORD_TABLE_2_MASK:
//		/*
//		 * TABLE 2: Indirect access to a table of variable sized values
//		 *
//		 * This table consist of six values, each containing
//		 *     - dword containing the starting offset of the data
//		 *     - dword containing the lengh in the first 16bits
//		 *       and the count in the second 16bits
//		 */
//
//		/* remove the table id from the ordinal */
////		ord &= IPW_ORD_TABLE_VALUE_MASK;
//
//		/* boundary check */
////		if (ord > priv->table2_len) {
////			IPW_DEBUG_ORD("ordinal value too long\n");
////			return -EINVAL;
////		}
//
//		/* get the address of statistic */
////		addr = ipw_read_reg32(priv, priv->table2_addr + (ord << 3));
//
//		/* get the second DW of statistics ;
//		 * two 16-bit words - first is length, second is count */
////		field_info =
////		    ipw_read_reg32(priv,
////				   priv->table2_addr + (ord << 3) +
////				   sizeof(u32));
//
//		/* get each entry length */
////		field_len = *((u16 *) & field_info);
//
//		/* get number of entries */
////		field_count = *(((u16 *) & field_info) + 1);
//
//		/* abort if not enought memory */
////		total_len = field_len * field_count;
////		if (total_len > *len) {
////			*len = total_len;
////			return -EINVAL;
////		}
//
////		*len = total_len;
////		if (!total_len)
////			return 0;
//
////		IPW_DEBUG_ORD("addr = 0x%08x, total_len = %i, "
////			      "field_info = 0x%08x\n",
////			      addr, total_len, field_info);
////		ipw_read_indirect(priv, addr, val, total_len);
////		break;
//
////	default:
////		IPW_DEBUG_ORD("Invalid ordinal!\n");
////		return -EINVAL;
//
////	}
//
////	return 0;
////}
///*
//static void ipw_init_ordinals(struct ipw_priv *priv)
//{
//	priv->table0_addr = IPW_ORDINALS_TABLE_LOWER;
//	priv->table0_len = ipw_read32(priv, priv->table0_addr);
//
//	IPW_DEBUG_ORD("table 0 offset at 0x%08x, len = %i\n",
//		      priv->table0_addr, priv->table0_len);
//
//	priv->table1_addr = ipw_read32(priv, IPW_ORDINALS_TABLE_1);
//	priv->table1_len = ipw_read_reg32(priv, priv->table1_addr);
//
//	IPW_DEBUG_ORD("table 1 offset at 0x%08x, len = %i\n",
//		      priv->table1_addr, priv->table1_len);
//*/
////	priv->table2_addr = ipw_read32(priv, IPW_ORDINALS_TABLE_2);
////	priv->table2_len = ipw_read_reg32(priv, priv->table2_addr);
////	priv->table2_len &= 0x0000ffff;	/* use first two bytes */
//
////	IPW_DEBUG_ORD("table 2 offset at 0x%08x, len = %i\n",
////		      priv->table2_addr, priv->table2_len);
//
////}
///*
//static u32 ipw_register_toggle(u32 reg)
//{
//	reg &= ~IPW_START_STANDBY;
//	if (reg & IPW_GATE_ODMA)
//		reg &= ~IPW_GATE_ODMA;
//	if (reg & IPW_GATE_IDMA)
//		reg &= ~IPW_GATE_IDMA;
//	if (reg & IPW_GATE_ADMA)
//		reg &= ~IPW_GATE_ADMA;
//	return reg;
//}
//*/
///*
// * LED behavior:
// * - On radio ON, turn on any LEDs that require to be on during start
// * - On initialization, start unassociated blink
// * - On association, disable unassociated blink
// * - On disassociation, start unassociated blink
// * - On radio OFF, turn off any LEDs started during radio on
// *
// */
////#define LD_TIME_LINK_ON msecs_to_jiffies(300)
////#define LD_TIME_LINK_OFF msecs_to_jiffies(2700)
////#define LD_TIME_ACT_ON msecs_to_jiffies(250)
//
////static void ipw_led_link_on(struct ipw_priv *priv)
////{
////	unsigned long flags;
////	u32 led;
//
//	/* If configured to not use LEDs, or nic_type is 1,
//	 * then we don't toggle a LINK led */
////	if (priv->config & CFG_NO_LED || priv->nic_type == EEPROM_NIC_TYPE_1)
////		return;
//
////	spin_lock_irqsave(&priv->lock, flags);
//
////	if (!(priv->status & STATUS_RF_KILL_MASK) &&
////	    !(priv->status & STATUS_LED_LINK_ON)) {
////		IPW_DEBUG_LED("Link LED On\n");
////		led = ipw_read_reg32(priv, IPW_EVENT_REG);
////		led |= priv->led_association_on;
//
////		led = ipw_register_toggle(led);
//
////		IPW_DEBUG_LED("Reg: 0x%08X\n", led);
////		ipw_write_reg32(priv, IPW_EVENT_REG, led);
//
////		priv->status |= STATUS_LED_LINK_ON;
//
//		/* If we aren't associated, schedule turning the LED off */
////		if (!(priv->status & STATUS_ASSOCIATED))
////			queue_delayed_work(priv->workqueue,
////					   &priv->led_link_off,
////					   LD_TIME_LINK_ON);
////	}
//
////	spin_unlock_irqrestore(&priv->lock, flags);
////}
//
////static void ipw_bg_led_link_on(struct work_struct *work)
////{
////	struct ipw_priv *priv =
////		container_of(work, struct ipw_priv, led_link_on.work);
////	mutex_lock(&priv->mutex);
////	ipw_led_link_on(priv);
////	mutex_unlock(&priv->mutex);
////}
//
////static void ipw_led_link_off(struct ipw_priv *priv)
////{
////	unsigned long flags;
////	u32 led;
//
//	/* If configured not to use LEDs, or nic type is 1,
//	 * then we don't goggle the LINK led. */
////	if (priv->config & CFG_NO_LED || priv->nic_type == EEPROM_NIC_TYPE_1)
////		return;
//
////	spin_lock_irqsave(&priv->lock, flags);
//
////	if (priv->status & STATUS_LED_LINK_ON) {
////		led = ipw_read_reg32(priv, IPW_EVENT_REG);
////		led &= priv->led_association_off;
////		led = ipw_register_toggle(led);
//
////		IPW_DEBUG_LED("Reg: 0x%08X\n", led);
////		ipw_write_reg32(priv, IPW_EVENT_REG, led);
//
////		IPW_DEBUG_LED("Link LED Off\n");
//
////		priv->status &= ~STATUS_LED_LINK_ON;
//
//		/* If we aren't associated and the radio is on, schedule
//		 * turning the LED on (blink while unassociated) */
////		if (!(priv->status & STATUS_RF_KILL_MASK) &&
////		    !(priv->status & STATUS_ASSOCIATED))
////			queue_delayed_work(priv->workqueue, &priv->led_link_on,
////					   LD_TIME_LINK_OFF);
//
////	}
//
////	spin_unlock_irqrestore(&priv->lock, flags);
////}
///*
//static void ipw_bg_led_link_off(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, led_link_off.work);
//	mutex_lock(&priv->mutex);
//	ipw_led_link_off(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static void __ipw_led_activity_on(struct ipw_priv *priv)
//{
//	u32 led;
//
//	if (priv->config & CFG_NO_LED)
//		return;
//
//	if (priv->status & STATUS_RF_KILL_MASK)
//		return;
//
//	if (!(priv->status & STATUS_LED_ACT_ON)) {
//		led = ipw_read_reg32(priv, IPW_EVENT_REG);
//		led |= priv->led_activity_on;
//
//		led = ipw_register_toggle(led);
//
//		IPW_DEBUG_LED("Reg: 0x%08X\n", led);
//		ipw_write_reg32(priv, IPW_EVENT_REG, led);
//
//		IPW_DEBUG_LED("Activity LED On\n");
//
//		priv->status |= STATUS_LED_ACT_ON;
//
//		cancel_delayed_work(&priv->led_act_off);
//		queue_delayed_work(priv->workqueue, &priv->led_act_off,
//				   LD_TIME_ACT_ON);
//	} else {*/
//		/* Reschedule LED off for full time period */
////		cancel_delayed_work(&priv->led_act_off);
////		queue_delayed_work(priv->workqueue, &priv->led_act_off,
////				   LD_TIME_ACT_ON);
////	}
////}
//
////#if 0
////void ipw_led_activity_on(struct ipw_priv *priv)
////{
////	unsigned long flags;
////	spin_lock_irqsave(&priv->lock, flags);
////	__ipw_led_activity_on(priv);
////	spin_unlock_irqrestore(&priv->lock, flags);
////}
////#endif  /*  0  */
///*
//static void ipw_led_activity_off(struct ipw_priv *priv)
//{
//	unsigned long flags;
//	u32 led;
//
//	if (priv->config & CFG_NO_LED)
//		return;
//
//	spin_lock_irqsave(&priv->lock, flags);
//
//	if (priv->status & STATUS_LED_ACT_ON) {
//		led = ipw_read_reg32(priv, IPW_EVENT_REG);
//		led &= priv->led_activity_off;
//
//		led = ipw_register_toggle(led);
//
//		IPW_DEBUG_LED("Reg: 0x%08X\n", led);
//		ipw_write_reg32(priv, IPW_EVENT_REG, led);
//
//		IPW_DEBUG_LED("Activity LED Off\n");
//
//		priv->status &= ~STATUS_LED_ACT_ON;
//	}
//
//	spin_unlock_irqrestore(&priv->lock, flags);
//}
//
//static void ipw_bg_led_activity_off(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, led_act_off.work);
//	mutex_lock(&priv->mutex);
//	ipw_led_activity_off(priv);
//	mutex_unlock(&priv->mutex);
//}
//*/
////static void ipw_led_band_on(struct ipw_priv *priv)
////{
////	unsigned long flags;
////	u32 led;
//
//	/* Only nic type 1 supports mode LEDs */
////	if (priv->config & CFG_NO_LED ||
////	    priv->nic_type != EEPROM_NIC_TYPE_1 || !priv->assoc_network)
////		return;
//
////	spin_lock_irqsave(&priv->lock, flags);
//
////	led = ipw_read_reg32(priv, IPW_EVENT_REG);
////	if (priv->assoc_network->mode == IEEE_A) {
////		led |= priv->led_ofdm_on;
////		led &= priv->led_association_off;
////		IPW_DEBUG_LED("Mode LED On: 802.11a\n");
////	} else if (priv->assoc_network->mode == IEEE_G) {
////		led |= priv->led_ofdm_on;
////		led |= priv->led_association_on;
////		IPW_DEBUG_LED("Mode LED On: 802.11g\n");
////	} else {
////		led &= priv->led_ofdm_off;
////		led |= priv->led_association_on;
////		IPW_DEBUG_LED("Mode LED On: 802.11b\n");
////	}
//
////	led = ipw_register_toggle(led);
//
////	IPW_DEBUG_LED("Reg: 0x%08X\n", led);
////	ipw_write_reg32(priv, IPW_EVENT_REG, led);
//
////	spin_unlock_irqrestore(&priv->lock, flags);
////}
//
////static void ipw_led_band_off(struct ipw_priv *priv)
////{
////	unsigned long flags;
////	u32 led;
//
//	/* Only nic type 1 supports mode LEDs */
////	if (priv->config & CFG_NO_LED || priv->nic_type != EEPROM_NIC_TYPE_1)
////		return;
//
////	spin_lock_irqsave(&priv->lock, flags);
//
////	led = ipw_read_reg32(priv, IPW_EVENT_REG);
////	led &= priv->led_ofdm_off;
////	led &= priv->led_association_off;
//
////	led = ipw_register_toggle(led);
//
////	IPW_DEBUG_LED("Reg: 0x%08X\n", led);
////	ipw_write_reg32(priv, IPW_EVENT_REG, led);
//
////	spin_unlock_irqrestore(&priv->lock, flags);
////}
///*
//static void ipw_led_radio_on(struct ipw_priv *priv)
//{
//	ipw_led_link_on(priv);
//}
//
//static void ipw_led_radio_off(struct ipw_priv *priv)
//{
//	ipw_led_activity_off(priv);
//	ipw_led_link_off(priv);
//}
//*/
////static void ipw_led_link_up(struct ipw_priv *priv)
////{
//	/* Set the Link Led on for all nic types */
////	ipw_led_link_on(priv);
////}
//
////static void ipw_led_link_down(struct ipw_priv *priv)
////{
////	ipw_led_activity_off(priv);
////	ipw_led_link_off(priv);
//
////	if (priv->status & STATUS_RF_KILL_MASK)
////		ipw_led_radio_off(priv);
////}
//
////static void ipw_led_init(struct ipw_priv *priv)
////{
////	priv->nic_type = priv->eeprom[EEPROM_NIC_TYPE];
//
//	/* Set the default PINs for the link and activity leds */
////	priv->led_activity_on = IPW_ACTIVITY_LED;
////	priv->led_activity_off = ~(IPW_ACTIVITY_LED);
//
////	priv->led_association_on = IPW_ASSOCIATED_LED;
////	priv->led_association_off = ~(IPW_ASSOCIATED_LED);
//
//	/* Set the default PINs for the OFDM leds */
////	priv->led_ofdm_on = IPW_OFDM_LED;
////	priv->led_ofdm_off = ~(IPW_OFDM_LED);
//
////	switch (priv->nic_type) {
////	case EEPROM_NIC_TYPE_1:
//		/* In this NIC type, the LEDs are reversed.... */
////		priv->led_activity_on = IPW_ASSOCIATED_LED;
////		priv->led_activity_off = ~(IPW_ASSOCIATED_LED);
////		priv->led_association_on = IPW_ACTIVITY_LED;
////		priv->led_association_off = ~(IPW_ACTIVITY_LED);
//
////		if (!(priv->config & CFG_NO_LED))
////			ipw_led_band_on(priv);
//
//		/* And we don't blink link LEDs for this nic, so
//		 * just return here */
////		return;
//
////	case EEPROM_NIC_TYPE_3:
////	case EEPROM_NIC_TYPE_2:
////	case EEPROM_NIC_TYPE_4:
////	case EEPROM_NIC_TYPE_0:
////		break;
//
////	default:
////		IPW_DEBUG_INFO("Unknown NIC type from EEPROM: %d\n",
////			       priv->nic_type);
////		priv->nic_type = EEPROM_NIC_TYPE_0;
////		break;
////	}
//
////	if (!(priv->config & CFG_NO_LED)) {
////		if (priv->status & STATUS_ASSOCIATED)
////			ipw_led_link_on(priv);
////		else
////			ipw_led_link_off(priv);
////	}
////}
///*
//static void ipw_led_shutdown(struct ipw_priv *priv)
//{
//	ipw_led_activity_off(priv);
//	ipw_led_link_off(priv);
//	ipw_led_band_off(priv);
//	cancel_delayed_work(&priv->led_link_on);
//	cancel_delayed_work(&priv->led_link_off);
//	cancel_delayed_work(&priv->led_act_off);
//}
//*/
///*
// * The following adds a new attribute to the sysfs representation
// * of this device driver (i.e. a new file in /sys/bus/pci/drivers/ipw/)
// * used for controling the debug level.
// *
// * See the level definitions in ipw for details.
// */
///*
//static ssize_t show_debug_level(struct device_driver *d, char *buf)
//{
//	return sprintf(buf, "0x%08X\n", ipw_debug_level);
//}
//
//static ssize_t store_debug_level(struct device_driver *d, const char *buf,
//				 size_t count)
//{
//	char *p = (char *)buf;
//	u32 val;
//
//	if (p[1] == 'x' || p[1] == 'X' || p[0] == 'x' || p[0] == 'X') {
//		p++;
//		if (p[0] == 'x' || p[0] == 'X')
//			p++;
//		val = simple_strtoul(p, &p, 16);
//	} else
//		val = simple_strtoul(p, &p, 10);
//	if (p == buf)
//		printk(KERN_INFO DRV_NAME
//		       ": %s is not in hex or decimal form.\n", buf);
//	else
//		ipw_debug_level = val;
//
//	return strnlen(buf, count);
//}
//
//static DRIVER_ATTR(debug_level, S_IWUSR | S_IRUGO,
//		   show_debug_level, store_debug_level);
//*/
////static inline u32 ipw_get_event_log_len(struct ipw_priv *priv)
////{
//	/* length = 1st dword in log */
////	return ipw_read_reg32(priv, ipw_read32(priv, IPW_EVENT_LOG));
////}
///*
//static void ipw_capture_event_log(struct ipw_priv *priv,
//				  u32 log_len, struct ipw_event *log)
//{
//	u32 base;
//
//	if (log_len) {
//		base = ipw_read32(priv, IPW_EVENT_LOG);
//		ipw_read_indirect(priv, base + sizeof(base) + sizeof(u32),
//				  (u8 *) log, sizeof(*log) * log_len);
//	}
//}
//
//static struct ipw_fw_error *ipw_alloc_error_log(struct ipw_priv *priv)
//{
//	struct ipw_fw_error *error;
//	u32 log_len = ipw_get_event_log_len(priv);
//	u32 base = ipw_read32(priv, IPW_ERROR_LOG);
//	u32 elem_len = ipw_read_reg32(priv, base);
//
//	error = kmalloc(sizeof(*error) +
//			sizeof(*error->elem) * elem_len +
//			sizeof(*error->log) * log_len, GFP_ATOMIC);
//	if (!error) {
//		IPW_ERROR("Memory allocation for firmware error log "
//			  "failed.\n");
//		return NULL;
//	}
//	error->jiffies = jiffies;
//	error->status = priv->status;
//	error->config = priv->config;
//	error->elem_len = elem_len;
//	error->log_len = log_len;
//	error->elem = (struct ipw_error_elem *)error->payload;
//	error->log = (struct ipw_event *)(error->elem + elem_len);
//
//	ipw_capture_event_log(priv, log_len, error->log);
//
//	if (elem_len)
//		ipw_read_indirect(priv, base + sizeof(base), (u8 *) error->elem,
//				  sizeof(*error->elem) * elem_len);
//
//	return error;
//}
//*/
////static ssize_t show_event_log(struct device *d,
////			      struct device_attribute *attr, char *buf)
////{
////	struct ipw_priv *priv = dev_get_drvdata(d);
////	u32 log_len = ipw_get_event_log_len(priv);
////	u32 log_size;
////	struct ipw_event *log;
////	u32 len = 0, i;
//
//	/* not using min() because of its strict type checking */
////	log_size = PAGE_SIZE / sizeof(*log) > log_len ?
////			sizeof(*log) * log_len : PAGE_SIZE;
////	log = kzalloc(log_size, GFP_KERNEL);
////	if (!log) {
////		IPW_ERROR("Unable to allocate memory for log\n");
////		return 0;
////	}
////	log_len = log_size / sizeof(*log);
////	ipw_capture_event_log(priv, log_len, log);
//
////	len += snprintf(buf + len, PAGE_SIZE - len, "%08X", log_len);
////	for (i = 0; i < log_len; i++)
////		len += snprintf(buf + len, PAGE_SIZE - len,
////				"\n%08X%08X%08X",
////				log[i].time, log[i].event, log[i].data);
////	len += snprintf(buf + len, PAGE_SIZE - len, "\n");
////	kfree(log);
////	return len;
////}
///*
//static DEVICE_ATTR(event_log, S_IRUGO, show_event_log, NULL);
//
//static ssize_t show_error(struct device *d,
//			  struct device_attribute *attr, char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	u32 len = 0, i;
//	if (!priv->error)
//		return 0;
//	len += snprintf(buf + len, PAGE_SIZE - len,
//			"%08lX%08X%08X%08X",
//			priv->error->jiffies,
//			priv->error->status,
//			priv->error->config, priv->error->elem_len);
//	for (i = 0; i < priv->error->elem_len; i++)
//		len += snprintf(buf + len, PAGE_SIZE - len,
//				"\n%08X%08X%08X%08X%08X%08X%08X",
//				priv->error->elem[i].time,
//				priv->error->elem[i].desc,
//				priv->error->elem[i].blink1,
//				priv->error->elem[i].blink2,
//				priv->error->elem[i].link1,
//				priv->error->elem[i].link2,
//				priv->error->elem[i].data);
//
//	len += snprintf(buf + len, PAGE_SIZE - len,
//			"\n%08X", priv->error->log_len);
//	for (i = 0; i < priv->error->log_len; i++)
//		len += snprintf(buf + len, PAGE_SIZE - len,
//				"\n%08X%08X%08X",
//				priv->error->log[i].time,
//				priv->error->log[i].event,
//				priv->error->log[i].data);
//	len += snprintf(buf + len, PAGE_SIZE - len, "\n");
//	return len;
//}
//
//static ssize_t clear_error(struct device *d,
//			   struct device_attribute *attr,
//			   const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	kfree(priv->error);
//	priv->error = NULL;
//	return count;
//}
//
//static DEVICE_ATTR(error, S_IRUGO | S_IWUSR, show_error, clear_error);
//
//static ssize_t show_cmd_log(struct device *d,
//			    struct device_attribute *attr, char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	u32 len = 0, i;
//	if (!priv->cmdlog)
//		return 0;
//	for (i = (priv->cmdlog_pos + 1) % priv->cmdlog_len;
//	     (i != priv->cmdlog_pos) && (PAGE_SIZE - len);
//	     i = (i + 1) % priv->cmdlog_len) {
//		len +=
//		    snprintf(buf + len, PAGE_SIZE - len,
//			     "\n%08lX%08X%08X%08X\n", priv->cmdlog[i].jiffies,
//			     priv->cmdlog[i].retcode, priv->cmdlog[i].cmd.cmd,
//			     priv->cmdlog[i].cmd.len);
//		len +=
//		    snprintk_buf(buf + len, PAGE_SIZE - len,
//				 (u8 *) priv->cmdlog[i].cmd.param,
//				 priv->cmdlog[i].cmd.len);
//		len += snprintf(buf + len, PAGE_SIZE - len, "\n");
//	}
//	len += snprintf(buf + len, PAGE_SIZE - len, "\n");
//	return len;
//}
//
//static DEVICE_ATTR(cmd_log, S_IRUGO, show_cmd_log, NULL);
//
//#ifdef CONFIG_IPW2200_PROMISCUOUS
//static void ipw_prom_free(struct ipw_priv *priv);
//static int ipw_prom_alloc(struct ipw_priv *priv);
//static ssize_t store_rtap_iface(struct device *d,
//			 struct device_attribute *attr,
//			 const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	int rc = 0;
//
//	if (count < 1)
//		return -EINVAL;
//
//	switch (buf[0]) {
//	case '0':
//		if (!rtap_iface)
//			return count;
//
//		if (netif_running(priv->prom_net_dev)) {
//			IPW_WARNING("Interface is up.  Cannot unregister.\n");
//			return count;
//		}
//
//		ipw_prom_free(priv);
//		rtap_iface = 0;
//		break;
//
//	case '1':
//		if (rtap_iface)
//			return count;
//
//		rc = ipw_prom_alloc(priv);
//		if (!rc)
//			rtap_iface = 1;
//		break;
//
//	default:
//		return -EINVAL;
//	}
//
//	if (rc) {
//		IPW_ERROR("Failed to register promiscuous network "
//			  "device (error %d).\n", rc);
//	}
//
//	return count;
//}
//
//static ssize_t show_rtap_iface(struct device *d,
//			struct device_attribute *attr,
//			char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	if (rtap_iface)
//		return sprintf(buf, "%s", priv->prom_net_dev->name);
//	else {
//		buf[0] = '-';
//		buf[1] = '1';
//		buf[2] = '\0';
//		return 3;
//	}
//}
//
//static DEVICE_ATTR(rtap_iface, S_IWUSR | S_IRUSR, show_rtap_iface,
//		   store_rtap_iface);
//
//static ssize_t store_rtap_filter(struct device *d,
//			 struct device_attribute *attr,
//			 const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	if (!priv->prom_priv) {
//		IPW_ERROR("Attempting to set filter without "
//			  "rtap_iface enabled.\n");
//		return -EPERM;
//	}
//
//	priv->prom_priv->filter = simple_strtol(buf, NULL, 0);
//
//	IPW_DEBUG_INFO("Setting rtap filter to " BIT_FMT16 "\n",
//		       BIT_ARG16(priv->prom_priv->filter));
//
//	return count;
//}
//
//static ssize_t show_rtap_filter(struct device *d,
//			struct device_attribute *attr,
//			char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	return sprintf(buf, "0x%04X",
//		       priv->prom_priv ? priv->prom_priv->filter : 0);
//}
//
//static DEVICE_ATTR(rtap_filter, S_IWUSR | S_IRUSR, show_rtap_filter,
//		   store_rtap_filter);
//#endif
//
//static ssize_t show_scan_age(struct device *d, struct device_attribute *attr,
//			     char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	return sprintf(buf, "%d\n", priv->ieee->scan_age);
//}
//
//static ssize_t store_scan_age(struct device *d, struct device_attribute *attr,
//			      const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	struct net_device *dev = priv->net_dev;
//	char buffer[] = "00000000";
//	unsigned long len =
//	    (sizeof(buffer) - 1) > count ? count : sizeof(buffer) - 1;
//	unsigned long val;
//	char *p = buffer;
//
//	IPW_DEBUG_INFO("enter\n");
//
//	strncpy(buffer, buf, len);
//	buffer[len] = 0;
//
//	if (p[1] == 'x' || p[1] == 'X' || p[0] == 'x' || p[0] == 'X') {
//		p++;
//		if (p[0] == 'x' || p[0] == 'X')
//			p++;
//		val = simple_strtoul(p, &p, 16);
//	} else
//		val = simple_strtoul(p, &p, 10);
//	if (p == buffer) {
//		IPW_DEBUG_INFO("%s: user supplied invalid value.\n", dev->name);
//	} else {
//		priv->ieee->scan_age = val;
//		IPW_DEBUG_INFO("set scan_age = %u\n", priv->ieee->scan_age);
//	}
//
//	IPW_DEBUG_INFO("exit\n");
//	return len;
//}
//
//static DEVICE_ATTR(scan_age, S_IWUSR | S_IRUGO, show_scan_age, store_scan_age);
//
//static ssize_t show_led(struct device *d, struct device_attribute *attr,
//			char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	return sprintf(buf, "%d\n", (priv->config & CFG_NO_LED) ? 0 : 1);
//}
//
//static ssize_t store_led(struct device *d, struct device_attribute *attr,
//			 const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	IPW_DEBUG_INFO("enter\n");
//
//	if (count == 0)
//		return 0;
//
//	if (*buf == 0) {
//		IPW_DEBUG_LED("Disabling LED control.\n");
//		priv->config |= CFG_NO_LED;
//		ipw_led_shutdown(priv);
//	} else {
//		IPW_DEBUG_LED("Enabling LED control.\n");
//		priv->config &= ~CFG_NO_LED;
//		ipw_led_init(priv);
//	}
//
//	IPW_DEBUG_INFO("exit\n");
//	return count;
//}
//
//static DEVICE_ATTR(led, S_IWUSR | S_IRUGO, show_led, store_led);
//
//static ssize_t show_status(struct device *d,
//			   struct device_attribute *attr, char *buf)
//{
//	struct ipw_priv *p = dev_get_drvdata(d);
//	return sprintf(buf, "0x%08x\n", (int)p->status);
//}
//
//static DEVICE_ATTR(status, S_IRUGO, show_status, NULL);
//
//static ssize_t show_cfg(struct device *d, struct device_attribute *attr,
//			char *buf)
//{
//	struct ipw_priv *p = dev_get_drvdata(d);
//	return sprintf(buf, "0x%08x\n", (int)p->config);
//}
//
//static DEVICE_ATTR(cfg, S_IRUGO, show_cfg, NULL);
//
//static ssize_t show_nic_type(struct device *d,
//			     struct device_attribute *attr, char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	return sprintf(buf, "TYPE: %d\n", priv->nic_type);
//}
//
//static DEVICE_ATTR(nic_type, S_IRUGO, show_nic_type, NULL);
//
//static ssize_t show_ucode_version(struct device *d,
//				  struct device_attribute *attr, char *buf)
//{
//	u32 len = sizeof(u32), tmp = 0;
//	struct ipw_priv *p = dev_get_drvdata(d);
//
//	if (ipw_get_ordinal(p, IPW_ORD_STAT_UCODE_VERSION, &tmp, &len))
//		return 0;
//
//	return sprintf(buf, "0x%08x\n", tmp);
//}
//
//static DEVICE_ATTR(ucode_version, S_IWUSR | S_IRUGO, show_ucode_version, NULL);
//
//static ssize_t show_rtc(struct device *d, struct device_attribute *attr,
//			char *buf)
//{
//	u32 len = sizeof(u32), tmp = 0;
//	struct ipw_priv *p = dev_get_drvdata(d);
//
//	if (ipw_get_ordinal(p, IPW_ORD_STAT_RTC, &tmp, &len))
//		return 0;
//
//	return sprintf(buf, "0x%08x\n", tmp);
//}
//
//static DEVICE_ATTR(rtc, S_IWUSR | S_IRUGO, show_rtc, NULL);
//*/
///*
// * Add a device attribute to view/control the delay between eeprom
// * operations.
// */
///*
//static ssize_t show_eeprom_delay(struct device *d,
//				 struct device_attribute *attr, char *buf)
//{
//	struct ipw_priv *p = dev_get_drvdata(d);
//	int n = p->eeprom_delay;
//	return sprintf(buf, "%i\n", n);
//}
//static ssize_t store_eeprom_delay(struct device *d,
//				  struct device_attribute *attr,
//				  const char *buf, size_t count)
//{
//	struct ipw_priv *p = dev_get_drvdata(d);
//	sscanf(buf, "%i", &p->eeprom_delay);
//	return strnlen(buf, count);
//}
//
//static DEVICE_ATTR(eeprom_delay, S_IWUSR | S_IRUGO,
//		   show_eeprom_delay, store_eeprom_delay);
//
//static ssize_t show_command_event_reg(struct device *d,
//				      struct device_attribute *attr, char *buf)
//{
//	u32 reg = 0;
//	struct ipw_priv *p = dev_get_drvdata(d);
//
//	reg = ipw_read_reg32(p, IPW_INTERNAL_CMD_EVENT);
//	return sprintf(buf, "0x%08x\n", reg);
//}
//static ssize_t store_command_event_reg(struct device *d,
//				       struct device_attribute *attr,
//				       const char *buf, size_t count)
//{
//	u32 reg;
//	struct ipw_priv *p = dev_get_drvdata(d);
//
//	sscanf(buf, "%x", &reg);
//	ipw_write_reg32(p, IPW_INTERNAL_CMD_EVENT, reg);
//	return strnlen(buf, count);
//}
//
//static DEVICE_ATTR(command_event_reg, S_IWUSR | S_IRUGO,
//		   show_command_event_reg, store_command_event_reg);
//
//static ssize_t show_mem_gpio_reg(struct device *d,
//				 struct device_attribute *attr, char *buf)
//{
//	u32 reg = 0;
//	struct ipw_priv *p = dev_get_drvdata(d);
//
//	reg = ipw_read_reg32(p, 0x301100);
//	return sprintf(buf, "0x%08x\n", reg);
//}
//static ssize_t store_mem_gpio_reg(struct device *d,
//				  struct device_attribute *attr,
//				  const char *buf, size_t count)
//{
//	u32 reg;
//	struct ipw_priv *p = dev_get_drvdata(d);
//
//	sscanf(buf, "%x", &reg);
//	ipw_write_reg32(p, 0x301100, reg);
//	return strnlen(buf, count);
//}
//
//static DEVICE_ATTR(mem_gpio_reg, S_IWUSR | S_IRUGO,
//		   show_mem_gpio_reg, store_mem_gpio_reg);
//
//static ssize_t show_indirect_dword(struct device *d,
//				   struct device_attribute *attr, char *buf)
//{
//	u32 reg = 0;
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	if (priv->status & STATUS_INDIRECT_DWORD)
//		reg = ipw_read_reg32(priv, priv->indirect_dword);
//	else
//		reg = 0;
//
//	return sprintf(buf, "0x%08x\n", reg);
//}
//static ssize_t store_indirect_dword(struct device *d,
//				    struct device_attribute *attr,
//				    const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	sscanf(buf, "%x", &priv->indirect_dword);
//	priv->status |= STATUS_INDIRECT_DWORD;
//	return strnlen(buf, count);
//}
//
//static DEVICE_ATTR(indirect_dword, S_IWUSR | S_IRUGO,
//		   show_indirect_dword, store_indirect_dword);
//
//static ssize_t show_indirect_byte(struct device *d,
//				  struct device_attribute *attr, char *buf)
//{
//	u8 reg = 0;
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	if (priv->status & STATUS_INDIRECT_BYTE)
//		reg = ipw_read_reg8(priv, priv->indirect_byte);
//	else
//		reg = 0;
//
//	return sprintf(buf, "0x%02x\n", reg);
//}
//static ssize_t store_indirect_byte(struct device *d,
//				   struct device_attribute *attr,
//				   const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	sscanf(buf, "%x", &priv->indirect_byte);
//	priv->status |= STATUS_INDIRECT_BYTE;
//	return strnlen(buf, count);
//}
//
//static DEVICE_ATTR(indirect_byte, S_IWUSR | S_IRUGO,
//		   show_indirect_byte, store_indirect_byte);
//
//static ssize_t show_direct_dword(struct device *d,
//				 struct device_attribute *attr, char *buf)
//{
//	u32 reg = 0;
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	if (priv->status & STATUS_DIRECT_DWORD)
//		reg = ipw_read32(priv, priv->direct_dword);
//	else
//		reg = 0;
//
//	return sprintf(buf, "0x%08x\n", reg);
//}
//static ssize_t store_direct_dword(struct device *d,
//				  struct device_attribute *attr,
//				  const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	sscanf(buf, "%x", &priv->direct_dword);
//	priv->status |= STATUS_DIRECT_DWORD;
//	return strnlen(buf, count);
//}
//
//static DEVICE_ATTR(direct_dword, S_IWUSR | S_IRUGO,
//		   show_direct_dword, store_direct_dword);
//
//static int rf_kill_active(struct ipw_priv *priv)
//{
//	if (0 == (ipw_read32(priv, 0x30) & 0x10000))
//		priv->status |= STATUS_RF_KILL_HW;
//	else
//		priv->status &= ~STATUS_RF_KILL_HW;
//
//	return (priv->status & STATUS_RF_KILL_HW) ? 1 : 0;
//}
//*/
////static ssize_t show_rf_kill(struct device *d, struct device_attribute *attr,
////			    char *buf)
////{
//	/* 0 - RF kill not enabled
//	   1 - SW based RF kill active (sysfs)
//	   2 - HW based RF kill active
//	   3 - Both HW and SW baed RF kill active */
////	struct ipw_priv *priv = dev_get_drvdata(d);
////	int val = ((priv->status & STATUS_RF_KILL_SW) ? 0x1 : 0x0) |
////	    (rf_kill_active(priv) ? 0x2 : 0x0);
////	return sprintf(buf, "%i\n", val);
////}
//
////static int ipw_radio_kill_sw(struct ipw_priv *priv, int disable_radio)
////{
////	if ((disable_radio ? 1 : 0) ==
////	    ((priv->status & STATUS_RF_KILL_SW) ? 1 : 0))
////		return 0;
//
////	IPW_DEBUG_RF_KILL("Manual SW RF Kill set to: RADIO  %s\n",
////			  disable_radio ? "OFF" : "ON");
//
////	if (disable_radio) {
////		priv->status |= STATUS_RF_KILL_SW;
//
////		if (priv->workqueue) {
////			cancel_delayed_work(&priv->request_scan);
////			cancel_delayed_work(&priv->request_direct_scan);
////			cancel_delayed_work(&priv->request_passive_scan);
////			cancel_delayed_work(&priv->scan_event);
////		}
////		queue_work(priv->workqueue, &priv->down);
////	} else {
////		priv->status &= ~STATUS_RF_KILL_SW;
////		if (rf_kill_active(priv)) {
////			IPW_DEBUG_RF_KILL("Can not turn radio back on - "
////					  "disabled by HW switch\n");
//			/* Make sure the RF_KILL check timer is running */
////			cancel_delayed_work(&priv->rf_kill);
////			queue_delayed_work(priv->workqueue, &priv->rf_kill,
////					   round_jiffies_relative(2 * HZ));
////		} else
////			queue_work(priv->workqueue, &priv->up);
////	}
//
////	return 1;
////}
///*
//static ssize_t store_rf_kill(struct device *d, struct device_attribute *attr,
//			     const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//
//	ipw_radio_kill_sw(priv, buf[0] == '1');
//
//	return count;
//}
//
//static DEVICE_ATTR(rf_kill, S_IWUSR | S_IRUGO, show_rf_kill, store_rf_kill);
//
//static ssize_t show_speed_scan(struct device *d, struct device_attribute *attr,
//			       char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	int pos = 0, len = 0;
//	if (priv->config & CFG_SPEED_SCAN) {
//		while (priv->speed_scan[pos] != 0)
//			len += sprintf(&buf[len], "%d ",
//				       priv->speed_scan[pos++]);
//		return len + sprintf(&buf[len], "\n");
//	}
//
//	return sprintf(buf, "0\n");
//}
//*/
////static ssize_t store_speed_scan(struct device *d, struct device_attribute *attr,
////				const char *buf, size_t count)
////{
////	struct ipw_priv *priv = dev_get_drvdata(d);
////	int channel, pos = 0;
////	const char *p = buf;
//
//	/* list of space separated channels to scan, optionally ending with 0 */
////	while ((channel = simple_strtol(p, NULL, 0))) {
////		if (pos == MAX_SPEED_SCAN - 1) {
////			priv->speed_scan[pos] = 0;
////			break;
////		}
//
////		if (libipw_is_valid_channel(priv->ieee, channel))
////			priv->speed_scan[pos++] = channel;
////		else
////			IPW_WARNING("Skipping invalid channel request: %d\n",
////				    channel);
////		p = strchr(p, ' ');
////		if (!p)
////			break;
////		while (*p == ' ' || *p == '\t')
////			p++;
////	}
//
////	if (pos == 0)
////		priv->config &= ~CFG_SPEED_SCAN;
////	else {
////		priv->speed_scan_pos = 0;
////		priv->config |= CFG_SPEED_SCAN;
////	}
//
////	return count;
////}
///*
//static DEVICE_ATTR(speed_scan, S_IWUSR | S_IRUGO, show_speed_scan,
//		   store_speed_scan);
//
//static ssize_t show_net_stats(struct device *d, struct device_attribute *attr,
//			      char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	return sprintf(buf, "%c\n", (priv->config & CFG_NET_STATS) ? '1' : '0');
//}
//
//static ssize_t store_net_stats(struct device *d, struct device_attribute *attr,
//			       const char *buf, size_t count)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	if (buf[0] == '1')
//		priv->config |= CFG_NET_STATS;
//	else
//		priv->config &= ~CFG_NET_STATS;
//
//	return count;
//}
//
//static DEVICE_ATTR(net_stats, S_IWUSR | S_IRUGO,
//		   show_net_stats, store_net_stats);
//
//static ssize_t show_channels(struct device *d,
//			     struct device_attribute *attr,
//			     char *buf)
//{
//	struct ipw_priv *priv = dev_get_drvdata(d);
//	const struct libipw_geo *geo = libipw_get_geo(priv->ieee);
//	int len = 0, i;
//
//	len = sprintf(&buf[len],
//		      "Displaying %d channels in 2.4Ghz band "
//		      "(802.11bg):\n", geo->bg_channels);
//
//	for (i = 0; i < geo->bg_channels; i++) {
//		len += sprintf(&buf[len], "%d: BSS%s%s, %s, Band %s.\n",
//			       geo->bg[i].channel,
//			       geo->bg[i].flags & LIBIPW_CH_RADAR_DETECT ?
//			       " (radar spectrum)" : "",
//			       ((geo->bg[i].flags & LIBIPW_CH_NO_IBSS) ||
//				(geo->bg[i].flags & LIBIPW_CH_RADAR_DETECT))
//			       ? "" : ", IBSS",
//			       geo->bg[i].flags & LIBIPW_CH_PASSIVE_ONLY ?
//			       "passive only" : "active/passive",
//			       geo->bg[i].flags & LIBIPW_CH_B_ONLY ?
//			       "B" : "B/G");
//	}
//
//	len += sprintf(&buf[len],
//		       "Displaying %d channels in 5.2Ghz band "
//		       "(802.11a):\n", geo->a_channels);
//	for (i = 0; i < geo->a_channels; i++) {
//		len += sprintf(&buf[len], "%d: BSS%s%s, %s.\n",
//			       geo->a[i].channel,
//			       geo->a[i].flags & LIBIPW_CH_RADAR_DETECT ?
//			       " (radar spectrum)" : "",
//			       ((geo->a[i].flags & LIBIPW_CH_NO_IBSS) ||
//				(geo->a[i].flags & LIBIPW_CH_RADAR_DETECT))
//			       ? "" : ", IBSS",
//			       geo->a[i].flags & LIBIPW_CH_PASSIVE_ONLY ?
//			       "passive only" : "active/passive");
//	}
//
//	return len;
//}
//
//static DEVICE_ATTR(channels, S_IRUSR, show_channels, NULL);
//
//static void notify_wx_assoc_event(struct ipw_priv *priv)
//{
//	union iwreq_data wrqu;
//	wrqu.ap_addr.sa_family = ARPHRD_ETHER;
//	if (priv->status & STATUS_ASSOCIATED)
//		memcpy(wrqu.ap_addr.sa_data, priv->bssid, ETH_ALEN);
//	else
//		memset(wrqu.ap_addr.sa_data, 0, ETH_ALEN);
//	wireless_send_event(priv->net_dev, SIOCGIWAP, &wrqu, NULL);
//}
//
//static void ipw_irq_tasklet(struct ipw_priv *priv)
//{
//	u32 inta, inta_mask, handled = 0;
//	unsigned long flags;
//	int rc = 0;
//
//	spin_lock_irqsave(&priv->irq_lock, flags);
//
//	inta = ipw_read32(priv, IPW_INTA_RW);
//	inta_mask = ipw_read32(priv, IPW_INTA_MASK_R);
//	inta &= (IPW_INTA_MASK_ALL & inta_mask);
//*/
//	/* Add any cached INTA values that need to be handled */
////	inta |= priv->isr_inta;
//
////	spin_unlock_irqrestore(&priv->irq_lock, flags);
//
////	spin_lock_irqsave(&priv->lock, flags);
//
//	/* handle all the justifications for the interrupt */
///*
//	if (inta & IPW_INTA_BIT_RX_TRANSFER) {
//		ipw_rx(priv);
//		handled |= IPW_INTA_BIT_RX_TRANSFER;
//	}
//
//	if (inta & IPW_INTA_BIT_TX_CMD_QUEUE) {
//		IPW_DEBUG_HC("Command completed.\n");
//		rc = ipw_queue_tx_reclaim(priv, &priv->txq_cmd, -1);
//		priv->status &= ~STATUS_HCMD_ACTIVE;
//		wake_up_interruptible(&priv->wait_command_queue);
//		handled |= IPW_INTA_BIT_TX_CMD_QUEUE;
//	}
//
//	if (inta & IPW_INTA_BIT_TX_QUEUE_1) {
//		IPW_DEBUG_TX("TX_QUEUE_1\n");
//		rc = ipw_queue_tx_reclaim(priv, &priv->txq[0], 0);
//		handled |= IPW_INTA_BIT_TX_QUEUE_1;
//	}
//
//	if (inta & IPW_INTA_BIT_TX_QUEUE_2) {
//		IPW_DEBUG_TX("TX_QUEUE_2\n");
//		rc = ipw_queue_tx_reclaim(priv, &priv->txq[1], 1);
//		handled |= IPW_INTA_BIT_TX_QUEUE_2;
//	}
//
//	if (inta & IPW_INTA_BIT_TX_QUEUE_3) {
//		IPW_DEBUG_TX("TX_QUEUE_3\n");
//		rc = ipw_queue_tx_reclaim(priv, &priv->txq[2], 2);
//		handled |= IPW_INTA_BIT_TX_QUEUE_3;
//	}
//
//	if (inta & IPW_INTA_BIT_TX_QUEUE_4) {
//		IPW_DEBUG_TX("TX_QUEUE_4\n");
//		rc = ipw_queue_tx_reclaim(priv, &priv->txq[3], 3);
//		handled |= IPW_INTA_BIT_TX_QUEUE_4;
//	}
//
//	if (inta & IPW_INTA_BIT_STATUS_CHANGE) {
//		IPW_WARNING("STATUS_CHANGE\n");
//		handled |= IPW_INTA_BIT_STATUS_CHANGE;
//	}
//
//	if (inta & IPW_INTA_BIT_BEACON_PERIOD_EXPIRED) {
//		IPW_WARNING("TX_PERIOD_EXPIRED\n");
//		handled |= IPW_INTA_BIT_BEACON_PERIOD_EXPIRED;
//	}
//
//	if (inta & IPW_INTA_BIT_SLAVE_MODE_HOST_CMD_DONE) {
//		IPW_WARNING("HOST_CMD_DONE\n");
//		handled |= IPW_INTA_BIT_SLAVE_MODE_HOST_CMD_DONE;
//	}
//
//	if (inta & IPW_INTA_BIT_FW_INITIALIZATION_DONE) {
//		IPW_WARNING("FW_INITIALIZATION_DONE\n");
//		handled |= IPW_INTA_BIT_FW_INITIALIZATION_DONE;
//	}
//
//	if (inta & IPW_INTA_BIT_FW_CARD_DISABLE_PHY_OFF_DONE) {
//		IPW_WARNING("PHY_OFF_DONE\n");
//		handled |= IPW_INTA_BIT_FW_CARD_DISABLE_PHY_OFF_DONE;
//	}
//
//	if (inta & IPW_INTA_BIT_RF_KILL_DONE) {
//		IPW_DEBUG_RF_KILL("RF_KILL_DONE\n");
//		priv->status |= STATUS_RF_KILL_HW;
//		wake_up_interruptible(&priv->wait_command_queue);
//		priv->status &= ~(STATUS_ASSOCIATED | STATUS_ASSOCIATING);
//		cancel_delayed_work(&priv->request_scan);
//		cancel_delayed_work(&priv->request_direct_scan);
//		cancel_delayed_work(&priv->request_passive_scan);
//		cancel_delayed_work(&priv->scan_event);
//		schedule_work(&priv->link_down);
//		queue_delayed_work(priv->workqueue, &priv->rf_kill, 2 * HZ);
//		handled |= IPW_INTA_BIT_RF_KILL_DONE;
//	}
//
//	if (inta & IPW_INTA_BIT_FATAL_ERROR) {
//		IPW_WARNING("Firmware error detected.  Restarting.\n");
//		if (priv->error) {
//			IPW_DEBUG_FW("Sysfs 'error' log already exists.\n");
//			if (ipw_debug_level & IPW_DL_FW_ERRORS) {
//				struct ipw_fw_error *error =
//				    ipw_alloc_error_log(priv);
//				ipw_dump_error_log(priv, error);
//				kfree(error);
//			}
//		} else {
//			priv->error = ipw_alloc_error_log(priv);
//			if (priv->error)
//				IPW_DEBUG_FW("Sysfs 'error' log captured.\n");
//			else
//				IPW_DEBUG_FW("Error allocating sysfs 'error' "
//					     "log.\n");
//			if (ipw_debug_level & IPW_DL_FW_ERRORS)
//				ipw_dump_error_log(priv, priv->error);
//		}
//*/
//		/* XXX: If hardware encryption is for WPA/WPA2,
//		 * we have to notify the supplicant. */
////		if (priv->ieee->sec.encrypt) {
////			priv->status &= ~STATUS_ASSOCIATED;
////			notify_wx_assoc_event(priv);
////		}
//
//		/* Keep the restart process from trying to send host
//		 * commands by clearing the INIT status bit */
////		priv->status &= ~STATUS_INIT;
//
//		/* Cancel currently queued command. */
///*		priv->status &= ~STATUS_HCMD_ACTIVE;
//		wake_up_interruptible(&priv->wait_command_queue);
//
//		queue_work(priv->workqueue, &priv->adapter_restart);
//		handled |= IPW_INTA_BIT_FATAL_ERROR;
//	}
//
//	if (inta & IPW_INTA_BIT_PARITY_ERROR) {
//		IPW_ERROR("Parity error\n");
//		handled |= IPW_INTA_BIT_PARITY_ERROR;
//	}
//
//	if (handled != inta) {
//		IPW_ERROR("Unhandled INTA bits 0x%08x\n", inta & ~handled);
//	}
//
//	spin_unlock_irqrestore(&priv->lock, flags);
//*/
//	/* enable all interrupts */
////	ipw_enable_interrupts(priv);
////}
///*
//#define IPW_CMD(x) case IPW_CMD_ ## x : return #x
//static char *get_cmd_string(u8 cmd)
//{
//	switch (cmd) {
//		IPW_CMD(HOST_COMPLETE);
//		IPW_CMD(POWER_DOWN);
//		IPW_CMD(SYSTEM_CONFIG);
//		IPW_CMD(MULTICAST_ADDRESS);
//		IPW_CMD(SSID);
//		IPW_CMD(ADAPTER_ADDRESS);
//		IPW_CMD(PORT_TYPE);
//		IPW_CMD(RTS_THRESHOLD);
//		IPW_CMD(FRAG_THRESHOLD);
//		IPW_CMD(POWER_MODE);
//		IPW_CMD(WEP_KEY);
//		IPW_CMD(TGI_TX_KEY);
//		IPW_CMD(SCAN_REQUEST);
//		IPW_CMD(SCAN_REQUEST_EXT);
//		IPW_CMD(ASSOCIATE);
//		IPW_CMD(SUPPORTED_RATES);
//		IPW_CMD(SCAN_ABORT);
//		IPW_CMD(TX_FLUSH);
//		IPW_CMD(QOS_PARAMETERS);
//		IPW_CMD(DINO_CONFIG);
//		IPW_CMD(RSN_CAPABILITIES);
//		IPW_CMD(RX_KEY);
//		IPW_CMD(CARD_DISABLE);
//		IPW_CMD(SEED_NUMBER);
//		IPW_CMD(TX_POWER);
//		IPW_CMD(COUNTRY_INFO);
//		IPW_CMD(AIRONET_INFO);
//		IPW_CMD(AP_TX_POWER);
//		IPW_CMD(CCKM_INFO);
//		IPW_CMD(CCX_VER_INFO);
//		IPW_CMD(SET_CALIBRATION);
//		IPW_CMD(SENSITIVITY_CALIB);
//		IPW_CMD(RETRY_LIMIT);
//		IPW_CMD(IPW_PRE_POWER_DOWN);
//		IPW_CMD(VAP_BEACON_TEMPLATE);
//		IPW_CMD(VAP_DTIM_PERIOD);
//		IPW_CMD(EXT_SUPPORTED_RATES);
//		IPW_CMD(VAP_LOCAL_TX_PWR_CONSTRAINT);
//		IPW_CMD(VAP_QUIET_INTERVALS);
//		IPW_CMD(VAP_CHANNEL_SWITCH);
//		IPW_CMD(VAP_MANDATORY_CHANNELS);
//		IPW_CMD(VAP_CELL_PWR_LIMIT);
//		IPW_CMD(VAP_CF_PARAM_SET);
//		IPW_CMD(VAP_SET_BEACONING_STATE);
//		IPW_CMD(MEASUREMENT);
//		IPW_CMD(POWER_CAPABILITY);
//		IPW_CMD(SUPPORTED_CHANNELS);
//		IPW_CMD(TPC_REPORT);
//		IPW_CMD(WME_INFO);
//		IPW_CMD(PRODUCTION_COMMAND);
//	default:
//		return "UNKNOWN";
//	}
//}
//
//#define HOST_COMPLETE_TIMEOUT HZ
//
//static int __ipw_send_cmd(struct ipw_priv *priv, struct host_cmd *cmd)
//{
//	int rc = 0;
//	unsigned long flags;
//
//	spin_lock_irqsave(&priv->lock, flags);
//	if (priv->status & STATUS_HCMD_ACTIVE) {
//		IPW_ERROR("Failed to send %s: Already sending a command.\n",
//			  get_cmd_string(cmd->cmd));
//		spin_unlock_irqrestore(&priv->lock, flags);
//		return -EAGAIN;
//	}
//
//	priv->status |= STATUS_HCMD_ACTIVE;
//
//	if (priv->cmdlog) {
//		priv->cmdlog[priv->cmdlog_pos].jiffies = jiffies;
//		priv->cmdlog[priv->cmdlog_pos].cmd.cmd = cmd->cmd;
//		priv->cmdlog[priv->cmdlog_pos].cmd.len = cmd->len;
//		memcpy(priv->cmdlog[priv->cmdlog_pos].cmd.param, cmd->param,
//		       cmd->len);
//		priv->cmdlog[priv->cmdlog_pos].retcode = -1;
//	}
//
//	IPW_DEBUG_HC("%s command (#%d) %d bytes: 0x%08X\n",
//		     get_cmd_string(cmd->cmd), cmd->cmd, cmd->len,
//		     priv->status);
//
//#ifndef DEBUG_CMD_WEP_KEY
//	if (cmd->cmd == IPW_CMD_WEP_KEY)
//		IPW_DEBUG_HC("WEP_KEY command masked out for secure.\n");
//	else
//#endif
//		printk_buf(IPW_DL_HOST_COMMAND, (u8 *) cmd->param, cmd->len);
//
//	rc = ipw_queue_tx_hcmd(priv, cmd->cmd, cmd->param, cmd->len, 0);
//	if (rc) {
//		priv->status &= ~STATUS_HCMD_ACTIVE;
//		IPW_ERROR("Failed to send %s: Reason %d\n",
//			  get_cmd_string(cmd->cmd), rc);
//		spin_unlock_irqrestore(&priv->lock, flags);
//		goto exit;
//	}
//	spin_unlock_irqrestore(&priv->lock, flags);
//
//	rc = wait_event_interruptible_timeout(priv->wait_command_queue,
//					      !(priv->
//						status & STATUS_HCMD_ACTIVE),
//					      HOST_COMPLETE_TIMEOUT);
//	if (rc == 0) {
//		spin_lock_irqsave(&priv->lock, flags);
//		if (priv->status & STATUS_HCMD_ACTIVE) {
//			IPW_ERROR("Failed to send %s: Command timed out.\n",
//				  get_cmd_string(cmd->cmd));
//			priv->status &= ~STATUS_HCMD_ACTIVE;
//			spin_unlock_irqrestore(&priv->lock, flags);
//			rc = -EIO;
//			goto exit;
//		}
//		spin_unlock_irqrestore(&priv->lock, flags);
//	} else
//		rc = 0;
//
//	if (priv->status & STATUS_RF_KILL_HW) {
//		IPW_ERROR("Failed to send %s: Aborted due to RF kill switch.\n",
//			  get_cmd_string(cmd->cmd));
//		rc = -EIO;
//		goto exit;
//	}
//
//      exit:
//	if (priv->cmdlog) {
//		priv->cmdlog[priv->cmdlog_pos++].retcode = rc;
//		priv->cmdlog_pos %= priv->cmdlog_len;
//	}
//	return rc;
//}
//
//static int ipw_send_cmd_simple(struct ipw_priv *priv, u8 command)
//{
//	struct host_cmd cmd = {
//		.cmd = command,
//	};
//
//	return __ipw_send_cmd(priv, &cmd);
//}
//
//static int ipw_send_cmd_pdu(struct ipw_priv *priv, u8 command, u8 len,
//			    void *data)
//{
//	struct host_cmd cmd = {
//		.cmd = command,
//		.len = len,
//		.param = data,
//	};
//
//	return __ipw_send_cmd(priv, &cmd);
//}
//
//static int ipw_send_host_complete(struct ipw_priv *priv)
//{
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_simple(priv, IPW_CMD_HOST_COMPLETE);
//}
//
//static int ipw_send_system_config(struct ipw_priv *priv)
//{
//	return ipw_send_cmd_pdu(priv, IPW_CMD_SYSTEM_CONFIG,
//				sizeof(priv->sys_config),
//				&priv->sys_config);
//}
//
//static int ipw_send_ssid(struct ipw_priv *priv, u8 * ssid, int len)
//{
//	if (!priv || !ssid) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_SSID, min(len, IW_ESSID_MAX_SIZE),
//				ssid);
//}
//
//static int ipw_send_adapter_address(struct ipw_priv *priv, u8 * mac)
//{
//	if (!priv || !mac) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	IPW_DEBUG_INFO("%s: Setting MAC to %pM\n",
//		       priv->net_dev->name, mac);
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_ADAPTER_ADDRESS, ETH_ALEN, mac);
//}
//*/
///*
// * NOTE: This must be executed from our workqueue as it results in udelay
// * being called which may corrupt the keyboard if executed on default
// * workqueue
// */
///* 
//static void ipw_adapter_restart(void *adapter)
//{
//	struct ipw_priv *priv = adapter;
//
//	if (priv->status & STATUS_RF_KILL_MASK)
//		return;
//
//	ipw_down(priv);
//
//	if (priv->assoc_network &&
//	    (priv->assoc_network->capability & WLAN_CAPABILITY_IBSS))
//		ipw_remove_current_network(priv);
//
//	if (ipw_up(priv)) {
//		IPW_ERROR("Failed to up device\n");
//		return;
//	}
//}
//
//static void ipw_bg_adapter_restart(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, adapter_restart);
//	mutex_lock(&priv->mutex);
//	ipw_adapter_restart(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//#define IPW_SCAN_CHECK_WATCHDOG (5 * HZ)
//
//static void ipw_scan_check(void *data)
//{
//	struct ipw_priv *priv = data;
//	if (priv->status & (STATUS_SCANNING | STATUS_SCAN_ABORTING)) {
//		IPW_DEBUG_SCAN("Scan completion watchdog resetting "
//			       "adapter after (%dms).\n",
//			       jiffies_to_msecs(IPW_SCAN_CHECK_WATCHDOG));
//		queue_work(priv->workqueue, &priv->adapter_restart);
//	}
//}
//
//static void ipw_bg_scan_check(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, scan_check.work);
//	mutex_lock(&priv->mutex);
//	ipw_scan_check(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static int ipw_send_scan_request_ext(struct ipw_priv *priv,
//				     struct ipw_scan_request_ext *request)
//{
//	return ipw_send_cmd_pdu(priv, IPW_CMD_SCAN_REQUEST_EXT,
//				sizeof(*request), request);
//}
//
//static int ipw_send_scan_abort(struct ipw_priv *priv)
//{
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_simple(priv, IPW_CMD_SCAN_ABORT);
//}
//
//static int ipw_set_sensitivity(struct ipw_priv *priv, u16 sens)
//{
//	struct ipw_sensitivity_calib calib = {
//		.beacon_rssi_raw = cpu_to_le16(sens),
//	};
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_SENSITIVITY_CALIB, sizeof(calib),
//				&calib);
//}
//
//static int ipw_send_associate(struct ipw_priv *priv,
//			      struct ipw_associate *associate)
//{
//	if (!priv || !associate) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_ASSOCIATE, sizeof(*associate),
//				associate);
//}
//
//static int ipw_send_supported_rates(struct ipw_priv *priv,
//				    struct ipw_supported_rates *rates)
//{
//	if (!priv || !rates) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_SUPPORTED_RATES, sizeof(*rates),
//				rates);
//}
//
//static int ipw_set_random_seed(struct ipw_priv *priv)
//{
//	u32 val;
//
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	get_random_bytes(&val, sizeof(val));
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_SEED_NUMBER, sizeof(val), &val);
//}
//
//static int ipw_send_card_disable(struct ipw_priv *priv, u32 phy_off)
//{
//	__le32 v = cpu_to_le32(phy_off);
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_CARD_DISABLE, sizeof(v), &v);
//}
//
//static int ipw_send_tx_power(struct ipw_priv *priv, struct ipw_tx_power *power)
//{
//	if (!priv || !power) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_TX_POWER, sizeof(*power), power);
//}
//
//static int ipw_set_tx_power(struct ipw_priv *priv)
//{
//	const struct libipw_geo *geo = libipw_get_geo(priv->ieee);
//	struct ipw_tx_power tx_power;
//	s8 max_power;
//	int i;
//
//	memset(&tx_power, 0, sizeof(tx_power));
//*/
//	/* configure device for 'G' band */
///*	tx_power.ieee_mode = IPW_G_MODE;
//	tx_power.num_channels = geo->bg_channels;
//	for (i = 0; i < geo->bg_channels; i++) {
//		max_power = geo->bg[i].max_power;
//		tx_power.channels_tx_power[i].channel_number =
//		    geo->bg[i].channel;
//		tx_power.channels_tx_power[i].tx_power = max_power ?
//		    min(max_power, priv->tx_power) : priv->tx_power;
//	}
//	if (ipw_send_tx_power(priv, &tx_power))
//		return -EIO;
//*/
//	/* configure device to also handle 'B' band */
////	tx_power.ieee_mode = IPW_B_MODE;
////	if (ipw_send_tx_power(priv, &tx_power))
////		return -EIO;
//
//	/* configure device to also handle 'A' band */
///*	if (priv->ieee->abg_true) {
//		tx_power.ieee_mode = IPW_A_MODE;
//		tx_power.num_channels = geo->a_channels;
//		for (i = 0; i < tx_power.num_channels; i++) {
//			max_power = geo->a[i].max_power;
//			tx_power.channels_tx_power[i].channel_number =
//			    geo->a[i].channel;
//			tx_power.channels_tx_power[i].tx_power = max_power ?
//			    min(max_power, priv->tx_power) : priv->tx_power;
//		}
//		if (ipw_send_tx_power(priv, &tx_power))
//			return -EIO;
//	}
//	return 0;
//}
//
//static int ipw_send_rts_threshold(struct ipw_priv *priv, u16 rts)
//{
//	struct ipw_rts_threshold rts_threshold = {
//		.rts_threshold = cpu_to_le16(rts),
//	};
//
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_RTS_THRESHOLD,
//				sizeof(rts_threshold), &rts_threshold);
//}
//
//static int ipw_send_frag_threshold(struct ipw_priv *priv, u16 frag)
//{
//	struct ipw_frag_threshold frag_threshold = {
//		.frag_threshold = cpu_to_le16(frag),
//	};
//
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_FRAG_THRESHOLD,
//				sizeof(frag_threshold), &frag_threshold);
//}
//
//static int ipw_send_power_mode(struct ipw_priv *priv, u32 mode)
//{
//	__le32 param;
//
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//*/
//	/* If on battery, set to 3, if AC set to CAM, else user
//	 * level */
///*	switch (mode) {
//	case IPW_POWER_BATTERY:
//		param = cpu_to_le32(IPW_POWER_INDEX_3);
//		break;
//	case IPW_POWER_AC:
//		param = cpu_to_le32(IPW_POWER_MODE_CAM);
//		break;
//	default:
//		param = cpu_to_le32(mode);
//		break;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_POWER_MODE, sizeof(param),
//				&param);
//}
//
//static int ipw_send_retry_limit(struct ipw_priv *priv, u8 slimit, u8 llimit)
//{
//	struct ipw_retry_limit retry_limit = {
//		.short_retry_limit = slimit,
//		.long_retry_limit = llimit
//	};
//
//	if (!priv) {
//		IPW_ERROR("Invalid args\n");
//		return -1;
//	}
//
//	return ipw_send_cmd_pdu(priv, IPW_CMD_RETRY_LIMIT, sizeof(retry_limit),
//				&retry_limit);
//}
//*/
///*
// * The IPW device contains a Microwire compatible EEPROM that stores
// * various data like the MAC address.  Usually the firmware has exclusive
// * access to the eeprom, but during device initialization (before the
// * device driver has sent the HostComplete command to the firmware) the
// * device driver has read access to the EEPROM by way of indirect addressing
// * through a couple of memory mapped registers.
// *
// * The following is a simplified implementation for pulling data out of the
// * the eeprom, along with some helper functions to find information in
// * the per device private data's copy of the eeprom.
// *
// * NOTE: To better understand how these functions work (i.e what is a chip
// *       select and why do have to keep driving the eeprom clock?), read
// *       just about any data sheet for a Microwire compatible EEPROM.
// */
//
///* write a 32 bit value into the indirect accessor register */
///*static inline void eeprom_write_reg(struct ipw_priv *p, u32 data)
//{
//	ipw_write_reg32(p, FW_MEM_REG_EEPROM_ACCESS, data);
//*/
//	/* the eeprom requires some time to complete the operation */
///*	udelay(p->eeprom_delay);
//
//	return;
//}
//*/
///* perform a chip select operation */
///*static void eeprom_cs(struct ipw_priv *priv)
//{
//	eeprom_write_reg(priv, 0);
//	eeprom_write_reg(priv, EEPROM_BIT_CS);
//	eeprom_write_reg(priv, EEPROM_BIT_CS | EEPROM_BIT_SK);
//	eeprom_write_reg(priv, EEPROM_BIT_CS);
//}
//*/
///* perform a chip select operation */
///*static void eeprom_disable_cs(struct ipw_priv *priv)
//{
//	eeprom_write_reg(priv, EEPROM_BIT_CS);
//	eeprom_write_reg(priv, 0);
//	eeprom_write_reg(priv, EEPROM_BIT_SK);
//}
//*/
///* push a single bit down to the eeprom */
///*static inline void eeprom_write_bit(struct ipw_priv *p, u8 bit)
//{
//	int d = (bit ? EEPROM_BIT_DI : 0);
//	eeprom_write_reg(p, EEPROM_BIT_CS | d);
//	eeprom_write_reg(p, EEPROM_BIT_CS | d | EEPROM_BIT_SK);
//}
//*/
///* push an opcode followed by an address down to the eeprom */
///*static void eeprom_op(struct ipw_priv *priv, u8 op, u8 addr)
//{
//	int i;
//
//	eeprom_cs(priv);
//	eeprom_write_bit(priv, 1);
//	eeprom_write_bit(priv, op & 2);
//	eeprom_write_bit(priv, op & 1);
//	for (i = 7; i >= 0; i--) {
//		eeprom_write_bit(priv, addr & (1 << i));
//	}
//}
//*/
///* pull 16 bits off the eeprom, one bit at a time */
////static u16 eeprom_read_u16(struct ipw_priv *priv, u8 addr)
////{
////	int i;
////	u16 r = 0;
//
//	/* Send READ Opcode */
////	eeprom_op(priv, EEPROM_CMD_READ, addr);
//
//	/* Send dummy bit */
////	eeprom_write_reg(priv, EEPROM_BIT_CS);
//
//	/* Read the byte off the eeprom one bit at a time */
///*	for (i = 0; i < 16; i++) {
//		u32 data = 0;
//		eeprom_write_reg(priv, EEPROM_BIT_CS | EEPROM_BIT_SK);
//		eeprom_write_reg(priv, EEPROM_BIT_CS);
//		data = ipw_read_reg32(priv, FW_MEM_REG_EEPROM_ACCESS);
//		r = (r << 1) | ((data & EEPROM_BIT_DO) ? 1 : 0);
//	}
//*/
//	/* Send another dummy bit */
///*	eeprom_write_reg(priv, 0);
//	eeprom_disable_cs(priv);
//
//	return r;
//}
//*/
///* helper function for pulling the mac address out of the private */
///* data's copy of the eeprom data                                 */
///*static void eeprom_parse_mac(struct ipw_priv *priv, u8 * mac)
//{
//	memcpy(mac, &priv->eeprom[EEPROM_MAC_ADDRESS], 6);
//}
//*/
///*
// * Either the device driver (i.e. the host) or the firmware can
// * load eeprom data into the designated region in SRAM.  If neither
// * happens then the FW will shutdown with a fatal error.
// *
// * In order to signal the FW to load the EEPROM, the EEPROM_LOAD_DISABLE
// * bit needs region of shared SRAM needs to be non-zero.
// */
////static void ipw_eeprom_init_sram(struct ipw_priv *priv)
////{
////	int i;
////	__le16 *eeprom = (__le16 *) priv->eeprom;
//
////	IPW_DEBUG_TRACE(">>\n");
//
//	/* read entire contents of eeprom into private buffer */
////	for (i = 0; i < 128; i++)
////		eeprom[i] = cpu_to_le16(eeprom_read_u16(priv, (u8) i));
//
//	/*
//	   If the data looks correct, then copy it to our private
//	   copy.  Otherwise let the firmware know to perform the operation
//	   on its own.
//	 */
////	if (priv->eeprom[EEPROM_VERSION] != 0) {
////		IPW_DEBUG_INFO("Writing EEPROM data into SRAM\n");
//
//		/* write the eeprom data to sram */
////		for (i = 0; i < IPW_EEPROM_IMAGE_SIZE; i++)
////			ipw_write8(priv, IPW_EEPROM_DATA + i, priv->eeprom[i]);
//
//		/* Do not load eeprom data on fatal error or suspend */
////		ipw_write32(priv, IPW_EEPROM_LOAD_DISABLE, 0);
////	} else {
////		IPW_DEBUG_INFO("Enabling FW initializationg of SRAM\n");
//
//		/* Load eeprom data on fatal error or suspend */
////		ipw_write32(priv, IPW_EEPROM_LOAD_DISABLE, 1);
////	}
//
////	IPW_DEBUG_TRACE("<<\n");
////}
///*
//static void ipw_zero_memory(struct ipw_priv *priv, u32 start, u32 count)
//{
//	count >>= 2;
//	if (!count)
//		return;
//	_ipw_write32(priv, IPW_AUTOINC_ADDR, start);
//	while (count--)
//		_ipw_write32(priv, IPW_AUTOINC_DATA, 0);
//}
//
//static inline void ipw_fw_dma_reset_command_blocks(struct ipw_priv *priv)
//{
//	ipw_zero_memory(priv, IPW_SHARED_SRAM_DMA_CONTROL,
//			CB_NUMBER_OF_ELEMENTS_SMALL *
//			sizeof(struct command_block));
//}
//*/
////static int ipw_fw_dma_enable(struct ipw_priv *priv)
////{				/* start dma engine but no transfers yet */
//
////	IPW_DEBUG_FW(">> : \n");
//
//	/* Start the dma */
////	ipw_fw_dma_reset_command_blocks(priv);
//
//	/* Write CB base address */
////	ipw_write_reg32(priv, IPW_DMA_I_CB_BASE, IPW_SHARED_SRAM_DMA_CONTROL);
//
////	IPW_DEBUG_FW("<< : \n");
////	return 0;
////}
//
////static void ipw_fw_dma_abort(struct ipw_priv *priv)
////{
////	u32 control = 0;
//
////	IPW_DEBUG_FW(">> :\n");
//
//	/* set the Stop and Abort bit */
////	control = DMA_CONTROL_SMALL_CB_CONST_VALUE | DMA_CB_STOP_AND_ABORT;
////	ipw_write_reg32(priv, IPW_DMA_I_DMA_CONTROL, control);
////	priv->sram_desc.last_cb_index = 0;
//
////	IPW_DEBUG_FW("<< \n");
////}
///*
//static int ipw_fw_dma_write_command_block(struct ipw_priv *priv, int index,
//					  struct command_block *cb)
//{
//	u32 address =
//	    IPW_SHARED_SRAM_DMA_CONTROL +
//	    (sizeof(struct command_block) * index);
//	IPW_DEBUG_FW(">> :\n");
//
//	ipw_write_indirect(priv, address, (u8 *) cb,
//			   (int)sizeof(struct command_block));
//
//	IPW_DEBUG_FW("<< :\n");
//	return 0;
//
//}
//
//static int ipw_fw_dma_kick(struct ipw_priv *priv)
//{
//	u32 control = 0;
//	u32 index = 0;
//
//	IPW_DEBUG_FW(">> :\n");
//
//	for (index = 0; index < priv->sram_desc.last_cb_index; index++)
//		ipw_fw_dma_write_command_block(priv, index,
//					       &priv->sram_desc.cb_list[index]);
//*/
//	/* Enable the DMA in the CSR register */
////	ipw_clear_bit(priv, IPW_RESET_REG,
////		      IPW_RESET_REG_MASTER_DISABLED |
////		      IPW_RESET_REG_STOP_MASTER);
//
//	/* Set the Start bit. */
////	control = DMA_CONTROL_SMALL_CB_CONST_VALUE | DMA_CB_START;
////	ipw_write_reg32(priv, IPW_DMA_I_DMA_CONTROL, control);
//
////	IPW_DEBUG_FW("<< :\n");
////	return 0;
////}
//
////static void ipw_fw_dma_dump_command_block(struct ipw_priv *priv)
////{
////	u32 address;
////	u32 register_value = 0;
////	u32 cb_fields_address = 0;
//
////	IPW_DEBUG_FW(">> :\n");
////	address = ipw_read_reg32(priv, IPW_DMA_I_CURRENT_CB);
////	IPW_DEBUG_FW_INFO("Current CB is 0x%x \n", address);
//
//	/* Read the DMA Controlor register */
////	register_value = ipw_read_reg32(priv, IPW_DMA_I_DMA_CONTROL);
////	IPW_DEBUG_FW_INFO("IPW_DMA_I_DMA_CONTROL is 0x%x \n", register_value);
//
//	/* Print the CB values */
///*	cb_fields_address = address;
//	register_value = ipw_read_reg32(priv, cb_fields_address);
//	IPW_DEBUG_FW_INFO("Current CB ControlField is 0x%x \n", register_value);
//
//	cb_fields_address += sizeof(u32);
//	register_value = ipw_read_reg32(priv, cb_fields_address);
//	IPW_DEBUG_FW_INFO("Current CB Source Field is 0x%x \n", register_value);
//
//	cb_fields_address += sizeof(u32);
//	register_value = ipw_read_reg32(priv, cb_fields_address);
//	IPW_DEBUG_FW_INFO("Current CB Destination Field is 0x%x \n",
//			  register_value);
//
//	cb_fields_address += sizeof(u32);
//	register_value = ipw_read_reg32(priv, cb_fields_address);
//	IPW_DEBUG_FW_INFO("Current CB Status Field is 0x%x \n", register_value);
//
//	IPW_DEBUG_FW(">> :\n");
//}
//
//static int ipw_fw_dma_command_block_index(struct ipw_priv *priv)
//{
//	u32 current_cb_address = 0;
//	u32 current_cb_index = 0;
//
//	IPW_DEBUG_FW("<< :\n");
//	current_cb_address = ipw_read_reg32(priv, IPW_DMA_I_CURRENT_CB);
//
//	current_cb_index = (current_cb_address - IPW_SHARED_SRAM_DMA_CONTROL) /
//	    sizeof(struct command_block);
//
//	IPW_DEBUG_FW_INFO("Current CB index 0x%x address = 0x%X \n",
//			  current_cb_index, current_cb_address);
//
//	IPW_DEBUG_FW(">> :\n");
//	return current_cb_index;
//
//}
//
//static int ipw_fw_dma_add_command_block(struct ipw_priv *priv,
//					u32 src_address,
//					u32 dest_address,
//					u32 length,
//					int interrupt_enabled, int is_last)
//{
//
//	u32 control = CB_VALID | CB_SRC_LE | CB_DEST_LE | CB_SRC_AUTOINC |
//	    CB_SRC_IO_GATED | CB_DEST_AUTOINC | CB_SRC_SIZE_LONG |
//	    CB_DEST_SIZE_LONG;
//	struct command_block *cb;
//	u32 last_cb_element = 0;
//
//	IPW_DEBUG_FW_INFO("src_address=0x%x dest_address=0x%x length=0x%x\n",
//			  src_address, dest_address, length);
//
//	if (priv->sram_desc.last_cb_index >= CB_NUMBER_OF_ELEMENTS_SMALL)
//		return -1;
//
//	last_cb_element = priv->sram_desc.last_cb_index;
//	cb = &priv->sram_desc.cb_list[last_cb_element];
//	priv->sram_desc.last_cb_index++;
//*/
//	/* Calculate the new CB control word */
////	if (interrupt_enabled)
////		control |= CB_INT_ENABLED;
//
////	if (is_last)
////		control |= CB_LAST_VALID;
//
////	control |= length;
//
//	/* Calculate the CB Element's checksum value */
////	cb->status = control ^ src_address ^ dest_address;
//
//	/* Copy the Source and Destination addresses */
////	cb->dest_addr = dest_address;
////	cb->source_addr = src_address;
//
//	/* Copy the Control Word last */
////	cb->control = control;
//
////	return 0;
////}
///*
//static int ipw_fw_dma_add_buffer(struct ipw_priv *priv, dma_addr_t *src_address,
//				 int nr, u32 dest_address, u32 len)
//{
//	int ret, i;
//	u32 size;
//
//	IPW_DEBUG_FW(">> \n");
//	IPW_DEBUG_FW_INFO("nr=%d dest_address=0x%x len=0x%x\n",
//			  nr, dest_address, len);
//
//	for (i = 0; i < nr; i++) {
//		size = min_t(u32, len - i * CB_MAX_LENGTH, CB_MAX_LENGTH);
//		ret = ipw_fw_dma_add_command_block(priv, src_address[i],
//						   dest_address +
//						   i * CB_MAX_LENGTH, size,
//						   0, 0);
//		if (ret) {
//			IPW_DEBUG_FW_INFO(": Failed\n");
//			return -1;
//		} else
//			IPW_DEBUG_FW_INFO(": Added new cb\n");
//	}
//
//	IPW_DEBUG_FW("<< \n");
//	return 0;
//}
//
//static int ipw_fw_dma_wait(struct ipw_priv *priv)
//{
//	u32 current_index = 0, previous_index;
//	u32 watchdog = 0;
//
//	IPW_DEBUG_FW(">> : \n");
//
//	current_index = ipw_fw_dma_command_block_index(priv);
//	IPW_DEBUG_FW_INFO("sram_desc.last_cb_index:0x%08X\n",
//			  (int)priv->sram_desc.last_cb_index);
//
//	while (current_index < priv->sram_desc.last_cb_index) {
//		udelay(50);
//		previous_index = current_index;
//		current_index = ipw_fw_dma_command_block_index(priv);
//
//		if (previous_index < current_index) {
//			watchdog = 0;
//			continue;
//		}
//		if (++watchdog > 400) {
//			IPW_DEBUG_FW_INFO("Timeout\n");
//			ipw_fw_dma_dump_command_block(priv);
//			ipw_fw_dma_abort(priv);
//			return -1;
//		}
//	}
//
//	ipw_fw_dma_abort(priv);
//*/
//	/*Disable the DMA in the CSR register */
////	ipw_set_bit(priv, IPW_RESET_REG,
////		    IPW_RESET_REG_MASTER_DISABLED | IPW_RESET_REG_STOP_MASTER);
//
////	IPW_DEBUG_FW("<< dmaWaitSync \n");
////	return 0;
////}
///*
//static void ipw_remove_current_network(struct ipw_priv *priv)
//{
//	struct list_head *element, *safe;
//	struct libipw_network *network = NULL;
//	unsigned long flags;
//
//	spin_lock_irqsave(&priv->ieee->lock, flags);
//	list_for_each_safe(element, safe, &priv->ieee->network_list) {
//		network = list_entry(element, struct libipw_network, list);
//		if (!memcmp(network->bssid, priv->bssid, ETH_ALEN)) {
//			list_del(element);
//			list_add_tail(&network->list,
//				      &priv->ieee->network_free_list);
//		}
//	}
//	spin_unlock_irqrestore(&priv->ieee->lock, flags);
//}
//*/
///**
// * Check that card is still alive.
// * Reads debug register from domain0.
// * If card is present, pre-defined value should
// * be found there.
// *
// * @param priv
// * @return 1 if card is present, 0 otherwise
// */
////static inline int ipw_alive(struct ipw_priv *priv)
////{
////	return ipw_read32(priv, 0x90) == 0xd55555d5;
////}
//
///* timeout in msec, attempted in 10-msec quanta */
///*static int ipw_poll_bit(struct ipw_priv *priv, u32 addr, u32 mask,
//			       int timeout)
//{
//	int i = 0;
//
//	do {
//		if ((ipw_read32(priv, addr) & mask) == mask)
//			return i;
//		mdelay(10);
//		i += 10;
//	} while (i < timeout);
//
//	return -ETIME;
//}
//*/
///* These functions load the firmware and micro code for the operation of
// * the ipw hardware.  It assumes the buffer has all the bits for the
// * image and the caller is handling the memory allocation and clean up.
// */
//
////static int ipw_stop_master(struct ipw_priv *priv)
////{
////	int rc;
//
////	IPW_DEBUG_TRACE(">> \n");
//	/* stop master. typical delay - 0 */
////	ipw_set_bit(priv, IPW_RESET_REG, IPW_RESET_REG_STOP_MASTER);
//
//	/* timeout is in msec, polled in 10-msec quanta */
////	rc = ipw_poll_bit(priv, IPW_RESET_REG,
////			  IPW_RESET_REG_MASTER_DISABLED, 100);
////	if (rc < 0) {
////		IPW_ERROR("wait for stop master failed after 100ms\n");
////		return -1;
////	}
//
////	IPW_DEBUG_INFO("stop master %dms\n", rc);
//
////	return rc;
////}
//
////static void ipw_arc_release(struct ipw_priv *priv)
////{
////	IPW_DEBUG_TRACE(">> \n");
////	mdelay(5);
//
////	ipw_clear_bit(priv, IPW_RESET_REG, CBD_RESET_REG_PRINCETON_RESET);
//
//	/* no one knows timing, for safety add some delay */
////	mdelay(5);
////}
//
////struct fw_chunk {
////	__le32 address;
////	__le32 length;
////};
//
////static int ipw_load_ucode(struct ipw_priv *priv, u8 * data, size_t len)
////{
////	int rc = 0, i, addr;
////	u8 cr = 0;
////	__le16 *image;
//
////	image = (__le16 *) data;
//
////	IPW_DEBUG_TRACE(">> \n");
//
////	rc = ipw_stop_master(priv);
//
////	if (rc < 0)
////		return rc;
//
////	for (addr = IPW_SHARED_LOWER_BOUND;
////	     addr < IPW_REGISTER_DOMAIN1_END; addr += 4) {
////		ipw_write32(priv, addr, 0);
////	}
//
//	/* no ucode (yet) */
////	memset(&priv->dino_alive, 0, sizeof(priv->dino_alive));
//	/* destroy DMA queues */
//	/* reset sequence */
//
////	ipw_write_reg32(priv, IPW_MEM_HALT_AND_RESET, IPW_BIT_HALT_RESET_ON);
////	ipw_arc_release(priv);
////	ipw_write_reg32(priv, IPW_MEM_HALT_AND_RESET, IPW_BIT_HALT_RESET_OFF);
////	mdelay(1);
//
//	/* reset PHY */
////	ipw_write_reg32(priv, IPW_INTERNAL_CMD_EVENT, IPW_BASEBAND_POWER_DOWN);
////	mdelay(1);
//
////	ipw_write_reg32(priv, IPW_INTERNAL_CMD_EVENT, 0);
////	mdelay(1);
//
//	/* enable ucode store */
////	ipw_write_reg8(priv, IPW_BASEBAND_CONTROL_STATUS, 0x0);
////	ipw_write_reg8(priv, IPW_BASEBAND_CONTROL_STATUS, DINO_ENABLE_CS);
////	mdelay(1);
//
//	/* write ucode */
//	/**
//	 * @bug
//	 * Do NOT set indirect address register once and then
//	 * store data to indirect data register in the loop.
//	 * It seems very reasonable, but in this case DINO do not
//	 * accept ucode. It is essential to set address each time.
//	 */
//	/* load new ipw uCode */
////	for (i = 0; i < len / 2; i++)
////		ipw_write_reg16(priv, IPW_BASEBAND_CONTROL_STORE,
////				le16_to_cpu(image[i]));
//
//	/* enable DINO */
////	ipw_write_reg8(priv, IPW_BASEBAND_CONTROL_STATUS, 0);
////	ipw_write_reg8(priv, IPW_BASEBAND_CONTROL_STATUS, DINO_ENABLE_SYSTEM);
//
//	/* this is where the igx / win driver deveates from the VAP driver. */
//
//	/* wait for alive response */
////	for (i = 0; i < 100; i++) {
//		/* poll for incoming data */
////		cr = ipw_read_reg8(priv, IPW_BASEBAND_CONTROL_STATUS);
////		if (cr & DINO_RXFIFO_DATA)
////			break;
////		mdelay(1);
////	}
//
////	if (cr & DINO_RXFIFO_DATA) {
//		/* alive_command_responce size is NOT multiple of 4 */
///*		__le32 response_buffer[(sizeof(priv->dino_alive) + 3) / 4];
//
//		for (i = 0; i < ARRAY_SIZE(response_buffer); i++)
//			response_buffer[i] =
//			    cpu_to_le32(ipw_read_reg32(priv,
//						       IPW_BASEBAND_RX_FIFO_READ));
//		memcpy(&priv->dino_alive, response_buffer,
//		       sizeof(priv->dino_alive));
//		if (priv->dino_alive.alive_command == 1
//		    && priv->dino_alive.ucode_valid == 1) {
//			rc = 0;
//			IPW_DEBUG_INFO
//			    ("Microcode OK, rev. %d (0x%x) dev. %d (0x%x) "
//			     "of %02d/%02d/%02d %02d:%02d\n",
//			     priv->dino_alive.software_revision,
//			     priv->dino_alive.software_revision,
//			     priv->dino_alive.device_identifier,
//			     priv->dino_alive.device_identifier,
//			     priv->dino_alive.time_stamp[0],
//			     priv->dino_alive.time_stamp[1],
//			     priv->dino_alive.time_stamp[2],
//			     priv->dino_alive.time_stamp[3],
//			     priv->dino_alive.time_stamp[4]);
//		} else {
//			IPW_DEBUG_INFO("Microcode is not alive\n");
//			rc = -EINVAL;
//		}
//	} else {
//		IPW_DEBUG_INFO("No alive response from DINO\n");
//		rc = -ETIME;
//	}
//*/
//	/* disable DINO, otherwise for some reason
//	   firmware have problem getting alive resp. */
////	ipw_write_reg8(priv, IPW_BASEBAND_CONTROL_STATUS, 0);
//
////	return rc;
////}
///*
//static int ipw_load_firmware(struct ipw_priv *priv, u8 * data, size_t len)
//{
//	int ret = -1;
//	int offset = 0;
//	struct fw_chunk *chunk;
//	int total_nr = 0;
//	int i;
//	struct pci_pool *pool;
//	u32 *virts[CB_NUMBER_OF_ELEMENTS_SMALL];
//	dma_addr_t phys[CB_NUMBER_OF_ELEMENTS_SMALL];
//
//	IPW_DEBUG_TRACE("<< : \n");
//
//	pool = pci_pool_create("ipw2200", priv->pci_dev, CB_MAX_LENGTH, 0, 0);
//	if (!pool) {
//		IPW_ERROR("pci_pool_create failed\n");
//		return -ENOMEM;
//	}
//*/
//	/* Start the Dma */
////	ret = ipw_fw_dma_enable(priv);
//
//	/* the DMA is already ready this would be a bug. */
////	BUG_ON(priv->sram_desc.last_cb_index > 0);
//
//	do {
//		u32 chunk_len;
//		u8 *start;
//		int size;
//		int nr = 0;
//
//		chunk = (struct fw_chunk *)(data + offset);
//		offset += sizeof(struct fw_chunk);
//		chunk_len = le32_to_cpu(chunk->length);
//		start = data + offset;
//
//		nr = (chunk_len + CB_MAX_LENGTH - 1) / CB_MAX_LENGTH;
//		for (i = 0; i < nr; i++) {
//			virts[total_nr] = pci_pool_alloc(pool, GFP_KERNEL,
//							 &phys[total_nr]);
//			if (!virts[total_nr]) {
//				ret = -ENOMEM;
//				goto out;
//			}
//			size = min_t(u32, chunk_len - i * CB_MAX_LENGTH,
//				     CB_MAX_LENGTH);
//			memcpy(virts[total_nr], start, size);
//			start += size;
//			total_nr++;
//			/* We don't support fw chunk larger than 64*8K */
//			BUG_ON(total_nr > CB_NUMBER_OF_ELEMENTS_SMALL);
//		}
//
//		/* build DMA packet and queue up for sending */
//		/* dma to chunk->address, the chunk->length bytes from data +
//		 * offeset*/
//		/* Dma loading */
//		ret = ipw_fw_dma_add_buffer(priv, &phys[total_nr - nr],
//					    nr, le32_to_cpu(chunk->address),
//					    chunk_len);
//		if (ret) {
//			IPW_DEBUG_INFO("dmaAddBuffer Failed\n");
//			goto out;
//		}
//
//		offset += chunk_len;
//	} while (offset < len);
//
//	/* Run the DMA and wait for the answer */
//	ret = ipw_fw_dma_kick(priv);
//	if (ret) {
//		IPW_ERROR("dmaKick Failed\n");
//		goto out;
//	}
//
//	ret = ipw_fw_dma_wait(priv);
//	if (ret) {
//		IPW_ERROR("dmaWaitSync Failed\n");
//		goto out;
//	}
// out:
//	for (i = 0; i < total_nr; i++)
//		pci_pool_free(pool, virts[i], phys[i]);
//
//	pci_pool_destroy(pool);
//
//	return ret;
//}
//
///* stop nic */
//static int ipw_stop_nic(struct ipw_priv *priv)
//{
//	int rc = 0;
//
//	/* stop */
//	ipw_write32(priv, IPW_RESET_REG, IPW_RESET_REG_STOP_MASTER);
//
//	rc = ipw_poll_bit(priv, IPW_RESET_REG,
//			  IPW_RESET_REG_MASTER_DISABLED, 500);
//	if (rc < 0) {
//		IPW_ERROR("wait for reg master disabled failed after 500ms\n");
//		return rc;
//	}
//
//	ipw_set_bit(priv, IPW_RESET_REG, CBD_RESET_REG_PRINCETON_RESET);
//
//	return rc;
//}
//
//static void ipw_start_nic(struct ipw_priv *priv)
//{
//	IPW_DEBUG_TRACE(">>\n");
//
//	/* prvHwStartNic  release ARC */
//	ipw_clear_bit(priv, IPW_RESET_REG,
//		      IPW_RESET_REG_MASTER_DISABLED |
//		      IPW_RESET_REG_STOP_MASTER |
//		      CBD_RESET_REG_PRINCETON_RESET);
//
//	/* enable power management */
//	ipw_set_bit(priv, IPW_GP_CNTRL_RW,
//		    IPW_GP_CNTRL_BIT_HOST_ALLOWS_STANDBY);
//
//	IPW_DEBUG_TRACE("<<\n");
//}
//
//static int ipw_init_nic(struct ipw_priv *priv)
//{
//	int rc;
//
//	IPW_DEBUG_TRACE(">>\n");
//	/* reset */
//	/*prvHwInitNic */
//	/* set "initialization complete" bit to move adapter to D0 state */
//	ipw_set_bit(priv, IPW_GP_CNTRL_RW, IPW_GP_CNTRL_BIT_INIT_DONE);
//
//	/* low-level PLL activation */
//	ipw_write32(priv, IPW_READ_INT_REGISTER,
//		    IPW_BIT_INT_HOST_SRAM_READ_INT_REGISTER);
//
//	/* wait for clock stabilization */
//	rc = ipw_poll_bit(priv, IPW_GP_CNTRL_RW,
//			  IPW_GP_CNTRL_BIT_CLOCK_READY, 250);
//	if (rc < 0)
//		IPW_DEBUG_INFO("FAILED wait for clock stablization\n");
//
//	/* assert SW reset */
//	ipw_set_bit(priv, IPW_RESET_REG, IPW_RESET_REG_SW_RESET);
//
//	udelay(10);
//
//	/* set "initialization complete" bit to move adapter to D0 state */
//	ipw_set_bit(priv, IPW_GP_CNTRL_RW, IPW_GP_CNTRL_BIT_INIT_DONE);
//
//	IPW_DEBUG_TRACE(">>\n");
//	return 0;
//}
//
///* Call this function from process context, it will sleep in request_firmware.
// * Probe is an ok place to call this from.
// */
//static int ipw_reset_nic(struct ipw_priv *priv)
//{
//	int rc = 0;
//	unsigned long flags;
//
//	IPW_DEBUG_TRACE(">>\n");
//
//	rc = ipw_init_nic(priv);
//
//	spin_lock_irqsave(&priv->lock, flags);
//	/* Clear the 'host command active' bit... */
//	priv->status &= ~STATUS_HCMD_ACTIVE;
//	wake_up_interruptible(&priv->wait_command_queue);
//	priv->status &= ~(STATUS_SCANNING | STATUS_SCAN_ABORTING);
//	wake_up_interruptible(&priv->wait_state);
//	spin_unlock_irqrestore(&priv->lock, flags);
//
//	IPW_DEBUG_TRACE("<<\n");
//	return rc;
//}
//
//
//struct ipw_fw {
//	__le32 ver;
//	__le32 boot_size;
//	__le32 ucode_size;
//	__le32 fw_size;
//	u8 data[0];
//};
//
//static int ipw_get_fw(struct ipw_priv *priv,
//		      const struct firmware **raw, const char *name)
//{
//	struct ipw_fw *fw;
//	int rc;
//
//	/* ask firmware_class module to get the boot firmware off disk */
//	rc = request_firmware(raw, name, &priv->pci_dev->dev);
//	if (rc < 0) {
//		IPW_ERROR("%s request_firmware failed: Reason %d\n", name, rc);
//		return rc;
//	}
//
//	if ((*raw)->size < sizeof(*fw)) {
//		IPW_ERROR("%s is too small (%zd)\n", name, (*raw)->size);
//		return -EINVAL;
//	}
//
//	fw = (void *)(*raw)->data;
//
//	if ((*raw)->size < sizeof(*fw) + le32_to_cpu(fw->boot_size) +
//	    le32_to_cpu(fw->ucode_size) + le32_to_cpu(fw->fw_size)) {
//		IPW_ERROR("%s is too small or corrupt (%zd)\n",
//			  name, (*raw)->size);
//		return -EINVAL;
//	}
//
//	IPW_DEBUG_INFO("Read firmware '%s' image v%d.%d (%zd bytes)\n",
//		       name,
//		       le32_to_cpu(fw->ver) >> 16,
//		       le32_to_cpu(fw->ver) & 0xff,
//		       (*raw)->size - sizeof(*fw));
//	return 0;
//}
//
//#define IPW_RX_BUF_SIZE (3000)
//
//static void ipw_rx_queue_reset(struct ipw_priv *priv,
//				      struct ipw_rx_queue *rxq)
//{
//	unsigned long flags;
//	int i;
//
//	spin_lock_irqsave(&rxq->lock, flags);
//
//	INIT_LIST_HEAD(&rxq->rx_free);
//	INIT_LIST_HEAD(&rxq->rx_used);
//
//	/* Fill the rx_used queue with _all_ of the Rx buffers */
//	for (i = 0; i < RX_FREE_BUFFERS + RX_QUEUE_SIZE; i++) {
//		/* In the reset function, these buffers may have been allocated
//		 * to an SKB, so we need to unmap and free potential storage */
//		if (rxq->pool[i].skb != NULL) {
//			pci_unmap_single(priv->pci_dev, rxq->pool[i].dma_addr,
//					 IPW_RX_BUF_SIZE, PCI_DMA_FROMDEVICE);
//			dev_kfree_skb(rxq->pool[i].skb);
//			rxq->pool[i].skb = NULL;
//		}
//		list_add_tail(&rxq->pool[i].list, &rxq->rx_used);
//	}
//
//	/* Set us so that we have processed and used all buffers, but have
//	 * not restocked the Rx queue with fresh buffers */
//	rxq->read = rxq->write = 0;
//	rxq->free_count = 0;
//	spin_unlock_irqrestore(&rxq->lock, flags);
//}
//
//#ifdef CONFIG_PM
//static int fw_loaded = 0;
//static const struct firmware *raw = NULL;
//
//static void free_firmware(void)
//{
//	if (fw_loaded) {
//		release_firmware(raw);
//		raw = NULL;
//		fw_loaded = 0;
//	}
//}
//#else
//#define free_firmware() do {} while (0)
//#endif
//
//static int ipw_load(struct ipw_priv *priv)
//{
//#ifndef CONFIG_PM
//	const struct firmware *raw = NULL;
//#endif
//	struct ipw_fw *fw;
//	u8 *boot_img, *ucode_img, *fw_img;
//	u8 *name = NULL;
//	int rc = 0, retries = 3;
//
//	switch (priv->ieee->iw_mode) {
//	case IW_MODE_ADHOC:
//		name = "ipw2200-ibss.fw";
//		break;
//#ifdef CONFIG_IPW2200_MONITOR
//	case IW_MODE_MONITOR:
//		name = "ipw2200-sniffer.fw";
//		break;
//#endif
//	case IW_MODE_INFRA:
//		name = "ipw2200-bss.fw";
//		break;
//	}
//
//	if (!name) {
//		rc = -EINVAL;
//		goto error;
//	}
//
//#ifdef CONFIG_PM
//	if (!fw_loaded) {
//#endif
//		rc = ipw_get_fw(priv, &raw, name);
//		if (rc < 0)
//			goto error;
//#ifdef CONFIG_PM
//	}
//#endif
//
//	fw = (void *)raw->data;
//	boot_img = &fw->data[0];
//	ucode_img = &fw->data[le32_to_cpu(fw->boot_size)];
//	fw_img = &fw->data[le32_to_cpu(fw->boot_size) +
//			   le32_to_cpu(fw->ucode_size)];
//
//	if (rc < 0)
//		goto error;
//
//	if (!priv->rxq)
//		priv->rxq = ipw_rx_queue_alloc(priv);
//	else
//		ipw_rx_queue_reset(priv, priv->rxq);
//	if (!priv->rxq) {
//		IPW_ERROR("Unable to initialize Rx queue\n");
//		goto error;
//	}
//
//      retry:
//	/* Ensure interrupts are disabled */
//	ipw_write32(priv, IPW_INTA_MASK_R, ~IPW_INTA_MASK_ALL);
//	priv->status &= ~STATUS_INT_ENABLED;
//
//	/* ack pending interrupts */
//	ipw_write32(priv, IPW_INTA_RW, IPW_INTA_MASK_ALL);
//
//	ipw_stop_nic(priv);
//
//	rc = ipw_reset_nic(priv);
//	if (rc < 0) {
//		IPW_ERROR("Unable to reset NIC\n");
//		goto error;
//	}
//
//	ipw_zero_memory(priv, IPW_NIC_SRAM_LOWER_BOUND,
//			IPW_NIC_SRAM_UPPER_BOUND - IPW_NIC_SRAM_LOWER_BOUND);
//
//	/* DMA the initial boot firmware into the device */
//	rc = ipw_load_firmware(priv, boot_img, le32_to_cpu(fw->boot_size));
//	if (rc < 0) {
//		IPW_ERROR("Unable to load boot firmware: %d\n", rc);
//		goto error;
//	}
//
//	/* kick start the device */
//	ipw_start_nic(priv);
//
//	/* wait for the device to finish its initial startup sequence */
//	rc = ipw_poll_bit(priv, IPW_INTA_RW,
//			  IPW_INTA_BIT_FW_INITIALIZATION_DONE, 500);
//	if (rc < 0) {
//		IPW_ERROR("device failed to boot initial fw image\n");
//		goto error;
//	}
//	IPW_DEBUG_INFO("initial device response after %dms\n", rc);
//
//	/* ack fw init done interrupt */
//	ipw_write32(priv, IPW_INTA_RW, IPW_INTA_BIT_FW_INITIALIZATION_DONE);
//
//	/* DMA the ucode into the device */
//	rc = ipw_load_ucode(priv, ucode_img, le32_to_cpu(fw->ucode_size));
//	if (rc < 0) {
//		IPW_ERROR("Unable to load ucode: %d\n", rc);
//		goto error;
//	}
//
//	/* stop nic */
//	ipw_stop_nic(priv);
//
//	/* DMA bss firmware into the device */
//	rc = ipw_load_firmware(priv, fw_img, le32_to_cpu(fw->fw_size));
//	if (rc < 0) {
//		IPW_ERROR("Unable to load firmware: %d\n", rc);
//		goto error;
//	}
//#ifdef CONFIG_PM
//	fw_loaded = 1;
//#endif
//
//	ipw_write32(priv, IPW_EEPROM_LOAD_DISABLE, 0);
//
//	rc = ipw_queue_reset(priv);
//	if (rc < 0) {
//		IPW_ERROR("Unable to initialize queues\n");
//		goto error;
//	}
//
//	/* Ensure interrupts are disabled */
//	ipw_write32(priv, IPW_INTA_MASK_R, ~IPW_INTA_MASK_ALL);
//	/* ack pending interrupts */
//	ipw_write32(priv, IPW_INTA_RW, IPW_INTA_MASK_ALL);
//
//	/* kick start the device */
//	ipw_start_nic(priv);
//
//	if (ipw_read32(priv, IPW_INTA_RW) & IPW_INTA_BIT_PARITY_ERROR) {
//		if (retries > 0) {
//			IPW_WARNING("Parity error.  Retrying init.\n");
//			retries--;
//			goto retry;
//		}
//
//		IPW_ERROR("TODO: Handle parity error -- schedule restart?\n");
//		rc = -EIO;
//		goto error;
//	}
//
//	/* wait for the device */
//	rc = ipw_poll_bit(priv, IPW_INTA_RW,
//			  IPW_INTA_BIT_FW_INITIALIZATION_DONE, 500);
//	if (rc < 0) {
//		IPW_ERROR("device failed to start within 500ms\n");
//		goto error;
//	}
//	IPW_DEBUG_INFO("device response after %dms\n", rc);
//
//	/* ack fw init done interrupt */
//	ipw_write32(priv, IPW_INTA_RW, IPW_INTA_BIT_FW_INITIALIZATION_DONE);
//
//	/* read eeprom data and initialize the eeprom region of sram */
//	priv->eeprom_delay = 1;
//	ipw_eeprom_init_sram(priv);
//
//	/* enable interrupts */
//	ipw_enable_interrupts(priv);
//
//	/* Ensure our queue has valid packets */
//	ipw_rx_queue_replenish(priv);
//
//	ipw_write32(priv, IPW_RX_READ_INDEX, priv->rxq->read);
//
//	/* ack pending interrupts */
//	ipw_write32(priv, IPW_INTA_RW, IPW_INTA_MASK_ALL);
//
//#ifndef CONFIG_PM
//	release_firmware(raw);
//#endif
//	return 0;
//
//      error:
//	if (priv->rxq) {
//		ipw_rx_queue_free(priv, priv->rxq);
//		priv->rxq = NULL;
//	}
//	ipw_tx_queue_free(priv);
//	if (raw)
//		release_firmware(raw);
//#ifdef CONFIG_PM
//	fw_loaded = 0;
//	raw = NULL;
//#endif
//
//	return rc;
//}
//
///**
// * DMA services
// *
// * Theory of operation
// *
// * A queue is a circular buffers with 'Read' and 'Write' pointers.
// * 2 empty entries always kept in the buffer to protect from overflow.
// *
// * For Tx queue, there are low mark and high mark limits. If, after queuing
// * the packet for Tx, free space become < low mark, Tx queue stopped. When
// * reclaiming packets (on 'tx done IRQ), if free space become > high mark,
// * Tx queue resumed.
// *
// * The IPW operates with six queues, one receive queue in the device's
// * sram, one transmit queue for sending commands to the device firmware,
// * and four transmit queues for data.
// *
// * The four transmit queues allow for performing quality of service (qos)
// * transmissions as per the 802.11 protocol.  Currently Linux does not
// * provide a mechanism to the user for utilizing prioritized queues, so
// * we only utilize the first data transmit queue (queue1).
// */
//
///**
// * Driver allocates buffers of this size for Rx
// */
//
///**
// * ipw_rx_queue_space - Return number of free slots available in queue.
// */
//static int ipw_rx_queue_space(const struct ipw_rx_queue *q)
//{
//	int s = q->read - q->write;
//	if (s <= 0)
//		s += RX_QUEUE_SIZE;
//	/* keep some buffer to not confuse full and empty queue */
//	s -= 2;
//	if (s < 0)
//		s = 0;
//	return s;
//}
//
//static inline int ipw_tx_queue_space(const struct clx2_queue *q)
//{
//	int s = q->last_used - q->first_empty;
//	if (s <= 0)
//		s += q->n_bd;
//	s -= 2;			/* keep some reserve to not confuse empty and full situations */
//	if (s < 0)
//		s = 0;
//	return s;
//}
//
//static inline int ipw_queue_inc_wrap(int index, int n_bd)
//{
//	return (++index == n_bd) ? 0 : index;
//}
//
///**
// * Initialize common DMA queue structure
// *
// * @param q                queue to init
// * @param count            Number of BD's to allocate. Should be power of 2
// * @param read_register    Address for 'read' register
// *                         (not offset within BAR, full address)
// * @param write_register   Address for 'write' register
// *                         (not offset within BAR, full address)
// * @param base_register    Address for 'base' register
// *                         (not offset within BAR, full address)
// * @param size             Address for 'size' register
// *                         (not offset within BAR, full address)
// */
//static void ipw_queue_init(struct ipw_priv *priv, struct clx2_queue *q,
//			   int count, u32 read, u32 write, u32 base, u32 size)
//{
//	q->n_bd = count;
//
//	q->low_mark = q->n_bd / 4;
//	if (q->low_mark < 4)
//		q->low_mark = 4;
//
//	q->high_mark = q->n_bd / 8;
//	if (q->high_mark < 2)
//		q->high_mark = 2;
//
//	q->first_empty = q->last_used = 0;
//	q->reg_r = read;
//	q->reg_w = write;
//
//	ipw_write32(priv, base, q->dma_addr);
//	ipw_write32(priv, size, count);
//	ipw_write32(priv, read, 0);
//	ipw_write32(priv, write, 0);
//
//	_ipw_read32(priv, 0x90);
//}
//
//static int ipw_queue_tx_init(struct ipw_priv *priv,
//			     struct clx2_tx_queue *q,
//			     int count, u32 read, u32 write, u32 base, u32 size)
//{
//	struct pci_dev *dev = priv->pci_dev;
//
//	q->txb = kmalloc(sizeof(q->txb[0]) * count, GFP_KERNEL);
//	if (!q->txb) {
//		IPW_ERROR("vmalloc for auxilary BD structures failed\n");
//		return -ENOMEM;
//	}
//
//	q->bd =
//	    pci_alloc_consistent(dev, sizeof(q->bd[0]) * count, &q->q.dma_addr);
//	if (!q->bd) {
//		IPW_ERROR("pci_alloc_consistent(%zd) failed\n",
//			  sizeof(q->bd[0]) * count);
//		kfree(q->txb);
//		q->txb = NULL;
//		return -ENOMEM;
//	}
//
//	ipw_queue_init(priv, &q->q, count, read, write, base, size);
//	return 0;
//}
//
///**
// * Free one TFD, those at index [txq->q.last_used].
// * Do NOT advance any indexes
// *
// * @param dev
// * @param txq
// */
//static void ipw_queue_tx_free_tfd(struct ipw_priv *priv,
//				  struct clx2_tx_queue *txq)
//{
//	struct tfd_frame *bd = &txq->bd[txq->q.last_used];
//	struct pci_dev *dev = priv->pci_dev;
//	int i;
//
//	/* classify bd */
//	if (bd->control_flags.message_type == TX_HOST_COMMAND_TYPE)
//		/* nothing to cleanup after for host commands */
//		return;
//
//	/* sanity check */
//	if (le32_to_cpu(bd->u.data.num_chunks) > NUM_TFD_CHUNKS) {
//		IPW_ERROR("Too many chunks: %i\n",
//			  le32_to_cpu(bd->u.data.num_chunks));
//		/** @todo issue fatal error, it is quite serious situation */
//		return;
//	}
//
//	/* unmap chunks if any */
//	for (i = 0; i < le32_to_cpu(bd->u.data.num_chunks); i++) {
//		pci_unmap_single(dev, le32_to_cpu(bd->u.data.chunk_ptr[i]),
//				 le16_to_cpu(bd->u.data.chunk_len[i]),
//				 PCI_DMA_TODEVICE);
//		if (txq->txb[txq->q.last_used]) {
//			libipw_txb_free(txq->txb[txq->q.last_used]);
//			txq->txb[txq->q.last_used] = NULL;
//		}
//	}
//}
//
///**
// * Deallocate DMA queue.
// *
// * Empty queue by removing and destroying all BD's.
// * Free all buffers.
// *
// * @param dev
// * @param q
// */
//static void ipw_queue_tx_free(struct ipw_priv *priv, struct clx2_tx_queue *txq)
//{
//	struct clx2_queue *q = &txq->q;
//	struct pci_dev *dev = priv->pci_dev;
//
//	if (q->n_bd == 0)
//		return;
//
//	/* first, empty all BD's */
//	for (; q->first_empty != q->last_used;
//	     q->last_used = ipw_queue_inc_wrap(q->last_used, q->n_bd)) {
//		ipw_queue_tx_free_tfd(priv, txq);
//	}
//
//	/* free buffers belonging to queue itself */
//	pci_free_consistent(dev, sizeof(txq->bd[0]) * q->n_bd, txq->bd,
//			    q->dma_addr);
//	kfree(txq->txb);
//
//	/* 0 fill whole structure */
//	memset(txq, 0, sizeof(*txq));
//}
//
///**
// * Destroy all DMA queues and structures
// *
// * @param priv
// */
//static void ipw_tx_queue_free(struct ipw_priv *priv)
//{
//	/* Tx CMD queue */
//	ipw_queue_tx_free(priv, &priv->txq_cmd);
//
//	/* Tx queues */
//	ipw_queue_tx_free(priv, &priv->txq[0]);
//	ipw_queue_tx_free(priv, &priv->txq[1]);
//	ipw_queue_tx_free(priv, &priv->txq[2]);
//	ipw_queue_tx_free(priv, &priv->txq[3]);
//}
//
//static void ipw_create_bssid(struct ipw_priv *priv, u8 * bssid)
//{
//	/* First 3 bytes are manufacturer */
//	bssid[0] = priv->mac_addr[0];
//	bssid[1] = priv->mac_addr[1];
//	bssid[2] = priv->mac_addr[2];
//
//	/* Last bytes are random */
//	get_random_bytes(&bssid[3], ETH_ALEN - 3);
//
//	bssid[0] &= 0xfe;	/* clear multicast bit */
//	bssid[0] |= 0x02;	/* set local assignment bit (IEEE802) */
//}
//
//static u8 ipw_add_station(struct ipw_priv *priv, u8 * bssid)
//{
//	struct ipw_station_entry entry;
//	int i;
//
//	for (i = 0; i < priv->num_stations; i++) {
//		if (!memcmp(priv->stations[i], bssid, ETH_ALEN)) {
//			/* Another node is active in network */
//			priv->missed_adhoc_beacons = 0;
//			if (!(priv->config & CFG_STATIC_CHANNEL))
//				/* when other nodes drop out, we drop out */
//				priv->config &= ~CFG_ADHOC_PERSIST;
//
//			return i;
//		}
//	}
//
//	if (i == MAX_STATIONS)
//		return IPW_INVALID_STATION;
//
//	IPW_DEBUG_SCAN("Adding AdHoc station: %pM\n", bssid);
//
//	entry.reserved = 0;
//	entry.support_mode = 0;
//	memcpy(entry.mac_addr, bssid, ETH_ALEN);
//	memcpy(priv->stations[i], bssid, ETH_ALEN);
//	ipw_write_direct(priv, IPW_STATION_TABLE_LOWER + i * sizeof(entry),
//			 &entry, sizeof(entry));
//	priv->num_stations++;
//
//	return i;
//}
//
//static u8 ipw_find_station(struct ipw_priv *priv, u8 * bssid)
//{
//	int i;
//
//	for (i = 0; i < priv->num_stations; i++)
//		if (!memcmp(priv->stations[i], bssid, ETH_ALEN))
//			return i;
//
//	return IPW_INVALID_STATION;
//}
//
//static void ipw_send_disassociate(struct ipw_priv *priv, int quiet)
//{
//	int err;
//
//	if (priv->status & STATUS_ASSOCIATING) {
//		IPW_DEBUG_ASSOC("Disassociating while associating.\n");
//		queue_work(priv->workqueue, &priv->disassociate);
//		return;
//	}
//
//	if (!(priv->status & STATUS_ASSOCIATED)) {
//		IPW_DEBUG_ASSOC("Disassociating while not associated.\n");
//		return;
//	}
//
//	IPW_DEBUG_ASSOC("Disassocation attempt from %pM "
//			"on channel %d.\n",
//			priv->assoc_request.bssid,
//			priv->assoc_request.channel);
//
//	priv->status &= ~(STATUS_ASSOCIATING | STATUS_ASSOCIATED);
//	priv->status |= STATUS_DISASSOCIATING;
//
//	if (quiet)
//		priv->assoc_request.assoc_type = HC_DISASSOC_QUIET;
//	else
//		priv->assoc_request.assoc_type = HC_DISASSOCIATE;
//
//	err = ipw_send_associate(priv, &priv->assoc_request);
//	if (err) {
//		IPW_DEBUG_HC("Attempt to send [dis]associate command "
//			     "failed.\n");
//		return;
//	}
//
//}
//
//static int ipw_disassociate(void *data)
//{
//	struct ipw_priv *priv = data;
//	if (!(priv->status & (STATUS_ASSOCIATED | STATUS_ASSOCIATING)))
//		return 0;
//	ipw_send_disassociate(data, 0);
//	netif_carrier_off(priv->net_dev);
//	return 1;
//}
//
//static void ipw_bg_disassociate(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, disassociate);
//	mutex_lock(&priv->mutex);
//	ipw_disassociate(priv);
//	mutex_unlock(&priv->mutex);
//}
//
//static void ipw_system_config(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, system_config);
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
//	ipw_send_system_config(priv);
//}
//
//struct ipw_status_code {
//	u16 status;
//	const char *reason;
//};
//
//static const struct ipw_status_code ipw_status_codes[] = {
//	{0x00, "Successful"},
//	{0x01, "Unspecified failure"},
//	{0x0A, "Cannot support all requested capabilities in the "
//	 "Capability information field"},
//	{0x0B, "Reassociation denied due to inability to confirm that "
//	 "association exists"},
//	{0x0C, "Association denied due to reason outside the scope of this "
//	 "standard"},
//	{0x0D,
//	 "Responding station does not support the specified authentication "
//	 "algorithm"},
//	{0x0E,
//	 "Received an Authentication frame with authentication sequence "
//	 "transaction sequence number out of expected sequence"},
//	{0x0F, "Authentication rejected because of challenge failure"},
//	{0x10, "Authentication rejected due to timeout waiting for next "
//	 "frame in sequence"},
//	{0x11, "Association denied because AP is unable to handle additional "
//	 "associated stations"},
//	{0x12,
//	 "Association denied due to requesting station not supporting all "
//	 "of the datarates in the BSSBasicServiceSet Parameter"},
//	{0x13,
//	 "Association denied due to requesting station not supporting "
//	 "short preamble operation"},
//	{0x14,
//	 "Association denied due to requesting station not supporting "
//	 "PBCC encoding"},
//	{0x15,
//	 "Association denied due to requesting station not supporting "
//	 "channel agility"},
//	{0x19,
//	 "Association denied due to requesting station not supporting "
//	 "short slot operation"},
//	{0x1A,
//	 "Association denied due to requesting station not supporting "
//	 "DSSS-OFDM operation"},
//	{0x28, "Invalid Information Element"},
//	{0x29, "Group Cipher is not valid"},
//	{0x2A, "Pairwise Cipher is not valid"},
//	{0x2B, "AKMP is not valid"},
//	{0x2C, "Unsupported RSN IE version"},
//	{0x2D, "Invalid RSN IE Capabilities"},
//	{0x2E, "Cipher suite is rejected per security policy"},
//};
//
//static const char *ipw_get_status_code(u16 status)
//{
//	int i;
//	for (i = 0; i < ARRAY_SIZE(ipw_status_codes); i++)
//		if (ipw_status_codes[i].status == (status & 0xff))
//			return ipw_status_codes[i].reason;
//	return "Unknown status value.";
//}
//
//static void inline average_init(struct average *avg)
//{
//	memset(avg, 0, sizeof(*avg));
//}
//
//#define DEPTH_RSSI 8
//#define DEPTH_NOISE 16
//static s16 exponential_average(s16 prev_avg, s16 val, u8 depth)
//{
//	return ((depth-1)*prev_avg +  val)/depth;
//}
//
//static void average_add(struct average *avg, s16 val)
//{
//	avg->sum -= avg->entries[avg->pos];
//	avg->sum += val;
//	avg->entries[avg->pos++] = val;
//	if (unlikely(avg->pos == AVG_ENTRIES)) {
//		avg->init = 1;
//		avg->pos = 0;
//	}
//}
//
//static s16 average_value(struct average *avg)
//{
//	if (!unlikely(avg->init)) {
//		if (avg->pos)
//			return avg->sum / avg->pos;
//		return 0;
//	}
//
//	return avg->sum / AVG_ENTRIES;
//}
//
//static void ipw_reset_stats(struct ipw_priv *priv)
//{
//	u32 len = sizeof(u32);
//
//	priv->quality = 0;
//
//	average_init(&priv->average_missed_beacons);
//	priv->exp_avg_rssi = -60;
//	priv->exp_avg_noise = -85 + 0x100;
//
//	priv->last_rate = 0;
//	priv->last_missed_beacons = 0;
//	priv->last_rx_packets = 0;
//	priv->last_tx_packets = 0;
//	priv->last_tx_failures = 0;
//
//	/* Firmware managed, reset only when NIC is restarted, so we have to
//	 * normalize on the current value */
//	ipw_get_ordinal(priv, IPW_ORD_STAT_RX_ERR_CRC,
//			&priv->last_rx_err, &len);
//	ipw_get_ordinal(priv, IPW_ORD_STAT_TX_FAILURE,
//			&priv->last_tx_failures, &len);
//
//	/* Driver managed, reset with each association */
//	priv->missed_adhoc_beacons = 0;
//	priv->missed_beacons = 0;
//	priv->tx_packets = 0;
//	priv->rx_packets = 0;
//
//}
//
//static u32 ipw_get_max_rate(struct ipw_priv *priv)
//{
//	u32 i = 0x80000000;
//	u32 mask = priv->rates_mask;
//	/* If currently associated in B mode, restrict the maximum
//	 * rate match to B rates */
//	if (priv->assoc_request.ieee_mode == IPW_B_MODE)
//		mask &= LIBIPW_CCK_RATES_MASK;
//
//	/* TODO: Verify that the rate is supported by the current rates
//	 * list. */
//
//	while (i && !(mask & i))
//		i >>= 1;
//	switch (i) {
//	case LIBIPW_CCK_RATE_1MB_MASK:
//		return 1000000;
//	case LIBIPW_CCK_RATE_2MB_MASK:
//		return 2000000;
//	case LIBIPW_CCK_RATE_5MB_MASK:
//		return 5500000;
//	case LIBIPW_OFDM_RATE_6MB_MASK:
//		return 6000000;
//	case LIBIPW_OFDM_RATE_9MB_MASK:
//		return 9000000;
//	case LIBIPW_CCK_RATE_11MB_MASK:
//		return 11000000;
//	case LIBIPW_OFDM_RATE_12MB_MASK:
//		return 12000000;
//	case LIBIPW_OFDM_RATE_18MB_MASK:
//		return 18000000;
//	case LIBIPW_OFDM_RATE_24MB_MASK:
//		return 24000000;
//	case LIBIPW_OFDM_RATE_36MB_MASK:
//		return 36000000;
//	case LIBIPW_OFDM_RATE_48MB_MASK:
//		return 48000000;
//	case LIBIPW_OFDM_RATE_54MB_MASK:
//		return 54000000;
//	}
//
//	if (priv->ieee->mode == IEEE_B)
//		return 11000000;
//	else
//		return 54000000;
//}
//
//static u32 ipw_get_current_rate(struct ipw_priv *priv)
//{
//	u32 rate, len = sizeof(rate);
//	int err;
//
//	if (!(priv->status & STATUS_ASSOCIATED))
//		return 0;
//
//	if (priv->tx_packets > IPW_REAL_RATE_RX_PACKET_THRESHOLD) {
//		err = ipw_get_ordinal(priv, IPW_ORD_STAT_TX_CURR_RATE, &rate,
//				      &len);
//		if (err) {
//			IPW_DEBUG_INFO("failed querying ordinals.\n");
//			return 0;
//		}
//	} else
//		return ipw_get_max_rate(priv);
//
//	switch (rate) {
//	case IPW_TX_RATE_1MB:
//		return 1000000;
//	case IPW_TX_RATE_2MB:
//		return 2000000;
//	case IPW_TX_RATE_5MB:
//		return 5500000;
//	case IPW_TX_RATE_6MB:
//		return 6000000;
//	case IPW_TX_RATE_9MB:
//		return 9000000;
//	case IPW_TX_RATE_11MB:
//		return 11000000;
//	case IPW_TX_RATE_12MB:
//		return 12000000;
//	case IPW_TX_RATE_18MB:
//		return 18000000;
//	case IPW_TX_RATE_24MB:
//		return 24000000;
//	case IPW_TX_RATE_36MB:
//		return 36000000;
//	case IPW_TX_RATE_48MB:
//		return 48000000;
//	case IPW_TX_RATE_54MB:
//		return 54000000;
//	}
//
//	return 0;
//}
//
//#define IPW_STATS_INTERVAL (2 * HZ)
//static void ipw_gather_stats(struct ipw_priv *priv)
//{
//	u32 rx_err, rx_err_delta, rx_packets_delta;
//	u32 tx_failures, tx_failures_delta, tx_packets_delta;
//	u32 missed_beacons_percent, missed_beacons_delta;
//	u32 quality = 0;
//	u32 len = sizeof(u32);
//	s16 rssi;
//	u32 beacon_quality, signal_quality, tx_quality, rx_quality,
//	    rate_quality;
//	u32 max_rate;
//
//	if (!(priv->status & STATUS_ASSOCIATED)) {
//		priv->quality = 0;
//		return;
//	}
//
//	/* Update the statistics */
//	ipw_get_ordinal(priv, IPW_ORD_STAT_MISSED_BEACONS,
//			&priv->missed_beacons, &len);
//	missed_beacons_delta = priv->missed_beacons - priv->last_missed_beacons;
//	priv->last_missed_beacons = priv->missed_beacons;
//	if (priv->assoc_request.beacon_interval) {
//		missed_beacons_percent = missed_beacons_delta *
//		    (HZ * le16_to_cpu(priv->assoc_request.beacon_interval)) /
//		    (IPW_STATS_INTERVAL * 10);
//	} else {
//		missed_beacons_percent = 0;
//	}
//	average_add(&priv->average_missed_beacons, missed_beacons_percent);
//
//	ipw_get_ordinal(priv, IPW_ORD_STAT_RX_ERR_CRC, &rx_err, &len);
//	rx_err_delta = rx_err - priv->last_rx_err;
//	priv->last_rx_err = rx_err;
//
//	ipw_get_ordinal(priv, IPW_ORD_STAT_TX_FAILURE, &tx_failures, &len);
//	tx_failures_delta = tx_failures - priv->last_tx_failures;
//	priv->last_tx_failures = tx_failures;
//
//	rx_packets_delta = priv->rx_packets - priv->last_rx_packets;
//	priv->last_rx_packets = priv->rx_packets;
//
//	tx_packets_delta = priv->tx_packets - priv->last_tx_packets;
//	priv->last_tx_packets = priv->tx_packets;
//
//	/* Calculate quality based on the following:
//	 *
//	 * Missed beacon: 100% = 0, 0% = 70% missed
//	 * Rate: 60% = 1Mbs, 100% = Max
//	 * Rx and Tx errors represent a straight % of total Rx/Tx
//	 * RSSI: 100% = > -50,  0% = < -80
//	 * Rx errors: 100% = 0, 0% = 50% missed
//	 *
//	 * The lowest computed quality is used.
//	 *
//	 */
//#define BEACON_THRESHOLD 5
//	beacon_quality = 100 - missed_beacons_percent;
//	if (beacon_quality < BEACON_THRESHOLD)
//		beacon_quality = 0;
//	else
//		beacon_quality = (beacon_quality - BEACON_THRESHOLD) * 100 /
//		    (100 - BEACON_THRESHOLD);
//	IPW_DEBUG_STATS("Missed beacon: %3d%% (%d%%)\n",
//			beacon_quality, missed_beacons_percent);
//
//	priv->last_rate = ipw_get_current_rate(priv);
//	max_rate = ipw_get_max_rate(priv);
//	rate_quality = priv->last_rate * 40 / max_rate + 60;
//	IPW_DEBUG_STATS("Rate quality : %3d%% (%dMbs)\n",
//			rate_quality, priv->last_rate / 1000000);
//
//	if (rx_packets_delta > 100 && rx_packets_delta + rx_err_delta)
//		rx_quality = 100 - (rx_err_delta * 100) /
//		    (rx_packets_delta + rx_err_delta);
//	else
//		rx_quality = 100;
//	IPW_DEBUG_STATS("Rx quality   : %3d%% (%u errors, %u packets)\n",
//			rx_quality, rx_err_delta, rx_packets_delta);
//
//	if (tx_packets_delta > 100 && tx_packets_delta + tx_failures_delta)
//		tx_quality = 100 - (tx_failures_delta * 100) /
//		    (tx_packets_delta + tx_failures_delta);
//	else
//		tx_quality = 100;
//	IPW_DEBUG_STATS("Tx quality   : %3d%% (%u errors, %u packets)\n",
//			tx_quality, tx_failures_delta, tx_packets_delta);
//
//	rssi = priv->exp_avg_rssi;
//	signal_quality =
//	    (100 *
//	     (priv->ieee->perfect_rssi - priv->ieee->worst_rssi) *
//	     (priv->ieee->perfect_rssi - priv->ieee->worst_rssi) -
//	     (priv->ieee->perfect_rssi - rssi) *
//	     (15 * (priv->ieee->perfect_rssi - priv->ieee->worst_rssi) +
//	      62 * (priv->ieee->perfect_rssi - rssi))) /
//	    ((priv->ieee->perfect_rssi - priv->ieee->worst_rssi) *
//	     (priv->ieee->perfect_rssi - priv->ieee->worst_rssi));
//	if (signal_quality > 100)
//		signal_quality = 100;
//	else if (signal_quality < 1)
//		signal_quality = 0;
//
//	IPW_DEBUG_STATS("Signal level : %3d%% (%d dBm)\n",
//			signal_quality, rssi);
//
//	quality = min(rx_quality, signal_quality);
//	quality = min(tx_quality, quality);
//	quality = min(rate_quality, quality);
//	quality = min(beacon_quality, quality);
//	if (quality == beacon_quality)
//		IPW_DEBUG_STATS("Quality (%d%%): Clamped to missed beacons.\n",
//				quality);
//	if (quality == rate_quality)
//		IPW_DEBUG_STATS("Quality (%d%%): Clamped to rate quality.\n",
//				quality);
//	if (quality == tx_quality)
//		IPW_DEBUG_STATS("Quality (%d%%): Clamped to Tx quality.\n",
//				quality);
//	if (quality == rx_quality)
//		IPW_DEBUG_STATS("Quality (%d%%): Clamped to Rx quality.\n",
//				quality);
//	if (quality == signal_quality)
//		IPW_DEBUG_STATS("Quality (%d%%): Clamped to signal quality.\n",
//				quality);
//
//	priv->quality = quality;
//
//	queue_delayed_work(priv->workqueue, &priv->gather_stats,
//			   IPW_STATS_INTERVAL);
//}
//
//static void ipw_bg_gather_stats(struct work_struct *work)
//{
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, gather_stats.work);
//	mutex_lock(&priv->mutex);
//	ipw_gather_stats(priv);
//	mutex_unlock(&priv->mutex);
//}
//
///* Missed beacon behavior:
// * 1st missed -> roaming_threshold, just wait, don't do any scan/roam.
// * roaming_threshold -> disassociate_threshold, scan and roam for better signal.
// * Above disassociate threshold, give up and stop scanning.
// * Roaming is disabled if disassociate_threshold <= roaming_threshold  */
//static void ipw_handle_missed_beacon(struct ipw_priv *priv,
//					    int missed_count)
//{
//	priv->notif_missed_beacons = missed_count;
//
//	if (missed_count > priv->disassociate_threshold &&
//	    priv->status & STATUS_ASSOCIATED) {
//		/* If associated and we've hit the missed
//		 * beacon threshold, disassociate, turn
//		 * off roaming, and abort any active scans */
//		IPW_DEBUG(IPW_DL_INFO | IPW_DL_NOTIF |
//			  IPW_DL_STATE | IPW_DL_ASSOC,
//			  "Missed beacon: %d - disassociate\n", missed_count);
//		priv->status &= ~STATUS_ROAMING;
//		if (priv->status & STATUS_SCANNING) {
//			IPW_DEBUG(IPW_DL_INFO | IPW_DL_NOTIF |
//				  IPW_DL_STATE,
//				  "Aborting scan with missed beacon.\n");
//			queue_work(priv->workqueue, &priv->abort_scan);
//		}
//
//		queue_work(priv->workqueue, &priv->disassociate);
//		return;
//	}
//
//	if (priv->status & STATUS_ROAMING) {
//		/* If we are currently roaming, then just
//		 * print a debug statement... */
//		IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE,
//			  "Missed beacon: %d - roam in progress\n",
//			  missed_count);
//		return;
//	}
//
//	if (roaming &&
//	    (missed_count > priv->roaming_threshold &&
//	     missed_count <= priv->disassociate_threshold)) {
//		/* If we are not already roaming, set the ROAM
//		 * bit in the status and kick off a scan.
//		 * This can happen several times before we reach
//		 * disassociate_threshold. */
//		IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE,
//			  "Missed beacon: %d - initiate "
//			  "roaming\n", missed_count);
//		if (!(priv->status & STATUS_ROAMING)) {
//			priv->status |= STATUS_ROAMING;
//			if (!(priv->status & STATUS_SCANNING))
//				queue_delayed_work(priv->workqueue,
//						   &priv->request_scan, 0);
//		}
//		return;
//	}
//
//	if (priv->status & STATUS_SCANNING &&
//	    missed_count > IPW_MB_SCAN_CANCEL_THRESHOLD) {
//		/* Stop scan to keep fw from getting
//		 * stuck (only if we aren't roaming --
//		 * otherwise we'll never scan more than 2 or 3
//		 * channels..) */
//		IPW_DEBUG(IPW_DL_INFO | IPW_DL_NOTIF | IPW_DL_STATE,
//			  "Aborting scan with missed beacon.\n");
//		queue_work(priv->workqueue, &priv->abort_scan);
//	}
//
//	IPW_DEBUG_NOTIF("Missed beacon: %d\n", missed_count);
//}
//
//static void ipw_scan_event(struct work_struct *work)
//{
//	union iwreq_data wrqu;
//
//	struct ipw_priv *priv =
//		container_of(work, struct ipw_priv, scan_event.work);
//
//	wrqu.data.length = 0;
//	wrqu.data.flags = 0;
//	wireless_send_event(priv->net_dev, SIOCGIWSCAN, &wrqu, NULL);
//}
//
//static void handle_scan_event(struct ipw_priv *priv)
//{
//	/* Only userspace-requested scan completion events go out immediately */
//	if (!priv->user_requested_scan) {
//		if (!delayed_work_pending(&priv->scan_event))
//			queue_delayed_work(priv->workqueue, &priv->scan_event,
//					 round_jiffies_relative(msecs_to_jiffies(4000)));
//	} else {
//		union iwreq_data wrqu;
//
//		priv->user_requested_scan = 0;
//		cancel_delayed_work(&priv->scan_event);
//
//		wrqu.data.length = 0;
//		wrqu.data.flags = 0;
//		wireless_send_event(priv->net_dev, SIOCGIWSCAN, &wrqu, NULL);
//	}
//}
//
///**
// * Handle host notification packet.
// * Called from interrupt routine
// */
//static void ipw_rx_notification(struct ipw_priv *priv,
//				       struct ipw_rx_notification *notif)
//{
//	DECLARE_SSID_BUF(ssid);
//	u16 size = le16_to_cpu(notif->size);
//
//	IPW_DEBUG_NOTIF("type = %i (%d bytes)\n", notif->subtype, size);
//
//	switch (notif->subtype) {
//	case HOST_NOTIFICATION_STATUS_ASSOCIATED:{
//			struct notif_association *assoc = &notif->u.assoc;
//
//			switch (assoc->state) {
//			case CMAS_ASSOCIATED:{
//					IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//						  IPW_DL_ASSOC,
//						  "associated: '%s' %pM \n",
//						  print_ssid(ssid, priv->essid,
//							     priv->essid_len),
//						  priv->bssid);
//
//					switch (priv->ieee->iw_mode) {
//					case IW_MODE_INFRA:
//						memcpy(priv->ieee->bssid,
//						       priv->bssid, ETH_ALEN);
//						break;
//
//					case IW_MODE_ADHOC:
//						memcpy(priv->ieee->bssid,
//						       priv->bssid, ETH_ALEN);
//
//						/* clear out the station table */
//						priv->num_stations = 0;
//
//						IPW_DEBUG_ASSOC
//						    ("queueing adhoc check\n");
//						queue_delayed_work(priv->
//								   workqueue,
//								   &priv->
//								   adhoc_check,
//								   le16_to_cpu(priv->
//								   assoc_request.
//								   beacon_interval));
//						break;
//					}
//
//					priv->status &= ~STATUS_ASSOCIATING;
//					priv->status |= STATUS_ASSOCIATED;
//					queue_work(priv->workqueue,
//						   &priv->system_config);
//
//#ifdef CONFIG_IPW2200_QOS
//#define IPW_GET_PACKET_STYPE(x) WLAN_FC_GET_STYPE( \
//			 le16_to_cpu(((struct ieee80211_hdr *)(x))->frame_control))
//					if ((priv->status & STATUS_AUTH) &&
//					    (IPW_GET_PACKET_STYPE(&notif->u.raw)
//					     == IEEE80211_STYPE_ASSOC_RESP)) {
//						if ((sizeof
//						     (struct
//						      libipw_assoc_response)
//						     <= size)
//						    && (size <= 2314)) {
//							struct
//							libipw_rx_stats
//							    stats = {
//								.len = size - 1,
//							};
//
//							IPW_DEBUG_QOS
//							    ("QoS Associate "
//							     "size %d\n", size);
//							libipw_rx_mgt(priv->
//									 ieee,
//									 (struct
//									  libipw_hdr_4addr
//									  *)
//									 &notif->u.raw, &stats);
//						}
//					}
//#endif
//
//					schedule_work(&priv->link_up);
//
//					break;
//				}
//
//			case CMAS_AUTHENTICATED:{
//					if (priv->
//					    status & (STATUS_ASSOCIATED |
//						      STATUS_AUTH)) {
//						struct notif_authenticate *auth
//						    = &notif->u.auth;
//						IPW_DEBUG(IPW_DL_NOTIF |
//							  IPW_DL_STATE |
//							  IPW_DL_ASSOC,
//							  "deauthenticated: '%s' "
//							  "%pM"
//							  ": (0x%04X) - %s \n",
//							  print_ssid(ssid,
//								     priv->
//								     essid,
//								     priv->
//								     essid_len),
//							  priv->bssid,
//							  le16_to_cpu(auth->status),
//							  ipw_get_status_code
//							  (le16_to_cpu
//							   (auth->status)));
//
//						priv->status &=
//						    ~(STATUS_ASSOCIATING |
//						      STATUS_AUTH |
//						      STATUS_ASSOCIATED);
//
//						schedule_work(&priv->link_down);
//						break;
//					}
//
//					IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//						  IPW_DL_ASSOC,
//						  "authenticated: '%s' %pM\n",
//						  print_ssid(ssid, priv->essid,
//							     priv->essid_len),
//						  priv->bssid);
//					break;
//				}
//
//			case CMAS_INIT:{
//					if (priv->status & STATUS_AUTH) {
//						struct
//						    libipw_assoc_response
//						*resp;
//						resp =
//						    (struct
//						     libipw_assoc_response
//						     *)&notif->u.raw;
//						IPW_DEBUG(IPW_DL_NOTIF |
//							  IPW_DL_STATE |
//							  IPW_DL_ASSOC,
//							  "association failed (0x%04X): %s\n",
//							  le16_to_cpu(resp->status),
//							  ipw_get_status_code
//							  (le16_to_cpu
//							   (resp->status)));
//					}
//
//					IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//						  IPW_DL_ASSOC,
//						  "disassociated: '%s' %pM \n",
//						  print_ssid(ssid, priv->essid,
//							     priv->essid_len),
//						  priv->bssid);
//
//					priv->status &=
//					    ~(STATUS_DISASSOCIATING |
//					      STATUS_ASSOCIATING |
//					      STATUS_ASSOCIATED | STATUS_AUTH);
//					if (priv->assoc_network
//					    && (priv->assoc_network->
//						capability &
//						WLAN_CAPABILITY_IBSS))
//						ipw_remove_current_network
//						    (priv);
//
//					schedule_work(&priv->link_down);
//
//					break;
//				}
//
//			case CMAS_RX_ASSOC_RESP:
//				break;
//
//			default:
//				IPW_ERROR("assoc: unknown (%d)\n",
//					  assoc->state);
//				break;
//			}
//
//			break;
//		}
//
//	case HOST_NOTIFICATION_STATUS_AUTHENTICATE:{
//			struct notif_authenticate *auth = &notif->u.auth;
//			switch (auth->state) {
//			case CMAS_AUTHENTICATED:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE,
//					  "authenticated: '%s' %pM \n",
//					  print_ssid(ssid, priv->essid,
//						     priv->essid_len),
//					  priv->bssid);
//				priv->status |= STATUS_AUTH;
//				break;
//
//			case CMAS_INIT:
//				if (priv->status & STATUS_AUTH) {
//					IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//						  IPW_DL_ASSOC,
//						  "authentication failed (0x%04X): %s\n",
//						  le16_to_cpu(auth->status),
//						  ipw_get_status_code(le16_to_cpu
//								      (auth->
//								       status)));
//				}
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC,
//					  "deauthenticated: '%s' %pM\n",
//					  print_ssid(ssid, priv->essid,
//						     priv->essid_len),
//					  priv->bssid);
//
//				priv->status &= ~(STATUS_ASSOCIATING |
//						  STATUS_AUTH |
//						  STATUS_ASSOCIATED);
//
//				schedule_work(&priv->link_down);
//				break;
//
//			case CMAS_TX_AUTH_SEQ_1:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "AUTH_SEQ_1\n");
//				break;
//			case CMAS_RX_AUTH_SEQ_2:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "AUTH_SEQ_2\n");
//				break;
//			case CMAS_AUTH_SEQ_1_PASS:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "AUTH_SEQ_1_PASS\n");
//				break;
//			case CMAS_AUTH_SEQ_1_FAIL:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "AUTH_SEQ_1_FAIL\n");
//				break;
//			case CMAS_TX_AUTH_SEQ_3:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "AUTH_SEQ_3\n");
//				break;
//			case CMAS_RX_AUTH_SEQ_4:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "RX_AUTH_SEQ_4\n");
//				break;
//			case CMAS_AUTH_SEQ_2_PASS:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "AUTH_SEQ_2_PASS\n");
//				break;
//			case CMAS_AUTH_SEQ_2_FAIL:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "AUT_SEQ_2_FAIL\n");
//				break;
//			case CMAS_TX_ASSOC:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "TX_ASSOC\n");
//				break;
//			case CMAS_RX_ASSOC_RESP:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "RX_ASSOC_RESP\n");
//
//				break;
//			case CMAS_ASSOCIATED:
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE |
//					  IPW_DL_ASSOC, "ASSOCIATED\n");
//				break;
//			default:
//				IPW_DEBUG_NOTIF("auth: failure - %d\n",
//						auth->state);
//				break;
//			}
//			break;
//		}
//
//	case HOST_NOTIFICATION_STATUS_SCAN_CHANNEL_RESULT:{
//			struct notif_channel_result *x =
//			    &notif->u.channel_result;
//
//			if (size == sizeof(*x)) {
//				IPW_DEBUG_SCAN("Scan result for channel %d\n",
//					       x->channel_num);
//			} else {
//				IPW_DEBUG_SCAN("Scan result of wrong size %d "
//					       "(should be %zd)\n",
//					       size, sizeof(*x));
//			}
//			break;
//		}
//
//	case HOST_NOTIFICATION_STATUS_SCAN_COMPLETED:{
//			struct notif_scan_complete *x = &notif->u.scan_complete;
//			if (size == sizeof(*x)) {
//				IPW_DEBUG_SCAN
//				    ("Scan completed: type %d, %d channels, "
//				     "%d status\n", x->scan_type,
//				     x->num_channels, x->status);
//			} else {
//				IPW_ERROR("Scan completed of wrong size %d "
//					  "(should be %zd)\n",
//					  size, sizeof(*x));
//			}
//
//			priv->status &=
//			    ~(STATUS_SCANNING | STATUS_SCAN_ABORTING);
//
//			wake_up_interruptible(&priv->wait_state);
//			cancel_delayed_work(&priv->scan_check);
//
//			if (priv->status & STATUS_EXIT_PENDING)
//				break;
//
//			priv->ieee->scans++;
//
//#ifdef CONFIG_IPW2200_MONITOR
//			if (priv->ieee->iw_mode == IW_MODE_MONITOR) {
//				priv->status |= STATUS_SCAN_FORCED;
//				queue_delayed_work(priv->workqueue,
//						   &priv->request_scan, 0);
//				break;
//			}
//			priv->status &= ~STATUS_SCAN_FORCED;
//#endif				/* CONFIG_IPW2200_MONITOR */
//
//			/* Do queued direct scans first */
//			if (priv->status & STATUS_DIRECT_SCAN_PENDING) {
//				queue_delayed_work(priv->workqueue,
//						   &priv->request_direct_scan, 0);
//			}
//
//			if (!(priv->status & (STATUS_ASSOCIATED |
//					      STATUS_ASSOCIATING |
//					      STATUS_ROAMING |
//					      STATUS_DISASSOCIATING)))
//				queue_work(priv->workqueue, &priv->associate);
//			else if (priv->status & STATUS_ROAMING) {
//				if (x->status == SCAN_COMPLETED_STATUS_COMPLETE)
//					/* If a scan completed and we are in roam mode, then
//					 * the scan that completed was the one requested as a
//					 * result of entering roam... so, schedule the
//					 * roam work */
//					queue_work(priv->workqueue,
//						   &priv->roam);
//				else
//					/* Don't schedule if we aborted the scan */
//					priv->status &= ~STATUS_ROAMING;
//			} else if (priv->status & STATUS_SCAN_PENDING)
//				queue_delayed_work(priv->workqueue,
//						   &priv->request_scan, 0);
//			else if (priv->config & CFG_BACKGROUND_SCAN
//				 && priv->status & STATUS_ASSOCIATED)
//				queue_delayed_work(priv->workqueue,
//						   &priv->request_scan,
//						   round_jiffies_relative(HZ));
//
//			/* Send an empty event to user space.
//			 * We don't send the received data on the event because
//			 * it would require us to do complex transcoding, and
//			 * we want to minimise the work done in the irq handler
//			 * Use a request to extract the data.
//			 * Also, we generate this even for any scan, regardless
//			 * on how the scan was initiated. User space can just
//			 * sync on periodic scan to get fresh data...
//			 * Jean II */
//			if (x->status == SCAN_COMPLETED_STATUS_COMPLETE)
//				handle_scan_event(priv);
//			break;
//		}
//
//	case HOST_NOTIFICATION_STATUS_FRAG_LENGTH:{
//			struct notif_frag_length *x = &notif->u.frag_len;
//
//			if (size == sizeof(*x))
//				IPW_ERROR("Frag length: %d\n",
//					  le16_to_cpu(x->frag_length));
//			else
//				IPW_ERROR("Frag length of wrong size %d "
//					  "(should be %zd)\n",
//					  size, sizeof(*x));
//			break;
//		}
//
//	case HOST_NOTIFICATION_STATUS_LINK_DETERIORATION:{
//			struct notif_link_deterioration *x =
//			    &notif->u.link_deterioration;
//
//			if (size == sizeof(*x)) {
//				IPW_DEBUG(IPW_DL_NOTIF | IPW_DL_STATE,
//					"link deterioration: type %d, cnt %d\n",
//					x->silence_notification_type,
//					x->silence_count);
//				memcpy(&priv->last_link_deterioration, x,
//				       sizeof(*x));
//			} else {
//				IPW_ERROR("Link Deterioration of wrong size %d "
//					  "(should be %zd)\n",
//					  size, sizeof(*x));
//			}
//			break;
//		}
//
//	case HOST_NOTIFICATION_DINO_CONFIG_RESPONSE:{
//			IPW_ERROR("Dino config\n");
//			if (priv->hcmd
//			    && priv->hcmd->cmd != HOST_CMD_DINO_CONFIG)
//				IPW_ERROR("Unexpected DINO_CONFIG_RESPONSE\n");
//
//			break;
//		}
//
//	case HOST_NOTIFICATION_STATUS_BEACON_STATE:{
//			struct notif_beacon_state *x = &notif->u.beacon_state;
//			if (size != sizeof(*x)) {
//				IPW_ERROR
//				    ("Beacon state of wrong size %d (should "
//				     "be %zd)\n", size, sizeof(*x));
//				break;
//			}
//
//			if (le32_to_cpu(x->state) ==
//			    HOST_NOTIFICATION_STATUS_BEACON_MISSING)
//				ipw_handle_missed_beacon(priv,
//							 le32_to_cpu(x->
//								     number));
//
//			break;
//		}
//
//	case HOST_NOTIFICATION_STATUS_TGI_TX_KEY:{
//			struct notif_tgi_tx_key *x = &notif->u.tgi_tx_key;
//			if (size == sizeof(*x)) {
//				IPW_ERROR("TGi Tx Key: state 0x%02x sec type "
//					  "0x%02x station %d\n",
//					  x->key_state, x->security_type,
//					  x->station_index);
//				break;
//			}
//
//			IPW_ERROR
//			    ("TGi Tx Key of wrong size %d (should be %zd)\n",
//			     size, sizeof(*x));
//			break;
//		}
//
//	case HOST_NOTIFICATION_CALIB_KEEP_RESULTS:{
//			struct notif_calibration *x = &notif->u.calibration;
//
//			if (size == sizeof(*x)) {
//				memcpy(&priv->calib, x, sizeof(*x));
//				IPW_DEBUG_INFO("TODO: Calibration\n");
//				break;
//			}
//
//			IPW_ERROR
//			    ("Calibration of wrong size %d (should be %zd)\n",
//			     size, sizeof(*x));
//			break;
//		}
//
//	case HOST_NOTIFICATION_NOISE_STATS:{
//			if (size == sizeof(u32)) {
//				priv->exp_avg_noise =
//				    exponential_average(priv->exp_avg_noise,
//				    (u8) (le32_to_cpu(notif->u.noise.value) & 0xff),
//				    DEPTH_NOISE);
//				break;
//			}
//
//			IPW_ERROR
//			    ("Noise stat is wrong size %d (should be %zd)\n",
//			     size, sizeof(u32));
//			break;
//		}
//
//	default:
//		IPW_DEBUG_NOTIF("Unknown notification: "
//				"subtype=%d,flags=0x%2x,size=%d\n",
//				notif->subtype, notif->flags, size);
//	}
//}
//
///**
// * Destroys all DMA structures and initialise them again
// *
// * @param priv
// * @return error code
// */
//static int ipw_queue_reset(struct ipw_priv *priv)
//{
//	int rc = 0;
//	/** @todo customize queue sizes */
//	int nTx = 64, nTxCmd = 8;
//	ipw_tx_queue_free(priv);
//	/* Tx CMD queue */
//	rc = ipw_queue_tx_init(priv, &priv->txq_cmd, nTxCmd,
//			       IPW_TX_CMD_QUEUE_READ_INDEX,
//			       IPW_TX_CMD_QUEUE_WRITE_INDEX,
//			       IPW_TX_CMD_QUEUE_BD_BASE,
//			       IPW_TX_CMD_QUEUE_BD_SIZE);
//	if (rc) {
//		IPW_ERROR("Tx Cmd queue init failed\n");
//		goto error;
//	}
//	/* Tx queue(s) */
//	rc = ipw_queue_tx_init(priv, &priv->txq[0], nTx,
//			       IPW_TX_QUEUE_0_READ_INDEX,
//			       IPW_TX_QUEUE_0_WRITE_INDEX,
//			       IPW_TX_QUEUE_0_BD_BASE, IPW_TX_QUEUE_0_BD_SIZE);
//	if (rc) {
//		IPW_ERROR("Tx 0 queue init failed\n");
//		goto error;
//	}
//	rc = ipw_queue_tx_init(priv, &priv->txq[1], nTx,
//			       IPW_TX_QUEUE_1_READ_INDEX,
//			       IPW_TX_QUEUE_1_WRITE_INDEX,
//			       IPW_TX_QUEUE_1_BD_BASE, IPW_TX_QUEUE_1_BD_SIZE);
//	if (rc) {
//		IPW_ERROR("Tx 1 queue init failed\n");
//		goto error;
//	}
//	rc = ipw_queue_tx_init(priv, &priv->txq[2], nTx,
//			       IPW_TX_QUEUE_2_READ_INDEX,
//			       IPW_TX_QUEUE_2_WRITE_INDEX,
//			       IPW_TX_QUEUE_2_BD_BASE, IPW_TX_QUEUE_2_BD_SIZE);
//	if (rc) {
//		IPW_ERROR("Tx 2 queue init failed\n");
//		goto error;
//	}
//	rc = ipw_queue_tx_init(priv, &priv->txq[3], nTx,
//			       IPW_TX_QUEUE_3_READ_INDEX,
//			       IPW_TX_QUEUE_3_WRITE_INDEX,
//			       IPW_TX_QUEUE_3_BD_BASE, IPW_TX_QUEUE_3_BD_SIZE);
//	if (rc) {
//		IPW_ERROR("Tx 3 queue init failed\n");
//		goto error;
//	}
//	/* statistics */
//	priv->rx_bufs_min = 0;
//	priv->rx_pend_max = 0;
//	return rc;
//
//      error:
//	ipw_tx_queue_free(priv);
//	return rc;
//}
//
///**
// * Reclaim Tx queue entries no more used by NIC.
// *
// * When FW advances 'R' index, all entries between old and
// * new 'R' index need to be reclaimed. As result, some free space
// * forms. If there is enough free space (> low mark), wake Tx queue.
// *
// * @note Need to protect against garbage in 'R' index
// * @param priv
// * @param txq
// * @param qindex
// * @return Number of used entries remains in the queue
// */
//static int ipw_queue_tx_reclaim(struct ipw_priv *priv,
//				struct clx2_tx_queue *txq, int qindex)
//{
//	u32 hw_tail;
//	int used;
//	struct clx2_queue *q = &txq->q;
//
//	hw_tail = ipw_read32(priv, q->reg_r);
//	if (hw_tail >= q->n_bd) {
//		IPW_ERROR
//		    ("Read index for DMA queue (%d) is out of range [0-%d)\n",
//		     hw_tail, q->n_bd);
//		goto done;
//	}
//	for (; q->last_used != hw_tail;
//	     q->last_used = ipw_queue_inc_wrap(q->last_used, q->n_bd)) {
//		ipw_queue_tx_free_tfd(priv, txq);
//		priv->tx_packets++;
//	}
//      done:
//	if ((ipw_tx_queue_space(q) > q->low_mark) &&
//	    (qindex >= 0))
//		netif_wake_queue(priv->net_dev);
//	used = q->first_empty - q->last_used;
//	if (used < 0)
//		used += q->n_bd;
//
//	return used;
//}
//
//static int ipw_queue_tx_hcmd(struct ipw_priv *priv, int hcmd, void *buf,
//			     int len, int sync)
//{
//	struct clx2_tx_queue *txq = &priv->txq_cmd;
//	struct clx2_queue *q = &txq->q;
//	struct tfd_frame *tfd;
//
//	if (ipw_tx_queue_space(q) < (sync ? 1 : 2)) {
//		IPW_ERROR("No space for Tx\n");
//		return -EBUSY;
//	}
//
//	tfd = &txq->bd[q->first_empty];
//	txq->txb[q->first_empty] = NULL;
//
//	memset(tfd, 0, sizeof(*tfd));
//	tfd->control_flags.message_type = TX_HOST_COMMAND_TYPE;
//	tfd->control_flags.control_bits = TFD_NEED_IRQ_MASK;
//	priv->hcmd_seq++;
//	tfd->u.cmd.index = hcmd;
//	tfd->u.cmd.length = len;
//	memcpy(tfd->u.cmd.payload, buf, len);
//	q->first_empty = ipw_queue_inc_wrap(q->first_empty, q->n_bd);
//	ipw_write32(priv, q->reg_w, q->first_empty);
//	_ipw_read32(priv, 0x90);
//
//	return 0;
//}
//
