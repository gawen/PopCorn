/*
    Copyright (C) 2003 The SilverCoders
    
    This file is part of PopCorn Library.

    Foobar is free software; you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation; either version 2 of the License, or
    (at your option) any later version.

    Foobar is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Foobar; if not, write to the Free Software
    Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA  02111-1307  USA
*/

#include "popcorn.hpp"

namespace PopCorn
{
    _Camera::_Camera(void)
    {
        this->LookAt.Z = 1.0f;
        this->VectorUp.Y = 1.0f;
    }
    
    void _Camera::Define(void)
    {
        float vec[6];
		double mMatrix[16];
        
        gluLookAt(this->Position.X, this->Position.Y, this->Position.Z,
                  this->LookAt.X,   this->LookAt.Y,   this->LookAt.Z,
                  this->VectorUp.X, this->VectorUp.Y, this->VectorUp.Z
        );

    	glGetDoublev(GL_MODELVIEW_MATRIX,mMatrix);
		vec[0]=-mMatrix[1];
		vec[1]=-mMatrix[6];
		vec[2]=-mMatrix[10];
		vec[3]=mMatrix[1];
		vec[4]=mMatrix[5];
		vec[5]=mMatrix[9];
        
        alListener3f(AL_POSITION, this->Position.X, this->Position.Y, this->Position.Z);
        alListenerfv(AL_ORIENTATION, vec);
	}
};

