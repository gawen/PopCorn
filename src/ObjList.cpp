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
    ObjList::ObjList(void)
    {
        this->Obj = NULL;
        this->Id = 0;
        this->Loaded = false;
    }
    
    ObjList::~ObjList(void)
    {
        this->Clear();
    }
    
    void ObjList::Clear(void)
    {
        glDeleteLists(this->Id, 1);
        this->Obj = NULL;
        this->Id = 0;
        this->Loaded = false;
    }
    
    void ObjList::Verify(void)
    {
        if(this->Id == 0) this->Id = glGenLists(1);
        if(this->Id == 0) _PopCornError("Unable to load a OpenGl List.");
    }
    
    void ObjList::Load(Object* Obj)
    {
        if(Obj == NULL)
        {
            glDeleteLists(this->Id, 1);
            this->Id = 0;
            this->Loaded = false;
        } else {
            this->Obj = Obj;
            this->Build();
        }
    }
    
    void ObjList::Build(void)
    {
        static GLenum glerror;
        static char StrError[128];
        MatTexture* Tex;
        
        this->Verify();
        
        glNewList(this->Id, GL_COMPILE);
            if(this->Obj != NULL)
            {
                Tex = this->Obj->Material.Tex;
                this->Obj->Material.Tex = NULL;
                this->Obj->Draw();
                this->Obj->Material.Tex = Tex;
            }
        glEndList();
        
        if((glerror=glGetError()) != GL_NO_ERROR)
        {
            sprintf(StrError, "File %s, line %u : \"%s\"", __FILE__, __LINE__, gluErrorString(glerror));
            _PopCornError(StrError);
        }
        
        this->Loaded = true;
    }
    
    void ObjList::Begin(void)
    {
        this->Verify();
        glNewList(this->Id, GL_COMPILE);
    }
    
    void ObjList::End(void)
    {
        static GLenum glerror;
        static char StrError[128];
        glEndList();
        
        if((glerror=glGetError()) != GL_NO_ERROR)
        {
            sprintf(StrError, "File %s, line %u : \"%s\"", __FILE__, __LINE__, gluErrorString(glerror));
            _PopCornError(StrError);
        }
        
        this->Loaded = true;
    }
    
    void ObjList::_draw(GLERROR* Position, double Advance)
    {
        if(this->Id != 0) glCallList(this->Id);
        
        _PopCornSetInfoFile(Position);
    }
};

