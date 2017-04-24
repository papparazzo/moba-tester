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
    ctrlhardwarestate.cpp \
    frmmain.cpp


moba_tester_LDADD = -lMoBaCommon $(GTKMM_LIBS)
LDFLAGS += -L/usr/local/lib

AM_CPPFLAGS = $(GTKMM_CFLAGS) -ggdb -g3 -O0

CLEANFILES = *~
