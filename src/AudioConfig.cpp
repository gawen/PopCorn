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
        this->Context = NULL;
		this->Device = NULL;
		this->DopplerVelocity = 332.0f;	// Vitesse du son	1.0f unité = 1 mètre
		this->DopplerFactor = 1.0f;		// 100%
    }
	
    bool _AudioConfig::Init(void)
    {
        if((this->Device = alcOpenDevice((ALubyte*)NULL)) == NULL)
		{
#           ifdef __DEBUG__
			fprintf(Debug.Fp, "Can't open a sound device.\n");
#           endif
			return false;
		}

		if((this->Context=alcCreateContext(this->Device, NULL)) == NULL)
		{
#           ifdef __DEBUG__
			fprintf(Debug.Fp, "Can't create a sound context.\n");
#           endif
			return false;
		}
		alcMakeContextCurrent(this->Context);

		alDopplerFactor(this->DopplerFactor);
		alDopplerVelocity(this->DopplerVelocity);
        
		{
			ALenum error;
			
			while((error=alGetError()) != AL_NO_ERROR)
			{
				switch(error)
				{
				case AL_INVALID_NAME:      _PopCornError("OpenAL> Invalid Name parameter.");
				case AL_INVALID_ENUM:      _PopCornError("OpenAL> Invalid parameter.");
				case AL_INVALID_VALUE:     _PopCornError("OpenAL> Invalid enum parameter value.");
				case AL_INVALID_OPERATION: _PopCornError("OpenAL> Illegal call.");
				case AL_OUT_OF_MEMORY:     _PopCornError("OpenAL> Unable to allocate memory.");
				}
			}
		}

		return true;
	}
	
	_AudioConfig::~_AudioConfig()
	{
        alcMakeContextCurrent(NULL);
        alcDestroyContext(this->Context);
        alcCloseDevice(this->Device);

	}
	
	void _AudioConfig::Flush(void)
	{
		ALenum error;
		
		alDopplerFactor(this->DopplerFactor);
		alDopplerVelocity(this->DopplerVelocity);

		while((error=alGetError()) != AL_NO_ERROR)
		{
			switch(error)
			{
			case AL_INVALID_NAME:      _PopCornError("OpenAL> Invalid Name parameter.");
			case AL_INVALID_ENUM:      _PopCornError("OpenAL> Invalid parameter.");
			case AL_INVALID_VALUE:     _PopCornError("OpenAL> Invalid enum parameter value.");
			case AL_INVALID_OPERATION: _PopCornError("OpenAL> Illegal call.");
			case AL_OUT_OF_MEMORY:     _PopCornError("OpenAL> Unable to allocate memory.");
			}
		}
	}
};
