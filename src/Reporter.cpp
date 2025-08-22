#include "atm/Reporter.hpp"
#include <filesystem>
#include <iomanip>

namespace fs = std::filesystem;

namespace atm {

static void ensure_parent(const std::string& p) {
    fs::create_directories(fs::path(p).parent_path());
}

void CSVReporter::write(const std::vector<MetricRecord>& records) {
    ensure_parent(outPath_);
    std::ofstream out(outPath_);
    out << "timestamp_utc,model_id,model_version,dataset_id,test_type,metric_name,metric_value,threshold,pass,notes\n";
    out << std::boolalpha;
    for (const auto& r : records) {
        out << r.timestampUtc << "," << r.modelId << "," << r.modelVersion << ","
            << r.datasetId << "," << r.testType << "," << r.metricName << ","
            << std::setprecision(10) << r.metricValue << "," << r.threshold << ","
            << (r.pass ? "true" : "false") << "," << r.notes << "\n";
    }
}

void JUnitXmlReporter::write(const std::vector<MetricRecord>& records) {
    ensure_parent(outPath_);
    std::ofstream out(outPath_);
    size_t failures = 0;
    for (const auto& r : records) if (!r.pass) ++failures;
    out << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n";
    out << "<testsuite name=\"atm-risk-testing\" tests=\"" << records.size()
        << "\" failures=\"" << failures << "\">\n";
    for (const auto& r : records) {
        out << "  <testcase classname=\"" << r.modelId << "\" name=\"" << r.metricName << "\">\n";
        if (!r.pass) {
            out << "    <failure message=\"metric threshold exceeded\">"
                << "value=" << r.metricValue << ", threshold=" << r.threshold
                << ", dataset=" << r.datasetId << ", notes=" << r.notes
                << "</failure>\n";
        }
        out << "  </testcase>\n";
    }
    out << "</testsuite>\n";
}

void JsonReporter::write(const std::vector<MetricRecord>& records) {
    ensure_parent(outPath_);
    std::ofstream out(outPath_);
    out << "{\n  \"records\": [\n";
    for (size_t i = 0; i < records.size(); ++i) {
        const auto& r = records[i];
        out << "    {\n"
            << "      \"timestamp_utc\": \"" << r.timestampUtc << "\",\n"
            << "      \"model_id\": \"" << r.modelId << "\",\n"
            << "      \"model_version\": \"" << r.modelVersion << "\",\n"
            << "      \"dataset_id\": \"" << r.datasetId << "\",\n"
            << "      \"test_type\": \"" << r.testType << "\",\n"
            << "      \"metric_name\": \"" << r.metricName << "\",\n"
            << "      \"metric_value\": " << r.metricValue << ",\n"
            << "      \"threshold\": " << r.threshold << ",\n"
            << "      \"pass\": " << (r.pass ? "true" : "false") << ",\n"
            << "      \"notes\": \"" << r.notes << "\"\n"
            << "    }" << (i + 1 < records.size() ? "," : "") << "\n";
    }
    out << "  ]\n}\n";
}

} // namespace atm
