//
// Created by wyken on 9/12/2021.
//

#include "TestFixture.h"


BaseTestFixture::BaseTestFixture(string _fixtureName) :
fixtureName(std::move(_fixtureName))
{}

FixtureReport BaseTestFixture::run() {
    cout << "Starting Fixture : " << fixtureName << endl;

    time_point<steady_clock> p = steady_clock::now();
    invokeTests();
    duration<double, milli> diff (steady_clock::now() - p);
    cout << "\tReport: " << report.success << " success(s) " << report.failure << " failure(s) runtime: " << diff.count() << "ms" << endl;
    return report;
}

void BaseTestFixture::test(const string& testName, std::function<void()> func) {
    time_point<steady_clock> p = steady_clock::now();
    try {
        func();
        duration<double, milli> diff (steady_clock::now() - p);
        cout << "\tTest: \"" << testName << "\" " << "SUCCESS time:" << diff.count() << "ms" << endl;
        report.success++;
    } catch (exception& assertionFailure) {
        duration<double, milli> diff (steady_clock::now() - p);
        cout << "\tTest: \"" << testName << "\" " << "FAILURE time:" << diff.count() << "ms" << endl;
        cout << assertionFailure.what() << endl;
        report.failure++;
    }
    report.totalTest++;
}

void BaseTestFixture::assertTrue(const string& assertionText, bool test) {
    if (!test) {
        throw AssertionFailure(assertionText);
    }
}

void BaseTestFixture::assertFalse(const string& assertionText, bool test) {
    if (test) {
        throw AssertionFailure(assertionText);
    }
}