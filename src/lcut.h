/*
 * Copyright (c) 2005-2010 Tony Bai <bigwhite.cn@gmail.com>
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * 
 * http://www.apache.org/licenses/LICENSE-2.0
 * 
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

/*
 * @file lcut.h
 *
 * @brief a Lightweight C Unit Testing framework,
 *        working under single process and single-threaded environment
 *
 * Here is a diagram to illustrate the orgnization of LCUT
 *     A logical Test
 *           |
 *           |
 *      +-------------+
 *     TS-1    ...  TS-N
 *      |             |
 *      |             |
 * +-------+ ...   +--------+
 * TC-1   TC-N     TC-1     TC-N
 *
 * TS - Test Suite, TC - Test Case
 */

#ifndef _LCUT_H
#define _LCUT_H

#ifdef _cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h> /* for exit */

#include "apr_ring.h"

#define LCUT_MAX_NAME_LEN 128
#define LCUT_MAX_STR_LEN 128

#define LCUT_LOGO \
"*********************************************************\n\
\tLCUT -- Lightweight C Unit Testing framework\n \
\t\t By Tony Bai\n\
*********************************************************"

#define SUCCESS_TIP_FMT "\t\tCase '%s': Passed\n"
#define FAILURE_TIP_FMT "\t\t\033[31mCase '%s': Failure occur in %s, %d line in file %s, %s\033[0m\n"

#define REDBAR \
"\n=======================\n\
\t\033[31mRED BAR!\033[0m\n\
=======================\n"

#define GREENBAR \
"\n==========================\n\
\t\033[32mGREEN BAR!\033[0m\n\
==========================\n"

/* indicates the result of the Test Case executing */
enum {
    TEST_CASE_SUCCESS = 0,
    TEST_CASE_FAILURE = 1
};

typedef struct lcut_tc_t lcut_tc_t;
typedef void (*tc_func)(lcut_tc_t *tc, void *data);
typedef void (*fixture_func)(void);

struct lcut_tc_t {
    APR_RING_ENTRY(lcut_tc_t)   link;
    char                        desc[LCUT_MAX_NAME_LEN];    /* the description literal of the test case */
    tc_func                     func;                       /* the executive body of the test case */
    void                        *para;                      /* the parameter passed into the func above */
    fixture_func                before;                     /* invoked before the test case func executed */
    fixture_func                after;                      /* invoked after the test case func executed */
    int                         status;                     /* the result of th test case executing*/
    char                        fname[LCUT_MAX_NAME_LEN];   /* indicates the file name which the case lies in */
    char                        fcname[LCUT_MAX_NAME_LEN];  /* indicates the func name when the case failed */
    int                         line;                       /* indicates the line number when the case failed */
    char                        reason[LCUT_MAX_STR_LEN];   /* string literal indicates the failed reason */
};
typedef APR_RING_HEAD(lcut_tc_head_t, lcut_tc_t) lcut_tc_head_t;

typedef struct lcut_ts_t {
    APR_RING_ENTRY(lcut_ts_t)   link;
    char                        desc[LCUT_MAX_NAME_LEN];    /* the description literal of the test suite */
    lcut_tc_head_t              tc_head;                    /* the head node of the test case ring */
    fixture_func                setup;                      /* setup function */
    fixture_func                teardown;                   /* teardown fucntion */
    int                         ran;                        /* the total count of test cases */
    int                         failed;                     /* the count of failed test case */
} lcut_ts_t;
typedef APR_RING_HEAD(lcut_ts_head_t, lcut_ts_t) lcut_ts_head_t;

typedef struct lcut_test_t {
    char                        desc[LCUT_MAX_NAME_LEN];    /* the description of a logical unit test */
    lcut_ts_head_t              ts_head;                    /* the head node of the test suite ring */
    fixture_func                setup;                      /* most top-level setup for the logic unit test */
    fixture_func                teardown;                   /* most top-level teardown for the logic unit test */
    int                         suites;                     /* the total count of test suites */
    int                         cases;                      /* the total count of test cases */
} lcut_test_t;

