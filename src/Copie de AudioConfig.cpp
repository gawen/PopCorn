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
    class _AudioConfig Audio;
    
    struct AudioConfig
    {
        int mixrate;
        int maxsoftwarechannels;
    };

    /*********************************************
	Classe _AudioConfig
    **********************************************/
    _AudioConfig::_AudioConfig(void)
    {
        this->mixrate = 44100;
        this->channels = 32;
    }
    
    void _AudioConfig::SetAudioResolution(int mixrate,int channels)
    {
        //on enregistre le desirata de l'utilisateurs
		this->channels = channels;
        if(4000 < mixrate && 65535 > mixrate)
            this->mixrate = mixrate;
    }

	
    bool _AudioConfig::InitSound(int mixrate, int maxsoftwarechannels)
    {
        if(!FSOUND_Init(mixrate, maxsoftwarechannels, 0))
		{
#		    ifdef __DEBUG__
			fprintf(Debug.Fp,"[Echec]\n");
#			endif
            return false;
        }
        
        return true;
    }
    
    bool _AudioConfig::Init(void)
	{
		struct AudioConfig AConfig[5];
		int i;
		bool AudioInit=false;
		AConfig[0].mixrate = this->mixrate; AConfig[0].maxsoftwarechannels = this->channels;
		AConfig[1].mixrate = 44100; AConfig[1].maxsoftwarechannels = 32;
		AConfig[2].mixrate = 22050; AConfig[2].maxsoftwarechannels = 32;
		AConfig[3].mixrate = 44100; AConfig[3].maxsoftwarechannels = 16;
		AConfig[4].mixrate = 22050; AConfig[4].maxsoftwarechannels = 16;
		
        #ifdef __DEBUG__
		fprintf(Debug.Fp, "***Initialisation audio ...\n");
		#endif
        
        for(i=0; i<5; i++)
        {
#           ifdef __DEBUG__
            fprintf(Debug.Fp,"Le moteur va tenter d'utiliser %uHz, %u Cannaux ", AConfig[i].mixrate, AConfig[i].maxsoftwarechannels);
#           endif
            if(this->InitSound(AConfig[i].mixrate, AConfig[i].maxsoftwarechannels))
            {
#               ifdef __DEBUG__
                fprintf(Debug.Fp,"[OK]\n");
#               endif
            	this->channels = AConfig[i].maxsoftwarechannels;
				this->mixrate = AConfig[i].mixrate;
                AudioInit = true;
                break;
            }
#           ifdef __DEBUG__
            fprintf(Debug.Fp,"[ECHEC]\n");
#           endif
        }
        if(!AudioInit)
        {
            fprintf(stderr, "Can't initialize the sound driver\n ...");
            exit(-1);
        }

#       ifdef __DEBUG__
        fprintf(Debug.Fp, "Initialisation audio [OK]\n");
#       endif
        
		
		#ifdef __DEBUG__
		fprintf(Debug.Fp,"Driver son : %s\n",FSOUND_GetDriverName(FSOUND_GetDriver()));
		fprintf(Debug.Fp,"Sortie Enchatillonage : %d\n",FSOUND_GetOutputRate());
		fprintf(Debug.Fp,"Nombre de Cannaux alloués : %d\n",FSOUND_GetMaxChannels());
		if(FSOUND_GetOutput() == FSOUND_OUTPUT_NOSOUND) fprintf(Debug.Fp,"Pas de driver de son...\n");
		if(FSOUND_GetOutput() == FSOUND_OUTPUT_DSOUND) fprintf(Debug.Fp,"Driver DirectSound...\n");
		if(FSOUND_GetOutput() == FSOUND_OUTPUT_OSS) fprintf(Debug.Fp,"Kernel Linux Driver\n");
		if(FSOUND_GetOutput() == FSOUND_OUTPUT_ESD) fprintf(Debug.Fp,"Enlightment Linux Driver\n");
		if(FSOUND_GetOutput() == FSOUND_OUTPUT_ALSA) fprintf(Debug.Fp,"Alsa Linux Driver\n");
		if(FSOUND_GetMixer() == FSOUND_MIXER_QUALITY_FPU) fprintf(Debug.Fp,"Interpolation utilisant le FPU mixer\n");
		if(FSOUND_GetMixer() == FSOUND_MIXER_QUALITY_MMXP5) fprintf(Debug.Fp,"Interpolation utilisant le P5 MMX mixer\n");
		if(FSOUND_GetMixer() == FSOUND_MIXER_QUALITY_MMXP6) fprintf(Debug.Fp,"Interpolation utilisant le ppro+ MMX mixer\n");
		fprintf(Debug.Fp,"Nombre de drivers sonnores : %d\n",FSOUND_GetNumDrivers());
		fprintf(Debug.Fp,"\n");
		#endif
		
		return true;
	}
	
	_AudioConfig::~_AudioConfig()
	{
//	    FSOUND_Close();  // <-- Bizarre, provoque une erreur
	}
	
	void _AudioConfig::Flush(void)
	{
	    static int Error;
        static const char* StrError;
        FSOUND_Update();
        
	    // Vérification des erreurs possibles
	    while( (Error=FSOUND_GetError()) != FMOD_ERR_NONE )
	    { // Erreur
	        switch(Error)
	        {
	            case FMOD_ERR_BUSY: StrError = "Sound> Cannot call this command after FSOUND_Init. Call FSOUND_Close first.\n"; break;
	            case FMOD_ERR_UNINITIALIZED: StrError = "Sound> This command failed because FSOUND_Init or FSOUND_SetOutput was not called.\n"; break;
	            case FMOD_ERR_INIT: StrError = "Sound> Error initializing output device.\n"; break;
	            case FMOD_ERR_ALLOCATED: StrError = "Sound> Error initializing output device, but more specifically, the output device is already in use and cannot be reused.\n"; break;
	            case FMOD_ERR_PLAY: StrError = "Sound> Playing the sound failed.\n"; break;
	            case FMOD_ERR_OUTPUT_FORMAT: StrError = "Sound> Soundcard does not support the features needed for this soundsystem (16bit stereo output).\n"; break;
	            case FMOD_ERR_COOPERATIVELEVEL: StrError = "Sound> Error setting cooperative level for hardware.\n"; break;
	            case FMOD_ERR_CREATEBUFFER: StrError = "Sound> Error creating hardware sound buffer.\n"; break;
	            case FMOD_ERR_FILE_NOTFOUND: StrError = "Sound> File not found.\n"; break;
	            case FMOD_ERR_FILE_FORMAT: StrError = "Sound> Unknown file format.\n"; break;
	            case FMOD_ERR_FILE_BAD: StrError = "Sound> Error loading file.\n"; break;
	            case FMOD_ERR_MEMORY: StrError = "Sound> Not enough memory or resources.\n"; break;
	            case FMOD_ERR_VERSION: StrError = "Sound> The version number of this file format is not supported.\n"; break;
	            case FMOD_ERR_INVALID_PARAM: StrError = "Sound> An invalid parameter was passed to this function.\n"; break;
	            case FMOD_ERR_NO_EAX: StrError = "Sound> Tried to use an EAX command on a non EAX enabled channel or output.\n"; break;
	            case FMOD_ERR_CHANNEL_ALLOC: StrError = "Sound> Failed to allocate a new channel.\n"; break;
	            case FMOD_ERR_RECORD: StrError = "Sound> Recording is not supported on this machine.\n"; break;
	            case FMOD_ERR_MEDIAPLAYER: StrError = "Sound> Windows Media Player not installed so cannot play wma or use internet streaming.\n"; break;
	            case FMOD_ERR_CDDEVICE: StrError = "Sound> An error occured trying to open the specified CD device.\n"; break;
	        }
	        
	        _PopCornError(StrError);
        }
        
	}
};
