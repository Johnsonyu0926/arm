/// @file disconnect_options.h

#ifndef __mqtt_disconnect_options_h
#define __mqtt_disconnect_options_h

#include "MQTTAsync.h"
#include "mqtt/types.h"
#include "mqtt/token.h"
#include "mqtt/properties.h"
#include <chrono>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Options for disconnecting from an MQTT broker.
 */
class disconnect_options
{
	/** The default C struct */
	static const MQTTAsync_disconnectOptions DFLT_C_STRUCT;

	/** The default C struct */
	static const MQTTAsync_disconnectOptions DFLT_C_STRUCT5;

	/** The underlying C disconnect options */
	MQTTAsync_disconnectOptions opts_;

	/** Shared token pointer for context, if any */
	token_ptr tok_;

	/** Disconnect message properties */
	properties props_;

	/** The client has special access */
	friend class async_client;

	/** The options builder has special access */
	friend class disconnect_options_builder;

	/** Updates the underlying C structure to match our cached data. */
	void update_c_struct();

	/** Construct options from a C struct */
	disconnect_options(const MQTTAsync_disconnectOptions& copts) : opts_{copts} {}

public:
	/** Create an empty delivery response object. */
	disconnect_options();
	/** Creates disconnect options tied to the specific token. */
	disconnect_options(int timeout) : disconnect_options() {
		set_timeout(timeout);
	}
	/** Creates disconnect options tied to the specific token. */
	template <class Rep, class Period>
	disconnect_options(const std::chrono::duration<Rep, Period>& to)
			: disconnect_options() {
		set_timeout(to);
	}
	/** Copy constructor. */
	disconnect_options(const disconnect_options& opt);
	/** Move constructor. */
	disconnect_options(disconnect_options&& opt);
	/** Creates default options for an MQTT v3.x connection. */
	static disconnect_options v3();
	/** Creates default options for an MQTT v5 connection. */
	static disconnect_options v5();
	/** Copy assignment. */
	disconnect_options& operator=(const disconnect_options& opt);
	/** Move assignment. */
	disconnect_options& operator=(disconnect_options&& opt);
	/** Expose the underlying C struct for the unit tests. */
	#if defined(UNIT_TESTS)
		const MQTTAsync_disconnectOptions& c_struct() const { return opts_; }
	#endif
	/** Gets the timeout used for disconnecting. */
	std::chrono::milliseconds get_timeout() const {
		return std::chrono::milliseconds(opts_.timeout);
	}
	/** Sets the disconnect timeout, in milliseconds. */
	void set_timeout(int timeout) { opts_.timeout = timeout; }
	/** Sets the disconnect timeout with a duration. */
	template <class Rep, class Period>
	void set_timeout(const std::chrono::duration<Rep, Period>& to) {
		// TODO: check range
		set_timeout((int) to_milliseconds_count(to));
	}
	/** Sets the callback context to a delivery token. */
	void set_token(const token_ptr& tok, int mqttVersion);
	/** Gets the callback context to a delivery token. */
	token_ptr get_token() const { return tok_; }
	/** Gets the disconnect properties. */
	const properties& get_properties() const { return props_; }
	/** Gets a mutable reference to the disconnect properties. */
	properties& get_properties() { return props_; }
	/** Sets the properties for the connect. */
	void set_properties(const properties& props) {
		props_ = props;
		opts_.properties = props_.c_struct();
	}
	/** Moves the properties for the connect. */
	void set_properties(properties&& props) {
		props_ = std::move(props);
		opts_.properties = props_.c_struct();
	}
	/** Gets the reason code for the disconnect. */
	ReasonCode get_reason_code() const {
		return ReasonCode(opts_.reasonCode);
	}
	/** Sets the reason code for the disconnect. */
	void set_reason_code(ReasonCode code) {
		opts_.reasonCode = MQTTReasonCodes(code);
	}
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Class to build connect options.
 */
class disconnect_options_builder
{
	/** The underlying options */
	disconnect_options opts_;

	/** Construct options builder from a C struct */
	disconnect_options_builder(const MQTTAsync_disconnectOptions& copts) : opts_{copts} {}

public:
	/** This class */
	using self = disconnect_options_builder;
	/** Default constructor. */
	disconnect_options_builder() {}
	/** Creates default options builder for an MQTT v3.x connection. */
	static disconnect_options_builder v3();
	/** Creates default options builder for an MQTT v5 connection. */
	static disconnect_options_builder v5();
	/** Sets the properties for the disconnect message. */
	auto properties(mqtt::properties&& props) -> self& {
		opts_.set_properties(std::move(props));
		return *this;
	}
	/** Sets the properties for the disconnect message. */
	auto properties(const mqtt::properties& props) -> self& {
		opts_.set_properties(props);
		return *this;
	}
	/** Sets the disconnect connect timeout. */
	template <class Rep, class Period>
	auto timeout(const std::chrono::duration<Rep, Period>& to) -> self&{
		opts_.set_timeout(to);
		return *this;
	}
	/** Sets the reason code for the disconnect. */
	auto reason_code(ReasonCode code) -> self& {
		opts_.set_reason_code(code);
		return *this;
	}
	/** Finish building the options and return them. */
	disconnect_options finalize() { return opts_; }
};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_disconnect_options_h

// By GST @Date