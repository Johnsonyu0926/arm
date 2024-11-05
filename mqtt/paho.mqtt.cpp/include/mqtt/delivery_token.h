/// @file delivery_token.h

#ifndef __mqtt_delivery_token_h
#define __mqtt_delivery_token_h

#include "MQTTAsync.h"
#include "mqtt/token.h"
#include "mqtt/message.h"
#include <memory>

namespace mqtt {

/////////////////////////////////////////////////////////////////////////////

/**
 * Provides a mechanism to track the delivery progress of a message.
 * Used to track the delivery progress of a message when a publish is
 * executed in a non-blocking manner (run in the background) action.
 */
class delivery_token : public token
{
	/** The message being tracked. */
	const_message_ptr msg_;

	/** Client has special access. */
	friend class async_client;

	/** Sets the message to which this token corresponds. */
	void set_message(const_message_ptr msg) { msg_ = msg; }

public:
	/** Smart/shared pointer to an object of this class */
	using ptr_t = std::shared_ptr<delivery_token>;
	/** Smart/shared pointer to a const object of this class */
	using const_ptr_t = std::shared_ptr<delivery_token>;
	/** Weak pointer to an object of this class */
	using weak_ptr_t = std::weak_ptr<delivery_token>;

	/** Creates an empty delivery token connected to a particular client. */
	delivery_token(iasync_client& cli) : token(token::Type::PUBLISH, cli) {}
	/** Creates a delivery token connected to a particular client. */
	delivery_token(iasync_client& cli, const_message_ptr msg)
			: token(token::Type::PUBLISH, cli, msg->get_topic()), msg_(std::move(msg)) {}
	/** Creates a delivery token connected to a particular client. */
	delivery_token(iasync_client& cli, const_message_ptr msg,
				   void* userContext, iaction_listener& cb)
			: token(token::Type::PUBLISH, cli, msg->get_topic(), userContext, cb), msg_(std::move(msg)) {}
	/** Creates an empty delivery token connected to a particular client. */
	static ptr_t create(iasync_client& cli) {
		return std::make_shared<delivery_token>(cli);
	}
	/** Creates a delivery token connected to a particular client. */
	static ptr_t create(iasync_client& cli, const_message_ptr msg) {
		return std::make_shared<delivery_token>(cli, msg);
	}
	/** Creates a delivery token connected to a particular client. */
	static ptr_t create(iasync_client& cli, const_message_ptr msg,
						void* userContext, iaction_listener& cb) {
		return std::make_shared<delivery_token>(cli, msg, userContext, cb);
	}
	/** Gets the message associated with this token. */
	virtual const_message_ptr get_message() const { return msg_; }
};

/** Smart/shared pointer to a delivery_token */
using delivery_token_ptr = delivery_token::ptr_t;

/** Smart/shared pointer to a const delivery_token */
using const_delivery_token_ptr = delivery_token::const_ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif		// __mqtt_delivery_token_h

// By GST @Date