/// @file connect_options.h

#ifndef __mqtt_connect_options_h
#define __mqtt_connect_options_h

#include "MQTTAsync.h"
#include "mqtt/types.h"
#include "mqtt/message.h"
#include "mqtt/topic.h"
#include "mqtt/token.h"
#include "mqtt/string_collection.h"
#include "mqtt/will_options.h"
#include "mqtt/ssl_options.h"
#include "mqtt/platform.h"
#include <vector>
#include <map>
#include <chrono>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Holds the set of options that control how the client connects to a
 * server.
 */
class connect_options
{
	/** The default C struct for non-WebSocket connections */
	PAHO_MQTTPP_EXPORT static const MQTTAsync_connectOptions DFLT_C_STRUCT;

	/** The default C struct for non-Websocket MQTT v5 connections */
	PAHO_MQTTPP_EXPORT static const MQTTAsync_connectOptions DFLT_C_STRUCT5;

	/** The default C struct for WebSocket connections */
	PAHO_MQTTPP_EXPORT static const MQTTAsync_connectOptions DFLT_C_STRUCT_WS;

	/** The default C struct for Websocket MQTT v5 connections */
	PAHO_MQTTPP_EXPORT static const MQTTAsync_connectOptions DFLT_C_STRUCT5_WS;

	/** The underlying C connection options */
	MQTTAsync_connectOptions opts_;

	/** The LWT options */
	will_options will_;

	/** The SSL options  */
	ssl_options ssl_;

	/** The user name to use for the connection. */
	string_ref userName_;

	/** The password to use for the connection. */
	binary_ref password_;

	/** Shared token pointer for context, if any */
	token_ptr tok_;

	/** Collection of server URIs, if any */
	const_string_collection_ptr serverURIs_;

	/** The connect properties */
	properties props_;

	/** HTTP Headers */
	name_value_collection httpHeaders_;

	/** HTTP proxy for websockets */
	string httpProxy_;

	/** Secure HTTPS proxy for websockets */
	string httpsProxy_;

	/** The client has special access */
	friend class async_client;

	/** Gets a pointer to the C-language NUL-terminated strings for the struct. */
	const char* c_str(const string_ref& sr) {
		return sr.empty() ? nullptr : sr.c_str();
	}
	const char* c_str(const string& s) {
		return s.empty() ? nullptr : s.c_str();
	}
	/** Updates the underlying C structure to match our strings. */
	void update_c_struct();

	/** Creates the options from a C option struct. */
	connect_options(const MQTTAsync_connectOptions& copts) : opts_(copts) {}

public:
	/** Smart/shared pointer to an object of this class. */
	using ptr_t = std::shared_ptr<connect_options>;
	/** Smart/shared pointer to a const object of this class. */
	using const_ptr_t = std::shared_ptr<const connect_options>;

