lib_LTLIBRARIES = libDemeter.la
libDemeter_la_SOURCES = Terrain.cpp Brush.cpp TerrainVertex.cpp TerrainLattice.cpp SkyBox.cpp Loader.cpp DetailTexture.cpp Globals.cpp Plane.cpp Settings.cpp TerrainBlock.cpp TextureCell.cpp Texture.cpp TextureFactory.cpp TextureGenerator.cpp TextureSet.cpp Triangle.cpp TriangleFan.cpp TriangleStrip.cpp Vector.cpp
INCLUDES = -I../include
library_includedir=$(includedir)/Demeter
library_include_HEADERS = \
	../include/Demeter/BitArray.h \
	../include/Demeter/config.h \
	../include/Demeter/Box.h \
	../include/Demeter/Brush.h \
	../include/Demeter/Demeter.h \
	../include/Demeter/DemeterDrawable.h \
	../include/Demeter/DemeterException.h \
	../include/Demeter/DetailTexture.h \
	../include/Demeter/Globals.h \
	../include/Demeter/Loader.h \
	../include/Demeter/Plane.h \
	../include/Demeter/Ray.h \
	../include/Demeter/Settings.h \
	../include/Demeter/SkyBox.h \
	../include/Demeter/StdAfx.h \
	../include/Demeter/Terrain.h \
	../include/Demeter/TerrainBlock.h \
	../include/Demeter/TerrainLattice.h \
	../include/Demeter/TerrainLoadListener.h \
	../include/Demeter/TerrainVertex.h \
	../include/Demeter/Texture.h \
	../include/Demeter/TextureCell.h \
	../include/Demeter/TextureFactory.h \
	../include/Demeter/TextureGenerator.h \
	../include/Demeter/TextureSet.h \
	../include/Demeter/Triangle.h \
	../include/Demeter/TriangleFan.h \
	../include/Demeter/TriangleStrip.h \
	../include/Demeter/Vector.h

EXTRA_DIST = StdAfx.cpp DemeterEntryPoint.cpp

if HAVE_LIBOSG
OSGDIR = osg
endif

SUBDIRS = $(OSGDIR) elevationloaders textureloaders

