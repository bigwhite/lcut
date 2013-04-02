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

#include <stdio.h>

#include "lcut.h"

int foo(int inparameter, int *outparameter) {
    (*outparameter) = (int)LCUT_MOCK_ARG();
    return (int)LCUT_MOCK_RETV();
}

int bar_invoke_foo_once(int *outparameter) {
    int i = 10;
    int ret = foo(5, &i);
    (*outparameter) = i;
    return ret;
}

int bar_invoke_foo_multi_times(int *outparameter) {
    int i, j, k;
    int r1, r2, r3;

    r1 = foo(5, &i);

    r2 = foo(5, &j);

    r3 = foo(5, &k);

    (*outparameter) = i + j + k;
    return r1 + r2 + r3;
}

void tc_test_bar_invoke_foo_once(lcut_tc_t *tc, void *data) {
    int i;
    LCUT_RETV_RETURN(foo, 7);
    LCUT_ARG_RETURN(foo, 5);

    LCUT_INT_EQUAL(tc, 7, bar_invoke_foo_once(&i));
    LCUT_INT_EQUAL(tc, 5, i); 

    LCUT_RETV_RETURN(foo, 13);
    LCUT_ARG_RETURN(foo, 17);

    LCUT_INT_EQUAL(tc, 13, bar_invoke_foo_once(&i));
    LCUT_INT_EQUAL(tc, 17, i); 
}

void tc_test_bar_invoke_foo_multi_times_using_return_count(lcut_tc_t *tc, void *data) {
    int i;
    LCUT_RETV_RETURN_COUNT(foo, 13, 3);
    LCUT_ARG_RETURN_COUNT(foo, 17, 3);

    LCUT_INT_EQUAL(tc, 39, bar_invoke_foo_multi_times(&i));
    LCUT_INT_EQUAL(tc, 51, i); 
}

void tc_test_bar_invoke_foo_multi_times_using_always_return_mock(lcut_tc_t *tc, void *data) {
    int i;

    LCUT_RETV_RETURN(foo, 7);
    LCUT_ARG_RETURN(foo, 5);

    LCUT_INT_EQUAL(tc, 7, bar_invoke_foo_once(&i));
    LCUT_INT_EQUAL(tc, 5, i); 

    LCUT_RETV_RETURN_COUNT(foo, 13, -1);/* make foo always return 13 */
    LCUT_ARG_RETURN_COUNT(foo, 17, -1); /* make foo's outparameter always evaluated to 17 */

    LCUT_INT_EQUAL(tc, 13, bar_invoke_foo_once(&i));
    LCUT_INT_EQUAL(tc, 17, i); 

    LCUT_INT_EQUAL(tc, 13, bar_invoke_foo_once(&i));
    LCUT_INT_EQUAL(tc, 17, i); 

    LCUT_INT_EQUAL(tc, 39, bar_invoke_foo_multi_times(&i));
    LCUT_INT_EQUAL(tc, 51, i); 

}

int main() {
    lcut_ts_t   *suite = NULL;

    LCUT_TEST_BEGIN("lcut mock interface test", NULL, NULL);

    LCUT_TS_INIT(suite, "mock unit test suite", NULL, NULL);
    LCUT_TC_ADD(suite, "tc_test_bar_invoke_foo_multi_times_using_always_return_mock!",
                tc_test_bar_invoke_foo_multi_times_using_always_return_mock, NULL, NULL, NULL);
    LCUT_TC_ADD(suite, "tc_test_bar_invoke_foo_multi_times_using_return_count!",
                tc_test_bar_invoke_foo_multi_times_using_return_count , NULL, NULL, NULL);
    LCUT_TC_ADD(suite, "tc_test_bar_invoke_foo_once!",
                tc_test_bar_invoke_foo_once, NULL, NULL, NULL);
    LCUT_TS_ADD(suite);

    LCUT_TEST_RUN();
    LCUT_TEST_REPORT();
    LCUT_TEST_END();

    LCUT_TEST_RESULT();
}
