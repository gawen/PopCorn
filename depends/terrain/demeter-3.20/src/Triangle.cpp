// Demeter Terrain Visualization Library by Clay Fowler
// Copyright (C) 2002 Clay Fowler

// $ID$

/*
This library is free software; you can redistribute it and/or
modify it under the terms of the GNU Library General Public
License as published by the Free Software Foundation; either
version 2 of the License, or (at your option) any later version.

This library is distributed in the hope that it will be useful,
but WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
Library General Public License for more details.

You should have received a copy of the GNU Library General Public
License along with this library; if not, write to the
Free Software Foundation, Inc., 59 Temple Place - Suite 330,
Boston, MA  02111-1307, USA.
*/

#include "Demeter/Triangle.h"

using namespace Demeter;

Triangle::Triangle()
{
}

Triangle::~Triangle()
{
}

void Triangle::DefineFromPoints(Vector & p1, Vector & p2, Vector & p3)
{
	m_pVertices[0].x = p1.x;
	m_pVertices[0].y = p1.y;
	m_pVertices[0].z = p1.z;
	m_pVertices[1].x = p2.x;
	m_pVertices[1].y = p2.y;
	m_pVertices[1].z = p2.z;
	m_pVertices[2].x = p3.x;
	m_pVertices[2].y = p3.y;
	m_pVertices[2].z = p3.z;
#if _USE_RAYTRACING_SUPPORT_ > 0
	m_Plane.DefineFromPoints(p3, p2, p1);
#endif
}

Vector *Triangle::GetVertex(int index)
{
	return &m_pVertices[index];
}

#if _USE_RAYTRACING_SUPPORT_ > 0
Plane *Triangle::GetPlane()
{
	return &m_Plane;
}
#endif
