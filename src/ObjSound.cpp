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
    ObjSound::ObjSound(void)
    {
		this->Source = 0;
		this->pPosition = &this->Position;
		this->_ObjLoaded = NULL;
    }
    
    ObjSound::~ObjSound()
    {
        this->Clear();
    }
    
    bool ObjSound::LoadWAV(const char* Sound, bool Looping)
    {
        ALboolean Loop = Looping;
		char* Buffer=NULL;
		ALenum FormatBuffer=0;
		long BufferLen=0;
		ALsizei FreqBuffer=0;

		alutLoadWAVFile((ALbyte*)Sound, &FormatBuffer, (void**) &Buffer, (unsigned int *)&BufferLen, &FreqBuffer, &Loop);
		alGenSources(1, &this->Source);
		alGenBuffers(1, &this->SampleSet);
		
		alBufferData(this->SampleSet, FormatBuffer, Buffer, BufferLen, FreqBuffer);
		alSourcei(this->Source, AL_BUFFER, this->SampleSet);
		
		alutUnloadWAV(FormatBuffer, Buffer, BufferLen, FreqBuffer);			
		alSourcef(this->Source, AL_PITCH, 1.0f);
		alSourcef(this->Source, AL_GAIN, 1.0f);
		this->SetLooping(Looping);
		this->Set3dSound(false);

		this->_ObjLoaded = new unsigned int;
		(*this->_ObjLoaded)++;
    }

	void ObjSound::Clear(void)
	{
		alDeleteSources(1,&this->Source);
		alDeleteBuffers(1,&this->SampleSet);

		(*this->_ObjLoaded)--;
		if((*this->_ObjLoaded) == 0)	// Dernier objet
			delete this->_ObjLoaded;
	}
    
    void ObjSound::Play(void)
    {
        alSourcePlay(this->Source);
		Scene->ListSound.AddElem((void*)this);
    }
    
    // ObjSoundControl
    void ObjSound::Stop(void)
    {
        alSourceStop(this->Source);
		if(Scene) Scene->ListSound.DeleteElem((void*)this);
    }
    
    void ObjSound::SetVolume(unsigned char Volume)
    {
    }
    
    void ObjSound::SetLooping(bool Enabled)
    {
        alSourcei(this->Source, AL_LOOPING, Enabled ? AL_TRUE : AL_FALSE);
    }

	void ObjSound::Set3dSound(bool Enabled)
	{
		alSourcei(this->Source, AL_SOURCE_RELATIVE, Enabled ? AL_TRUE : AL_FALSE);
	}
    
    ObjSound& ObjSound::operator=(ObjSound& Sound)
    {
        this->Source = Sound.Source;
		this->SampleSet = Sound.SampleSet;
		this->pPosition = Sound.pPosition;
		this->Position = Sound.Position;
		this->_ObjLoaded = Sound._ObjLoaded;
    }

	void ObjSound::Refresh(void)
	{
		alSource3f(this->Source, AL_POSITION, this->pPosition->X, this->pPosition->Y, this->pPosition->Z);
        alSource3f(this->Source, AL_VELOCITY, this->Velocity.X,   this->Velocity.Y,   this->Velocity.Z);

	}
};

