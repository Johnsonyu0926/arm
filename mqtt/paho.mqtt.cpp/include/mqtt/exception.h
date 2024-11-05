/// @file exception.h

#ifndef __mqtt_exception_h
#define __mqtt_exception_h

#include "MQTTAsync.h"
#include "mqtt/types.h"
#include <iostream>
#include <vector>
#include <memory>
#include <exception>
#include <stdexcept>

namespace mqtt {

/** Bring std::bad_cast into the mqtt namespace */
using bad_cast = std::bad_cast;

/////////////////////////////////////////////////////////////////////////////

/**
 * Base mqtt::exception.
 * This wraps the error codes which originate from the underlying C library.
 */
class exception : public std::runtime_error
{
protected:
	/** The error return code from the C library */
	int rc_;
	/** The reason code from the server */
	ReasonCode reasonCode_;
	/** The error message from the C library */
	string msg_;

public:
	/** Creates an MQTT exception. */
	explicit exception(int rc)
		: exception(rc, error_str(rc)) {}
	/** Creates an MQTT exception. */
	explicit exception(int rc, ReasonCode reasonCode)
		: exception(rc, reasonCode, error_str(rc)) {}
	/** Creates an MQTT exception. */
	exception(int rc, const string& msg)
		: std::runtime_error(printable_error(rc, ReasonCode::SUCCESS, msg)),
			rc_(rc), reasonCode_(ReasonCode::SUCCESS), msg_(msg) {}
	/** Creates an MQTT exception. */
	exception(int rc, ReasonCode reasonCode, const string& msg)
		: std::runtime_error(printable_error(rc, reasonCode, msg)),
			rc_(rc), reasonCode_(reasonCode), msg_(msg) {}
	/** Gets an error message from an error code. */
	static string error_str(int rc) {
		const char *msg = ::MQTTAsync_strerror(rc);
		return msg ? string(msg) : string();
	}
	/** Gets a string describing the MQTT v5 reason code. */
	static string reason_code_str(int reasonCode) {
		if (reasonCode != MQTTPP_V3_CODE) {
			auto msg = ::MQTTReasonCode_toString(MQTTReasonCodes(reasonCode));
			if (msg) return string(msg);
		}
		return string();
	}
	/** Gets a detailed error message for an error code. */
	static string printable_error(int rc, int reasonCode=ReasonCode::SUCCESS,
								  const string& msg=string()) {
		string s = "MQTT error [" + std::to_string(rc) + "]";
		if (!msg.empty())
			s += string(": ") + msg;
		if (reasonCode != MQTTPP_V3_CODE && reasonCode != ReasonCode::SUCCESS)
			s += string(". Reason: ") + reason_code_str(reasonCode);
		return s;
	}
	/** Returns the return code for this exception. */
	int get_return_code() const { return rc_; }
	/** Gets a string of the error code. */
	string get_error_str() const { return error_str(rc_); }
	/** Returns the reason code for this exception. */
	int get_reason_code() const {
		return reasonCode_ == MQTTPP_V3_CODE ? rc_ : reasonCode_;
	}
	/** Gets a string for the reason code. */
	string get_reason_code_str() const {
		return reason_code_str(reasonCode_);
	}
	/** Returns the error message for this exception. */
	string get_message() const { return msg_; }
	/** Gets a string representation of this exception. */
	string to_string() const { return string(what()); }
};

/** Stream inserter writes a fairly verbose message */
inline std::ostream& operator<<(std::ostream& os, const exception& exc) {
	os << exc.what();
	return os;
}

/////////////////////////////////////////////////////////////////////////////

/** Exception thrown when an expected server response is missing. */
class missing_response : public exception
{
public:
	/** Create a missing response error. */
	missing_response(const string& rsp)
		: exception(MQTTASYNC_FAILURE, "Missing "+rsp+" response") {}
};

/////////////////////////////////////////////////////////////////////////////

/** A timeout exception, particularly from the synchronous client. */
class timeout_error : public exception
{
public:
	/** Create a timeout error. */
	timeout_error() : exception(MQTTASYNC_FAILURE, "Timeout") {}
};

/////////////////////////////////////////////////////////////////////////////

/** This exception is thrown by the implementor of the persistence interface if there is a problem reading or writing persistent data. */
class persistence_exception : public exception
{
public:
	/** Creates an MQTT persistence exception. */
	persistence_exception() : exception(MQTTCLIENT_PERSISTENCE_ERROR) {}
	/** Creates an MQTT persistence exception. */
	explicit persistence_exception(int code) : exception(code) {}
	/** Creates an MQTT persistence exception. */
	explicit persistence_exception(const string& msg)
				: exception(MQTTCLIENT_PERSISTENCE_ERROR, msg) {}
	/** Creates an MQTT persistence exception. */
	persistence_exception(int code, const string& msg)
				: exception(code, msg) {}
};

/////////////////////////////////////////////////////////////////////////////

/** Thrown when a client is not authorized to perform an operation, or if there is a problem with the security configuration. */
class security_exception : public exception
{
public:
	/** Creates an MQTT security exception */
	explicit security_exception(int code) : exception(code) {}
	/** Creates an MQTT security exception */
	security_exception(int code, const string& msg) : exception(code, msg) {}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_exception_h

// By GST @Date