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
    extern GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord, GLuint Tex);
    extern GLuint CreateTexture(void* Image, float w, float h, GLuint Tex);

    MatTexImage::MatTexImage(void)
    {
        this->Tex = 0;
        this->RelationWidth = 0.0f; this->RelationHeight = 0.0f;
        this->Mirror = false;
        this->Col.Set(255, 255, 255, 255);        // White
    }
    
    MatTexImage::~MatTexImage(void)
    {
        this->Clear();
    }
    
    void MatTexImage::Clear(void)
    {
        glDeleteTextures(1, &this->Tex);
        this->Tex = 0;
    }
    
    bool MatTexImage::Load(const char* Str)
    {
        SDL_Surface* Surface = NULL;
        
        if( (Surface=IMG_Load(Str)) == NULL)
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, "Can't load the file %s\n", Str);
#           endif
            return false;
        }
        
        GLfloat* TexCoord = new GLfloat[4];
        this->Tex = SDL_GL_LoadTexture(Surface, TexCoord, this->Tex);
        SDL_FreeSurface(Surface);
        
        this->RelationWidth = TexCoord[2]/TexCoord[0];
        this->RelationHeight = TexCoord[3]/TexCoord[1];     
        this->Size.Set(TexCoord[2], TexCoord[3]);
        delete TexCoord;
    }

    bool MatTexImage::Load(unsigned int Width, unsigned int Height, void* Image)
    {
	    SDL_Surface* Surface;

	    Surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
                                       Width, Height,
                                       32,
		                           	   0xFF000000,
		                           	   0x00FF0000, 
		                               0x0000FF00, 
		                               0x000000FF
            );
        memcpy(Surface->pixels, Image, Width*Height * 4);
        
        GLfloat* TexCoord = new GLfloat[4];
        this->Tex = SDL_GL_LoadTexture(Surface, TexCoord, this->Tex);
        SDL_FreeSurface(Surface);
        
        this->RelationWidth = TexCoord[2]/TexCoord[0];
        this->RelationHeight = TexCoord[3]/TexCoord[1];        
        this->Size.Set(TexCoord[2], TexCoord[3]);
        delete TexCoord;
    }
    
    bool MatTexImage::Load(unsigned int Width, unsigned int Height)
    {
	    SDL_Surface* Surface;

	    Surface = SDL_CreateRGBSurface(SDL_SWSURFACE | SDL_SRCALPHA,
                                       Width, Height,
                                       32,
		                           	   0xFF000000,
		                           	   0x00FF0000, 
		                               0x0000FF00, 
		                               0x000000FF
            );

        memset(Surface->pixels, 0, Width*Height*4);
        
        GLfloat* TexCoord = new GLfloat[4];
        this->Tex = SDL_GL_LoadTexture(Surface, TexCoord, this->Tex);
        SDL_FreeSurface(Surface);
        
        this->RelationWidth = TexCoord[2]/TexCoord[0];
        this->RelationHeight = TexCoord[3]/TexCoord[1];        
        this->Size.Set(TexCoord[2], TexCoord[3]);
        delete TexCoord;
    }
    
    bool MatTexImage::Load(MatImage& Image)
    {
        this->Tex = CreateTexture(Image.Image, Image.Size.X, Image.Size.Y, this->Tex);
        this->Size = Image.Size;
        this->RelationWidth = Image.RelationWidth;
        this->RelationHeight = Image.RelationHeight;
    }
    
    bool MatTexImage::Build(ObjScene& Scene, ObjTag Tag)
    {
        Vector SizeTmp;
        Vector PositionTmp;
        ObjTag TagTmp;
        
        SizeTmp = Scene.Size;
        PositionTmp = Scene.Position;
        TagTmp = Scene.Tag;

        Scene.Size = this->Size;
        Scene.Position.Set(0.0f, 0.0f);
        Scene.Tag = Tag;
        
        glPushMatrix();
            Scene.Draw();
            glBindTexture(GL_TEXTURE_2D, this->Tex);
            
    	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	        glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glCopyTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, 0, 0, int(this->Size.X), int(this->Size.Y), 0);
        glPopMatrix();
	    
        Scene.Size = SizeTmp;
        Scene.Position = PositionTmp;
        Scene.Tag = TagTmp;
    }
    
    void MatTexImage::operator=(MatTexImage& Tex)
    {
        this->RelationWidth = Tex.RelationWidth;
        this->RelationHeight = Tex.RelationHeight;        
        this->Size = Tex.Size;
        this->Tex = Tex.Tex;
    }
    
    void MatTexImage::Enable(void)
    {
        if(this->Mirror)
        {
            glTexGeni(GL_S, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
            glTexGeni(GL_T, GL_TEXTURE_GEN_MODE, GL_SPHERE_MAP);
        
            glEnable(GL_TEXTURE_GEN_S);
            glEnable(GL_TEXTURE_GEN_T);
        }
                
        this->Col.Enable();
        glBindTexture(GL_TEXTURE_2D, this->Tex);
    }
    
    void MatTexImage::Disable(void)
    {
        if(this->Mirror)
        {
            glDisable(GL_TEXTURE_GEN_S);
            glDisable(GL_TEXTURE_GEN_T);
        }
        
        glBindTexture(GL_TEXTURE_2D, 0);
        this->Col.Disable();
    }
    
    void MatTexImage::TexCoord(float s, float t)
    {
        glTexCoord2f(s * this->RelationWidth, 1.0f-(t * this->RelationHeight));
    }
};

