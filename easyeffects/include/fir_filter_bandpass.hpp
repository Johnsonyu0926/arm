// Filename: FirFilterBandpass.hpp
// Score: 95

#pragma once

#include <string>
#include "fir_filter_base.hpp"

class FirFilterBandpass : public FirFilterBase {
public:
    FirFilterBandpass(std::string tag);
    FirFilterBandpass(const FirFilterBandpass&) = delete;
    auto operator=(const FirFilterBandpass&) -> FirFilterBandpass& = delete;
    FirFilterBandpass(const FirFilterBandpass&&) = delete;
    auto operator=(const FirFilterBandpass&&) -> FirFilterBandpass& = delete;
    ~FirFilterBandpass() override;

    void setup() override;
};
// By GST @Date