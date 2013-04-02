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

#include "database.h"
#include "lcut.h"

extern int get_total_count_of_employee();

database_conn *connect_to_database(const char *user,
                                   const char *passwd,
                                   const char *serviceid) {
    return (database_conn*)LCUT_MOCK_RETV();
}

int table_row_count(const database_conn *conn,
                    const char *table_name,
                    int *total_count) {
    (*total_count) = (int)LCUT_MOCK_ARG();
    return (int)LCUT_MOCK_RETV();
}


void tc_get_total_count_of_employee_ok(lcut_tc_t *tc, void *data) {
    LCUT_RETV_RETURN(connect_to_database, 0x1234);
    LCUT_ARG_RETURN(table_row_count, 5);
    LCUT_RETV_RETURN(table_row_count, 0);

    LCUT_INT_EQUAL(tc, 5, get_total_count_of_employee());
}

void tc_get_total_count_of_employee_db_conn_failed(lcut_tc_t *tc, void *data) {
    LCUT_RETV_RETURN(connect_to_database, NULL);

    LCUT_INT_EQUAL(tc, -1, get_total_count_of_employee());
}

void tc_get_total_count_of_employee_fetch_rowcount_failed(lcut_tc_t *tc, void *data) {
    LCUT_RETV_RETURN(connect_to_database, 0x1234);
    LCUT_ARG_RETURN(table_row_count, 5);
    LCUT_RETV_RETURN(table_row_count, -1);

    LCUT_INT_EQUAL(tc, -1, get_total_count_of_employee());
}

int main() {
    lcut_ts_t   *suite = NULL;

    LCUT_TEST_BEGIN("product database test", NULL, NULL);

    LCUT_TS_INIT(suite, "product database unit test - normal result suite", NULL, NULL);
    LCUT_TC_ADD(suite, "get total count of employees ok!", tc_get_total_count_of_employee_ok, NULL, NULL, NULL);
    LCUT_TS_ADD(suite);

    LCUT_TS_INIT(suite, "product database unit test - exceptional result suite", NULL, NULL);
    LCUT_TC_ADD(suite, "tc when database conn failed", tc_get_total_count_of_employee_db_conn_failed, 
                NULL, NULL, NULL);
    LCUT_TC_ADD(suite, "tc when fetch row counts failed", tc_get_total_count_of_employee_fetch_rowcount_failed, 
                NULL, NULL, NULL);
    LCUT_TS_ADD(suite);

    LCUT_TEST_RUN();
    LCUT_TEST_REPORT();
    LCUT_TEST_END();

    LCUT_TEST_RESULT();
}
