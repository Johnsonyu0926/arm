// Filename: token.cpp
// Score: 95

#include "mqtt/token.h"
#include "mqtt/async_client.h"
#include <cstring>
#include <iostream>

namespace mqtt {

token::token(Type typ, iasync_client& cli, const_string_collection_ptr topics)
				: type_(typ), cli_(&cli), rc_(0), reasonCode_(ReasonCode::SUCCESS),
						msgId_(MQTTAsync_token(0)), topics_(topics),
						userContext_(nullptr), listener_(nullptr), nExpected_(0),
						complete_(false)
{
}

token::token(Type typ, iasync_client& cli, const_string_collection_ptr topics,
			 void* userContext, iaction_listener& cb)
				: type_(typ), cli_(&cli), rc_(0), reasonCode_(ReasonCode::SUCCESS),
						msgId_(MQTTAsync_token(0)), topics_(topics),
						userContext_(userContext), listener_(&cb), nExpected_(0),
						complete_(false)
{
}

token::token(Type typ, iasync_client& cli, MQTTAsync_token tok)
				: type_(typ), cli_(&cli), rc_(0), reasonCode_(ReasonCode::SUCCESS),
					msgId_(tok), userContext_(nullptr),
					listener_(nullptr), nExpected_(0), complete_(false)
{
}

void token::on_success(void* context, MQTTAsync_successData* rsp)
{
	if (context)
		static_cast<token*>(context)->on_success(rsp);
}

void token::on_success5(void* context, MQTTAsync_successData5* rsp)
{
	if (context)
		static_cast<token*>(context)->on_success5(rsp);
}

void token::on_failure(void* context, MQTTAsync_failureData* rsp)
{
	if (context)
		static_cast<token*>(context)->on_failure(rsp);
}

void token::on_failure5(void* context, MQTTAsync_failureData5* rsp)
{
	if (context)
		static_cast<token*>(context)->on_failure5(rsp);
}

void token::on_success(MQTTAsync_successData* rsp)
{
	unique_lock g(lock_);
	iaction_listener* listener = listener_;

	if (rsp) {
		msgId_ = rsp->token;

		switch (type_) {
			case Type::CONNECT:
				connRsp_.reset(new connect_response(rsp));
				break;

			case Type::SUBSCRIBE:
				subRsp_.reset(new subscribe_response(nExpected_, rsp));
				break;

			case Type::UNSUBSCRIBE:
				unsubRsp_.reset(new unsubscribe_response(rsp));
				break;

			default:
				break;
		}
	}

	rc_ = MQTTASYNC_SUCCESS;
	complete_ = true;
	g.unlock();

	if (listener)
		listener->on_success(*this);
	cond_.notify_all();

	cli_->remove_token(this);
}

void token::on_success5(MQTTAsync_successData5* rsp)
{
	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	if (rsp) {
		msgId_ = rsp->token;
		reasonCode_ = ReasonCode(rsp->reasonCode);

		switch (type_) {
			case Type::CONNECT:
				connRsp_.reset(new connect_response(rsp));
				break;

			case Type::SUBSCRIBE:
				subRsp_.reset(new subscribe_response(rsp));
				break;

			case Type::UNSUBSCRIBE:
				unsubRsp_.reset(new unsubscribe_response(rsp));
				break;

			default:
				break;
		}
	}
	rc_ = MQTTASYNC_SUCCESS;
	complete_ = true;
	g.unlock();

	if (listener)
		listener->on_success(*this);
	cond_.notify_all();

	cli_->remove_token(this);
}

void token::on_failure(MQTTAsync_failureData* rsp)
{
	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	if (rsp) {
		msgId_ = rsp->token;
		rc_ = rsp->code;

		reasonCode_ = ReasonCode(MQTTPP_V3_CODE);

		if (rsp->message)
			errMsg_ = string(rsp->message);
	}
	else {
		rc_ = -1;
	}
	complete_ = true;
	g.unlock();

	if (listener)
		listener->on_failure(*this);
	cond_.notify_all();

	cli_->remove_token(this);
}

void token::on_failure5(MQTTAsync_failureData5* rsp)
{
	unique_lock g(lock_);
	iaction_listener* listener = listener_;
	if (rsp) {
		msgId_ = rsp->token;
		reasonCode_ = ReasonCode(rsp->reasonCode);
		rc_ = rsp->code;
		if (rsp->message)
			errMsg_ = string(rsp->message);
	}
	else {
		rc_ = -1;
	}
	complete_ = true;
	g.unlock();

	if (listener)
		listener->on_failure(*this);
	cond_.notify_all();

	cli_->remove_token(this);
}

void token::reset()
{
	guard g(lock_);
	complete_ = false;
	rc_ = MQTTASYNC_SUCCESS;
	reasonCode_ = ReasonCode::SUCCESS;
	errMsg_.clear();
}

void token::wait()
{
	unique_lock g(lock_);
	cond_.wait(g, [this]{return complete_;});
	check_ret();
}

connect_response token::get_connect_response() const
{
	if (type_ != Type::CONNECT)
		throw bad_cast();

	unique_lock g(lock_);
	cond_.wait(g, [this]{return complete_;});
	check_ret();

	if (!connRsp_)
		throw missing_response("connect");

	return *connRsp_;
}

subscribe_response token::get_subscribe_response() const
{
	if (type_ != Type::SUBSCRIBE)
		throw bad_cast();

	unique_lock g(lock_);
	cond_.wait(g, [this]{return complete_;});
	check_ret();

	if (!subRsp_)
		throw missing_response("subscribe");

	return *subRsp_;
}

unsubscribe_response token::get_unsubscribe_response() const
{
	if (type_ != Type::UNSUBSCRIBE)
		throw bad_cast();

	unique_lock g(lock_);
	cond_.wait(g, [this]{return complete_;});
	check_ret();

	if (!unsubRsp_)
		throw missing_response("unsubscribe");

	return *unsubRsp_;
}

} // end namespace mqtt
// By GST @Date