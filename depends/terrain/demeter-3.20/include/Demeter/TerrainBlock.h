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

#ifndef _TERRAIN_BLOCK_DEMETER_H_
#define _TERRAIN_BLOCK_DEMETER_H_

#include "Demeter/Globals.h"
#include "Demeter/TriangleStrip.h"
#include "Demeter/TriangleFan.h"
#include "Demeter/Ray.h"
#include "Demeter/Terrain.h"
#include "Demeter/Vector.h"
#include "Demeter/Triangle.h"

#if _USE_RAYTRACING_SUPPORT_ > 0
int RayPlaneIntersect(const Demeter::Ray * ray, const Demeter::Plane * plane, Demeter::Vector * point, float *distance);
int RayBoxIntersect(const Demeter::Ray * ray, const Demeter::Box * box, Demeter::Vector * point, float *distance);
#endif

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	class TerrainBlock
	{
	      public:
		TerrainBlock(TerrainBlock * pParent);
		TerrainBlock(int homeVertex, int stride, Terrain * pTerrain, TerrainBlock * pParent);
		 ~TerrainBlock();
		void Tessellate(const double *pMatrixModelview, const double *pMatrixProjection, const int *pViewport, TriangleStrip * pTriangleStrips, unsigned int *pCountStrips, Terrain * pTerrain);
		void Write(FILE * file);
		void Read(FILE * file, Terrain * pTerrain);
		bool IsActive();
		void RepairCracks(Terrain * pTerrain, TriangleFan * pTriangleFans, unsigned int *pCountFans);
		int GetStride();
		void EnableStrip(bool bEnabled);
		int GetHomeIndex();
		void CalculateGeometry(Terrain * pTerrain);
		void VertexChanged(Terrain * pTerrain);
		void VertexChanged(Terrain * pTerrain, int index1);
		void VertexChanged(Terrain * pTerrain, int index1, int index2);
#if _USE_RAYTRACING_SUPPORT_ > 0
		void IntersectRay(const Ray & ray, Vector & intersectionPoint, float &lowestDistance, const Terrain * pTerrain);
#endif
	      private:
		void CreateTriangleStrip(TriangleStrip * pTriangleStrips, unsigned int *pCount, Terrain * pTerrain);
		TerrainBlock **m_pChildren;
		int m_HomeIndex;
		short int m_Stride;
		bool m_bIsActive:1;
		bool m_bChildrenActive:1;
		TriangleStrip *m_pTriangleStrip;
		float m_MinElevation, m_MaxElevation;
		// FIXME: Don't need both BPlane_Max_D/Min_D and Box.m_Max/Min.z
		// Bounding Planes given by A,B,Max_D and A,B,Min_D - C defined as 1
		float m_BPlane_A, m_BPlane_B, m_BPlane_Max_D, m_BPlane_Min_D;
#if _USE_RAYTRACING_SUPPORT_ > 0
		Triangle *m_pTriangles;
#endif

		friend class Terrain;
	};
}

#endif
