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

#define length 128

namespace PopCorn
{
    bool Objects::PushObject(Object& Obj)
    {
        if(this->List.Find((void*)&Obj) != -1)
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, "*ERROR* You load two times the same object ...\n");
#           endif
            return false;
        }
        return this->List.AddElem((void*)&Obj);
    }
    
    bool Objects::PopObject(Object& Obj)
    {
        if(this->List.DeleteElem((void*)&Obj))
        {
            Obj._InitTimers();
            return true;
        } else 
            return false;
    }
    
    void Objects::_draw(GLERROR* Position, double Advance)
    {
        _PopCornSetInfoFile(Position);
        
        for(LISTE* i=this->List.m_pDebut; i != NULL; i=i->pSuivant)
            if( ((Object*)i->val)->Tag & Scene->Tag )
                ((Object*)i->val)->Draw();
    }
};

