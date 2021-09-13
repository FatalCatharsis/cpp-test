//
// Created by wyken on 9/12/2021.
//

#include "TestRunner.h"

void TestRunner::add(const shared_ptr<BaseTestFixture>& testFixture) {
    fixtures.emplace_back(testFixture);
}

void TestRunner::run() {
    int totalSuccess{};
    int totalFailures{};
    time_point<steady_clock> p = steady_clock::now();
    for (auto& fixture : fixtures) {
        FixtureReport report = fixture->run();
        totalSuccess += report.success;
        totalFailures += report.failure;
    }
    duration<double, milli> diff (steady_clock::now() - p);

    cout << "All Tests result: " << (totalFailures == 0 ? "SUCCESS" : "SOME FAILURES") << "\n"
    << "\t" << totalSuccess + totalFailures << " tests performed\n"
    << "\t" << totalSuccess  << " success(es)\n"
    << "\t" << totalFailures << " failure(s)\n"
    << "\t" << static_cast<float>(totalSuccess)/(totalSuccess + totalFailures) * 100 << "% pass rate\n"
    << "\t" << diff.count() << "ms elapsed time" << endl;
}