#include "atm/Runner.hpp"
#include "atm/Reporter.hpp"
#include "atm/Metrics.hpp"
#include "atm/Utils.hpp"
#include "/Users/teddy/Downloads/atm-risk-testing/models/BasicVaR.cpp"
#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <stdexcept>
#include <filesystem>
#include <cstring>

namespace fs = std::filesystem;

struct Cmd {
    std::string outdir = "reports";
};

Cmd parse(int argc, char** argv) {
    Cmd c;
    for (int i = 1; i < argc; ++i) {
        if ((std::strcmp(argv[i], "--outdir") == 0 || std::strcmp(argv[i], "-o") == 0) && i + 1 < argc) {
            c.outdir = argv[++i];
        }
    }
    return c;
}

int main(int argc, char** argv) {
    auto cmd = parse(argc, argv);
    fs::create_directories(cmd.outdir);
    atm::CSVReporter csv(cmd.outdir + "/results.csv");
    atm::JUnitXmlReporter junit(cmd.outdir + "/junit.xml");
    atm::JsonReporter json(cmd.outdir + "/summary.json");

    // Load PnL data from data/pnl_volatility.csv
    std::vector<double> pnl;
    std::ifstream file("data/pnl_volatility.csv");
    if (!file.is_open()) {
        throw std::runtime_error("Failed to open data/pnl_volatility.csv");
    }

    std::string line;
    bool first_line = true;
    while (std::getline(file, line)) {
        // Skip the header line ("pnl")
        if (first_line) {
            first_line = false;
            continue;
        }
        // Skip empty lines
        if (line.empty()) {
            continue;
        }
        try {
            // Convert line to double
            double value = std::stod(line);
            pnl.push_back(value);
        } catch (const std::exception& e) {
            throw std::runtime_error("Failed to parse value in data/pnl_volatility.csv: " + line);
        }
    }
    file.close();

    // Verify data was loaded
    if (pnl.size() < 251) { // Need at least 250 + 1 for rolling window tests
        throw std::runtime_error("Insufficient data in data/pnl_volatility.csv: got " + 
                                 std::to_string(pnl.size()) + ", need at least 251");
    }

    // Create model & context
    BasicVaR95 model(250);
    std::string datasetId = "pnl_volatility";
    std::string testType = "VaRBacktest";

    // Define test(s)
    atm::Runner runner;

    // Test 1: Exception rate close to 5% for 95% VaR
    runner.addTest(atm::RiskTest{
        "VaR exception rate within bounds",
        [&]() -> std::vector<atm::MetricRecord> {
            std::vector<atm::MetricRecord> recs;
            // Rolling 250-day VaR vs next-day exceptions
            size_t window = 250;
            size_t N = pnl.size();
            size_t count = 0;
            size_t exceptions = 0;
            for (size_t t = window; t + 1 < N; ++t) {
                double var = model.riskMeasure({pnl.begin() + (t - window), pnl.begin() + t});
                double next = pnl[t + 1];
                if (next < -var) ++exceptions;
                ++count;
            }
            double rate = (count == 0) ? 0.0 : static_cast<double>(exceptions) / static_cast<double>(count);

            // Regulatory-style bounds (Kupiec-like rough bounds: 5% ± 2% for demo)
            double lower = 0.03, upper = 0.07;
            bool ok = (rate >= lower && rate <= upper);

            atm::MetricRecord r;
            r.timestampUtc = atm::nowUtcIso();
            r.modelId = model.id();
            r.modelVersion = model.version();
            r.datasetId = datasetId;
            r.testType = testType;
            r.metricName = "var_exception_rate";
            r.metricValue = rate;
            r.threshold = upper; // we treat threshold as the upper bound in this record
            r.pass = ok;
            r.notes = "target ~0.05 (95% VaR); acceptable range [0.03, 0.07]";
            recs.push_back(r);
            return recs;
        }
    });

    // Test 2: Stability check — VaR should not explode
    runner.addTest(atm::RiskTest{
        "VaR magnitude sanity",
        [&]() -> std::vector<atm::MetricRecord> {
            std::vector<atm::MetricRecord> recs;
            double var = model.riskMeasure({pnl.end() - 250, pnl.end()});
            bool ok = (var > 0.5 && var < 5.0); // Adjusted for volatility clustering
            atm::MetricRecord r;
            r.timestampUtc = atm::nowUtcIso();
            r.modelId = model.id();
            r.modelVersion = model.version();
            r.datasetId = datasetId;
            r.testType = "Sanity";
            r.metricName = "VaR95_value";
            r.metricValue = var;
            r.threshold = 5.0;
            r.pass = ok;
            r.notes = "Expected reasonable VaR for volatile PnL data";
            recs.push_back(r);
            return recs;
        }
    });

    runner.addReporter(&csv);
    runner.addReporter(&junit);
    runner.addReporter(&json);

    int rc = runner.runAll();
    std::cout << "Completed tests. See reports in: " << cmd.outdir << "\n";
    return rc;
}