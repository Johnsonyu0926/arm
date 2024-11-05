/// @file iasync_client.h

#ifndef __mqtt_iasync_client_h
#define __mqtt_iasync_client_h

#include "mqtt/types.h"
#include "mqtt/token.h"
#include "mqtt/delivery_token.h"
#include "mqtt/iclient_persistence.h"
#include "mqtt/iaction_listener.h"
#include "mqtt/connect_options.h"
#include "mqtt/disconnect_options.h"
#include "mqtt/subscribe_options.h"
#include "mqtt/exception.h"
#include "mqtt/message.h"
#include "mqtt/callback.h"
#include <vector>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Enables an application to communicate with an MQTT server using
 * non-blocking methods.
 *
 * It provides applications a simple programming interface to all features
 * of the MQTT version 3.1 specification including:
 *
 * @li connect
 * @li publish
 * @li subscribe
 * @li unsubscribe
 * @li disconnect
 */
class iasync_client
{
	friend class token;
	virtual void remove_token(token* tok) =0;

public:
	/** Type for a collection of QOS values */
	using qos_collection = std::vector<int>;

	/** Virtual destructor */
	virtual ~iasync_client() {}
	/** Connects to an MQTT server using the default options. */
	virtual token_ptr connect() =0;
	/** Connects to an MQTT server using the provided connect options. */
	virtual token_ptr connect(connect_options options) =0;
	/** Connects to an MQTT server using the specified options. */
	virtual token_ptr connect(connect_options options, void* userContext,
							  iaction_listener& cb) =0;
	/** Connects to an MQTT server using the specified options. */
	virtual token_ptr connect(void* userContext, iaction_listener& cb) =0;
	/** Reconnects the client using options from the previous connect. */
	virtual token_ptr reconnect() =0;
	/** Disconnects from the server. */
	virtual token_ptr disconnect() =0;
	/** Disconnects from the server. */
	virtual token_ptr disconnect(disconnect_options opts) =0;
	/** Disconnects from the server. */
	virtual token_ptr disconnect(int timeout) =0;
	/** Disconnects from the server. */
	virtual token_ptr disconnect(int timeout, void* userContext, iaction_listener& cb) =0;
	/** Disconnects from the server. */
	virtual token_ptr disconnect(void* userContext, iaction_listener& cb) =0;
	/** Returns the delivery token for the specified message ID. */
	virtual delivery_token_ptr get_pending_delivery_token(int msgID) const =0;
	/** Returns the delivery tokens for any outstanding publish operations. */
	virtual std::vector<delivery_token_ptr> get_pending_delivery_tokens() const =0;
	/** Returns the client ID used by this client. */
	virtual string get_client_id() const =0;
	/** Returns the address of the server used by this client. */
	virtual string get_server_uri() const =0;
	/** Determines if this client is currently connected to the server. */
	virtual bool is_connected() const =0;
	/** Publishes a message to a topic on the server */
	virtual delivery_token_ptr publish(string_ref topic,
									   const void* payload, size_t n,
									   int qos, bool retained) =0;
	/** Publishes a message to a topic on the server */
	virtual delivery_token_ptr publish(string_ref topic,
									   const void* payload, size_t n) =0;
	/** Publishes a message to a topic on the server */
	virtual delivery_token_ptr publish(string_ref topic,
									   const void* payload, size_t n,
									   int qos, bool retained,
									   void* userContext, iaction_listener& cb) =0;
	/** Publishes a message to a topic on the server */
	virtual delivery_token_ptr publish(string_ref topic, binary_ref payload,
									   int qos, bool retained) =0;
	/** Publishes a message to a topic on the server. */
	virtual delivery_token_ptr publish(string_ref topic, binary_ref payload) =0;
	/** Publishes a message to a topic on the server Takes an Message message and delivers it to the server at the requested quality of service. */
	virtual delivery_token_ptr publish(const_message_ptr msg) =0;
	/** Publishes a message to a topic on the server. */
	virtual delivery_token_ptr publish(const_message_ptr msg,
									   void* userContext, iaction_listener& cb) =0;
	/** Sets a callback listener to use for events that happen asynchronously. */
	virtual void set_callback(callback& cb) =0;
	/** Stops the callbacks. */
	virtual void disable_callbacks() =0;
	/** Subscribe to a topic, which may include wildcards. */
	virtual token_ptr subscribe(const string& topicFilter, int qos,
								const subscribe_options& opts=subscribe_options(),
								const properties& props=properties()) =0;
	/** Subscribe to a topic, which may include wildcards. */
	virtual token_ptr subscribe(const string& topicFilter, int qos,
								void* userContext, iaction_listener& callback,
								const subscribe_options& opts=subscribe_options(),
								const properties& props=properties()) =0;
	/** Subscribe to multiple topics, each of which may include wildcards. */
	virtual token_ptr subscribe(const_string_collection_ptr topicFilters,
								const qos_collection& qos,
								const std::vector<subscribe_options>& opts=std::vector<subscribe_options>(),
								const properties& props=properties()) =0;
	/** Subscribes to multiple topics, each of which may include wildcards. */
	virtual token_ptr subscribe(const_string_collection_ptr topicFilters,
								const qos_collection& qos,
								void* userContext, iaction_listener& callback,
								const std::vector<subscribe_options>& opts=std::vector<subscribe_options>(),
								const properties& props=properties()) =0;
	/** Requests the server unsubscribe the client from a topic. */
	virtual token_ptr unsubscribe(const string& topicFilter,
								  const properties& props=properties()) =0;
	/** Requests the server unsubscribe the client from one or more topics. */
	virtual token_ptr unsubscribe(const_string_collection_ptr topicFilters,
								  const properties& props=properties()) =0;
	/** Requests the server unsubscribe the client from one or more topics. */
	virtual token_ptr unsubscribe(const_string_collection_ptr topicFilters,
								  void* userContext, iaction_listener& cb,
								  const properties& props=properties()) =0;
	/** Requests the server unsubscribe the client from a topics. */
	virtual token_ptr unsubscribe(const string& topicFilter,
								  void* userContext, iaction_listener& cb,
								  const properties& props=properties()) =0;
	/** Start consuming messages. */
	virtual void start_consuming() =0;
	/** Stop consuming messages. */
	virtual void stop_consuming() =0;
	/** Read the next message from the queue. */
	virtual const_message_ptr consume_message() =0;
	/** Try to read the next message from the queue without blocking. */
	virtual bool try_consume_message(const_message_ptr* msg) =0;
};

/////////////////////////////////////////////////////////////////////////////
// end namespace 'mqtt'
}

#endif		// __mqtt_iasync_client_h

// By GST @Date