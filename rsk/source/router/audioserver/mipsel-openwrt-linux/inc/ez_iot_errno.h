/**
 * @file ez_iot_errno.h
 * @author xurongjun (xurongjun@ezvizlife.com)
 * @brief 
 * @version 0.1
 * @date 2019-11-07
 * 
 * @copyright Copyright (c) 2019
 * 
 */

#ifndef _EZ_IOT_ERRNO_H_
#define _EZ_IOT_ERRNO_H_

typedef enum ez_err
{
    ez_errno_succ,                                              ///< success
    ez_errno_not_init               = ez_errno_succ + 0x01,     ///< The sdk core module is not initialized
    ez_errno_not_ready              = ez_errno_succ + 0x02,     ///< The sdk core module is not started
    ez_errno_param_invalid          = ez_errno_succ + 0x03,     ///< The input parameters is illegal, it may be that some parameters can not be null or out of range
    ez_errno_internal               = ez_errno_succ + 0x04,     ///< Unknown error
    ez_errno_memory                 = ez_errno_succ + 0x05,     ///< Out of memory

    ez_errno_ota_base               = 0x00020000,               ///< Tsl interface error code base
    ez_errno_ota_not_init           = ez_errno_ota_base + 0x01, ///< The ota module is not initialized
    ez_errno_ota_not_ready          = ez_errno_ota_base + 0x02, ///< The sdk core module is not started
    ez_errno_ota_param_invalid      = ez_errno_ota_base + 0x03, ///< The input parameters is illegal, it may be that some parameters can not be null or out of range
    ez_errno_ota_internal           = ez_errno_ota_base + 0x04, ///< Unknown error
    ez_errno_ota_memory             = ez_errno_ota_base + 0x05, ///< Out of memory
    ez_errno_ota_register_failed    = ez_errno_ota_base + 0x06, ///< register_failed 
    ez_errno_ota_json_creat_err     = ez_errno_ota_base + 0x07, ///< json_creat_err
    ez_errno_ota_json_format_err    = ez_errno_ota_base + 0x08, ///< json_format_err 
    ez_errno_ota_msg_send_err       = ez_errno_ota_base + 0x09, ///< msg_send_err
    ez_errno_ota_time_out           = ez_errno_ota_base + 0xA,  ///< time_out
    ez_errno_ota_device_offline     = ez_errno_ota_base + 0xB,  ///< device_offline 
    ez_errno_ota_report_frequency   = ez_errno_ota_base + 0xC,  ///< process report frequency
    ez_errno_ota_download_already   = ez_errno_ota_base + 0xD,  ///< download already 

    ez_errno_hub_base               = 0x00030000,               ///< hub interface error code base
    ez_errno_hub_init               = ez_errno_hub_base + 0x01, ///< The hub module is not initialized
    ez_errno_hub_not_ready          = ez_errno_hub_base + 0x02, ///< The sdk core module is not started
    ez_errno_hub_param_invalid      = ez_errno_hub_base + 0x03, ///< The input parameters is illegal, it may be that some parameters can not be null or out of range
    ez_errno_hub_internal           = ez_errno_hub_base + 0x04, ///< Unknown error
    ez_errno_hub_storage            = ez_errno_hub_base + 0x05, ///< flash operation has failed.
    ez_errno_hub_subdev_not_found   = ez_errno_hub_base + 0x06, ///< Can not found subdev
    ez_errno_hub_enum_end           = ez_errno_hub_base + 0x07, ///< End of enumeration, no more data

    ez_errno_tsl_base               = 0x00040000,               ///< Tsl interface error code base
    ez_errno_tsl_not_init           = ez_errno_tsl_base + 0x01, ///< The tsl module is not initialized
    ez_errno_tsl_not_ready          = ez_errno_tsl_base + 0x02, ///< The sdk core module is not started or the profile not loaded
    ez_errno_tsl_param_invalid      = ez_errno_tsl_base + 0x03, ///< The input parameters is illegal, it may be that some parameters can not be null or out of range
    ez_errno_tsl_internal           = ez_errno_tsl_base + 0x04, ///< Unknown error
    ez_errno_tsl_memory             = ez_errno_tsl_base + 0x05, ///< Out of memory
    ez_errno_tsl_rsctype_not_found  = ez_errno_tsl_base + 0x06, ///< The rsc_type is illegal, is not defined in the profile
    ez_errno_tsl_index_not_found    = ez_errno_tsl_base + 0x07, ///< The local index is illegal, is not defined in the profile
    ez_errno_tsl_domain_not_found   = ez_errno_tsl_base + 0x08, ///< The domain is illegal, is not defined in the profile
    ez_errno_tsl_key_not_found      = ez_errno_tsl_base + 0x09, ///< The Key is illegal, is not defined in the profile
    ez_errno_tsl_value_type         = ez_errno_tsl_base + 0x0a, ///< The type of the value does not match the definition
    ez_errno_tsl_value_illegal      = ez_errno_tsl_base + 0x0b, ///< The value out of the defined range
} ez_err_e;

#endif