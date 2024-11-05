// @file server_response.h

#ifndef __mqtt_server_response_h
#define __mqtt_server_response_h

#include "MQTTAsync.h"
#include "mqtt/types.h"
#include "mqtt/properties.h"

#include <iostream>

namespace mqtt {

/**
 * Base class for responses from the server.
 */
class server_response
{
	/** The properties from the acknowledge  */
	properties props_;

public:
	/** Creates a response with empty property list. */
	server_response() {}
	/** Creates a server response with the specified properties. */
	server_response(const properties& props)
		: props_(props) {}
	/** Creates a server response with the specified properties. */
	server_response(properties&& props)
		: props_(std::move(props)) {}
	/** Virtual destructor. */
	virtual ~server_response() {}
	/** Gets the properties from the response. */
	const properties& get_properties() const { return props_; }
};

/**
 * Response for a connect request
 */
class connect_response : public server_response
{
	/** The connection string of the server */
	string serverURI_;
	/** The version of MQTT being used */
	int mqttVersion_;
	/** The session present flag returned from the server */
	bool sessionPresent_;

	friend class token;

	connect_response(const MQTTAsync_successData5* rsp) :
		server_response(properties(rsp->properties)),
		serverURI_(string(rsp->alt.connect.serverURI)),
		mqttVersion_(rsp->alt.connect.MQTTVersion),
		sessionPresent_(to_bool(rsp->alt.connect.sessionPresent)) {
	}

	connect_response(const MQTTAsync_successData* rsp) :
		serverURI_(string(rsp->alt.connect.serverURI)),
		mqttVersion_(rsp->alt.connect.MQTTVersion),
		sessionPresent_(to_bool(rsp->alt.connect.sessionPresent)) {
	}

public:
	/** Gets the URI of the broker to which we connected. */
	string get_server_uri() const { return serverURI_; }
	/** Gets the MQTT version for the connection. */
	int get_mqtt_version() const { return mqttVersion_; }
	/** Determines whether a session already existed for this client on the server. */
	bool is_session_present() const { return sessionPresent_; }
};

/**
 * Response for subscribe messages
 */
struct subscribe_response : public server_response
{
	/** The reason/result code for each topic request. */
	std::vector<ReasonCode> reasonCodes_;

	friend class token;

	/** Create v5 subscribe response. */
	subscribe_response(MQTTAsync_successData5* rsp)
			: server_response(properties(rsp->properties)) {
		if (rsp->alt.sub.reasonCodeCount < 2)
			reasonCodes_.push_back(ReasonCode(rsp->reasonCode));
		else if (rsp->alt.sub.reasonCodes) {
			for (int i=0; i<rsp->alt.sub.reasonCodeCount; ++i)
				reasonCodes_.push_back(ReasonCode(rsp->alt.sub.reasonCodes[i]));
		}
	}

	/** Create v3 subscribe response. */
	subscribe_response(size_t n, MQTTAsync_successData* rsp) {
		if (n < 2)
			reasonCodes_.push_back(ReasonCode(rsp->alt.qos));
		else if (rsp->alt.qosList) {
			for (size_t i=0; i<n; ++i)
				reasonCodes_.push_back(ReasonCode(rsp->alt.qosList[i]));
		}
	}

public:
	/** Gets the reason codes from the server response. */
	const std::vector<ReasonCode>& get_reason_codes() const {
		return reasonCodes_;
	}
};

/**
 * Response for unsubscribe messages.
 */
class unsubscribe_response : public server_response
{
	/** The reason/result code for each topic request. */
	std::vector<ReasonCode> reasonCodes_;

	friend class token;

	unsubscribe_response(MQTTAsync_successData5* rsp)
			: server_response(properties(rsp->properties)) {
		if (rsp->alt.unsub.reasonCodeCount < 2)
			reasonCodes_.push_back(ReasonCode(rsp->reasonCode));
		else if (rsp->alt.unsub.reasonCodes) {
			for (int i=0; i<rsp->alt.unsub.reasonCodeCount; ++i)
				reasonCodes_.push_back(ReasonCode(rsp->alt.unsub.reasonCodes[i]));
		}
	}

	unsubscribe_response(MQTTAsync_successData* /*rsp*/) {}

public:
	/** Gets the reason codes from the server response. */
	const std::vector<ReasonCode>& get_reason_codes() const {
		return reasonCodes_;
	}
};

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_server_response_h

// By GST @Date