/////////////////////////////////////////////////////////////////////////////
/// @file export.h
/// Library symbol export definitions, primarily for Windows MSVC DLL's

#ifndef __mqtt_export_h
#define __mqtt_export_h

#if defined(_WIN32) && defined(_MSC_VER)
	#if defined(PAHO_MQTTPP_EXPORTS)
		#define PAHO_MQTTPP_EXPORT __declspec(dllexport)
	#elif defined(PAHO_MQTTPP_IMPORTS)
		#define PAHO_MQTTPP_EXPORT __declspec(dllimport)
	#else
		#define PAHO_MQTTPP_EXPORT
	#endif
#else
	#if defined(PAHO_MQTTPP_EXPORTS)
		#define PAHO_MQTTPP_EXPORT  __attribute__ ((visibility ("default")))
	#else
		#define PAHO_MQTTPP_EXPORT
	#endif
#endif

#endif    // __mqtt_export_h

// By GST @Date

