## Makefile.am -- Process this file with automake to produce Makefile.in

if HAVE_LIBGDAL
GDALDIR = SampleGDALApplication
endif

if HAVE_LIBXML2
DEMETERDIR = SampleDemeterApplication
if HAVE_LIBOSG
OSGDIR = SampleOSGApplication
endif
endif

SUBDIRS = SampleRawApplication SampleSkyBoxApplication SamplePaintApplication SampleEditApplication SampleProceduralTextureApplication SampleTerrainLatticeApplication $(GDALDIR) $(OSGDIR) $(DEMETERDIR)

