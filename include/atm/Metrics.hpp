#pragma once
#include <vector>
#include <cmath>
#include <algorithm>

namespace atm::metrics {

inline double rmse(const std::vector<double>& yTrue, const std::vector<double>& yPred) {
    size_t n = std::min(yTrue.size(), yPred.size());
    if (n == 0) return 0.0;
    double s = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double d = yPred[i] - yTrue[i];
        s += d * d;
    }
    return std::sqrt(s / static_cast<double>(n));
}

inline double bias(const std::vector<double>& yTrue, const std::vector<double>& yPred) {
    size_t n = std::min(yTrue.size(), yPred.size());
    if (n == 0) return 0.0;
    double s = 0.0;
    for (size_t i = 0; i < n; ++i) s += (yPred[i] - yTrue[i]);
    return s / static_cast<double>(n);
}

inline double mape(const std::vector<double>& yTrue, const std::vector<double>& yPred) {
    size_t n = std::min(yTrue.size(), yPred.size());
    if (n == 0) return 0.0;
    double s = 0.0;
    for (size_t i = 0; i < n; ++i) {
        double denom = (yTrue[i] == 0.0) ? 1e-12 : std::abs(yTrue[i]);
        s += std::abs((yPred[i] - yTrue[i]) / denom);
    }
    return 100.0 * s / static_cast<double>(n);
}

// Hit rate / coverage for VaR backtesting: fraction of times loss < -VaR (i.e., exception rate).
inline double var_exception_rate(const std::vector<double>& pnl, double varLevel) {
    if (pnl.empty()) return 0.0;
    size_t exc = 0;
    for (double x : pnl) if (x < -varLevel) ++exc;
    return static_cast<double>(exc) / static_cast<double>(pnl.size());
}

} // namespace atm::metrics
