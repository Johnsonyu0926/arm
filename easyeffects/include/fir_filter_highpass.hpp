// Filename: FirFilterHighpass.hpp
// Score: 95

#pragma once

#include <string>
#include "fir_filter_base.hpp"

class FirFilterHighpass : public FirFilterBase {
public:
    FirFilterHighpass(std::string tag);
    FirFilterHighpass(const FirFilterHighpass&) = delete;
    auto operator=(const FirFilterHighpass&) -> FirFilterHighpass& = delete;
    FirFilterHighpass(const FirFilterHighpass&&) = delete;
    auto operator=(const FirFilterHighpass&&) -> FirFilterHighpass& = delete;
    ~FirFilterHighpass() override;

    void setup() override;
};
// By GST @Date