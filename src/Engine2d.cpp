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
static int power_of_two(int input)
{
	int value = 1;

	while ( value < input ) {
		value <<= 1;
	}
	return value;
}    

void* SDL_GL_LoadArray(SDL_Surface *surface, GLfloat *texcoord)
{
	int w, h;
	SDL_Surface *image;
	SDL_Rect area;
	Uint32 saved_flags;
	Uint8  saved_alpha;
	void* Image;

	/* Use the surface width and height expanded to powers of 2 */
	w = power_of_two(surface->w);
	h = power_of_two(surface->h);
	texcoord[0] = (GLfloat)w;
	texcoord[1] = (GLfloat)h;
	texcoord[2] = (GLfloat)surface->w;	/* Max X */
	texcoord[3] = (GLfloat)surface->h;	/* Max Y */

	image = SDL_CreateRGBSurface(
			SDL_SWSURFACE,
			w, h,
			32,
#if SDL_BYTEORDER == SDL_LIL_ENDIAN /* OpenGL RGBA masks */
			0x000000FF, 
			0x0000FF00, 
			0x00FF0000, 
			0xFF000000
#else
			0xFF000000,
			0x00FF0000, 
			0x0000FF00, 
			0x000000FF
#endif
		       );
	if ( image == NULL ) {
		return 0;
	}

	/* Save the alpha blending attributes */
	saved_flags = surface->flags&(SDL_SRCALPHA|SDL_RLEACCELOK);
	saved_alpha = surface->format->alpha;
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, 0, 0);
	}

	/* Copy the surface into the GL texture image */
	area.x = 0;
	area.y = 0;
	area.w = surface->w;
	area.h = surface->h;
	SDL_BlitSurface(surface, &area, image, &area);

	/* Restore the alpha blending attributes */
	if ( (saved_flags & SDL_SRCALPHA) == SDL_SRCALPHA ) {
		SDL_SetAlpha(surface, saved_flags, saved_alpha);
	}
		
	Image = new char[w*h*4];
    memcpy(Image, image->pixels, w*h*4);
	
	SDL_FreeSurface(image); /* No longer needed */
	
	return Image;
}

GLuint CreateTexture(void* Image, float w, float h, GLuint Tex)
{
    GLuint texture;
    if(Tex == 0)
    {
	    glGenTextures(1, &texture);
        glBindTexture(GL_TEXTURE_2D, texture);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);

        glTexImage2D(GL_TEXTURE_2D,
	    	     0,
		         GL_RGBA,
		         int(w), int(h),
		         0,
		         GL_RGBA,
		         GL_UNSIGNED_BYTE,
		         Image);
        return texture;
    } else {
	    glBindTexture(GL_TEXTURE_2D, Tex);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	    glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	    
        glTexSubImage2D(GL_TEXTURE_2D,
                 0,
                 0, 0,
                 int(w), int(h),
                 GL_RGBA,
                 GL_UNSIGNED_BYTE,
                 Image);
        glBindTexture(GL_TEXTURE_2D, 0);
        return Tex;
    }
}

GLuint SDL_GL_LoadTexture(SDL_Surface *surface, GLfloat *texcoord, GLuint Tex)
{
	GLuint texture;
	void* Image;
	Image = SDL_GL_LoadArray(surface, texcoord);
	texture = CreateTexture(Image, texcoord[0], texcoord[1], Tex);

    delete (char*)Image;

	return texture;
}

};
