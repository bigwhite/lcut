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

#include "lcut.h"

void tc_str_equal(lcut_tc_t *tc, void *data) {
    LCUT_STR_EQUAL(tc, "hello", "hello");
    LCUT_STR_EQUAL(tc, NULL, NULL);

    /* Failed assert below:
     * LCUT_STR_EQUAL(tc, "hello", NULL); 
     * LCUT_STR_EQUAL(tc, NULL, "hello"); 
     * LCUT_STR_EQUAL(tc, "hello", "");
     * LCUT_STR_EQUAL(tc, "", "hello");
     */
}

void tc_str_nequal(lcut_tc_t *tc, void *data) {
    LCUT_STR_NEQUAL(tc, "hello", "hello1");
    LCUT_STR_NEQUAL(tc, "hello", NULL);
    LCUT_STR_NEQUAL(tc, NULL, "hello");
    /* Failed assert below: 
     * LCUT_STR_NEQUAL(tc, "hello", "hello");
     * LCUT_STR_NEQUAL(tc, NULL, NULL);
     */
}

int main() {
    lcut_ts_t   *suite = NULL;
    LCUT_TEST_BEGIN("a string equal and unequal test", NULL, NULL);

    LCUT_TS_INIT(suite, "a string equal test suite", NULL, NULL);
    LCUT_TC_ADD(suite, "string equal test", tc_str_equal, NULL, NULL, NULL);
    LCUT_TS_ADD(suite);

    LCUT_TS_INIT(suite, "a string unequal test suite", NULL, NULL);
    LCUT_TC_ADD(suite, "string nequal test", tc_str_nequal, NULL, NULL, NULL);
    LCUT_TS_ADD(suite);

    LCUT_TEST_RUN();
    LCUT_TEST_REPORT();
    LCUT_TEST_END();

    LCUT_TEST_RESULT();
}
