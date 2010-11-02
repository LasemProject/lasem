include $(top_srcdir)/Makefile.decl

INCLUDES = -g -I$(top_srcdir)/src/ $(LASEM_CFLAGS)

test_progs_ldadd = 					\
	$(top_builddir)/src/liblasem-@LASEM_API_VERSION@.la		\
	$(top_builddir)/itex2mml/libitex2mml.la
	$(LASEM_LIBS)

noinst_PROGRAMS = lsm-test

lsm_test_SOURCES = lsmtest.c
lsm_test_LDADD = $(test_progs_ldadd)

TEST_PROGS += dom

noinst_PROGRAMS += $(TEST_PROGS)

dom_SOURCES = dom.c
dom_LDADD = $(test_progs_ldadd)
