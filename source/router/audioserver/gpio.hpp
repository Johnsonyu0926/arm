#pragma once

class CGpio {
public:
    ~CGpio() = default;

    CGpio(const CGpio &) = delete;

    CGpio &operator=(const CGpio &) = delete;

    static CGpio &getInstance() {
        static CGpio instance;
        return instance;
    }

    int getGpioStatus() const {
        return m_gpioStatus;
    }

    void set_gpio_on() {
        system("echo 1 > /sys/class/gpio/gpio16/value");
        m_gpioStatus = 1;
        printf("set send dir! gpio is 1\n");
    }

    void set_gpio_off() {
        system("echo 0 > /sys/class/gpio/gpio16/value");
        m_gpioStatus = 0;
        printf("set receive dir! gpio is 0\n");
    }

    void setGpioModel(const int model) {
        m_gpioModel = model;
    }

    int getGpioModel() const {
        return m_gpioModel;
    }

private:
    CGpio() = default;

private:
    int m_gpioStatus;
    int m_gpioModel;
};