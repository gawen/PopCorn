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

// Video.cpp, spécialisé dans l'utilisation de la SDL.

#include "popcorn.hpp"

namespace PopCorn
{
    class _VideoConfig Video;
    bool _Picking;
    
    /*********************************************
	Classe _VideoConfig
    **********************************************/
    _VideoConfig::_VideoConfig(void)
    {
		//initialisation des valeurs par defaut : 800*600*32 fenetre
		//contexte OpenGL, acces direct au materiel et pas de decoration fenetre
		this->Width = 800;
		this->Height = 600;  
        this->FullScreen = false;
		this->flags = ( SDL_HWSURFACE | SDL_OPENGL | SDL_NOFRAME );
        this->ConsoleFont = NULL;
        this->ConsoleWriter = NULL;
        _Picking = false;
        this->MovementLifting = 2;
        
        this->MultiTexturing = false;
        this->VertexBuffer = false;
        this->EXTBlend = false;
    }
    
    void _VideoConfig::SetVideoResolution(unsigned short width, unsigned short height, bool fullscreen)
    {
        this->Width = width; this->Height = height; this->FullScreen = fullscreen;
    }
    
    bool _VideoConfig::Init(void)
    {
        //si fullscreen, on update le flags video,
		//on initialise le subsystem video de SDL et on enregistre un callback de sortit
		//on enregistre les parametres de l'utilisateur
		//on teste si le mode video demandé est possible
		//on retourne un booléen pour le resultat de l'opération
		
        #ifdef __DEBUG__
		SDL_Surface *VideoSurface;
		const SDL_VideoInfo *Info;
		GLint GLetat = 0;
		fprintf(Debug.Fp,"***Initialisation vidéo ");
		#endif
		
		if(this->FullScreen)
			this->flags = ( this->flags | SDL_FULLSCREEN );
		
        #ifdef __GDB__
        if(SDL_Init(SDL_INIT_VIDEO | SDL_INIT_NOPARACHUTE) < 0)
        #else
        if(SDL_Init(SDL_INIT_VIDEO) < 0)
		#endif
		{
            #ifdef __DEBUG__
		    fprintf(Debug.Fp, "[ERROR] : Init\n");
		    #endif
            fprintf(stderr,"PopCorn n'a pas pu initialiser le subsystem video de SDL: %s\n", SDL_GetError());
			return false;
		}
		
		if(!(SDL_VideoModeOK(this->Width, this->Height, 32, this->flags)>0)) {
            #ifdef __DEBUG__
		    fprintf(Debug.Fp, "[ERROR] : Video mode\n");
		    #endif
            fprintf(stderr,"PopCorn n'a pas pu utiliser le mode video %d*%d*32: %s\n",this->Width, this->Height,SDL_GetError());
		    return false;
        }
		
    	//on defini clairement le contexte openGL :
		//+ 8 bits pour les composantes R,G,B,Alpha (soit 32bits)
		//+ 16 bits de Z buffer
		//+ 32bits pour le frame buffer
		//+ 16 bits pour le stencils buffer
		//+ 32bits pour l' Accum buffer (8bits pour R,G,B,alpha)
		//+ Double Buffering
		//on tente de lancer l'affichage, si echec on met à jour le log
		SDL_GL_SetAttribute(SDL_GL_RED_SIZE,     8);
        SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE,   8);
        SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE,    8);
        SDL_GL_SetAttribute(SDL_GL_ALPHA_SIZE,   8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_RED_SIZE,	8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_GREEN_SIZE,8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_BLUE_SIZE, 8);
		SDL_GL_SetAttribute(SDL_GL_ACCUM_ALPHA_SIZE,8);
        SDL_GL_SetAttribute(SDL_GL_BUFFER_SIZE,  32);
        SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE,   16);
        SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 16);
        SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
		if(SDL_SetVideoMode(this->Width, this->Height, 32, this->flags) == NULL)
		{
            #ifdef __DEBUG__
		    fprintf(Debug.Fp, "[ERROR] : Resolution\n");
		    #endif
			fprintf(stderr,"PopCorn ne peut pas initialiser la video SDL: %s\n", SDL_GetError());
			return false;
		}
		#ifdef __DEBUG__
		    fprintf(Debug.Fp, "[OK]\n");
			VideoSurface = SDL_GetVideoSurface();
			fprintf(Debug.Fp,"**Info sur la vidéo\n");
			fprintf(Debug.Fp,"Résolution : %dx%dx32\n",VideoSurface->w, VideoSurface->h);
			fprintf(Debug.Fp,"Length of a surface scanline in bytes : %d, Surface flags : %x\n", VideoSurface->pitch, VideoSurface->flags);
			Info = SDL_GetVideoInfo();
			fprintf(Debug.Fp,"Hardware surfaces ? %x\n",Info->hw_available);
			fprintf(Debug.Fp,"Window manager available ? %x\n",Info->wm_available);
			fprintf(Debug.Fp,"Hardware :\n - blits accelerated ? %x\n",Info->blit_hw);
			fprintf(Debug.Fp," - colorkey blits accelerated ? %x\n",Info->blit_hw_CC);
			fprintf(Debug.Fp," - alpha blits accelerated ? %x\n",Info->blit_hw_A);
			fprintf(Debug.Fp," - blits accelerated ? %x\n",Info->blit_sw);
			fprintf(Debug.Fp," - colorkey blits accelerated ? %x\n",Info->blit_sw_CC);
			fprintf(Debug.Fp," - alpha blits accelerated ? %x\n",Info->blit_sw_A);
			fprintf(Debug.Fp,"Color fills accelerated ? %x\n",Info->blit_fill);
			fprintf(Debug.Fp,"Video memory : %d ko\n",Info->video_mem);
			
            fprintf(Debug.Fp,"**OpenGL :\n");
			//nombre de buffers auxilliaires
			glGetIntegerv(GL_AUX_BUFFERS,&GLetat);
			fprintf(Debug.Fp,"%d buffers auxilliaires\n", GLetat);

			//info sur le tampon d'accumulation
			fprintf(Debug.Fp,"Tampon d'accumulation : ");
			glGetIntegerv(GL_ACCUM_ALPHA_BITS,&GLetat);
			fprintf(Debug.Fp,"R%d",GLetat);
			glGetIntegerv(GL_ACCUM_BLUE_BITS,&GLetat);
			fprintf(Debug.Fp,"G%d",GLetat);
			glGetIntegerv(GL_ACCUM_GREEN_BITS,&GLetat);
			fprintf(Debug.Fp,"B%d",GLetat);
			glGetIntegerv(GL_ACCUM_RED_BITS,&GLetat);
			fprintf(Debug.Fp,"A%d\n",GLetat);

			glGetIntegerv(GL_MAX_CLIENT_ATTRIB_STACK_DEPTH,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_CLIENT_ATTRIB_STACK_DEPTH : %d\n",GLetat);
			glGetIntegerv(GL_MAX_ATTRIB_STACK_DEPTH,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_ATTRIB_STACK_DEPTH : %d\n",GLetat);
			glGetIntegerv(GL_MAX_CLIP_PLANES,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_CLIP_PLANES : %d\n",GLetat);
			glGetIntegerv(GL_MAX_EVAL_ORDER,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_EVAL_ORDER : %d\n",GLetat);
			glGetIntegerv(GL_MAX_LIGHTS,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_LIGHTS : %d\n",GLetat);
			glGetIntegerv(GL_MAX_LIST_NESTING,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_LIST_NESTING : %d\n",GLetat);
			glGetIntegerv(GL_MAX_MODELVIEW_STACK_DEPTH,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_MODELVIEW_STACK_DEPTH : %d\n",GLetat);
			glGetIntegerv(GL_MAX_NAME_STACK_DEPTH,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_NAME_STACK_DEPTH : %d\n",GLetat);
			glGetIntegerv(GL_MAX_PIXEL_MAP_TABLE,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_PIXEL_MAP_TABLE : %d\n",GLetat);
			glGetIntegerv(GL_MAX_PROJECTION_STACK_DEPTH,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_PROJECTION_STACK_DEPTH : %d\n",GLetat);
			glGetIntegerv(GL_MAX_TEXTURE_SIZE,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_TEXTURE_SIZE : %d\n",GLetat);
			glGetIntegerv(GL_MAX_TEXTURE_STACK_DEPTH,&GLetat);
			fprintf(Debug.Fp,"GL_MAX_TEXTURE_STACK_DEPTH : %d\n",GLetat);
			glGetIntegerv(GL_STEREO,&GLetat);
			fprintf(Debug.Fp,"GL_STEREO : %d\n",GLetat);

			fprintf(Debug.Fp,"Driver Company : %s\n",glGetString(GL_VENDOR));
			fprintf(Debug.Fp,"Renderer Name : %s\n",glGetString(GL_RENDERER));
			fprintf(Debug.Fp,"Version : %s\n",glGetString(GL_VERSION));
			fprintf(Debug.Fp,"GLU Version : %s\n",gluGetString(GLU_VERSION));
			fprintf(Debug.Fp,"GLU Extensions : %s\n",gluGetString(GLU_EXTENSIONS));
			fprintf(Debug.Fp,"OpenGL Extensions :\n");
		#endif
		
		// Chargement des extensions OpenGL
        
        InitGLExtensions();
        
        #ifdef __DEBUG__
		fprintf(Debug.Fp,"\n");
		#endif
		
		
		// Chargement des plug ins pour les particules
		_InitParticlesPlugins();
        
		
		return true;
    }
    
    void _VideoConfig::Flush(void)
	{
        SDL_GL_SwapBuffers();
    }
    
	void _VideoConfig::ShowMouse(bool Activate)
	{
	    SDL_ShowCursor( Activate ? SDL_ENABLE : SDL_DISABLE );
	}
	
/*	void _VideoConfig::InstallConsole(void)
	{
	    this->ConsoleWriter = new Obj2dWriter;
	    this->ConsoleWriter->Font = new Obj2dFont;
	    
	    this->ConsoleWriter->Font->Load("default.pcf");
	}
*/
	
    _VideoConfig::~_VideoConfig()
    {
        if(this->ConsoleFont != NULL)
        {
            delete this->ConsoleFont;
            delete this->ConsoleWriter;
        }
        
        SDL_Quit();
    }
    
    void _VideoConfig::SetCaption(char* Label)
    {
        SDL_WM_SetCaption(Label, NULL);
    }
};
