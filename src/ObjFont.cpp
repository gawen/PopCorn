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
#include <stdarg.h>

namespace PopCorn
{
    char Str_[512];
        
    MatFont::MatFont()
    {
        unsigned int i;
        for(i=0; i<256; i++)
	        this->Chars[i] = NULL;
    }
    
    MatFont::~MatFont()
    {
        unsigned int i;
        for(i=0; i<256; i++)
	        if(this->Chars[i] != NULL)
	            delete this->Chars[i];
    }
    
    void MatFont::Clear(void)
    {
        unsigned int i;
        for(i=0; i<256; i++)
	        if(this->Chars[i] != NULL)
	        {
	            delete this->Chars[i];
	            this->Chars[i] = NULL;
            }
    }
    
    bool MatFont::Load(const char* File)
    {
	    FILE* file;
	    FILE* a;
	    unsigned int i, j, _j,  y, x;
	    unsigned char h, w;
	    unsigned char* tmpa = NULL;
	    unsigned char* tmpb = NULL;
	    unsigned int tmplen;

#       ifdef __DEBUG__
        fprintf(Debug.Fp, "Loading of a MatFont (%s) ... ", File);
#       endif
        
        if((file = fopen(File, "rb")) == NULL)
		    return false;
	    
	    fread(&i, sizeof(char), 1, file);
		this->YInversed = i & 0x1;
	    
	    for(i=0; i<256; i++)
	        this->Chars[i] = NULL;

        while(!feof(file))
        {
            i=0;
            fread(&i, sizeof(char), 1, file);
            
            this->Chars[i] = new MatTexImage;
            fread(&w, sizeof(char), 1, file);
            fread(&h, sizeof(char), 1, file);
            
            tmplen = w * h;
            tmpb = new unsigned char[tmplen * 4];
            tmpa = new unsigned char[tmplen];
            if(tmpa == NULL || tmpb == NULL)
            {
                fprintf(stderr, "Can't load a image array for the character %u\n", i);
                return false;
            }

            fread(tmpa, tmplen, 1, file);
            
            // Conversion vers du RGBA_8_8_8_8 et inversement
/*            for(y=0; y < this->Chars[i]->y; y++)
                for(x=0; x < this->Chars[i]->x; x++)
                {
                    j = ((y * this->Chars[i]->x) + x) * 4;
                    _j = ((this->Chars[i]->y-y) * this->Chars[i]->x) + x;
                    
                    this->Chars[i]->alpha[j] = tmpa[_j];
                    this->Chars[i]->alpha[j+1] = 255;
                    this->Chars[i]->alpha[j+2] = 255;
                    this->Chars[i]->alpha[j+3] = 255;
                }
*/
              for(y=0; y<tmplen; y++)
              {
                    tmpb[(y*4)] = tmpa[y];
                    tmpb[(y*4)+1] = 255;
                    tmpb[(y*4)+2] = 255;
                    tmpb[(y*4)+3] = 255;
              }
              
              this->Chars[i]->Load(w, h, tmpb);
            
              delete tmpa;
              delete tmpb;
        }
        
#       ifdef __DEBUG__
        fprintf(Debug.Fp, "done\n", File);
#       endif
       
        fclose(file);
    }
    
/*    void MatFont::SetRGBA(unsigned char r, unsigned char g, unsigned char b, unsigned char a)
    {
        unsigned int i;
        unsigned char c;
        unsigned int tmplen;
        
        for(c=0; c<256; c++)
        {
            if(this->Chars[c] != NULL)
            {
                tmplen = this->Chars[c]->x * this->Chars[c]->y * 4;
                for(i=0; i<tmplen; i+=4)
                {
//                    if(this->Chars[c]->alpha[i] < a)
//                        this->Chars[c]->alpha[i] = 0;
//                    else
//                        this->Chars[c]->alpha[i] -= a;
                    this->Chars[c]->alpha[i+1] = b;
                    this->Chars[c]->alpha[i+2] = g;
                    this->Chars[c]->alpha[i+3] = r;
                }
            }
        }
    }*/
    
    Obj2dConsole::Obj2dConsole(void)
    {
        this->Font = NULL;
        this->Width = 0;
        this->Height = 0;
        this->Lines = 1;
    }
    
    void Obj2dConsole::Print(char* Str, ...)
    {
        unsigned int i;
        va_list list;
        
        if(this->Font == NULL) return;
        
        va_start(list, Str);
        vsprintf(Str_, Str, list);
        va_end(list);

        strcpy(this->Str + strlen(this->Str), Str_);
        
        this->Width = this->Font->WidthText(this->Str);
        this->Height = this->Font->HeightText(this->Str);
    }
    
