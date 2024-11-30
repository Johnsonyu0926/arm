// zephyr-3.7-branch/include/zephyr/net/mdns_responder.h

/** @file
 * @brief mDNS响应器API
 *
 * 本文件包含mDNS响应器API。这些API用于注册mDNS记录。
 */

#ifndef ZEPHYR_INCLUDE_NET_MDNS_RESPONDER_H_
#define ZEPHYR_INCLUDE_NET_MDNS_RESPONDER_H_

#include <stddef.h>
#include <zephyr/net/dns_sd.h>

/**
 * @brief 注册连续内存的@ref dns_sd_rec记录。
 *
 * mDNS响应器将首先迭代使用@ref DNS_SD_REGISTER_SERVICE注册的mDNS记录（如果有），然后遍历外部记录。
 *
 * @param records 指向mDNS记录数组的指针。它在内部存储而不复制内容，因此必须保持有效。可以设置为NULL，例如在释放内存块之前。
 * @param count 元素数量
 * @return 成功返回0；参数无效返回-EINVAL。
 */
int mdns_responder_set_ext_records(const struct dns_sd_rec *records, size_t count);

#endif /* ZEPHYR_INCLUDE_NET_MDNS_RESPONDER_H_ */

// By GST @Data