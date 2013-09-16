# Makefile.am

AM_CPPFLAGS = -std=c99 -Wall -fno-strict-aliasing

noinst_PROGRAMS = runtests calculator_test product_database_test string_test mock_test

runtests_SOURCES = runtests.c
runtests_LDADD = $(top_srcdir)/src/liblcut.la

calculator_test_SOURCES = calculator_test.c calculator.c
calculator_test_LDADD = $(top_srcdir)/src/liblcut.la

product_database_test_SOURCES = product_database_test.c product_database.c database.h
product_database_test_LDADD = $(top_srcdir)/src/liblcut.la

string_test_SOURCES = string_test.c
string_test_LDADD = $(top_srcdir)/src/liblcut.la

mock_test_SOURCES = mock_test.c
mock_test_LDADD = $(top_srcdir)/src/liblcut.la
