// Filename: navigation.h
// Score: 92

#ifndef ZEPHYR_INCLUDE_DATA_NAVIGATION_H_
#define ZEPHYR_INCLUDE_DATA_NAVIGATION_H_

#include <zephyr/types.h>

/**
 * @brief Navigation utilities
 * @defgroup navigation Navigation
 * @ingroup utilities
 * @{
 */

/**
 * @brief Navigation data structure
 *
 * @details The structure describes the momentary navigation details of a
 * point relative to a sphere (commonly Earth)
 */
struct navigation_data {
    /** Latitudal position in nanodegrees (0 to +-180E9) */
    int64_t latitude;
    /** Altitude in millimeters */
    int32_t altitude;
    /** Speed over ground in millimeters per second */
    uint32_t speed;
    /** Course over ground in degrees (0 to 360E3) */
    uint32_t course;
    /** Horizontal dilution of precision (HDOP) */
    uint16_t hdop;
    /** Vertical dilution of precision (VDOP) */
    uint16_t vdop;
    /** Number of satellites used in the navigation solution */
    uint8_t satellites;
};

/**
 * @brief Navigation status
 *
 * @details The structure describes the status of the navigation system
 */
struct navigation_status {
    /** Fix type */
    enum {
        NAVIGATION_FIX_NONE = 0,
        NAVIGATION_FIX_2D,
        NAVIGATION_FIX_3D,
    } fix;
    /** Fix quality */
    enum {
        NAVIGATION_FIX_QUALITY_NONE = 0,
        NAVIGATION_FIX_QUALITY_GPS,
        NAVIGATION_FIX_QUALITY_DGPS,
        NAVIGATION_FIX_QUALITY_PPS,
        NAVIGATION_FIX_QUALITY_RTK,
        NAVIGATION_FIX_QUALITY_FLOAT_RTK,
        NAVIGATION_FIX_QUALITY_ESTIMATED,
        NAVIGATION_FIX_QUALITY_MANUAL,
        NAVIGATION_FIX_QUALITY_SIMULATION,
    } quality;
    /** Fix status */
    enum {
        NAVIGATION_FIX_STATUS_NONE = 0,
        NAVIGATION_FIX_STATUS_FIX,
        NAVIGATION_FIX_STATUS_DGPS_FIX,
        NAVIGATION_FIX_STATUS_RTK_FIX,
    } status;
};

/**
 * @brief Navigation event
 *
 * @details The structure describes a navigation event
 */
struct navigation_event {
    /** Event type */
    enum {
        NAVIGATION_EVENT_POSITION = 0,
        NAVIGATION_EVENT_STATUS,
        NAVIGATION_EVENT_SATELLITE,
    } type;
    /** Event data */
    union {
        struct navigation_data position;
        struct navigation_status status;
        struct {
            uint8_t id;
            uint8_t elevation;
            uint16_t azimuth;
            uint8_t snr;
        } satellite;
    } data;
};

/**
 * @brief Navigation event callback
 *
 * @details The callback is called when a navigation event occurs
 *
 * @param event The navigation event
 * @param user_data User data
 */
typedef void (*navigation_event_cb_t)(const struct navigation_event *event, void *user_data);

/**
 * @brief Navigation interface
 *
 * @details The structure describes the navigation interface
 */
struct navigation_interface {
    /** Initialize the navigation interface */
    int (*init)(void);
    /** Start the navigation interface */
    int (*start)(void);
    /** Stop the navigation interface */
    int (*stop)(void);
    /** Register a navigation event callback */
    int (*register_event_cb)(navigation_event_cb_t cb, void *user_data);
    /** Unregister a navigation event callback */
    int (*unregister_event_cb)(navigation_event_cb_t cb);
};

/**
 * @brief Get the navigation interface
 *
 * @details The function returns the navigation interface
 *
 * @return The navigation interface
 */
const struct navigation_interface *navigation_get_interface(void);

#ifdef __cplusplus
}
#endif

/**
 * @}
 */

#endif /* ZEPHYR_INCLUDE_DATA_NAVIGATION_H_ */

// By GST @Date