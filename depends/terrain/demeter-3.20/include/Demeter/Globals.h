// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

// $ID$

/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Lesser General Public
License as published by the Free Software Foundation; either
version 2.1 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#ifndef _GLOBALS_DEMETER_H_
#define _GLOBALS_DEMETER_H_

#include <Demeter/config.h>

#if defined (_MSC_VER)
#pragma warning (disable : 4251 4786 4800 4018)
#endif

//#define _USE_RAYTRACING_SUPPORT_ 1
//_USE_RAYTRACING_SUPPORT_ = 1 enables the ray tracing methods on the Terrain class, but increases
//memory usage considerably. Demeter's ray tracing is very fast and can be used for mouse picking,
//line-of-sight tests, etc. If you enable this option, then be aware that MUCH more memory will be needed.

#ifdef _WIN32
#ifdef TERRAIN_EXPORTS
#define TERRAIN_API __declspec(dllexport)
#else
#define TERRAIN_API __declspec(dllimport)
#endif
#else
#define TERRAIN_API
#endif

typedef unsigned char Uint8;
typedef signed char Sint8;
typedef unsigned short Uint16;
typedef signed short Sint16;
typedef unsigned int Uint32;
typedef signed int Sint32;

#ifdef _WIN32
#ifdef _MSC_VER
#include <crtdbg.h>
#endif
#endif
#include "assert.h"

#ifdef _WIN32
#include <windows.h>		// Required by gl.h
#endif

#include <iostream>
#include <string.h>
#ifndef __APPLE__
#include <GL/gl.h>
#include <GL/glu.h>
#endif
#ifdef __APPLE__
#include <OpenGL/gl.h>
#include <OpenGL/glu.h>
#include <OpenGL/glext.h>
#endif
#include <stdlib.h>
#include <math.h>
#include <stdio.h>
#include <vector>
#include <map>

#ifndef M_PI
#define M_PI 3.141592654f
#endif

#define MAX_VERTICES_PER_FAN 10

#ifndef EPSILON
#define EPSILON 0.00001f
#endif

#ifndef INFINITY
#define INFINITY 999999999.0f
#endif

#define MAX_FILENAME_LENGTH		2048
#define TextureMaskBitsPerPixel 8

#define GL_CLAMP_TO_EDGE_EXT                0x812F
#define GL_TEXTURE0_ARB                     0x84C0
#define GL_TEXTURE1_ARB                     0x84C1
#define GL_COMBINE_RGB_EXT                  0x8571
#define GL_ARRAY_ELEMENT_LOCK_FIRST_EXT     0x81A8
#define GL_ARRAY_ELEMENT_LOCK_COUNT_EXT     0x81A9
#define COMPRESSED_RGB_S3TC_DXT1_EXT        0x83F0

#ifdef _WIN32
typedef void (APIENTRY * PFNGLMULTITEXCOORD2FARBPROC) (GLenum texture, GLfloat s, GLfloat t);
typedef void (APIENTRY * PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (APIENTRY * PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void (APIENTRY * PFNGLUNLOCKARRAYSEXTPROC) (void);
typedef void (APIENTRY * PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
#else
typedef void (*PFNGLMULTITEXCOORD2FARBPROC) (GLenum texture, GLfloat s, GLfloat t);
typedef void (*PFNGLACTIVETEXTUREARBPROC) (GLenum texture);
typedef void (*PFNGLLOCKARRAYSEXTPROC) (GLint first, GLsizei count);
typedef void (*PFNGLUNLOCKARRAYSEXTPROC) (void);
typedef void (*PFNGLCLIENTACTIVETEXTUREARBPROC) (GLenum texture);
#endif

#endif