int lcut_test_init(lcut_test_t **test, const char *title, fixture_func setup, fixture_func teardown);
void lcut_test_destroy(lcut_test_t **test);
int lcut_ts_init(lcut_ts_t **ts, const char *title, fixture_func setup, fixture_func teardown);
void lcut_ts_add(lcut_test_t *test, lcut_ts_t *ts);
int lcut_tc_add(lcut_ts_t *ts, const char *title, tc_func func,
                void *para, fixture_func before, fixture_func after);
void lcut_test_run(lcut_test_t *test, int *result);
void lcut_test_report(lcut_test_t *test);

/*
 * LCUT_TEST_BEGIN and LCUT_TEST_END should be used in pair!
 */
#define LCUT_TEST_BEGIN(s, setup, teardown) \
    lcut_test_t *_cut_test     = NULL; \
    int   _cut_status; \
    int   _cut_result    = TEST_CASE_SUCCESS; \
    if ((_cut_status = lcut_test_init(&_cut_test, (s), (setup), (teardown))) != 0) { \
        printf("[LCUT]: test init failed!, errcode[%d]\n", _cut_status); \
        exit(1); \
    }

#define LCUT_TEST_END() do { \
        lcut_test_destroy(&_cut_test); \
    } while(0)

/*
 * initialize a Test Suite
 *
 * p -- lcut_ts_t*
 * s -- suite description
 * setup -- setup fixture function
 * teardown -- teardown fixture function
 */
#define LCUT_TS_INIT(p, s, setup, teardown) do { \
        if ((_cut_status = lcut_ts_init(&(p), (s), (setup), (teardown))) != 0) { \
            printf("[LCUT]: test suite init failed!, errcode[%d]\n", _cut_status); \
            exit(1); \
        } \
    } while(0)

/*
 * Add a Test Suite to a logical Test
 *
 * p -- lcut_ts_t*
 *
 * !notice: you must not add a test case into a test suite
 * which has already been added into a logical test!
 */
#define LCUT_TS_ADD(p) do { \
        lcut_ts_add(_cut_test, (p)); \
    } while(0)

/*
 * Add a test case to a test suite
 *
 * p -- lcut_ts_t*
 * s -- test case description
 * f -- test case function
 * e -- extra parameter
 */
#define LCUT_TC_ADD(p, s, f, e, before, after) do { \
        if ((_cut_status = lcut_tc_add((p), (s), (f), (e), (before), (after))) != 0) { \
            printf("[LCUT]: test case add failed!, errcode[%d]\n", _cut_status); \
            exit(1); \
        } \
    } while(0)

/*
 * Run a logical unit test
 */
#define LCUT_TEST_RUN() do { \
        lcut_test_run(_cut_test, &_cut_result); \
    } while(0)

/*
 * Report the result of the logical test
 */
#define LCUT_TEST_REPORT() do { \
        lcut_test_report(_cut_test); \
    } while(0)

/*
 * Get the result of a logical test
 *
 * in order to integrated with a Continuous integration tool, e.g. cruisecontrol.rb
 */
#define LCUT_TEST_RESULT() do { \
        exit(_cut_result); \
    } while(0)

void lcut_int_equal(lcut_tc_t *tc, const int expected, const int actual, int lineno,
                    const char *fcname, const char *fname);
void lcut_int_nequal(lcut_tc_t *tc, const int expected, const int actual, int lineno,
                     const char *fcname, const char *fname);
void lcut_str_equal(lcut_tc_t *tc, const char *expected, const char *actual, int lineno,
                    const char *fcname, const char *fname);
void lcut_str_nequal(lcut_tc_t *tc, const char *expected, const char *actual, int lineno, 
                    const char *fcname, const char *fname);
void lcut_assert(lcut_tc_t *tc, const char *msg, int condition,
                 int lineno, const char *fcname, const char *fname);