	/** Constructs a new object using the default values. */
	explicit connect_options(int ver=MQTTVERSION_DEFAULT);
	/** Constructs a new object using the specified user name and password. */
	connect_options(
		string_ref userName, binary_ref password,
		int ver=MQTTVERSION_DEFAULT
    );
	/** Copy constructor. */
	connect_options(const connect_options& opt);
	/** Move constructor. */
	connect_options(connect_options&& opt);
	/** Creates default options for an MQTT v3.x connection. */
	static connect_options v3();
	/** Creates default options for an MQTT v5 connection. */
	static connect_options v5();
	/** Creates default options for an MQTT v3.x connection using WebSockets. */
	static connect_options ws() {
		return connect_options(DFLT_C_STRUCT_WS);
	}
	/** Creates default options for an MQTT v5 connection using WebSockets. */
	static connect_options v5_ws() {
		return connect_options(DFLT_C_STRUCT5_WS);
	}
	/** Copy assignment. */
	connect_options& operator=(const connect_options& opt);
	/** Move assignment. */
	connect_options& operator=(connect_options&& opt);
	/** Expose the underlying C struct for the unit tests. */
	#if defined(UNIT_TESTS)
		const MQTTAsync_connectOptions& c_struct() const { return opts_; }
	#endif
	/** Gets the "keep alive" interval. */
	std::chrono::seconds get_keep_alive_interval() const {
		return std::chrono::seconds(opts_.keepAliveInterval);
	}
	/** Gets the connection timeout. */
	std::chrono::seconds get_connect_timeout() const {
		return std::chrono::seconds(opts_.connectTimeout);
	}
	/** Gets the user name to use for the connection. */
	string get_user_name() const { return userName_ ? userName_.to_string() : string(); }
	/** Gets the password to use for the connection. */
	binary_ref get_password() const { return password_; }
	/** Gets the password to use for the connection. */
	string get_password_str() const {
		return password_ ? password_.to_string() : string();
	}
	/** Gets the maximum number of messages that can be in-flight simultaneously. */
	int get_max_inflight() const { return opts_.maxInflight; }
	/** Gets the topic to be used for last will and testament (LWT). */
	string get_will_topic() const {
		return will_.get_topic();
	}
	/** Gets the message to be sent as last will and testament (LWT). */
	const_message_ptr get_will_message() const {
		return will_.get_message();
	}
	/** Get the LWT options to use for the connection. */
	const will_options& get_will_options() const { return will_; }
	/** Get the SSL options to use for the connection. */
	const ssl_options& get_ssl_options() const { return ssl_; }
	/** Sets the SSL for the connection. */
	void set_ssl(const ssl_options& ssl);
	/** Sets the SSL for the connection. */
	void set_ssl(ssl_options&& ssl);
	/** Returns whether the server should remember state for the client across reconnects. */
	bool is_clean_session() const { return to_bool(opts_.cleansession); }
	/** Returns whether the server should remember state for the client across reconnects. */
	bool is_clean_start() const { return to_bool(opts_.cleanstart); }
	/** Gets the token used as the callback context. */
	token_ptr get_token() const { return tok_; }
	/** Gets the list of servers to which the client will connect. */
	const_string_collection_ptr get_servers() const { return serverURIs_; }
	/** Gets the version of MQTT to be used on the connect. */
	int get_mqtt_version() const { return opts_.MQTTVersion; }
	/** Determines if the options have been configured for automatic reconnect. */
	bool get_automatic_reconnect() const { return to_bool(opts_.automaticReconnect); }
	/** Gets the minimum retry interval for automatic reconnect. */
	std::chrono::seconds get_min_retry_interval() const {
		return std::chrono::seconds(opts_.minRetryInterval);
	}
	/** Gets the maximum retry interval for automatic reconnect. */
	std::chrono::seconds get_max_retry_interval() const {
		return std::chrono::seconds(opts_.maxRetryInterval);
	}
	/** Sets whether the server should remember state for the client across reconnects. */
	void set_clean_session(bool cleanSession);
	/** Sets whether the server should remember state for the client across reconnects. */
	void set_clean_start(bool cleanStart);
	/** Sets the "keep alive" interval. */
	void set_keep_alive_interval(int keepAliveInterval) {
		opts_.keepAliveInterval = keepAliveInterval;
	}
	/** Sets the "keep alive" interval with a chrono duration. */
	template <class Rep, class Period>
	void set_keep_alive_interval(const std::chrono::duration<Rep, Period>& interval) {
		set_keep_alive_interval((int) to_seconds_count(interval));
	}
	/** Sets the connect timeout in seconds. */
	void set_connect_timeout(int timeout) {
		opts_.connectTimeout = timeout;
	}
	/** Sets the connect timeout with a chrono duration. */
	template <class Rep, class Period>
	void set_connect_timeout(const std::chrono::duration<Rep, Period>& timeout) {
		set_connect_timeout((int) to_seconds_count(timeout));
	}
	/** Sets the user name to use for the connection. */
	void set_user_name(string_ref userName);
	/** Sets the password to use for the connection. */
	void set_password(binary_ref password);
	/** Sets the maximum number of messages that can be in-flight simultaneously. */
	void set_max_inflight(int n) { opts_.maxInflight = n; }
	/** Sets the "Last Will and Testament" (LWT) for the connection. */
	void set_will(const will_options& will);
	/** Sets the "Last Will and Testament" (LWT) for the connection. */
	void set_will(will_options&& will);
	/** Sets the "Last Will and Testament" (LWT) as a message. */
	void set_will_message(const message& msg) {
		set_will(will_options(msg));
	}
	/** Sets the "Last Will and Testament" (LWT) as a message. */
	void set_will_message(const_message_ptr msg) {
		if (msg) set_will(will_options(*msg));
	}
	/** Sets the callback context to a delivery token. */
	void set_token(const token_ptr& tok);
	/** Sets the list of servers to which the client will connect. */
	void set_servers(const_string_collection_ptr serverURIs);
	/** Sets the version of MQTT to be used on the connect. */
	void set_mqtt_version(int mqttVersion);
	/** Enable or disable automatic reconnects. */
	void set_automatic_reconnect(bool on) {
		opts_.automaticReconnect = to_int(on);
	}
	/** Enable or disable automatic reconnects. */
	void set_automatic_reconnect(int minRetryInterval, int maxRetryInterval);
	/** Enable or disable automatic reconnects. */
	template <class Rep1, class Period1, class Rep2, class Period2>
	void set_automatic_reconnect(const std::chrono::duration<Rep1, Period1>& minRetryInterval,
								 const std::chrono::duration<Rep2, Period2>& maxRetryInterval) {
		set_automatic_reconnect((int) to_seconds_count(minRetryInterval),
								(int) to_seconds_count(maxRetryInterval));
	}
	/** Gets the connect properties. */
	const properties& get_properties() const { return props_; }
	/** Gets a mutable reference to the connect properties. */
	properties& get_properties() { return props_; }
	/** Sets the properties for the connect. */
	void set_properties(const properties& props);
	/** Moves the properties for the connect. */
	void set_properties(properties&& props);
	/** Gets the HTTP headers. */
	const name_value_collection& get_http_headers() const {
		return httpHeaders_;
	}
	/** Sets the HTTP headers for the connection. */
	void set_http_headers(const name_value_collection& httpHeaders) {
		httpHeaders_ = httpHeaders;
		opts_.httpHeaders = httpHeaders_.empty() ? nullptr : httpHeaders_.c_arr();
	}
	/** Sets the HTTP headers for the connection. */
	void set_http_headers(name_value_collection&& httpHeaders) {
		httpHeaders_ = std::move(httpHeaders);
		opts_.httpHeaders = httpHeaders_.empty() ? nullptr : httpHeaders_.c_arr();
	}
	/** Gets the HTTP proxy setting. */
	string get_http_proxy() const { return httpProxy_; }
	/** Sets the HTTP proxy setting. */
	void set_http_proxy(const string& httpProxy);
	/** Gets the secure HTTPS proxy setting. */
	string get_https_proxy() const { return httpsProxy_; }
	/** Sets the secure HTTPS proxy setting. */
	void set_https_proxy(const string& httpsProxy);
};

