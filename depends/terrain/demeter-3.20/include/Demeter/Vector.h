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

#ifndef _VECTOR_DEMETER_H_
#define _VECTOR_DEMETER_H_

#include "Demeter/Globals.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief An extremely basic 3D vector class.

	/// Demeter does not rely on all of the bells and whistles of most public
	/// vector classes, so the external utility of this class is questionable. 
	/// No distinction is made between points and vectors.
	class TERRAIN_API Vector
	{
	      public:
		/// \brief Constructs a new Vector with undefined components.
		Vector()
		{
		}
		/// \brief Destroys the Vector.
		 ~Vector()
		{
		}
		/// \brief Returns the length of the Vector.

		/// Since this is done with a brute force sqrt() call, it is expensive.
		/// \return The length of the Vector.
		float GetLength();
		/// \brief Makes the Vector's length = 1, leaving all 3 components in their original proportions to one another.
		/// \param tolerance A number specifying the minimum length of a Vector that will actually be normalized. If the Vector is less than this length it will simply
		/// be ignored. This is done to avoid floating point imprecision and errors caused by dividing by values close to or equal to zero.
		/// \return The original length of the Vector before it was normalized.
		float Normalize(float tolerance = EPSILON);
		/// \brief Rotates the Vector about the z-axis.
		/// \param theta The angle of rotation in radians.
		void RotateZ(float theta);
		/// \brief Assigns all of the components of one Vector to another.
		Vector & operator =(const Vector & vector);

		/// The abscissa of the head of the 3D vector
		float x;
		/// The ordinate of the head of the 3D vector
		float y;
		/// The elevation of the head of the 3D vector
		float z;
	};
}

#endif
