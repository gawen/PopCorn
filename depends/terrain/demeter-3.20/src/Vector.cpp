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

#include "Demeter/Vector.h"

using namespace Demeter;

Vector & Vector::operator =(const Vector & v)
{
	x = v.x;
	y = v.y;
	z = v.z;
	return *this;
}

float Vector::Normalize(float tolerance)
{
	float length = GetLength();

	if (length > tolerance)
	{
		float invLength = 1.0f / length;
		x *= invLength;
		y *= invLength;
		z *= invLength;
	}
	else
	{
		length = 0.0;
	}

	return length;
}

float Vector::GetLength()
{
	return (float)sqrt(x * x + y * y + z * z);
}

void Vector::RotateZ(float theta)
{
	float newx, newy;

	newx = x * (float)cos(theta) - y * (float)sin(theta);
	newy = x * (float)sin(theta) + y * (float)cos(theta);
	x = newx;
	y = newy;
}
