bin_PROGRAMS = SampleOSGApplication
SampleOSGApplication_SOURCES = SampleOSGMain.cpp HoverCamera.cpp HoverCamera.h ../SampleUtilities/Data.cpp
INCLUDES = -I../../include
AM_CXXFLAGS = `sdl-config --cflags`
SampleOSGApplication_LDFLAGS = `sdl-config --libs` -losgDB -losgUtil -losg
SampleOSGApplication_LDADD = ../../src/libDemeter.la ../../src/osg/libDemeterOSG.la


