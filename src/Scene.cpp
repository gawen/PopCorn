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
    class ObjScene *Scene;
    unsigned int Width, Height;
    
    ObjScene::ObjScene(void)
    {
        int i;
        
        this->Enable3D = true;
        this->FrontGround = false;
        this->BackGround = false;
        this->ColBackGround.Set(0, 0, 0, 255);
        this->ColFrontGround.Set(0, 0, 0, 0);
        this->FrontFilter.Material.Tex = &this->ColFrontGround;
        this->FrontFilter.Position.Set(0, 0);
        
        this->Keyboard = NULL;
        this->MouseMotion = NULL;
        this->MouseClick = NULL;
        this->Initialization = NULL;
        this->Exit = NULL;
        this->Idle = NULL;
        
        this->Tag = CRN_TAG_VISIBLE;
        
        this->Lighting = false;
        for(i=0; i<8; i++)
        {
            this->Light[i].Enabled = false;
            
            this->Light[i].Ambient.Set(0, 0, 0, 255);
            if(i==0) this->Light[i].Diffuse.Set(255, 255, 255, 255);
                else this->Light[i].Diffuse.Set(0,   0,   0,   255);
            
            if(i==0) this->Light[i].Specular.Set(255, 255, 255, 255);
                else this->Light[i].Specular.Set(0,   0,   0,   255);
            
            this->Light[i].Position.Set(0.0f, 0.0f, 1.0f);
            this->Light[i].SpotDirection.Set(0.0f, 0.0f, -1.0f);
            this->Light[i].SpotExponent = 0.0f;
            this->Light[i].SpotCutoff = 180.0f;
            this->Light[i].ConstantAttenuation = 1.0f;
            this->Light[i].LinearAttenuation = 0.0f;
            this->Light[i].QuadraticAttenuation = 0.0f;
        }
    }
    
    void Init3d(void)
    {
        int i=0;

        glMatrixMode(GL_PROJECTION);
        glLoadIdentity();
        gluPerspective(45.0f,(GLfloat)Video.Width/(GLfloat)Video.Height,0.1f,1000.0f);
        glMatrixMode(GL_MODELVIEW);
        glLoadIdentity();
        
        if(Scene->Lighting)
        {
            glEnable(GL_LIGHTING);
	        for(i=0; i<8; i++)
	            if(Scene->Light[i].Enabled)
                {
                    glEnable(GL_LIGHT0+i);
                    glLightfv(GL_LIGHT0+i, GL_AMBIENT,               Scene->Light[i].Ambient.GetGLArray());
                    glLightfv(GL_LIGHT0+i, GL_DIFFUSE,               Scene->Light[i].Diffuse.GetGLArray());
                    glLightfv(GL_LIGHT0+i, GL_SPECULAR,              Scene->Light[i].Specular.GetGLArray());
                    glLightfv(GL_LIGHT0+i, GL_POSITION,              Scene->Light[i].Position.GetGLArray());
                    glLightfv(GL_LIGHT0+i, GL_SPOT_DIRECTION,        Scene->Light[i].SpotDirection.GetGLArray());
                    glLightf (GL_LIGHT0+i, GL_SPOT_EXPONENT,         Scene->Light[i].SpotExponent);
                    glLightf (GL_LIGHT0+i, GL_SPOT_CUTOFF,           Scene->Light[i].SpotCutoff);
                    glLightf (GL_LIGHT0+i, GL_CONSTANT_ATTENUATION,  Scene->Light[i].ConstantAttenuation);
                    glLightf (GL_LIGHT0+i, GL_LINEAR_ATTENUATION,    Scene->Light[i].LinearAttenuation);
                    glLightf (GL_LIGHT0+i, GL_QUADRATIC_ATTENUATION, Scene->Light[i].QuadraticAttenuation);
              } else glDisable(GL_LIGHT0+i);
        }
    }
    
    void Stop3d(void)
    {
        glDisable(GL_LIGHTING);
    }
    
    void Start2d(void)
    {
        glDisable(GL_DEPTH_TEST);
	    glMatrixMode(GL_PROJECTION);
	    glLoadIdentity();
//	    gluOrtho2D(0.0, (GLdouble)Scene->Size.X, (GLdouble)Scene->Size.Y, 0.0);
        glOrtho(0.0, (GLdouble)Width, (GLdouble)Height, 0.0, 0.0, 1.0);
	    glMatrixMode(GL_MODELVIEW);
	    glLoadIdentity();

//	    glTexEnvf(GL_TEXTURE_ENV, GL_TEXTURE_ENV_MODE, GL_MODULATE);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    }
    
    void End2d(void)
    {
        glEnable(GL_DEPTH_TEST);
    }
    
    void ObjScene::Draw(void)
    {
        glDepthFunc(GL_LESS);
        glEnable(GL_DEPTH_TEST);
        glEnable(GL_TEXTURE_2D);
        glDisable(GL_CULL_FACE);
        glDisable(GL_BLEND);
        glShadeModel(GL_SMOOTH);

        Scene->ListObjects.Draw();

        glClearColor(Scene->ColBackGround.Red / 255.0f, Scene->ColBackGround.Green / 255.0f, Scene->ColBackGround.Blue / 255.0f, Scene->ColBackGround.Alpha / 255.0f);
        glClearDepth(1.0);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        glLoadIdentity();
        glInitNames();
        Width  = this->Size.X==0.0f ? Video.Width :  int(this->Size.X);
        Height = this->Size.Y==0.0f ? Video.Height : int(this->Size.Y);
        glViewport(int(this->Position.X),
                   int(this->Position.Y),
                   Width, Height
        );

        if(!_Picking && Scene->BackGround)
        {
            // Les objets du background
            Start2d();
            Scene->ListBackObjects.Draw();
            End2d();
        }
        if(Scene->Enable3D)
        {
            // Les objets 3d
            Init3d();
            Scene->Camera.Define();

            if(_Picking) glPushName(0);
            Scene->List3dObjects.Draw();
            Stop3d();
         }
            
         if(!_Picking && Scene->FrontGround)
         {
             // Maintenant la 2d
             Start2d();
             Scene->List2dObjects.Draw();
             Scene->FrontFilter.Scale.Set(this->Size.X, this->Size.Y);
             Scene->FrontFilter.Draw();
             End2d();
         }

		 // Son
		 LISTE* i;
		 for(i=this->ListSound.m_pDebut; i != NULL; i=i->pSuivant)
            ((class ObjSound*)i->val)->Refresh();
         
    }
    
    void ObjScene::AddTag(ObjTag Tag)
    {
        this->Tag |= Tag;
    }
    
    void ObjScene::RemoveTag(ObjTag Tag)
    {
        this->Tag = this->Tag & ~Tag;
    }
};

