// Filename: will_options.cpp
// Score: 95

#include "mqtt/will_options.h"
#include <utility>
#include <cstring>

namespace mqtt {

PAHO_MQTTPP_EXPORT const int  will_options::DFLT_QOS = 0;
PAHO_MQTTPP_EXPORT const bool will_options::DFLT_RETAINED = false;

PAHO_MQTTPP_EXPORT const MQTTAsync_willOptions will_options::DFLT_C_STRUCT = MQTTAsync_willOptions_initializer;

will_options::will_options() : opts_(DFLT_C_STRUCT)
{
	set_topic(string());
}

will_options::will_options(string_ref top,
						   const void *payload, size_t payloadlen,
						   int qos, bool retained,
						   const properties& props)
		: opts_(DFLT_C_STRUCT), props_(props)
{
	opts_.qos = qos;
	opts_.retained = retained;
	set_topic(std::move(top));
	set_payload(binary_ref(static_cast<const binary_ref::value_type*>(payload), payloadlen));
}

will_options::will_options(const topic& top,
						   const void *payload, size_t payloadlen,
						   int qos, bool retained,
						   const properties& props)
		: will_options(top.get_name(), payload, payloadlen, qos, retained, props)
{
}

will_options::will_options(string_ref top, binary_ref payload,
						   int qos, bool retained,
						   const properties& props)
		: opts_(DFLT_C_STRUCT), props_(props)
{
	opts_.qos = qos;
	opts_.retained = retained;
	set_topic(std::move(top));
	set_payload(std::move(payload));
}

will_options::will_options(string_ref top, const string& payload,
						   int qos, bool retained,
						   const properties& props)
		: opts_(DFLT_C_STRUCT), props_(props)
{
	opts_.qos = qos;
	opts_.retained = retained;
	set_topic(std::move(top));
	set_payload(payload);
}

will_options::will_options(const message& msg)
	: will_options(msg.get_topic(), msg.get_payload(), msg.get_qos(),
				   msg.is_retained(), msg.get_properties())
{
}

will_options::will_options(const will_options& other)
	: opts_(other.opts_), props_(other.props_)
{
	set_topic(other.topic_);
	set_payload(other.payload_);
}

will_options::will_options(will_options&& other)
	: opts_(other.opts_), props_(std::move(other.props_))
{
	set_topic(std::move(other.topic_));
	set_payload(std::move(other.payload_));
}

will_options& will_options::operator=(const will_options& rhs)
{
	if (&rhs != this) {
		opts_ = rhs.opts_;
		set_topic(rhs.topic_);
		set_payload(rhs.payload_);
		props_ = rhs.props_;
	}
	return *this;
}

will_options& will_options::operator=(will_options&& rhs)
{
	if (&rhs != this) {
		opts_ = rhs.opts_;
		set_topic(std::move(rhs.topic_));
		set_payload(std::move(rhs.payload_));
		props_ = std::move(rhs.props_);
	}
	return *this;
}

void will_options::set_topic(string_ref top)
{
	topic_ = top ? std::move(top) : string_ref(string());
	opts_.topicName = topic_.c_str();
}

void will_options::set_payload(binary_ref msg)
{
	payload_ = msg ? std::move(msg) : binary_ref(binary());

	opts_.payload.len = (int) payload_.size();
	opts_.payload.data = payload_.data();
}

} // end namespace mqtt
// By GST @Date