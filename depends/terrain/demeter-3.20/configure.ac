dnl Process this file with autoconf to produce a configure script.
AC_INIT(demeter, 3.20)
AC_CONFIG_AUX_DIR(config)
AM_CONFIG_HEADER(include/Demeter/config.h)
AM_INIT_AUTOMAKE(demeter, 3.20)

dnl Checks for programs.
AC_PROG_CPP
AC_PROG_CXX
AC_PROG_INSTALL
AC_PROG_LIBTOOL
AC_PROG_RANLIB

dnl Checks for libraries.
AC_CHECK_LIB(GL, glBegin)
AC_CHECK_LIB(GLU, gluProject)
	
AC_CHECK_PROG(HAVE_LIBXML2,xml2-config,yes,no)
AC_CHECK_PROG(HAVE_LIBSDL,sdl-config,yes,no)
	
AC_CHECK_FUNCS(atexit)
AC_CHECK_FUNCS(memset)
AC_CHECK_FUNCS(pow)
AC_CHECK_FUNCS(sqrt)
AC_CHECK_FUNCS(strstr)

AC_ARG_ENABLE(raytracing,
[  --enable-raytracing     build Demeter with support for raytracing against the terrain (uses more memory)],
[AC_DEFINE(_USE_RAYTRACING_SUPPORT_,1)],
[AC_DEFINE(_USE_RAYTRACING_SUPPORT_,0)])

AC_MSG_CHECKING(for Open Scene Graph)
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE(
[#include<osgDB/Registry>],
[osgDB::Registry::instance()->setDataFilePathList("foo")],
[AC_MSG_RESULT(OSG Detected - building OSG support);HAVE_LIBOSG=yes],[AC_MSG_RESULT(OSG not found - building without OSG support!);HAVE_LIBOSG=no])
AM_CONDITIONAL(HAVE_LIBOSG, test $HAVE_LIBOSG = yes)
	
AC_MSG_CHECKING(for Geospatial Data Abstraction Library)
AC_LANG_CPLUSPLUS
AC_TRY_COMPILE(
[#include<gdal.h>],
[GDALAllRegister()],
[AC_MSG_RESULT(GDAL Detected - building GDAL support);HAVE_LIBGDAL=yes],[AC_MSG_RESULT(GDAL not found - building without GDAL support!);HAVE_LIBGDAL=no])
AM_CONDITIONAL(HAVE_LIBGDAL, test $HAVE_LIBGDAL = yes)

AM_CONDITIONAL(HAVE_LIBXML2, test $HAVE_LIBXML2 = yes)
AM_CONDITIONAL(HAVE_LIBSDL, test $HAVE_LIBSDL = yes)

dnl Checks for header files.
AC_HEADER_STDC
AC_CHECK_HEADERS(malloc.h)

dnl Checks for typedefs, structures, and compiler characteristics.
AC_C_CONST
AC_C_INLINE
AC_STRUCT_TM
AC_HEADER_STDBOOL

AC_OUTPUT(Makefile \
	  src/Makefile \
	  src/osg/Makefile \
	  src/elevationloaders/Makefile \
	  src/elevationloaders/demeter/Makefile \
	  src/elevationloaders/gdal/Makefile \
	  src/textureloaders/Makefile \
	  src/textureloaders/demeter/Makefile \
	  src/textureloaders/sdl/Makefile \
	  samples/Makefile \
	  samples/SampleRawApplication/Makefile \
	  samples/SampleGDALApplication/Makefile \
	  samples/SampleSkyBoxApplication/Makefile \
	  samples/SamplePaintApplication/Makefile \
	  samples/SampleOSGApplication/Makefile \
	  samples/SampleEditApplication/Makefile \
	  samples/SampleTerrainLatticeApplication/Makefile \
	  samples/SampleDemeterApplication/Makefile \
	  samples/SampleProceduralTextureApplication/Makefile \
)

