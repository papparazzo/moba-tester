AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = \
    moba-tester
moba_tester_SOURCES = \
    main.cpp \
    msgsender.cpp \
    ctrlstring.cpp \
    ctrlambience.cpp \
    ctrlambientlight.cpp \
    ctrlbool.cpp \
    ctrlblocklock.cpp \
    ctrlcolortheme.cpp \
    ctrlconnectivity.cpp \
    ctrlenvironment.cpp \
    ctrlglobaltimer.cpp \
    ctrlsetbrakevector.cpp \
    ctrltracklayout.cpp \
    frmmain.cpp


moba_tester_LDADD = -lmobacommon $(GTKMM_LIBS) $(srcdir)/../modules/lib-msghandling/src/moba/libmsghandling.a
LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = $(GTKMM_CFLAGS) -I$(srcdir)/../modules/lib-msghandling/src -ggdb -g3 -O3 -Wall -Wno-deprecated-declarations -DDATADIR='"$(datadir)"'

CLEANFILES = *~
