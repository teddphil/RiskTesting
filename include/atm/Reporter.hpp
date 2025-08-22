#pragma once
#include <string>
#include <vector>
#include <fstream>
#include <sstream>

namespace atm {

struct MetricRecord {
    std::string timestampUtc;
    std::string modelId;
    std::string modelVersion;
    std::string datasetId;
    std::string testType;
    std::string metricName;
    double metricValue;
    double threshold;
    bool pass;
    std::string notes;
};

class Reporter {
public:
    virtual ~Reporter() = default;
    virtual void write(const std::vector<MetricRecord>& records) = 0;
};

class CSVReporter : public Reporter {
    std::string outPath_;
public:
    explicit CSVReporter(std::string outPath): outPath_(std::move(outPath)) {}
    void write(const std::vector<MetricRecord>& records) override;
};

class JUnitXmlReporter : public Reporter {
    std::string outPath_;
public:
    explicit JUnitXmlReporter(std::string outPath): outPath_(std::move(outPath)) {}
    void write(const std::vector<MetricRecord>& records) override;
};

class JsonReporter : public Reporter {
    std::string outPath_;
public:
    explicit JsonReporter(std::string outPath): outPath_(std::move(outPath)) {}
    void write(const std::vector<MetricRecord>& records) override;
};

} // namespace atm
