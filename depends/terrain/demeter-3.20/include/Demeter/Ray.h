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

#ifndef _RAY_DEMETER_H_
#define _RAY_DEMETER_H_

#include "Demeter/Vector.h"

/// \brief The Demeter Terrain Engine.
namespace Demeter
{
	/// \brief Ray represents a geometric ray (a line that begins at a point and extends in some direction infinitely.)

	/// This class is used for ray tracing. It defines a starting point and a direction for a ray that can be tested for intersection
	/// with a Terrain's surface.
	class TERRAIN_API Ray
	{
	      public:
		/// \brief Creates a new Ray that has undefined origin and direction.
		Ray()
		{
		}
		/// \brief Destroys the Ray.
		 ~Ray()
		{
		}
		/// Defines the starting point of the Ray.
		Vector m_Origin;
		/// Defines the direction of the Ray.
		Vector m_Direction;
	};
}

#endif
