/*
    Copyright (C) 2003 Arab Gawen
    
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
    GLUquadricObj* quadObj=NULL;

    ObjList SphereList;
    
    #define QUAD_OBJ_INIT() { if(!quadObj) initQuadObj(); }

    void InitQuadObj(void)
    {
        quadObj = gluNewQuadric();
        if (!quadObj) _PopCornNotEnoughMemory();
    }
    
    void ObjSphere::_draw(GLERROR* Position, double Advance)
    {
/*        if(this->Radius != this->_Radius || this->Slices != this->_Slices || this->Stacks != this->_Stacks)
        { // Charge la teapot
*/            if(!quadObj) InitQuadObj();
           
            gluQuadricDrawStyle(quadObj, GLU_FILL);
            gluQuadricNormals(quadObj, GLU_SMOOTH);
            gluQuadricTexture(quadObj, GL_TRUE);
/*            
            SphereList.Begin();
*/                gluSphere(quadObj, this->Radius, this->Slices, this->Stacks);
/*            SphereList.End();

            this->_Radius = this->Radius;
            this->_Slices = this->Slices;
            this->_Stacks = this->Stacks;
        }
        
        SphereList.Draw();
*/                
        _PopCornSetInfoFile(Position);
    }
    
    ObjSphere::ObjSphere()
    {
        this->Radius = 1.0f;
        this->Slices = 15;
        this->Stacks = 15;
    }
};

