bin_PROGRAMS = SampleDemeterApplication
SampleDemeterApplication_SOURCES = SampleDemeterMain.cpp ../SampleUtilities/Camera.cpp ../SampleUtilities/Matrix.cpp ../SampleUtilities/Data.cpp
INCLUDES = -I../SampleUtilities -I../../include
AM_CXXFLAGS = `sdl-config --cflags` `xml2-config --cflags`
SampleDemeterApplication_LDFLAGS = `sdl-config --libs` `xml2-config --libs`
SampleDemeterApplication_LDADD = ../../src/libDemeter.la

