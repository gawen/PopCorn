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

#include "Demeter/Plane.h"

using namespace Demeter;

Plane::Plane(Vector & p1, Vector & p2, Vector & p3)
{
	DefineFromPoints(p1, p2, p3);
}

void Plane::DefineFromPoints(Vector & p1, Vector & p2, Vector & p3)
{
	Vector v1, v2, normal;
// Find the normal of the polygon defined by the three points(cross product of 2 vertex vectors)
	v1.x = p2.x - p1.x;
	v1.y = p2.y - p1.y;
	v1.z = p2.z - p1.z;

	v2.x = p3.x - p1.x;
	v2.y = p3.y - p1.y;
	v2.z = p3.z - p1.z;

	v1.Normalize();
	v2.Normalize();
// Find surface normal based on cross product.
	normal.x = v1.y * v2.z - v2.y * v1.z;
	normal.y = v2.x * v1.z - v1.x * v2.z;
	normal.z = v1.x * v2.y - v2.x * v1.y;
	normal.Normalize();
// This surface normal represents the a,b,c components of the plane equation.
	a = normal.x;
	b = normal.y;
	c = normal.z;
// The d component is calculated from Ax + By + Cz + D = 0
	d = -(a * p1.x + b * p1.y + c * p1.z);
}
