/// @file iaction_listener.h

#ifndef __mqtt_iaction_listener_h
#define __mqtt_iaction_listener_h

#include "MQTTAsync.h"
#include "mqtt/types.h"
#include <vector>

namespace mqtt {

class token;

/////////////////////////////////////////////////////////////////////////////

/**
 * Provides a mechanism for tracking the completion of an asynchronous
 * action.
 *
 * A listener is registered on a token and that token is associated with
 * an action like connect or publish. When used with tokens on the
 * async_client the listener will be called back on the MQTT client's
 * thread. The listener will be informed if the action succeeds or fails. It
 * is important that the listener returns control quickly otherwise the
 * operation of the MQTT client will be stalled.
 */
class iaction_listener
{
public:
    /** Smart/shared pointer to an object of this class. */
    using ptr_t = std::shared_ptr<iaction_listener>;
    /** Smart/shared pointer to a const object of this class. */
    using const_ptr_t = std::shared_ptr<const iaction_listener>;

    /** Virtual base destructor. */
    virtual ~iaction_listener() {}
    /** This method is invoked when an action fails. */
    virtual void on_failure(const token& asyncActionToken) =0;
    /** This method is invoked when an action has completed successfully. */
    virtual void on_success(const token& asyncActionToken) =0;
};

/** Smart/shared pointer to an action listener */
using iaction_listener_ptr = iaction_listener::ptr_t;

/** Smart/shared pointer to a const action listener */
using const_iaction_listener_ptr = iaction_listener::const_ptr_t;

/////////////////////////////////////////////////////////////////////////////
// end namespace mqtt
}

#endif      // __mqtt_iaction_listener_h

// By GST @Date