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

#ifndef _PLANE_DEMETER_H_
#define _PLANE_DEMETER_H_

#include "Demeter/Vector.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief An extremely basic 3D plane class.

	/// This simple class is only used by Demeter for ray tracing and elevation finding calculations.
	/// It is certainly not meant to be a generally useful class for any other purpose, but it since
	/// it represents the classic plane equation (Ax + By + Cz + D = 0), you might find it useful...
	class TERRAIN_API Plane
	{
	      public:
		/// \brief Creates a new plane that is coincident with the x-y plane.
		Plane()
		{
		}
		/// \brief Creates a new plane defined by the provided 3 points.
		/// \param p1 A 3D point (one of three specifying the orientation of the plane.)
		/// \param p2 A 3D point (one of three specifying the orientation of the plane.)
		/// \param p3 A 3D point (one of three specifying the orientation of the plane.)
		Plane(Vector & p1, Vector & p2, Vector & p3);
		/// \brief Destructor for Plane.
		 ~Plane()
		{
		}
		/// \brief Specifies that this plane is oriented such that its surface includes all 3 points.
		/// \param p1 A 3D point (one of three specifying the orientation of the plane.)
		/// \param p2 A 3D point (one of three specifying the orientation of the plane.)
		/// \param p3 A 3D point (one of three specifying the orientation of the plane.)
		void DefineFromPoints(Vector & p1, Vector & p2, Vector & p3);
		/// A in the classic plane equation (Ax + By + Cz + D = 0)
		float a;
		/// B in the classic plane equation (Ax + By + Cz + D = 0)
		float b;
		/// C in the classic plane equation (Ax + By + Cz + D = 0)
		float c;
		/// D in the classic plane equation (Ax + By + Cz + D = 0)
		float d;
	};
}

#endif
