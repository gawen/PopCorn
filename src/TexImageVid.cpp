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
    MatTexImageVid::MatTexImageVid(void)
    {
        this->Frame = 0;
        this->Frames = 0;
    }
    
    void MatTexImageVid::Enable(void)
    {
        MatTexImage* Tex=(MatTexImage*)this->Tex[this->Frame];
        
        if(Tex == NULL)
        {
            this->Frame = 0;
            Tex=(MatTexImage*)this->Tex[this->Frame];
        }
        
        this->Size = Tex->Size;
        Tex->Enable();
    }
    
    void MatTexImageVid::Disable(void)
    {
        ((MatTexImage*)this->Tex[this->Frame])->Disable();
        this->Frame++;
    }
    
    void MatTexImageVid::TexCoord(float s, float t)
    {
        ((MatTexImage*)this->Tex[this->Frame])->TexCoord(s, t);
    }
    
    void MatTexImageVid::Add(MatTexImage* Tex)
    {
        this->Tex.AddElem((void*)Tex);
        this->Frames++;
    }
    
    void MatTexImageVid::Remove(unsigned int Frame)
    {
        if(this->Tex.DeleteElem(Frame)) this->Frames--;
    }
};

