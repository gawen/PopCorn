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
    const double Pi180 = PI / 180.0f;
    
    /*
	=============================================
	Color
	=============================================
	Classe contenant une couleur
    */
    MatTexColor::MatTexColor(void)
    {
        this->Red = 255;
        this->Green = 255;
        this->Blue = 255;
        this->Alpha = 255;
    }
    
    void MatTexColor::Set(unsigned char Red, unsigned char Green, unsigned char Blue)
    {
        this->Red = Red;
        this->Green = Green;
        this->Blue = Blue;
    }
    
    void MatTexColor::Set(unsigned char Red, unsigned char Green, unsigned char Blue, unsigned char Alpha)
    {
        this->Red = Red;
        this->Green = Green;
        this->Blue = Blue;
        this->Alpha = Alpha;
    }
    
    void MatTexColor::EnableRGB(void)
    {
        glColor3f(this->Red / 255.0f, this->Green / 255.0f, this->Blue / 255.0f);
    }
    
    void MatTexColor::Enable(void)
    {
//        glBindTexture(GL_TEXTURE_2D, 0);
        glColor4f(this->Red / 255.0f, this->Green / 255.0f, this->Blue / 255.0f, this->Alpha / 255.0f);
    }
    
    void MatTexColor::DisableRGB(void)
    {
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    void MatTexColor::Disable(void)
    {
        glDisable(GL_BLEND);
        glColor4f(1.0f, 1.0f, 1.0f, 1.0f);
    }
    
    void MatTexColor::TexCoord(float s, float t)
    {
        // Rien :)
    }
    
    float* MatTexColor::GetGLArray(void)
    {
        this->Array[0]=this->Red / 255.0f;
        this->Array[1]=this->Green / 255.0f;
        this->Array[2]=this->Blue / 255.0f;
        this->Array[3]=this->Alpha / 255.0f;
        return this->Array;
    }
    
    
    /*
	=============================================
	Color
	=============================================
	Classe contenant une couleur
    */
    Vector::Vector(void)
    {
        this->X=0; this->Y=0; this->Z=0;
        this->Array[3] = 1.0f;
    }
    
    Vector::Vector(float X, float Y, float Z)
    {
        this->X=X; this->Y=Y; this->Z=Z;
    }
    
    void Vector::Set(float X, float Y, float Z)
    {
        this->X=X; this->Y=Y; this->Z=Z;
    }
    
    void Vector::Set(float X, float Y)
    {
        this->X=X; this->Y=Y; this->Z=0.0f;
    }
    
    void Vector::Position(void)
    {
        glTranslatef(this->X, this->Y, this->Z);
    }
    
    void Vector::Scale(void)
    {
        glScalef(this->X, this->Y, this->Z);
    }
    
    void Vector::Rotate(void)
    {
        if(this->X != 0.0f) glRotatef(this->X, 1.0f, 0.0f, 0.0f);
        if(this->Y != 0.0f) glRotatef(this->Y, 0.0f, 1.0f, 0.0f);
        if(this->Z != 0.0f) glRotatef(this->Z, 0.0f, 0.0f, 1.0f);
    }
    
    bool Vector::isNull(void)
    {
        return this->X == 0.0f && this->Y == 0.0f && this->Z == 0.0f;
    }
    
    void Vector::operator=(Vector& Coord)
    {
        this->X = Coord.X;
        this->Y = Coord.Y;
        this->Z = Coord.Z;
    }
    
    float* Vector::GetGLArray(void)
    {
        this->Array[0] = this->X;
        this->Array[1] = this->Y;
        this->Array[2] = this->Z;        
        return this->Array;
    }
    
	double Sin(float Angle)
	{
	    return sin(Angle*Pi180);
	}
	
	double Cos(float Angle)
	{
	    return cos(Angle*Pi180);
	}
	
	double Tan(float Angle)
	{
	    return tan(Angle*Pi180);
	}
};

