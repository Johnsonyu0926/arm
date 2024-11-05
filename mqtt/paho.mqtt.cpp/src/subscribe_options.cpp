// Filename: subscribe_options.cpp
// Score: 95

#include "mqtt/subscribe_options.h"

namespace mqtt {

PAHO_MQTTPP_EXPORT const bool subscribe_options::SUBSCRIBE_NO_LOCAL = true;
PAHO_MQTTPP_EXPORT const bool subscribe_options::SUBSCRIBE_LOCAL = false;

PAHO_MQTTPP_EXPORT const bool subscribe_options::NO_RETAIN_AS_PUBLISHED = false;
PAHO_MQTTPP_EXPORT const bool subscribe_options::RETAIN_AS_PUBLISHED = true;

} // end namespace mqtt
// By GST @Date