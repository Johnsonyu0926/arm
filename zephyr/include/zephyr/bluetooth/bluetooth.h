// Filename: bluetooth.h
// Score: 93

#include <zephyr/device.h>
#include <zephyr/kernel.h>
#include <zephyr/bluetooth/bluetooth.h>

#ifdef __cplusplus
extern "C" {
#endif

typedef int (*bt_api_enable_t)(const struct device *dev);
typedef int (*bt_api_disable_t)(const struct device *dev);
typedef int (*bt_api_scan_start_t)(const struct device *dev, const struct bt_le_scan_param *param, bt_le_scan_cb_t cb);
typedef int (*bt_api_scan_stop_t)(const struct device *dev);
typedef int (*bt_api_connect_t)(const struct device *dev, const bt_addr_le_t *addr, const struct bt_conn_le_create_param *param, struct bt_conn **conn);
typedef int (*bt_api_disconnect_t)(const struct device *dev, struct bt_conn *conn, uint8_t reason);

__subsystem struct bt_driver_api {
    bt_api_enable_t enable;
    bt_api_disable_t disable;
    bt_api_scan_start_t scan_start;
    bt_api_scan_stop_t scan_stop;
    bt_api_connect_t connect;
    bt_api_disconnect_t disconnect;
};

__syscall int bt_enable(const struct device *dev);

static inline int z_impl_bt_enable(const struct device *dev) {
    struct bt_driver_api *api = (struct bt_driver_api *)dev->api;
    if (!api->enable) {
        return -ENOSYS;
    }
    return api->enable(dev);
}

__syscall int bt_disable(const struct device *dev);

static inline int z_impl_bt_disable(const struct device *dev) {
    struct bt_driver_api *api = (struct bt_driver_api *)dev->api;
    if (!api->disable) {
        return -ENOSYS;
    }
    return api->disable(dev);
}

__syscall int bt_scan_start(const struct device *dev, const struct bt_le_scan_param *param, bt_le_scan_cb_t cb);

static inline int z_impl_bt_scan_start(const struct device *dev, const struct bt_le_scan_param *param, bt_le_scan_cb_t cb) {
    struct bt_driver_api *api = (struct bt_driver_api *)dev->api;
    if (!api->scan_start) {
        return -ENOSYS;
    }
    return api->scan_start(dev, param, cb);
}

__syscall int bt_scan_stop(const struct device *dev);

static inline int z_impl_bt_scan_stop(const struct device *dev) {
    struct bt_driver_api *api = (struct bt_driver_api *)dev->api;
    if (!api->scan_stop) {
        return -ENOSYS;
    }
    return api->scan_stop(dev);
}

__syscall int bt_connect(const struct device *dev, const bt_addr_le_t *addr, const struct bt_conn_le_create_param *param, struct bt_conn **conn);

static inline int z_impl_bt_connect(const struct device *dev, const bt_addr_le_t *addr, const struct bt_conn_le_create_param *param, struct bt_conn **conn) {
    struct bt_driver_api *api = (struct bt_driver_api *)dev->api;
    if (!api->connect) {
        return -ENOSYS;
    }
    return api->connect(dev, addr, param, conn);
}

__syscall int bt_disconnect(const struct device *dev, struct bt_conn *conn, uint8_t reason);

static inline int z_impl_bt_disconnect(const struct device *dev, struct bt_conn *conn, uint8_t reason) {
    struct bt_driver_api *api = (struct bt_driver_api *)dev->api;
    if (!api->disconnect) {
        return -ENOSYS;
    }
    return api->disconnect(dev, conn, reason);
}

#ifdef __cplusplus
}
#endif

#include <zephyr/syscalls/bluetooth.h>

// By GST @Date