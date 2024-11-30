#include "volume_adapt_schema_set.h"

// 异步任务启动函数
void start_async_task(void (*task)(void)) {
    cutils_async_wait(1, 0, 0, task);
}

// 成功处理函数
int do_success(const Request* request, Result* result) {
    if (request->data.monitorStatus && !acquisition_noise_get_monitor_status()) {
        printf("open rs485 noise\n");
        acquisition_noise_set_monitor_status(request->data.monitorStatus);
        rs485_uart_fcntl_set(FNDELAY);
        start_async_task([]{
            while (rs485_get_state()) {
                sleep(1);
            }
            rs485_noise_manage_start_collecting_noise();
            printf("noise worker end\n");
        });
    } else if (!request->data.monitorStatus && acquisition_noise_get_monitor_status()) {
        printf("open rs485 business\n");
        acquisition_noise_set_monitor_status(request->data.monitorStatus);
        acquisition_noise_set_decibel(0);
        rs485_uart_fcntl_set(FNDELAY);
        start_async_task([]{
            while (rs485_get_state()) {
                sleep(1);
            }
            RSBusinessManage rs;
            rs.worker();
            printf("business worker end\n");
        });
    }
    request->data.set_data();
    result->resultId = 1;
    result->result = strdup("success");
    return 1;
}