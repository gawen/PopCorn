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
    extern void* SDL_GL_LoadArray(SDL_Surface *surface, GLfloat *texcoord);

    MatImage::MatImage()
    {
        this->RelationHeight = 0;
        this->RelationWidth = 0;
        this->Image = NULL;
    }
    
    MatImage::~MatImage()
    {
        this->Clear();
    }
    
    void MatImage::Clear(void)
    {
        if(this->Image) delete (char*)this->Image;
        this->Size.Set(0, 0);
        this->RelationHeight = 0;
        this->RelationWidth = 0;
        this->Image = NULL;
    }
    
    void MatImage::operator=(MatImage& Tex)
    {
        this->Size = Tex.Size;
        this->RelationWidth = Tex.RelationWidth;
        this->RelationHeight = Tex.RelationHeight;
        memcpy(this->Image, Tex.Image, int(this->Size.X*this->Size.Y*4));
    }

#   define PixelArray(a, x, c)        *((unsigned char*)((unsigned long*)a+x)+c)
    void MatImage::SetBlur(bool h, bool v, bool d)
    {
        // !!! INVERSION AVEC Width ET Height !!!
        
        int x, y;
        int w_, h_;
        unsigned int r, g, b, a, i;
        void* p; void* p_;
        void* Image;
        Image = new char[int(this->Size.X)*int(this->Size.Y)*4];
        
        w_ = int(this->Size.X);
        h_ = int(this->Size.Y);
        
        for(y=0; y<h_; y++)
            for(x=0; x<w_; x++)
            {
                p=(char*)Image+x*4+(y*h_*4);
                p_=(char*)this->Image+x*4+(y*h_*4);
                i=1;
                
                r = PixelArray(p_,0,0);
                g = PixelArray(p_,0,1);
                b = PixelArray(p_,0,2);
                a = PixelArray(p_,0,3);
                
                if(h)
                {
                    if(x>0)
                    {
                        i++;
                        r += PixelArray(p_,-1,0);
                        g += PixelArray(p_,-1,1);
                        b += PixelArray(p_,-1,2);
                        a += PixelArray(p_,-1,3);
                    }

                    if(x<w_)
                    {
                        i++;
                        r += PixelArray(p_,1,0);
                        g += PixelArray(p_,1,1);
                        b += PixelArray(p_,1,2);
                        a += PixelArray(p_,1,3);
                    }
                }
                
                if(v)
                {
                    if(y>0)
                    {
                        i++;
                        r += PixelArray(p_,-h_,0);
                        g += PixelArray(p_,-h_,1);
                        b += PixelArray(p_,-h_,2);
                        a += PixelArray(p_,-h_,3);
                    }

                    if(y<h_)
                    {
                        i++;
                        r += PixelArray(p_,h_,0);
                        g += PixelArray(p_,h_,1);
                        b += PixelArray(p_,h_,2);
                        a += PixelArray(p_,h_,3);
                    }
                }
                
                if(d)
                {
                    if(x>0 && y>0)
                    {
                        i++;
                        r += PixelArray(p_,-h_-1,0);
                        g += PixelArray(p_,-h_-1,1);
                        b += PixelArray(p_,-h_-1,2);
                        a += PixelArray(p_,-h_-1,3);
                    }
                    
                    if(x<w_ && y>0)
                    {
                        i++;
                        r += PixelArray(p_,-h_+1,0);
                        g += PixelArray(p_,-h_+1,1);
                        b += PixelArray(p_,-h_+1,2);
                        a += PixelArray(p_,-h_+1,3);
                    }
                    
                    if(x>0 && y<h_)
                    {
                        i++;
                        r += PixelArray(p_,h_-1,0);
                        g += PixelArray(p_,h_-1,1);
                        b += PixelArray(p_,h_-1,2);
                        a += PixelArray(p_,h_-1,3);
                    }
                    
                    if(x<w_ && y<h_)
                    {
                        i++;
                        r += PixelArray(p_,h_+1,0);
                        g += PixelArray(p_,h_+1,1);
                        b += PixelArray(p_,h_+1,2);
                        a += PixelArray(p_,h_+1,3);
                    }
                }
                
                *((unsigned char*)((unsigned long*)p))   = char(r/i);
                *((unsigned char*)((unsigned long*)p)+1) = char(g/i);
                *((unsigned char*)((unsigned long*)p)+2) = char(b/i);
                *((unsigned char*)((unsigned long*)p)+3) = char(a/i);
                
            }
        
        delete (char*)this->Image;
        this->Image = (void*)Image;
    }
    
    bool MatImage::Load(const char* Str)
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
        this->Image = SDL_GL_LoadArray(Surface, TexCoord);
        SDL_FreeSurface(Surface);
        
        this->RelationWidth = TexCoord[2]/TexCoord[0];
        this->RelationHeight = TexCoord[3]/TexCoord[1];     
        this->Size.Set(TexCoord[2], TexCoord[3]);
        delete TexCoord;
    }
    
    bool MatImage::Load(unsigned int Width, unsigned int Height, void* Image)
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
        this->Image = SDL_GL_LoadArray(Surface, TexCoord);
        SDL_FreeSurface(Surface);
        
        this->RelationWidth = TexCoord[2]/TexCoord[0];
        this->RelationHeight = TexCoord[3]/TexCoord[1];        
        this->Size.Set(TexCoord[2], TexCoord[3]);
        delete TexCoord;
    }
};

