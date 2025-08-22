#include "atm/Runner.hpp"
#include <chrono>
#include <ctime>
#include <iomanip>
#include <sstream>

namespace atm {

std::string nowUtcIso() {
    using clock = std::chrono::system_clock;
    auto now = clock::now();
    std::time_t t = clock::to_time_t(now);
    std::tm tm{};
#if defined(_WIN32)
    gmtime_s(&tm, &t);
#else
    gmtime_r(&t, &tm);
#endif
    std::ostringstream oss;
    oss << std::put_time(&tm, "%Y-%m-%dT%H:%M:%SZ");
    return oss.str();
}

int Runner::runAll() const {
    std::vector<MetricRecord> all;
    for (const auto& t : tests_) {
        auto recs = t.fn();
        all.insert(all.end(), recs.begin(), recs.end());
    }
    for (auto* r : reporters_) r->write(all);

    // Return non-zero if any failed (useful for CI gating)
    for (const auto& r : all) if (!r.pass) return 1;
    return 0;
}

} // namespace atm
