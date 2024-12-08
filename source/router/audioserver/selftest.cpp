#include <iostream>
#include <fstream>
#include <ctime>
#include <string>
#include <sys/socket.h>
#include <arpa/inet.h>
#include "PlayStatus.hpp"
#include "CBusiness.hpp"
#include "spdlog/spdlog.h"
#include "spdlog/sinks/basic_file_sink.h"

const std::string MACHINE_ID = "Machine123";
const std::string ASSET_TAG = "Asset456";

bool checkConnection(const std::string &ip, int port) {
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    if (sock < 0) {
        return false;
    }

    struct sockaddr_in server;
    server.sin_addr.s_addr = inet_addr(ip.c_str());
    server.sin_family = AF_INET;
    server.sin_port = htons(port);

    bool isConnected = connect(sock, (struct sockaddr *)&server, sizeof(server)) >= 0;
    close(sock);
    return isConnected;
}

void pushLogToTFTP(const std::string &logFilePath, const std::string &tftpServer) {
    std::string command = "tftp " + tftpServer + " -c put " + logFilePath;
    system(command.c_str());
}

void testPlayStatus() {
    PlayStatus &playStatus = PlayStatus::getInstance();
    playStatus.init();
    spdlog::info("PlayStatus initialized");

    playStatus.setPlayId(1);
    int playId = playStatus.getPlayId();
    spdlog::info("Play ID: {}", playId);

    playStatus.setPriority(5);
    int priority = playStatus.getPriority();
    spdlog::info("Priority: {}", priority);

    playStatus.setProcessId(12345);
    pid_t processId = playStatus.getProcessId();
    spdlog::info("Process ID: {}", processId);

    std::string content = "Test Content";
    playStatus.setPlayConten(content);
    std::string playContent = playStatus.getPlayConten();
    spdlog::info("Play Content: {}", playContent);

    playStatus.setMqttPlayStatus(2);
    int mqttPlayStatus = playStatus.getMqttPlayStatus();
    spdlog::info("MQTT Play Status: {}", mqttPlayStatus);

    int playState = playStatus.getPlayState();
    spdlog::info("Play State: {}", playState);

    spdlog::info("PlayStatus test completed");
}

void testDeviceBaseInfo() {
    DeviceBaseInfo deviceInfo;
    deviceInfo.do_success();
    spdlog::info("DeviceBaseInfo initialized");

    spdlog::info("Code Version: {}", deviceInfo.codeVersion);
    spdlog::info("Core Version: {}", deviceInfo.coreVersion);
    spdlog::info("Relay Mode: {}", deviceInfo.relayMode);
    spdlog::info("IP: {}", deviceInfo.ip);
    spdlog::info("Storage Type: {}", deviceInfo.storageType);
    spdlog::info("Port: {}", deviceInfo.port);
    spdlog::info("Play Status: {}", deviceInfo.playStatus);
    spdlog::info("Volume: {}", deviceInfo.volume);
    spdlog::info("Relay Status: {}", deviceInfo.relayStatus);
    spdlog::info("Hardware Release Time: {}", deviceInfo.hardwareReleaseTime);
    spdlog::info("SPI Free Space: {}", deviceInfo.spiFreeSpace);
    spdlog::info("Flash Free Space: {}", deviceInfo.flashFreeSpace);
    spdlog::info("Hardware Version: {}", deviceInfo.hardwareVersion);
    spdlog::info("Hardware Model ID: {}", deviceInfo.hardwareModelId);
    spdlog::info("Password: {}", deviceInfo.password);
    spdlog::info("Temperature: {}", deviceInfo.temperature);
    spdlog::info("Netmask: {}", deviceInfo.netmask);
    spdlog::info("Address: {}", deviceInfo.address);
    spdlog::info("Gateway: {}", deviceInfo.gateway);
    spdlog::info("User Name: {}", deviceInfo.userName);
    spdlog::info("IMEI: {}", deviceInfo.imei);
    spdlog::info("Function Version: {}", deviceInfo.functionVersion);
    spdlog::info("Device Code: {}", deviceInfo.deviceCode);
    spdlog::info("Server Address: {}", deviceInfo.serverAddress);
    spdlog::info("Server Port: {}", deviceInfo.serverPort);

    spdlog::info("DeviceBaseInfo test completed");
}

void testDeviceBaseState() {
    DeviceBaseState deviceState;
    deviceState.do_success();
    spdlog::info("DeviceBaseState initialized");

    spdlog::info("Relay Status: {}", deviceState.relayStatus);
    spdlog::info("Volume: {}", deviceState.volume);
    spdlog::info("Storage Type: {}", deviceState.storageType);

    spdlog::info("DeviceBaseState test completed");
}

