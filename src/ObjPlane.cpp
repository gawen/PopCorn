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
    void ObjPlane::_draw(GLERROR* Position, double Advance)
    {
        _PopCornSetInfoFile(Position);
        
        glEnable(GL_NORMALIZE);
        glBegin(GL_QUADS);
            if(Material.Tex) this->Material.Tex->TexCoord(0.0f, 0.0f);
                glVertex2f(0.0f, 0.0f);
            
            if(Material.Tex) this->Material.Tex->TexCoord(0.0f, 1.0f);
                glVertex2f(0.0f, 1.0f);
            
            if(Material.Tex) this->Material.Tex->TexCoord(1.0f, 1.0f);
                glVertex2f(1.0f, 1.0f);
            
            if(Material.Tex) this->Material.Tex->TexCoord(1.0f, 0.0f);
                glVertex2f(1.0f, 0.0f);
        glEnd();
        glDisable(GL_NORMALIZE);
    }
};

