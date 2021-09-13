//
// Created by wyken on 9/6/2021.
//

#ifndef SMNT_MATH_TESTFIXTURE_H
#define SMNT_MATH_TESTFIXTURE_H

#include <functional>
#include <string>
#include <utility>
#include <vector>
#include <iostream>
#include <chrono>

using namespace std;
using namespace std::chrono;

class AssertionFailure : public exception {
    string errorText;
public:
    explicit AssertionFailure(string  error) :
    errorText(std::move(error))
    {}

    [[nodiscard]] const char *what() const override {
        return errorText.c_str();
    }
};

struct FixtureReport {
    int totalTest{};
    int success{};
    int failure{};
};

class BaseTestFixture {
protected:
    string fixtureName;
    FixtureReport report;

    virtual void invokeTests() = 0;

public:
    explicit BaseTestFixture(string  _fixtureName);

    FixtureReport run();

    void test(const string& testName, std::function<void()> func);

    void assertTrue(const string& assertionText, bool test);

    void assertFalse(const string& assertionText, bool test);


    template<class Value1, class Value2>
    void assertEquals(const string& assertionText, Value1 value1, Value2 value2) {
        if (value1 != value2) {
            stringstream ss(assertionText);
            ss << "\n\t\t" << "Actual value was [" << value1 << "] and the expected value was [" << value2 << "]" << endl;
            throw AssertionFailure(ss.str());
        }
    }

    template<class T>
    void assertExceptionThrown(const string& assertionText, const std::function<void()>& test) {
        try {
            test();
        } catch(T& exception) {
            return;
        }

        stringstream ss(assertionText);
        ss << "\n\t\t" << "The exception of type " << typeid(T).name() << " was not thrown when it was expected." << endl;
        throw AssertionFailure(ss.str());
    }
};

template<class Context>
class TestFixture : public BaseTestFixture {
public:
    explicit TestFixture(string _fixtureName) : BaseTestFixture(_fixtureName) {}

    virtual void init(Context& context) {};

    using BaseTestFixture::test;
    void test(const string& testName, std::function<void(Context&)> func) {
        Context context;
        init(context);

        time_point<steady_clock> p = steady_clock::now();
        try {
            func(context);
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
};

#endif //SMNT_MATH_TESTFIXTURE_H
