AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-tester
moba_tester_SOURCES = \
    main.cpp \
    msgsender.cpp \
    ctrlstring.cpp \
    ctrlbool.cpp \
    ctrlglobaltimer.cpp \
    ctrlcolortheme.cpp \
    ctrlenvironment.cpp \
    ctrlambience.cpp \
    ctrlambientlight.cpp \
    ctrlconnectivity.cpp \
    ctrltracklayout.cpp \
    frmmain.cpp


moba_tester_LDADD = -lMoBaCommon $(GTKMM_LIBS) $(srcdir)/../modules/lib-msghandling/src/moba/libmsghandling.a
LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = $(GTKMM_CFLAGS) -I$(srcdir)/../modules/lib-msghandling/src -ggdb -g3 -O0

CLEANFILES = *~