/** Smart/shared pointer to a connection options object. */
using connect_options_ptr = connect_options::ptr_t;

/////////////////////////////////////////////////////////////////////////////

/**
 * The connect options that can be updated before an automatic reconnect.
 */
class connect_data
{
	/** The default C struct */
	PAHO_MQTTPP_EXPORT static const MQTTAsync_connectData DFLT_C_STRUCT;

	/** The underlying C connect data  */
	MQTTAsync_connectData data_;

	/** The user name to use for the connection. */
	string_ref userName_;

	/** The password to use for the connection. (Optional) */
	binary_ref password_;

	/** The client has special access */
	friend class async_client;

	/** Updates the underlying C structure to match our strings. */
	void update_c_struct();

	/** Create data from a C struct. */
	connect_data(const MQTTAsync_connectData& cdata);

public:
	/** Creates an empty set of connection data. */
	connect_data();
	/** Creates connection data with a user name, but no password. */
	explicit connect_data(string_ref userName);
	/** Creates connection data with a user name and password. */
	connect_data(string_ref userName, binary_ref password);
	/** Copy constructor. */
	connect_data(const connect_data& other);
	/** Copy the connection data. */
	connect_data& operator=(const connect_data& rhs);
	/** Gets the user name to use for the connection. */
	string get_user_name() const { return userName_ ? userName_.to_string() : string(); }
	/** Gets the password to use for the connection. */
	binary_ref get_password() const { return password_; }
	/** Sets the user name to use for the connection. */
	void set_user_name(string_ref userName);
	/** Sets the password to use for the connection. */
	void set_password(binary_ref password);
};

/////////////////////////////////////////////////////////////////////////////

/**
 * Class to build connect options.
 */
