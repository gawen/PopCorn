bin_PROGRAMS = SampleGDALApplication
SampleGDALApplication_SOURCES = SampleGDALMain.cpp ../SampleUtilities/Camera.cpp ../SampleUtilities/Matrix.cpp ../SampleUtilities/Data.cpp
INCLUDES = -I../SampleUtilities -I../../include
AM_CXXFLAGS = `sdl-config --cflags`
SampleGDALApplication_LDFLAGS = `sdl-config --libs` -lgdal.1.1
SampleGDALApplication_LDADD = ../../src/libDemeter.la

