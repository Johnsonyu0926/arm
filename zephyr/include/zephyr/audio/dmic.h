/*
 * Copyright (c) 2018, Intel Corporation
 *
 * Author: Seppo Ingalsuo <seppo.ingalsuo@linux.intel.com>
 *         Sathish Kuttan <sathish.k.kuttan@intel.com>
 *
 * SPDX-License-Identifier: Apache-2.0
 */

/**
 * @file
 * @brief Public API header file for Digital Microphones
 *
 * This file contains the Digital Microphone APIs
 */

#ifndef ZEPHYR_INCLUDE_AUDIO_DMIC_H_
#define ZEPHYR_INCLUDE_AUDIO_DMIC_H_

/**
 * @defgroup audio_interface Audio
 * @{
 * @}
 */

/**
 * @brief Abstraction for digital microphones
 *
 * @defgroup audio_dmic_interface Digital Microphone Interface
 * @since 1.13
 * @version 0.1.0
 * @ingroup audio_interface
 * @{
 */

#include <zephyr/kernel.h>
#include <zephyr/device.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * DMIC driver states
 */
enum dmic_state {
    DMIC_STATE_UNINIT,       /**< Uninitialized */
    DMIC_STATE_INITIALIZED,  /**< Initialized */
    DMIC_STATE_CONFIGURED,   /**< Configured */
    DMIC_STATE_ACTIVE,       /**< Active */
    DMIC_STATE_PAUSED,       /**< Paused */
    DMIC_STATE_ERROR,        /**< Error */
};

/**
 * DMIC driver trigger commands
 */
enum dmic_trigger {
    DMIC_TRIGGER_STOP,       /**< Stop stream */
    DMIC_TRIGGER_START,      /**< Start stream */
    DMIC_TRIGGER_PAUSE,      /**< Pause stream */
    DMIC_TRIGGER_RELEASE,    /**< Release paused stream */
    DMIC_TRIGGER_RESET,      /**< Reset stream */
};

/**
 * PDM Channels LEFT / RIGHT
 */
enum pdm_lr {
    PDM_CHAN_LEFT,           /**< Left channel */
    PDM_CHAN_RIGHT,          /**< Right channel */
};

/**
 * PDM Input/Output signal configuration
 */
struct pdm_io_cfg {
    /**
     * @name Parameters common to all PDM controllers
     *@{
     */
    /** Minimum clock frequency supported by the mic */
    uint32_t min_pdm_clk_freq;
    /** Maximum clock frequency supported by the mic */
    uint32_t max_pdm_clk_freq;
    /** Minimum duty cycle in % supported by the mic */
    uint8_t min_pdm_clk_dc;
    /** Maximum duty cycle in % supported by the mic */
    uint8_t max_pdm_clk_dc;
    /**
     * @}
     */

    /**
     * @name Parameters unique to each PDM controller
     * @{
     */
    /** Bit mask to optionally invert PDM clock */
    uint8_t pdm_clk_pol;
    /** Bit mask to optionally invert mic data */
    uint8_t pdm_data_pol;
    /** Collection of clock skew values for each PDM port */
    uint32_t pdm_clk_skew;
    /**
     * @}
     */
};

/**
 * Configuration of the PCM streams to be output by the PDM hardware
 *
 * @note if either \ref pcm_rate or \ref pcm_width is set to 0 for a stream,
 * the stream would be disabled
 */
struct pcm_stream_cfg {
    /** PCM sample rate of stream */
    uint32_t pcm_rate;
    /** PCM sample width of stream */
    uint8_t pcm_width;
    /** PCM sample block size per transfer */
    uint16_t block_size;
    /** SLAB for DMIC driver to allocate buffers for stream */
    struct k_mem_slab *mem_slab;
};

/**
 * Mapping/ordering of the PDM channels to logical PCM output channel
 *
 * Since each controller can have 2 audio channels (stereo),
 * there can be a total of 8x2=16 channels. The actual number of channels
 * shall be described in \ref act_num_chan.
 *
 * If 2 streams are enabled, the channel order will be the same for
 * both streams.
 *
 * Each channel is described as a 4-bit number, the least significant
 * bit indicates LEFT/RIGHT selection of the PDM controller.
 *
 * The most significant 3 bits indicate the PDM controller number:
 *  - bits 0-3 are for channel 0, bit 0 indicates LEFT or RIGHT
 *  - bits 4-7 are for channel 1, bit 4 indicates LEFT or RIGHT
 * and so on.
 *
 * CONSTRAINT: The LEFT and RIGHT channels of EACH PDM controller needs
 * to be adjacent to each other.
 */
