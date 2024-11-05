// Filename: YangGpuEncoderFactory.h
// Score: 98

#ifndef YANGGPUENCODERFACTORY_H
#define YANGGPUENCODERFACTORY_H

#include <yangencoder/YangVideoEncoder.h>
#include <yangutil/yangavinfotype.h>

// Factory class for creating GPU encoders
class YangGpuEncoderFactory {
public:
    YangGpuEncoderFactory();
    ~YangGpuEncoderFactory();

    YangVideoEncoder* createGpuEncoder();
};

#endif // YANGGPUENCODERFACTORY_H
// By GST @2024/10/28