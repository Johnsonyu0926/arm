/////////////////////////////////////////////////////////////////////////////
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

	/**
	 * Gets a pointer to the C-language NUL-terminated strings for the
	 * struct.
	 * @note In the connect options, by default, the Paho C treats
	 * nullptr char arrays as unset values, so we keep that semantic and
	 * only set those char arrays if the string is non-empty.
	 * @param str The C++ string object.
	 * @return Pointer to a NUL terminated string. This is only valid until
	 *  	   the next time the string is updated.
	 */
	const char* c_str(const string_ref& sr) {
		return sr.empty() ? nullptr : sr.c_str();
	}
	const char* c_str(const string& s) {
		return s.empty() ? nullptr : s.c_str();
	}
	/**
	 * Updates the underlying C structure to match our strings.
	 */
	void update_c_struct();

	/**
	 * Creates the options from a C option struct.
	 * @param copts The C options struct.
	 */
	connect_options(const MQTTAsync_connectOptions& copts) : opts_(copts) {}

public:
	/** Smart/shared pointer to an object of this class. */
	using ptr_t = std::shared_ptr<connect_options>;
	/** Smart/shared pointer to a const object of this class. */
	using const_ptr_t = std::shared_ptr<const connect_options>;

	/**
	 * Constructs a new object using the default values.
	 *
	 * @param ver The MQTT protocol version.
	 */
	explicit connect_options(int ver=MQTTVERSION_DEFAULT);
	/**
	 * Constructs a new object using the specified user name and password.
	 * @param userName The name of the user for connecting to the server
	 * @param password The password for connecting to the server
	 * @param ver The MQTT protocol version.
	 */
	connect_options(
		string_ref userName, binary_ref password,
		int ver=MQTTVERSION_DEFAULT
    );
	/**
	 * Copy constructor.
	 * @param opt Another object to copy.
	 */
	connect_options(const connect_options& opt);
	/**
	 * Move constructor.
	 * @param opt Another object to move into this new one.
	 */
	connect_options(connect_options&& opt);
	/**
	 * Creates default options for an MQTT v3.x connection.
	 * @return Default options for an MQTT v3.x connection.
	 */
	static connect_options v3();
	/**
	 * Creates default options for an MQTT v5 connection.
	 * @return Default options for an MQTT v5 connection.
	 */
	static connect_options v5();
	/**
	 * Creates default options for an MQTT v3.x connection using WebSockets.
	 *
	 * The keepalive interval is set to 45 seconds to avoid webserver 60
	 * second inactivity timeouts.
	 *
	 * @return Default options for an MQTT v3.x connection using websockets.
	 */
	static connect_options ws() {
		return connect_options(DFLT_C_STRUCT_WS);
	}
	/**
	 * Creates default options for an MQTT v5 connection using WebSockets.
	 *
	 * The keepalive interval is set to 45 seconds to avoid webserver 60
	 * second inactivity timeouts.
	 *
	 * @return Default options for an MQTT v5 connection using websockets.
	 */
	static connect_options v5_ws() {
		return connect_options(DFLT_C_STRUCT5_WS);
	}
	/**
	 * Copy assignment.
	 * @param opt Another object to copy.
	 */
	connect_options& operator=(const connect_options& opt);
	/**
	 * Move assignment.
	 * @param opt Another object to move into this new one.
	 */
	connect_options& operator=(connect_options&& opt);
	/**
	 * Expose the underlying C struct for the unit tests.
	 */
	#if defined(UNIT_TESTS)
		const MQTTAsync_connectOptions& c_struct() const { return opts_; }
	#endif
	/**
	 * Gets the "keep alive" interval.
	 * @return The keep alive interval in seconds.
	 */
	std::chrono::seconds get_keep_alive_interval() const {
		return std::chrono::seconds(opts_.keepAliveInterval);
	}
	/**
	 * Gets the connection timeout.
	 * This is the amount of time the underlying library will wait for a
	 * timeout before failing.
	 * @return The connect timeout in seconds.
	 */
	std::chrono::seconds get_connect_timeout() const {
		return std::chrono::seconds(opts_.connectTimeout);
	}
	/**
	 * Gets the user name to use for the connection.
	 * @return The user name to use for the connection.
	 */
	string get_user_name() const { return userName_ ? userName_.to_string() : string(); }
	/**
	 * Gets the password to use for the connection.
	 * @return The password to use for the connection.
	 */
	binary_ref get_password() const { return password_; }
	/**
	 * Gets the password to use for the connection.
	 * @return The password to use for the connection.
	 */
	string get_password_str() const {
		return password_ ? password_.to_string() : string();
	}
	/**
	 * Gets the maximum number of messages that can be in-flight
	 * simultaneously.
	 * @return The maximum number of inflight messages.
	 */
	int get_max_inflight() const { return opts_.maxInflight; }
	/**
	 * Gets the topic to be used for last will and testament (LWT).
	 * @return The topic to be used for last will and testament (LWT).
	 */
	string get_will_topic() const {
		return will_.get_topic();
	}
	/**
	 * Gets the message to be sent as last will and testament (LWT).
	 * @return The message to be sent as last will and testament (LWT).
	 */
	const_message_ptr get_will_message() const {
		return will_.get_message();
	}
	/**
	 * Get the LWT options to use for the connection.
	 * @return The LWT options to use for the connection.
	 */
	const will_options& get_will_options() const { return will_; }
	/**
	 * Get the SSL options to use for the connection.
	 * @return The SSL options to use for the connection.
	 */
	const ssl_options& get_ssl_options() const { return ssl_; }
	/**
	 * Sets the SSL for the connection.
	 * These will only have an effect if compiled against the SSL version of
	 * the Paho C library, and using a secure connection, "ssl://" or
	 * "wss://".
	 * @param ssl The SSL options.
	 */
	void set_ssl(const ssl_options& ssl);
	/**
	 * Sets the SSL for the connection.
	 * These will only have an effect if compiled against the SSL version of
	 * the Paho C library, and using a secure connection, "ssl://" or
	 * "wss://".
	 * @param ssl The SSL options.
	 */
	void set_ssl(ssl_options&& ssl);
	/**
	 * Returns whether the server should remember state for the client
	 * across reconnects. This only applies to MQTT v3.x connections.
	 * @return @em true if requesting a clean session, @em false if not.
	 */
	bool is_clean_session() const { return to_bool(opts_.cleansession); }
	/**
	 * Returns whether the server should remember state for the client
	 * across reconnects. This only applies to MQTT v5 connections.
	 * @return @em true if requesting a clean start, @em false if not.
	 */
	bool is_clean_start() const { return to_bool(opts_.cleanstart); }
	/**
	 * Gets the token used as the callback context.
	 * @return The delivery token used as the callback context.
	 */
	token_ptr get_token() const { return tok_; }
	/**
	 * Gets the list of servers to which the client will connect.
	 * @return A collection of server URI's. Each entry should be of the
	 *  	   form @em protocol://host:port where @em protocol must be tcp
	 *  	   or @em ssl. For @em host, you can specify either an IP
	 *  	   address or a domain name.
	 */
	const_string_collection_ptr get_servers() const { return serverURIs_; }
	/**
      * Gets the version of MQTT to be used on the connect.
	  * @return
	  * @li MQTTVERSION_DEFAULT (0) = default: start with 3.1.1, and if that
	  *     fails, fall back to 3.1
	  * @li MQTTVERSION_3_1 (3) = only try version 3.1
	  * @li MQTTVERSION_3_1_1 (4) = only try version 3.1.1
	  */
	int get_mqtt_version() const { return opts_.MQTTVersion; }
	/**
	 * Determines if the options have been configured for automatic
	 * reconnect.
	 * @return @em true if configured for automatic reconnect, @em false if
	 *  	   not.
	 */
	bool get_automatic_reconnect() const { return to_bool(opts_.automaticReconnect); }
	/**
	 * Gets the minimum retry interval for automatic reconnect.
	 * @return The minimum retry interval for automatic reconnect, in
	 *  	   seconds.
	 */
	std::chrono::seconds get_min_retry_interval() const {
		return std::chrono::seconds(opts_.minRetryInterval);
	}
	/**
	 * Gets the maximum retry interval for automatic reconnect.
	 * @return The maximum retry interval for automatic reconnect, in
	 *  	   seconds.
	 */
	std::chrono::seconds get_max_retry_interval() const {
		return std::chrono::seconds(opts_.maxRetryInterval);
	}
	/**
	 * Sets whether the server should remember state for the client across
	 * reconnects. (MQTT v3.x only)
	 *
	 * This will only take effect if the version is _already_ set to v3.x
	 * (not v5).
	 *
	 * @param clean @em true if the server should NOT remember state for the
	 *  			client across reconnects, @em false otherwise.
	 */
	void set_clean_session(bool cleanSession);
	/**
	 * Sets whether the server should remember state for the client across
	 * reconnects. (MQTT v5 only)
	 *
	 * If a persistent session is desired (turning this off), then the app
	 * should also set the `Session Expiry Interval` property, and add that
	 * to the connect options.
	 *
	 * This will only take effect if the MQTT version is set to v5
	 *
	 * @param clean @em true if the server should NOT remember state for the
	 *  			client across reconnects, @em false otherwise.
	 */
	void set_clean_start(bool cleanStart);
	/**
	 * Sets the "keep alive" interval.
	 * This is the maximum time that should pass without communications
	 * between client and server. If no messages pass in this time, the
	 * client will ping the broker.
	 * @param keepAliveInterval The keep alive interval in seconds.
	 */
	void set_keep_alive_interval(int keepAliveInterval) {
		opts_.keepAliveInterval = keepAliveInterval;
	}
	/**
	 * Sets the "keep alive" interval with a chrono duration.
	 * This is the maximum time that should pass without communications
	 * between client and server. If no messages pass in this time, the
	 * client will ping the broker.
	 * @param interval The keep alive interval.
	 */
	template <class Rep, class Period>
	void set_keep_alive_interval(const std::chrono::duration<Rep, Period>& interval) {
		// TODO: Check range
		set_keep_alive_interval((int) to_seconds_count(interval));
	}
	/**
	 * Sets the connect timeout in seconds.
	 * This is the maximum time that the underlying library will wait for a
	 * connection before failing.
	 * @param timeout The connect timeout in seconds.
	 */
	void set_connect_timeout(int timeout) {
		opts_.connectTimeout = timeout;
	}
	/**
	 * Sets the connect timeout with a chrono duration.
	 * This is the maximum time that the underlying library will wait for a
	 * connection before failing.
	 * @param timeout The connect timeout in seconds.
	 */
	template <class Rep, class Period>
	void set_connect_timeout(const std::chrono::duration<Rep, Period>& timeout) {
		// TODO: check range
		set_connect_timeout((int) to_seconds_count(timeout));
	}
	/**
	 * Sets the user name to use for the connection.
	 * @param userName The user name for connecting to the MQTT broker.
	 */
	void set_user_name(string_ref userName);
	/**
	 * Sets the password to use for the connection.
	 * @param password The password for connecting to the MQTT broker.
	 */
	void set_password(binary_ref password);
	/**
	 * Sets the maximum number of messages that can be in-flight
	 * simultaneously.
	 * @param n The maximum number of inflight messages.
	 */
	void set_max_inflight(int n) { opts_.maxInflight = n; }
	/**
	 * Sets the "Last Will and Testament" (LWT) for the connection.
	 * @param will The LWT options.
	 */
	void set_will(const will_options& will);
	/**
	 * Sets the "Last Will and Testament" (LWT) for the connection.
	 * @param will The LWT options.
	 */
	void set_will(will_options&& will);
	/**
	 * Sets the "Last Will and Testament" (LWT) as a message
	 * @param msg The LWT message
	 */
	void set_will_message(const message& msg) {
		set_will(will_options(msg));
	}
	/**
	 * Sets the "Last Will and Testament" (LWT) as a message
	 * @param msg Pointer to a LWT message
	 */
	void set_will_message(const_message_ptr msg) {
		if (msg) set_will(will_options(*msg));
	}
	/**
	 * Sets the callback context to a delivery token.
	 * @param tok The delivery token to be used as the callback context.
	 */
	void set_token(const token_ptr& tok);
	/**
	 * Sets the list of servers to which the client will connect.
	 * @param serverURIs A pointer to a collection of server URI's. Each
	 *  				 entry should be of the form @em
	 *  				 protocol://host:port where @em protocol must be
	 *  				 @em tcp or @em ssl. For @em host, you can specify
	 *  				 either an IP address or a domain name.
	 */
	void set_servers(const_string_collection_ptr serverURIs);
	/**
	  * Sets the version of MQTT to be used on the connect.
	  *
	  * This will also set other connect options to legal values dependent on
	  * the selected version.
	  *
	  * @param mqttVersion The MQTT version to use for the connection:
	  *   @li MQTTVERSION_DEFAULT (0) = default: start with 3.1.1, and if
	  *       that fails, fall back to 3.1
	  *   @li MQTTVERSION_3_1 (3) = only try version 3.1
	  *   @li MQTTVERSION_3_1_1 (4) = only try version 3.1.1
	  *   @li MQTTVERSION_5 (5) = only try version 5
	  *
	  * @deprecated It is preferable to create the options for the desired
	  * version rather than using this function to change the version after
	  * some parameters have already been set. If you do use this function,
	  * call it before setting any other version-specific options. @sa
	  * connect_options::v5()
	  */
	void set_mqtt_version(int mqttVersion);
	/**
	 * Enable or disable automatic reconnects.
	 * The retry intervals are not affected.
	 * @param on Whether to turn reconnects on or off
	 */
	void set_automatic_reconnect(bool on) {
		opts_.automaticReconnect = to_int(on);
	}
	/**
	 * Enable or disable automatic reconnects.
	 * @param minRetryInterval Minimum retry interval in seconds.  Doubled
	 *  					   on each failed retry.
	 * @param maxRetryInterval Maximum retry interval in seconds.  The
	 *  					   doubling stops here on failed retries.
	 */
	void set_automatic_reconnect(int minRetryInterval, int maxRetryInterval);
	/**
	 * Enable or disable automatic reconnects.
	 * @param minRetryInterval Minimum retry interval. Doubled on each
	 *  					   failed retry.
	 * @param maxRetryInterval Maximum retry interval. The doubling stops
	 *  					   here on failed retries.
	 */
	template <class Rep1, class Period1, class Rep2, class Period2>
	void set_automatic_reconnect(const std::chrono::duration<Rep1, Period1>& minRetryInterval,
								 const std::chrono::duration<Rep2, Period2>& maxRetryInterval) {
		set_automatic_reconnect((int) to_seconds_count(minRetryInterval),
								(int) to_seconds_count(maxRetryInterval));
	}
	/**
	 * Gets the connect properties.
	 * @return A const reference to the properties for the connect.
	 */
	const properties& get_properties() const { return props_; }
	/**
	 * Gets a mutable reference to the connect properties.
	 * @return A reference to the properties for the connect.
	 */
	properties& get_properties() { return props_; }
	/**
	 * Sets the properties for the connect.
	 * @param props The properties to place into the message.
	 */
	void set_properties(const properties& props);
	/**
	 * Moves the properties for the connect.
	 * @param props The properties to move into the connect object.
	 */
	void set_properties(properties&& props);
	/**
	 * Gets the HTTP headers
	 * @return A const reference to the HTTP headers name/value collection.
	 */
	const name_value_collection& get_http_headers() const {
		return httpHeaders_;
	}
	/**
	 * Sets the HTTP headers for the connection.
	 * @param httpHeaders The header nam/value collection.
	 */
	void set_http_headers(const name_value_collection& httpHeaders) {
		httpHeaders_ = httpHeaders;
		opts_.httpHeaders = httpHeaders_.empty() ? nullptr : httpHeaders_.c_arr();
	}
	/**
	 * Sets the HTTP headers for the connection.
	 * @param httpHeaders The header nam/value collection.
	 */
	void set_http_headers(name_value_collection&& httpHeaders) {
		httpHeaders_ = std::move(httpHeaders);
		opts_.httpHeaders = httpHeaders_.empty() ? nullptr : httpHeaders_.c_arr();
	}
	/**
	 * Gets the HTTP proxy setting.
	 * @return The HTTP proxy setting. An empty string means no proxy.
	 */
	string get_http_proxy() const { return httpProxy_; }
	/**
	 * Sets the HTTP proxy setting.
	 * @param httpProxy The HTTP proxy setting. An empty string means no
	 *  			  proxy.
	 */
	void set_http_proxy(const string& httpProxy);
	/**
	 * Gets the secure HTTPS proxy setting.
	 * @return The HTTPS proxy setting. An empty string means no proxy.
	 */
	string get_https_proxy() const { return httpsProxy_; }
	/**
	 * Sets the secure HTTPS proxy setting.
	 * @param httpsProxy The HTTPS proxy setting. An empty string means no
	 *  			 proxy.
	 */
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

	/**
	 * Updates the underlying C structure to match our strings.
	 */
	void update_c_struct();

	/**
	 * Create data from a C struct
	 * This is a deep copy of the data from the C struct.
	 * @param cdata The C connect data.
	 */
	connect_data(const MQTTAsync_connectData& cdata);

public:
	/**
	 * Creates an empty set of connection data.
	 */
	connect_data();
	/**
	 * Creates connection data with a user name, but no password.
	 * @param userName The user name for reconnecting to the MQTT broker.
	 */
	explicit connect_data(string_ref userName);
	/**
	 * Creates connection data with a user name and password.
	 * @param userName The user name for reconnecting to the MQTT broker.
	 * 

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
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_connect_options_h

// By GST @Date

