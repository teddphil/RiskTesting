#pragma once
#include "atm/RiskModel.hpp"
#include <vector>
#include <algorithm>
#include <stdexcept>

class BasicVaR95 : public atm::RiskModel {
    size_t window_;
public:
    explicit BasicVaR95(size_t window = 250): window_(window) {}

    std::string id() const override { return "BasicVaR95"; }
    std::string version() const override { return "1.0.0"; }

    double riskMeasure(const std::vector<double>& pnl) const override {
        if (pnl.size() < window_) throw std::runtime_error("Insufficient history for VaR window");
        std::vector<double> window(pnl.end() - static_cast<long>(window_), pnl.end());
        std::sort(window.begin(), window.end()); // ascending losses/gains
        // VaR at 95% -> 5th percentile of loss distribution; we use negative PnL threshold.
        size_t idx = static_cast<size_t>(0.05 * window.size());
        if (idx >= window.size()) idx = window.size() - 1;
        double q = window[idx];
        return -std::min(0.0, q); // VaR is positive number (magnitude of tail loss)
    }
};