class connect_options_builder
{
	connect_options opts_;

public:
	/** This class */
	using self = connect_options_builder;
	/** Default constructor. */
	explicit connect_options_builder(int ver=MQTTVERSION_DEFAULT) : opts_(ver) {}
	/** Copy constructor from an existing set of options. */
	explicit connect_options_builder(const connect_options& opts) : opts_(opts) {}
	/** Move constructor from an existing set of options. */
	explicit connect_options_builder(const connect_options&& opts) : opts_(std::move(opts)) {}
	/** Creates the default options builder for an MQTT v3.x connection. */
	static connect_options_builder v3() {
		return connect_options_builder{ connect_options::v3() };
	}
	/** Creates the default options builder for an MQTT v5 connection. */
	static connect_options_builder v5() {
		return connect_options_builder{ connect_options::v5() };
	}
	/** Creates the default options builder for an MQTT v3.x connection using WebSockets. */
	static connect_options_builder ws() {
		return connect_options_builder{ connect_options::ws() };
	}
	/** Creates the default options builder for an MQTT v5 connection using WebSockets. */
	static connect_options_builder v5_ws() {
		return connect_options_builder{ connect_options::v5_ws() };
	}
	/** Sets whether the server should remember state for the client across reconnects. */
	auto clean_session(bool on=true) -> self& {
		opts_.set_clean_session(on);
		return *this;
	}
	/** Sets the "keep alive" interval with a chrono duration. */
	template <class Rep, class Period>
	auto keep_alive_interval(const std::chrono::duration<Rep, Period>& interval) -> self& {
		opts_.set_keep_alive_interval(interval);
		return *this;
	}
	/** Sets the connect timeout with a chrono duration. */
	template <class Rep, class Period>
	auto connect_timeout(const std::chrono::duration<Rep, Period>& timeout) -> self& {
		opts_.set_connect_timeout(timeout);
		return *this;
	}
	/** Sets the user name to use for the connection. */
	auto user_name(string_ref userName) -> self& {
		opts_.set_user_name(userName);
		return *this;
	}
	/** Sets the password to use for the connection. */
	auto password(binary_ref password) -> self& {
		opts_.set_password(password);
		return *this;
	}
	/** Sets the maximum number of messages that can be in-flight simultaneously. */
	auto max_inflight(int n) -> self& {
		opts_.set_max_inflight(n);
		return *this;
	}
	/** Sets the "Last Will and Testament" (LWT) for the connection. */
	auto will(const will_options& will) -> self& {
		opts_.set_will(will);
		return *this;
	}
	/** Sets the "Last Will and Testament" (LWT) for the connection. */
	auto will(will_options&& will) -> self& {
		opts_.set_will(std::move(will));
		return *this;
	}
	/** Sets the "Last Will and Testament" (LWT) as a message. */
	auto will(const message& msg) -> self& {
		opts_.set_will_message(msg);
		return *this;
	}
	/** Sets the SSL options for the connection. */
	auto ssl(const ssl_options& ssl) -> self& {
		opts_.set_ssl(ssl);
		return *this;
	}
	/** Sets the SSL options for the connection. */
	auto ssl(ssl_options&& ssl) -> self& {
		opts_.set_ssl(std::move(ssl));
		return *this;
	}
	/** Sets the callback context to a delivery token. */
	auto token(const token_ptr& tok) -> self& {
		opts_.set_token(tok);
		return *this;
	}
	/** Sets the list of servers to which the client will connect. */
	auto servers(const_string_collection_ptr serverURIs) -> self& {
		opts_.set_servers(serverURIs);
		return *this;
	}
	/** Sets the version of MQTT to be used on the connect. */
	auto mqtt_version(int ver) -> self& {
		opts_.set_mqtt_version(ver);
		return *this;
	}
	/** Enable or disable automatic reconnects. */
	auto automatic_reconnect(bool on=true) -> self& {
		opts_.set_automatic_reconnect(on);
		return *this;
	}
	/** Enable or disable automatic reconnects. */
	template <class Rep1, class Period1, class Rep2, class Period2>
	auto automatic_reconnect(const std::chrono::duration<Rep1, Period1>& minRetryInterval,
							 const std::chrono::duration<Rep2, Period2>& maxRetryInterval) -> self& {
		opts_.set_automatic_reconnect(minRetryInterval, maxRetryInterval);
		return *this;
	}
	/** Sets the 'clean start' flag for the connection. */
	auto clean_start(bool on=true) -> self& {
		opts_.set_clean_start(on);
		return *this;
	}
	/** Sets the properties for the connect message. */
	auto properties(const mqtt::properties& props) -> self& {
		opts_.set_properties(props);
		return *this;
	}
	/** Sets the properties for the connect message. */
	auto properties(mqtt::properties&& props) -> self& {
		opts_.set_properties(std::move(props));
		return *this;
	}
	/** Sets the HTTP headers for the connection. */
	auto http_headers(const name_value_collection& headers) -> self& {
		opts_.set_http_headers(headers);
		return *this;
	}
	/** Sets the HTTP headers for the connection. */
	auto http_headers(name_value_collection&& headers) -> self& {
		opts_.set_http_headers(std::move(headers));
		return *this;
	}
	/** Sets the HTTP proxy setting. */
	auto http_proxy(const string& httpProxy) -> self& {
		opts_.set_http_proxy(httpProxy);
		return *this;
	}

	/** Sets the secure HTTPS proxy setting. */
	auto https_proxy(const string& httpsProxy) -> self& {
		opts_.set_https_proxy(httpsProxy);
		return *this;
	}
	/** Finish building the options and return them. */
	connect_options finalize() { return opts_; }
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_connect_options_h

// By GST @Date