void lcut_true(lcut_tc_t *tc, int condition,
               int lineno, const char *fcname, const char *fname);

#define LCUT_INT_EQUAL(tc, expected, actual) do { \
        lcut_int_equal(tc, expected, actual, __LINE__, __FUNCTION__, __FILE__); \
    } while(0)

#define LCUT_INT_NEQUAL(tc, expected, actual) do { \
        lcut_int_nequal(tc, expected, actual, __LINE__, __FUNCTION__, __FILE__); \
    } while(0)

#define LCUT_STR_EQUAL(tc, expected, actual) do { \
        lcut_str_equal(tc, expected, actual, __LINE__, __FUNCTION__, __FILE__); \
    } while(0)

#define LCUT_STR_NEQUAL(tc, expected, actual) do { \
        lcut_str_nequal(tc, expected, actual, __LINE__, __FUNCTION__, __FILE__); \
    } while(0)

#define LCUT_ASSERT(tc, msg, condition) do { \
        lcut_assert(tc, msg, (condition), __LINE__, __FUNCTION__, __FILE__); \
    } while(0)

#define LCUT_TRUE(tc, condition) do { \
        lcut_true(tc, condition, __LINE__, __FUNCTION__, __FILE__); \
    } while(0)

/*
 * mock symbol list
 *
 * ------------
 * | symbol-#0|-> value_list
 * ------------
 * | symbol-#1|-> value_list
 * ------------
 * | symbol-#2|-> value_list
 * ------------
 * | ... ...  |-> value_list
 * ------------
 * | symbol-#n|-> value_list
 * ------------
 */

typedef struct lcut_value_t {
    APR_RING_ENTRY(lcut_value_t)    link;
    void                            *value;
} lcut_value_t;
typedef APR_RING_HEAD(lcut_value_head_t, lcut_value_t) lcut_value_head_t;

typedef struct lcut_symbol_t {
    APR_RING_ENTRY(lcut_symbol_t)   link;
    char                            desc[LCUT_MAX_NAME_LEN];
    int                             obj_type;
    int                             always_return_flag; /* 1: always return the same value; 0(default) */
    void*                           value;
    lcut_value_head_t               value_list;
} lcut_symbol_t;
typedef APR_RING_HEAD(lcut_symbol_head_t, lcut_symbol_t) lcut_symbol_head_t;

void* lcut_mock_obj(const char *fcname, int lineno, const char *fname, int obj_type); 
void lcut_mock_obj_return(const char *symbol_name, void *value, const char *fcname, 
                          int lineno, const char *fname, int obj_type, int count);

#define MOCK_ARG                0x0
#define MOCK_RETV               0x1

#define LCUT_MOCK_ARG() lcut_mock_obj(__FUNCTION__, __LINE__, __FILE__, MOCK_ARG)
#define LCUT_MOCK_RETV() lcut_mock_obj(__FUNCTION__, __LINE__, __FILE__, MOCK_RETV)

#define LCUT_ARG_RETURN(fcname, value) do { \
        lcut_mock_obj_return(#fcname, (void*)value, __FUNCTION__, __LINE__, __FILE__, MOCK_ARG, 1); \
    } while(0);

#define LCUT_ARG_RETURN_COUNT(fcname, value, count) do { \
        lcut_mock_obj_return(#fcname, (void*)value, __FUNCTION__, __LINE__, __FILE__, MOCK_ARG, count); \
    } while(0);

#define LCUT_RETV_RETURN(fcname, value) do { \
        lcut_mock_obj_return(#fcname, (void*)value, __FUNCTION__, __LINE__, __FILE__, MOCK_RETV, 1); \
    } while(0);

#define LCUT_RETV_RETURN_COUNT(fcname, value, count) do { \
        lcut_mock_obj_return(#fcname, (void*)value, __FUNCTION__, __LINE__, __FILE__, MOCK_RETV, count); \
    } while(0);

#ifdef _cplusplus
}
#endif

#endif /* _LCUT_H */
