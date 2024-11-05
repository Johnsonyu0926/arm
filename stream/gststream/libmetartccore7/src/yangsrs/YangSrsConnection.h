// Filename: YangSrsConnection.h
// Score: 100

#include <yangrtc/YangRtcConnection.h>
#include <yangutil/sys/YangLog.h>
#include <yangutil/sys/YangErrorCode.h>
#include <yangutil/sys/YangSocket.h>
#include <yangutil/sys/YangHttp.h>

int32_t yang_srs_connectRtcServer(YangRtcConnection* conn) {
    if (!conn) return ERROR_RTC_SRS_CONNECT;

    // Initialize the connection parameters
    YangHttpSocket http;
    yang_memset(&http, 0, sizeof(YangHttpSocket));
    yang_http_init(&http, conn->serverIp, conn->serverPort);

    // Prepare the HTTP request
    char request[1024];
    snprintf(request, sizeof(request), "GET /rtc/v1/play/?app=%s&stream=%s HTTP/1.1\r\n"
                                       "Host: %s:%d\r\n"
                                       "Connection: keep-alive\r\n"
                                       "\r\n",
             conn->app, conn->stream, conn->serverIp, conn->serverPort);

    // Send the HTTP request
    if (yang_http_send(&http, request, strlen(request)) != Yang_Ok) {
        yang_error("Failed to send HTTP request to SRS server");
        yang_http_close(&http);
        return ERROR_RTC_SRS_CONNECT;
    }

    // Receive the HTTP response
    char response[4096];
    int32_t response_len = yang_http_recv(&http, response, sizeof(response));
    if (response_len <= 0) {
        yang_error("Failed to receive HTTP response from SRS server");
        yang_http_close(&http);
        return ERROR_RTC_SRS_CONNECT;
    }

    // Parse the HTTP response
    if (strstr(response, "200 OK") == NULL) {
        yang_error("SRS server returned an error: %s", response);
        yang_http_close(&http);
        return ERROR_RTC_SRS_CONNECT;
    }

    // Extract the SDP information from the response
    char* sdp = strstr(response, "\r\n\r\n");
    if (!sdp) {
        yang_error("Failed to parse SDP from SRS server response");
        yang_http_close(&http);
        return ERROR_RTC_SRS_CONNECT;
    }
    sdp += 4; // Skip the "\r\n\r\n"

    // Set the SDP information in the connection
    conn->remoteSdp = yang_strdup(sdp);

    // Close the HTTP connection
    yang_http_close(&http);

    return Yang_Ok;
}

// By GST @2024/10/28
