//
// Created by wyken on 9/12/2021.
//

#ifndef SMNT_MATH_TESTRUNNER_H
#define SMNT_MATH_TESTRUNNER_H

#include "TestFixture.h"

class TestRunner {
    vector<shared_ptr<BaseTestFixture>> fixtures;
public:
    void add(const shared_ptr<BaseTestFixture>& testFixture);

    void run();
};

#endif //SMNT_MATH_TESTRUNNER_H
