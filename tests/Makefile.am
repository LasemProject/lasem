include $(top_srcdir)/Makefile.decl

AM_CPPFLAGS = -g -I$(top_srcdir)/src/ $(LASEM_CFLAGS)

AM_CFLAGS = -Wall

test_progs_ldadd = 					\
	$(top_builddir)/src/liblasem-@LASEM_API_VERSION@.la		\
	$(top_builddir)/itex2mml/libitex2mml.la		\
	$(LASEM_LIBS)

noinst_PROGRAMS = lsm-test

lsm_test_SOURCES = lsmtest.c
lsm_test_LDADD = $(test_progs_ldadd)

TEST_PROGS += dom str suite

noinst_PROGRAMS += $(TEST_PROGS)

str_SOURCES = str.c
str_LDADD = $(test_progs_ldadd)

dom_SOURCES = dom.c
dom_LDADD = $(test_progs_ldadd)

suite_SOURCES = suite.c
suite_LDADD = $(test_progs_ldadd)
suite_CFLAGS = -DSUITE_DATA_DIRECTORY="\"$(top_srcdir)/tests/data\"" -DSUITE_OPTION_FILE="\"$(top_srcdir)/tests/suite.ini\""

EXTRA_DIST = suite.ini
