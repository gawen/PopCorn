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
    Object::Object()
    {
        this->Solid = true;
        this->MouseClick = NULL;
        this->LastTime_ = 0;
        this->Tag = CRN_TAG_VISIBLE;
        this->enPosition = true;
        this->enScale = true;
        this->enRotation = true;
    }
    
    double Object::MoyLastTime(void)
    {
        int NbElem;
        int MoyTimeFrame=0;
        unsigned int i;
        LISTE* j;

        this->LastTime.AddElem((void*)(SDL_GetTicks_()-this->LastTime_));
        this->LastTime_ = SDL_GetTicks_();
        NbElem = this->LastTime.GetNbElem();
        while(NbElem > Video.MovementLifting)
        {
            this->LastTime.DeleteElem(0);
            NbElem--;
        }
        j=this->LastTime.m_pDebut;
        for(i=0; i<NbElem; i++)
        {
            MoyTimeFrame += (int)j->val;
            if(j->pSuivant != NULL) j=j->pSuivant; else break;
        }
        MoyTimeFrame = MoyTimeFrame / NbElem;
        
        return double(MoyTimeFrame)/1000.0;
    }
    
    void Object::_InitTimers(void)
    {
        this->LastTime.Delete();
        this->LastTime_ = 0;
    }
    
    void Object::Draw(void)
    {
        static GLERROR error;
        static GLenum glerror;
        static char StrError[128];
        
//        if(this->Tag & CRN_TAG_VISIBLE || ((this->Tag & CRN_TAG_VISIBLE) && _Picking && this->MouseClick != NULL))
//        {
            if(_Picking) glLoadName((GLuint)this);
            
            glPushMatrix();
            glPushAttrib(GL_ENABLE_BIT);
                if(this->enPosition && !this->Position.isNull()) this->Position.Position();
                if(this->enScale && !this->Scale.isNull()) this->Scale.Scale();
                if(this->enRotation && !this->Rotation.isNull()) this->Rotation.Rotate();
                
                if(this->Material.Tex) this->Material.Tex->Enable();
                if(this->Material.Blending)
                {
                    glBlendFunc(this->Material.BlendingSource, this->Material.BlendingDestination);
                    glEnable(GL_BLEND);
                }
                    this->_draw(&error, this->MoyLastTime());
                glDisable(GL_BLEND);
                if(this->Material.Tex) this->Material.Tex->Disable();
            glPopAttrib();
            glPopMatrix();
            
            if((glerror=glGetError()) != GL_NO_ERROR)
            {
                sprintf(StrError, "OpenGL> File %s, line %u : \"%s\"", error.file, error.line, gluErrorString(glerror));
                _PopCornError(StrError);
            }
//        } else this->LastTime_ = SDL_GetTicks_();
    }
    
    void Object::Center2d(void)
    {
        this->Position.X = (Video.Width/2) - (this->Scale.X/2);
        this->Position.Y = (Video.Height/2) - (this->Scale.Y/2);
    }
    
    bool Object::PushTo(Objects& Obj)
    {
        return Obj.PushObject(*this);
    }
    
    bool Object::PopTo(Objects& Obj)
    {
        return Obj.PopObject(*this);
    }
    
    bool Object::BePersistant(unsigned char Type)
    {
        if(Scene == NULL) return false;
        
        if(Type & CRN_3DOBJS)
            Scene->List3dObjects.PushObject(*this);
        
        if(Type & CRN_FRONTOBJS)
            Scene->List2dObjects.PushObject(*this);
        
        if(Type & CRN_BACKOBJS)
            Scene->ListBackObjects.PushObject(*this);
        
        if(Type & CRN_ALWOBJS)
            Scene->ListObjects.PushObject(*this);
        
        return true;
    }
    
    bool Object::BeNotPersistant(unsigned char Type)
    {
        if(Scene == NULL) return false;
        
        if(Type & CRN_3DOBJS)
            Scene->List3dObjects.PopObject(*this);
        
        if(Type & CRN_FRONTOBJS)
            Scene->List2dObjects.PopObject(*this);
        
        if(Type & CRN_BACKOBJS)
            Scene->ListBackObjects.PopObject(*this);
        
        if(Type & CRN_ALWOBJS)
            Scene->ListObjects.PopObject(*this);
        
        return true;
    }
    
    void Object::AddTag(ObjTag Tag)
    {
        this->Tag |= Tag;
    }
    
    void Object::RemoveTag(ObjTag Tag)
    {
        this->Tag = this->Tag & ~Tag;
    }

	void Object::operator=(Object& Obj)
	{
		this->_equ(Obj);
	}

	void Object::Clear(void)
	{
		this->_clear();
	}
};
