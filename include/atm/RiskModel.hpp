#pragma once
#include <string>
#include <vector>
#include <unordered_map>

namespace atm {

struct DataPoint {
    double value;
    std::string asOf; // ISO-8601 date string
};

struct Dataset {
    std::string id;
    std::vector<DataPoint> series;
};

class RiskModel {
public:
    virtual ~RiskModel() = default;
    virtual std::string id() const = 0;
    virtual std::string version() const = 0;

    // For general scoring models (e.g., PD/LGD), returns predictions aligned to inputs.
    virtual std::vector<double> predict(const std::vector<double>& x) const {
        (void)x;
        return {};
    }

    // For risk measures (e.g., VaR); returns a single scalar.
    virtual double riskMeasure(const std::vector<double>& pnl) const {
        (void)pnl;
        return 0.0;
    }
};

} // namespace atm