void testCBusiness() {
    asns::CBusiness business;
    CSocket client;

    std::vector<std::string> loginParams = {"", "", "", "", "admin", "password"};
    int loginResult = business.Login(loginParams, &client);
    spdlog::info("Login result: {}", loginResult);

    std::vector<std::string> audioPlayParams = {"", "", "", "", "test.mp3"};
    int audioPlayResult = business.AudioPlay(audioPlayParams, &client);
    spdlog::info("AudioPlay result: {}", audioPlayResult);

    int audioStopResult = business.AudioStop(&client);
    spdlog::info("AudioStop result: {}", audioStopResult);

    std::vector<std::string> volumeSetParams = {"", "", "", "", "5"};
    int volumeSetResult = business.VolumeSet(volumeSetParams, &client);
    spdlog::info("VolumeSet result: {}", volumeSetResult);

    int rebootResult = business.Reboot(&client);
    spdlog::info("Reboot result: {}", rebootResult);

    int deviceStatusResult = business.GetDeviceStatus(&client);
    spdlog::info("GetDeviceStatus result: {}", deviceStatusResult);

    int restoreResult = business.Restore(&client);
    spdlog::info("Restore result: {}", restoreResult);

    std::vector<std::string> flashConfigParams = {"", "", "", "", "1", "1"};
    int flashConfigResult = business.FlashConfig(flashConfigParams, &client);
    spdlog::info("FlashConfig result: {}", flashConfigResult);

    std::vector<std::string> ttsPlayParams = {"", "", "", "", "48656c6c6f"};
    int ttsPlayResult = business.TtsPlay(ttsPlayParams, &client);
    spdlog::info("TtsPlay result: {}", ttsPlayResult);

    std::vector<std::string> ttsNumTimePlayParams = {"", "", "", "", "48656c6c6f", "1", "10"};
    int ttsNumTimePlayResult = business.TtsNumTimePlay(ttsNumTimePlayParams, &client);
    spdlog::info("TtsNumTimePlay result: {}", ttsNumTimePlayResult);

    std::vector<std::string> audioNumberOrTimePlayParams = {"", "", "", "", "test.mp3", "1", "10"};
    int audioNumberOrTimePlayResult = business.AudioNumberOrTimePlay(audioNumberOrTimePlayParams, &client);
    spdlog::info("AudioNumberOrTimePlay result: {}", audioNumberOrTimePlayResult);

    int deviceBaseInfoResult = business.GetDeviceBaseInfo(&client);
    spdlog::info("GetDeviceBaseInfo result: {}", deviceBaseInfoResult);

    std::vector<std::string> networkSetParams = {"", "", "", "", "admin", "password", "192.168.1.1", "255.255.255.0", "192.168.1.254"};
    int networkSetResult = business.NetworkSet(networkSetParams, &client);
    spdlog::info("NetworkSet result: {}", networkSetResult);

    std::vector<std::string> tcpNetworkSetParams = {"", "", "", "", "admin", "password", "192.168.1.1", "255.255.255.0"};
    int tcpNetworkSetResult = business.TCPNetworkSet(tcpNetworkSetParams, &client);
    spdlog::info("TCPNetworkSet result: {}", tcpNetworkSetResult);

    std::vector<std::string> updatePwdParams = {"", "", "", "", "admin", "password", "newpassword"};
    int updatePwdResult = business.UpdatePwd(updatePwdParams, &client);
    spdlog::info("UpdatePwd result: {}", updatePwdResult);

    std::vector<std::string> getAudioListParams = {"", "", "", "", "admin", "password"};
    int getAudioListResult = business.GetAudioList(getAudioListParams, &client);
    spdlog::info("GetAudioList result: {}", getAudioListResult);

    std::vector<std::string> recordParams = {"", "", "", "", "10", "http://example.com/upload"};
    int recordResult = business.Record(recordParams, &client);
    spdlog::info("Record result: {}", recordResult);

    std::vector<std::string> recordBeginParams = {"", "", "", "", "admin", "password"};
    int recordBeginResult = business.RecordBegin(recordBeginParams, &client);
    spdlog::info("RecordBegin result: {}", recordBeginResult);

    std::vector<std::string> recordEndParams = {"", "", "", "", "admin", "password"};
    int recordEndResult = business.RecordEnd(recordEndParams, &client);
    spdlog::info("RecordEnd result: {}", recordEndResult);

    std::vector<std::string> fileUploadParams = {"", "", "", "", "http://example.com/upload", "test.mp3"};
    int fileUploadResult = business.FileUpload(fileUploadParams, &client);
    spdlog::info("FileUpload result: {}", fileUploadResult);

    std::vector<std::string> audioFileUploadParams = {"", "", "", "", "1024", "http://example.com/upload", "test.mp3"};
    int audioFileUploadResult = business.AudioFileUpload(audioFileUploadParams, &client);
    spdlog::info("AudioFileUpload result: {}", audioFileUploadResult);

    std::vector<std::string> remoteFileUpgradeParams = {"", "", "", "", "http://example.com/upgrade"};
    int remoteFileUpgradeResult = business.RemoteFileUpgrade(remoteFileUpgradeParams, &client);
    spdlog::info("RemoteFileUpgrade result: {}", remoteFileUpgradeResult);

    std::vector<std::string> dspManagementParams = {"", "", "", "", "lineen", "1"};
    int dspManagementResult = business.DspManagement(dspManagementParams, &client);
    spdlog::info("DspManagement result: {}", dspManagementResult);

    spdlog::info("CBusiness test completed");
}

int main() {
    // 初始化日志记录器
    auto logger = spdlog::basic_logger_mt("basic_logger", "logs/system_check.log");
    spdlog::set_default_logger(logger);
    spdlog::set_level(spdlog::level::info); // 设置日志记录级别

    // 运行测试用例
    testPlayStatus();
    testDeviceBaseInfo();
    testDeviceBaseState();
    testCBusiness();

    // 检测是否能连接到 172.16.11.240 的 69 端口
    bool canConnect = checkConnection("172.16.11.240", 69);

    // 获取当前日期和时间
    std::time_t now = std::time(nullptr);
    char buf[80];
    std::strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", std::localtime(&now));
    std::string dateTime(buf);

    // 设置日志头
    std::string logHeader;
    if (canConnect) {
        logHeader = "welldone";
    } else {
        logHeader = "error " + dateTime + " " + MACHINE_ID + " " + ASSET_TAG;
    }

    // 将日志头写入日志文件
    std::ofstream logFile("logs/system_check.log", std::ios_base::app);
    logFile << logHeader << std::endl;
    logFile.close();

    // 如果可以连接，将日志通过 TFTP 推送到 172.16.11.240
    if (canConnect) {
        pushLogToTFTP("logs/system_check.log", "172.16.11.240");
    }

    return 0;
}