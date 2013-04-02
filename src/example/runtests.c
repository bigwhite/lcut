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

/*
 * A case that test nothing and succeed
 */
void tc_null_test(lcut_tc_t *tc, void *data) {

}

int main() {
    lcut_ts_t   *suite = NULL;
    LCUT_TEST_BEGIN("a null test", NULL, NULL);

    LCUT_TS_INIT(suite, "a null test suite", NULL, NULL);
    LCUT_TC_ADD(suite, "null test", tc_null_test, NULL, NULL, NULL);
    LCUT_TS_ADD(suite);

    LCUT_TEST_RUN();
    LCUT_TEST_REPORT();
    LCUT_TEST_END();

    LCUT_TEST_RESULT();
}
