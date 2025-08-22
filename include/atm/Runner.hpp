#pragma once
#include "Reporter.hpp"
#include <functional>
#include <vector>
#include <string>
#include <chrono>

namespace atm {

struct Threshold {
    std::string metricName;
    double maxValue; // pass if metric <= maxValue (or interpret accordingly)
};

struct TestContext {
    std::string datasetId;
    std::string testType;
    std::vector<Threshold> thresholds;
    std::string notes;
};

using TestFunc = std::function<std::vector<MetricRecord>()>;

struct RiskTest {
    std::string name;
    TestFunc fn;
};

class Runner {
    std::vector<RiskTest> tests_;
    std::vector<Reporter*> reporters_;
public:
    void addTest(RiskTest t) { tests_.push_back(std::move(t)); }
    void addReporter(Reporter* r) { reporters_.push_back(r); }

    int runAll() const;
};

std::string nowUtcIso();

} // namespace atm