struct pdm_chan_cfg {
    /**
     * @name Requested channel map
     * @{
     */
    uint32_t req_chan_map_lo; /**< Channels 0 to 7 */
    uint32_t req_chan_map_hi; /**< Channels 8 to 15 */
    /** @} */

    /**
     * @name Actual channel map that the driver could configure
     * @{
     */
    uint32_t act_chan_map_lo; /**< Channels 0 to 7 */
    uint32_t act_chan_map_hi; /**< Channels 8 to 15 */
    /** @} */

    /** Requested number of channels */
    uint8_t req_num_chan;
    /** Actual number of channels that the driver could configure */
    uint8_t act_num_chan;
    /** Requested number of streams for each channel */
    uint8_t req_num_streams;
    /** Actual number of streams that the driver could configure */
    uint8_t act_num_streams;
};

/**
 * Input configuration structure for the DMIC configuration API
 */
struct dmic_cfg {
    struct pdm_io_cfg io;
    /**
     * Array of pcm_stream_cfg for application to provide
     * configuration for each stream
     */
    struct pcm_stream_cfg *streams;
    struct pdm_chan_cfg channel;
};

/**
 * Function pointers for the DMIC driver operations
 */
struct _dmic_ops {
    int (*configure)(const struct device *dev, struct dmic_cfg *config);
    int (*trigger)(const struct device *dev, enum dmic_trigger cmd);
    int (*read)(const struct device *dev, uint8_t stream, void **buffer, size_t *size, int32_t timeout);
};

/**
 * Build the channel map to populate struct pdm_chan_cfg
 *
 * Returns the map of PDM controller and LEFT/RIGHT channel shifted to
 * the bit position
请从第197行续写
回复 yuJohnson: 
请从第197行续写
 * Build the channel map to populate struct pdm_chan_cfg
 *
 * Returns the map of PDM controller and LEFT/RIGHT channel shifted to
 * the bit position
 *
 * @param pdm_controller PDM controller number
 * @param lr LEFT/RIGHT channel
 * @param bit_pos Bit position to shift the map
 *
 * @return Channel map
 */
static inline uint32_t dmic_build_channel_map(uint8_t pdm_controller, enum pdm_lr lr, uint8_t bit_pos)
{
    return ((pdm_controller << 1) | lr) << bit_pos;
}
/**
 * @brief Configure the DMIC device
 *
 * Configure the DMIC device according to the configuration parameters provided as input
 *
 * @param dev Pointer to the device structure for the DMIC driver instance.
 * @param config Pointer to the structure containing the DMIC configuration.
 *
 * @return 0 on success, negative error code on failure
 */
static inline int dmic_configure(const struct device *dev, struct dmic_cfg *config)
{
    const struct _dmic_ops *api = (const struct _dmic_ops *)dev->api;
    return api->configure(dev, config);
}
/**
 * @brief Trigger the DMIC device
 *
 * Trigger the DMIC device with the specified command
 *
 * @param dev Pointer to the device structure for the DMIC driver instance.
 * @param cmd The trigger command to be issued to the DMIC device.
 *
 * @return 0 on success, negative error code on failure
 */
static inline int dmic_trigger(const struct device *dev, enum dmic_trigger cmd)
{
    const struct _dmic_ops *api = (const struct _dmic_ops *)dev->api;
    return api->trigger(dev, cmd);
}
/**
 * @brief Read data from the DMIC device
 *
 * Read data from the DMIC device for the specified stream
 *
 * @param dev Pointer to the device structure for the DMIC driver instance.
 * @param stream The stream number to read data from.
 * @param buffer Pointer to the buffer where the read data will be stored.
 * @param size Pointer to the variable where the size of the read data will be stored.
 * @param timeout Timeout value for the read operation.
 *
 * @return 0 on success, negative error code on failure
 */
static inline int dmic_read(const struct device *dev, uint8_t stream, void **buffer, size_t *size, int32_t timeout)
{
    const struct _dmic_ops *api = (const struct _dmic_ops *)dev->api;
    return api->read(dev, stream, buffer, size, timeout);
}
#ifdef __cplusplus
}
#endif
/**
 * @}
 */
#endif /* ZEPHYR_INCLUDE_AUDIO_DMIC_H_ */
// By GST @Date