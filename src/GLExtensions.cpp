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

#include <stdlib.h>

#define IsExtensionSupported(a,b)  strstr(a,b)

namespace PopCorn
{
    PFNGLACTIVETEXTUREARBPROC glActiveTextureARB = NULL;
    PFNGLMULTITEXCOORD2FARBPROC glMultiTexCoord2fARB = NULL;

    PFNGLGENBUFFERSARBPROC glGenBuffersARB = NULL;
    PFNGLBINDBUFFERARBPROC glBindBufferARB = NULL;
    PFNGLBUFFERDATAARBPROC glBufferDataARB = NULL;
    PFNGLDELETEBUFFERSARBPROC glDeleteBuffersARB = NULL;
    PFNGLBLENDEQUATIONEXTPROC glBlendEquationEXT = NULL;
    
    void* LoadExtensions(const char* Str)
    {
		void *extproc;

		extproc = (void *) SDL_GL_GetProcAddress(Str);
		
		if(extproc == NULL)
		{
		    fprintf(stderr, "Can't load the OpenGL function %s.\n", Str);
		}

    	return extproc;
    }
    
    void InitGLExtensions(void)
    {
        const char* Extensions = (char*)glGetString(GL_EXTENSIONS);
        
        if(IsExtensionSupported(Extensions, "GL_ARB_multitexture"))
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, " - Le multitexturing est géré ...\n");
#           endif

            glActiveTextureARB = (PFNGLACTIVETEXTUREARBPROC)LoadExtensions("glActiveTextureARB");
            glMultiTexCoord2fARB = (PFNGLMULTITEXCOORD2FARBPROC)LoadExtensions("glMultiTexCoord2fARB");
            Video.MultiTexturing = true;
        }
#           ifdef __DEBUG__
            else fprintf(Debug.Fp, " - Le multitexturing n'est pas géré ...\n");
#           endif
        
        if(IsExtensionSupported(Extensions, "GL_ARB_vertex_buffer_object"))
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, " - Le Vertex Buffer est géré ...\n");
#           endif

		    glGenBuffersARB = (PFNGLGENBUFFERSARBPROC)LoadExtensions("glGenBuffersARB");
		    glBindBufferARB = (PFNGLBINDBUFFERARBPROC)LoadExtensions("glBindBufferARB");
		    glBufferDataARB = (PFNGLBUFFERDATAARBPROC)LoadExtensions("glBufferDataARB");
		    glDeleteBuffersARB = (PFNGLDELETEBUFFERSARBPROC)LoadExtensions("glDeleteBuffersARB");
		    
		    Video.VertexBuffer = true;
        }
#           ifdef __DEBUG__
            else fprintf(Debug.Fp, " - Le Vertex Buffer n'est pas géré ...\n");
#           endif
        
        if(IsExtensionSupported(Extensions, "GL_EXT_blend_minmax"))
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp, " - Le Blending évolué est géré ...\n");
#           endif
            
            glBlendEquationEXT = (PFNGLBLENDEQUATIONEXTPROC)LoadExtensions("glBlendEquationEXT");
            
            Video.EXTBlend = true;
        }
#           ifdef __DEBUG__
            else fprintf(Debug.Fp, " - Le Blending évolué n'est pas géré ...\n");
#           endif
    }

}
