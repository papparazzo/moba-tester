cmake_minimum_required(VERSION 3.20)

project(moba-tester VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

configure_file(config.h.in config.h)

add_executable(
    moba-tester 

    src/ctrlambience.cpp     
    src/ctrlambientlight.cpp  
    src/ctrlblocklock.cpp     
    src/ctrlbool.cpp          
    src/ctrlcolortheme.cpp    
    src/ctrlconnectivity.cpp  
    src/ctrlcontact.cpp       
    src/ctrlenvironment.cpp     
    src/ctrlglobaltimer.cpp     
    src/ctrlsetbrakevector.cpp  
    src/ctrlstring.cpp
    src/ctrltracklayout.cpp
    src/frmmain.cpp
    src/main.cpp
    src/msgsender.cpp
)

install(TARGETS moba-tester)

set(CMAKE_CXX_COMPILER "/usr/bin/clang++")

target_include_directories(moba-tester PUBLIC "${PROJECT_BINARY_DIR}")

target_link_libraries(moba-tester mobacommon)
target_link_libraries(moba-tester ${CMAKE_SOURCE_DIR}/modules/lib-msghandling/src/moba/libmsghandling.a)
target_link_libraries(moba-tester ${CMAKE_SOURCE_DIR}/modules/lib-tracklayout/src/moba/libtracklayout.a)

include_directories(${CMAKE_SOURCE_DIR}/modules/lib-msghandling/src)
include_directories(${CMAKE_SOURCE_DIR}/modules/lib-tracklayout/src)

#AM_CPPFLAGS = \
#    $(GTKMM_CFLAGS) \
#    -I$(srcdir)/../modules/lib-msghandling/src \
#    -I$(srcdir)/../modules/lib-tracklayout/src \
#    -g3 -g -ggdb  -Wall
#
### Process this file with automake to generate Makefile.in
#SUBDIRS = \
#    modules/lib-msghandling/src/moba \
#    modules/lib-tracklayout/src/moba \
#    src
#