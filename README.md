LCUT - Lightweight C Unit Test Framework
========================================

Recent News
-----------

 - 2013-04-02 fork a copy to https://github.com/bigwhite/lcut.git.
 - 2012-04-13 lcut 0.3.0 Released!
 - 2012-04-10 lcut 0.3.0-rc1 published!
   - add LCUT_ARG_RETURN_COUNT and LCUT_RETV_RETURN_COUNT macros.
 - 2010-12-16 lcut 0.2.0 Released!
 - 2010-10-29 lcut 0.2.0-beta1 published!
   - add mock support
 - 2010-09-30 The first verison of lcut - LCUT 0.1.0 Released.

Motivation
----------

In late 2005，the "TDD" concept was so popular and there were many strong and mature unit testing framework available 
for Java programmers, C# programmers and even python programmers, But there were few for C programmers. I have googled 
some c unit testing framework and gave me a shot, but eventually none of them made me comfortable. so I decided 
to implement a new one for my daily work and then lcut born.

Introduction
------------

lcut is short for "Lightweight C Unit Test framework". lightweight means it is very small and easy to learn&use. 
The framework is inspired by the well-known JUnit which is invented by Kent Beck.The structure of lcut could be 
illustrated as below:

    A logical Test
          |  
          |  
      +-------------+
      TS-1    ...  TS-N
      |             |  
      |             |  
    +-------+ ... +--------+
    TC-1 TC-N TC-1 TC-N
    
    TS - Test Suite，TC - Test Case

A unit test using lcut is divided into three levels: logical test, test suite and test case. A logical test contains 
several test suites and each test suite also contains several test cases. test case is the most basic and the smallest
unit in this framework. And the three-level concept is helpful for you to organize your unit testing well.

Feature list
------------

 - small and easy to learn&use
 - inspired by xUnit
 - three-level concepts to help organizing your test well and each level could have independent setup and teardown
   fixtures
 - provide mock support (inspired by cmockery)

For more information, please open and read the lcut project wiki. lcut_user_guide and its Chinese version are ready 
for you.

A Typical Example
-------
    /* calculator_test.c */
    #include "lcut.h"

    extern int add(int a, int b);
    extern int subtract(int a, int b);
    extern int multiply(int a, int b);
    extern int divide(int a, int b);

    void tc_add(lcut_tc_t *tc, void *data) {
        LCUT_INT_EQUAL(tc, 10, add(2, 8));
        LCUT_INT_EQUAL(tc, -6, add(2, -8));
        LCUT_INT_EQUAL(tc, 0, add(2, -2));
        LCUT_INT_NEQUAL(tc, 1, add(2, -2));

        LCUT_TRUE(tc, 0 == add(2, -2));
        LCUT_ASSERT(tc, "2 + (-2) should equal to 0", 0 == add(2, -2));
    }

    void tc_subtract(lcut_tc_t *tc, void *data) {
        LCUT_INT_EQUAL(tc, 6, subtract(8, 2));
        LCUT_INT_EQUAL(tc, 10, subtract(2, -8));
        LCUT_INT_EQUAL(tc, 0, subtract(2, 2));
    }

    void tc_multiply(lcut_tc_t *tc, void *data) {
        LCUT_INT_EQUAL(tc, 16, multiply(8, 2));
        LCUT_INT_EQUAL(tc, -16, multiply(2, -8));
        LCUT_INT_EQUAL(tc, 0, multiply(0, 2));
    }

    void tc_divide(lcut_tc_t *tc, void *data) {
        LCUT_INT_EQUAL(tc, 4, divide(8, 2));
        LCUT_INT_EQUAL(tc, 0, divide(0, 8));
        LCUT_INT_EQUAL(tc, 1, divide(2, 2));
    }

    int main() {
        lcut_ts_t   *suite = NULL;
        LCUT_TEST_BEGIN("a simple calculator test", NULL, NULL);

        LCUT_TS_INIT(suite, "a simple calculator unit test suite", NULL, NULL);
        LCUT_TC_ADD(suite, "add test case", tc_add, NULL, NULL, NULL);
        LCUT_TC_ADD(suite, "subtract test case", tc_subtract, NULL, NULL, NULL);
        LCUT_TC_ADD(suite, "multiply test case", tc_multiply, NULL, NULL, NULL);
        LCUT_TC_ADD(suite, "divide test case", tc_divide, NULL, NULL, NULL);
        LCUT_TS_ADD(suite);

        LCUT_TEST_RUN();
        LCUT_TEST_REPORT();
        LCUT_TEST_END();

        LCUT_TEST_RESULT();
    }

The result of the example could be:

    *********************************************************
         LCUT -- Lightweight C Unit Testing framework
 		                By Tony Bai
    ********************************************************* 
    Unit Test for 'a simple calculator test':

	   Suite <a simple calculator unit test suite>: 
		    Case 'add test case': Passed
		    Case 'subtract test case': Passed
		    Case 'multiply test case': Passed
		    Case 'divide test case': Passed

    Summary: 
	    Total Suites: 1 
	    Failed Suites: 0 
	    Total Cases: 4 
	    Failed Cases: 0 

    ==========================
	       GREEN BAR!
    ==========================

Build
------
 - Download the source code package
 - unzip the package
 - configure->make->make install
 
if you want to compile in 64-bit mode, pass "CPPFLAGS=-m64 LDFLAGS=-m64" to configure.
sometimes, you may encounter such error:

	mv: cannot stat `.deps/lcut.Tpo': No such file or directory
	make[1]: *** [lcut.lo] Error 1
	
a solution for this is execute "libtoolize -f" before "configure".

User Guide
-------------
For more information, please open and read the [lcut_user_guide](https://github.com/bigwhite/lcut/wiki/lcut-user-guide).And Chinese version is [here](https://github.com/bigwhite/lcut/wiki/lcut%E7%94%A8%E6%88%B7%E6%8C%87%E5%8D%97).