    unsigned short MatFont::HeightText(const char* Str, ...)
    {
        unsigned int length;
        float max = 0.0f;
        unsigned int i;
        va_list list;
        
        va_start(list, Str);
        vsprintf(Str_, Str, list);
        va_end(list);
        
        length = strlen(Str_);
        
        for(i=0; i<length; i++)
            if(max < this->Chars[Str_[i]]->Size.Y)
                max = this->Chars[Str_[i]]->Size.Y;
        
        return int(max);
    }
    
    unsigned short MatFont::WidthText(const char* Str, ...)
    {
        unsigned int length;
        float cursor = 0.0f, max = 0.0f;
        unsigned int i;
        va_list list;
        
        va_start(list, Str);
        vsprintf(Str_, Str, list);
        va_end(list);
        
        length = strlen(Str_);
        
        for(i=0; i<length; i++)
        {
            if(Str_[i] == '\n')
                cursor = 0;
            else
                cursor += this->Chars[Str_[i]]->Size.X;
            
            if(max < cursor)
                max = cursor;
        }
        
        return int(max);
    }
    
    void MatFont::PrintChar(MatTexColor* Col, float x, float y, char c)
    {
        MatTexImage* Tex_;
        
        if(this->Chars[c] == NULL) return;
        
//        glEnable(GL_BLEND);
        Tex_ = this->Chars[c];
        
        glEnable(GL_BLEND);
        glPushMatrix();
            Tex_->Enable();
            Col->Enable();
            glTranslatef(x, y, 0.0f);
            glScalef(Tex_->Size.X, Tex_->Size.Y, 0.0f);
        
            glBegin(GL_QUADS);
                if(this->YInversed)
					 Tex_->TexCoord(0.0f, 0.0f);
				else Tex_->TexCoord(0.0f, 1.0f);
                    glVertex2f(0.0f, 0.0f);
            
                if(this->YInversed)
					 Tex_->TexCoord(0.0f, 1.0f);
				else Tex_->TexCoord(0.0f, 0.0f);
                    glVertex2f(0.0f, 1.0f);
            
                if(this->YInversed)
					 Tex_->TexCoord(1.0f, 1.0f);
				else Tex_->TexCoord(1.0f, 0.0f);
                    glVertex2f(1.0f, 1.0f);
            
                if(this->YInversed)
					 Tex_->TexCoord(1.0f, 0.0f);
				else Tex_->TexCoord(1.0f, 1.0f);
                    glVertex2f(1.0f, 0.0f);
            glEnd();
            glDisable(GL_BLEND);
        
            Col->Disable();
            Tex_->Disable();
        glPopMatrix();
        
//        glDisable(GL_BLEND);
    }
    
    void Obj2dConsole::Clear(void)
    {
        this->Str[0] = 0;
        this->Lines = 1;
    }

  void Obj2dConsole::_draw(GLERROR* Position, double Advance)
    {
        unsigned int i;
        unsigned int length;
        float posX, posY;
        float maxY;
        
        _PopCornSetInfoFile(Position);
        
        if(this->Font == NULL) return;
        
        length = strlen(this->Str);
        posX = this->Position.X;
        maxY = this->Font->HeightText(this->Str);
        posY = this->Position.Y;
        
        for(i=0; i<length; i++)
        {
            if(this->Str[i] == '\n')
            {
                posY += maxY;
                posX = this->Position.X;
                this->Lines++;
            } else {
                this->Font->PrintChar(&this->Col, posX, posY, this->Str[i]);
                posX += this->Font->Chars[this->Str[i]]->Size.X;
            }
        }
    }
    
    Obj2dLabel::Obj2dLabel(void)
    {
        this->Label[0] = '\0';
        this->Font = NULL;
        this->Width = 0;
        this->Height = 0;
    }
    
    void Obj2dLabel::_draw(GLERROR* Position, double Advance)
    {
        unsigned int i;
        unsigned int length;
        float posX, posY;
        float maxY;
        
        _PopCornSetInfoFile(Position);
        
        if(this->Font == NULL) return;
        
        length = strlen(this->Label);
        posX = 0.0f; posY = 0.0f;
        maxY = this->Height;
        
        for(i=0; i<length; i++)
        {
            if(this->Label[i] == '\n')
            {
                posY += maxY;
                posX = this->Position.X;
            } else {
                this->Font->PrintChar(&this->Col, posX, posY, this->Label[i]);
                posX += this->Font->Chars[this->Label[i]]->Size.X;
            }
        }
    }
    
    void Obj2dLabel::SetLabel(char *Label, ...)
    {
        va_list list;
        if(this->Font == NULL) return;
        
        if(Label == NULL)
            this->Label[0] = '\0';
        else
        {
            va_start(list, Label);
            vsprintf(this->Label, Label, list);
            va_end(list);
            
            this->Width = this->Font->WidthText(Label);
            this->Height = this->Font->HeightText(Label);
        }
    }
};

