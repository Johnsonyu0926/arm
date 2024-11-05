// Filename: YangWhipConnection.h
// Score: 98

#ifndef YANGRTP_YangWhipConnection_H_
#define YANGRTP_YangWhipConnection_H_

#include <yangrtc/YangRtcConnection.h>

// Function to connect to WHIP peer
int32_t yang_whip_connectPeer(YangRtcConnection* conn, const char* url);

#endif // YANGRTP_YangWhipConnection_H_
// By GST @2024/10/28