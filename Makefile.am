AUTOMAKE_OPTIONS = subdir-objects
bin_PROGRAMS = moba-tester
moba_tester_SOURCES = src/main.cpp src/environmenthandler.cpp src/helper.cpp src/systemhandler.cpp src/basehandler.cpp src/layouthandler.cpp
moba_tester_LDADD = -lMoBaCommon
LDFLAGS += -L/usr/local/lib -pthread

AM_CPPFLAGS = -std=c++11