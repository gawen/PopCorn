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
#include "string.h"

namespace PopCorn
{
    ObjMusic::ObjMusic(void)
    {
        this->Loaded = false;
        this->Volume = 255;
        this->Loop = false;
        this->Channel = -1;
    }
    
    ObjMusic::~ObjMusic()
    {
        if(this->Loaded)
            if(this->IsMusicFormat)
                FMUSIC_FreeSong(this->Sound.Music);
            else
                FSOUND_Stream_Close(this->Sound.Stream);
    }
    
    bool ObjMusic::LoadStream(const char* Music)
    {
        if((this->Sound.Stream = FSOUND_Stream_OpenFile(Music, FSOUND_NORMAL | FSOUND_LOOP_NORMAL, 0)) == NULL) {
            #ifdef __DEBUG__
            fprintf(Debug.Fp, "Impossible de charger la musique %s ...", Music);
            #endif
        }
        
        this->IsMusicFormat = false;
        this->Loaded = true;
    }
    
    bool ObjMusic::LoadMusic(const char* Music)
    {
        if((this->Sound.Music = FMUSIC_LoadSong(Music)) == NULL) {
            #ifdef __DEBUG__
            fprintf(Debug.Fp, "Impossible de charger la musique %s ...", Music);
            #endif
        }
        
        this->IsMusicFormat = true;
        this->Loaded = true;
    }
    
    void ObjMusic::Play(void)
    {
        
        if(this->IsMusicFormat)
        {
            FMUSIC_PlaySong(this->Sound.Music);
            FMUSIC_SetMasterVolume(this->Sound.Music, this->Volume);
            FMUSIC_SetLooping(this->Sound.Music, (this->Loop ? true : false));
        }
        else
        {
            this->Channel = FSOUND_Stream_Play(FSOUND_FREE, this->Sound.Stream);
            FSOUND_SetVolume(this->Channel, this->Volume);
            FSOUND_Stream_SetLoopCount(this->Sound.Stream, (this->Loop ? -1 : 0));
        }
    }
    
    void ObjMusic::Stop(void)
    {
        if(this->IsMusicFormat)
            FMUSIC_StopSong(this->Sound.Music);
        else
        {
            FSOUND_Stream_Stop(this->Sound.Stream);
            this->Channel = -1;
        }
    }
    
    void ObjMusic::Pause(void)
    {
        if(this->IsMusicFormat)
            FMUSIC_SetPaused(this->Sound.Music, true);
        else
            FSOUND_SetPaused(this->Channel, true);
    }
    
    void ObjMusic::Resume(void)
    {
        if(this->IsMusicFormat)
            FMUSIC_SetPaused(this->Sound.Music, false);
        else
            FSOUND_SetPaused(this->Channel, false);
    }
    
    void ObjMusic::SetLooping(bool Enabled)
    {
        if(this->IsMusicFormat)
            FMUSIC_SetLooping(this->Sound.Music, (Enabled ? true : false));
        else
            if(this->Channel != -1)
                FSOUND_Stream_SetLoopCount(this->Sound.Stream, (Enabled ? -1 : 0));
        
        this->Loop = Enabled;
    }
    
    void ObjMusic::SetVolume(unsigned char Volume)
    {
        if(this->IsMusicFormat)
            FMUSIC_SetMasterVolume(this->Sound.Music, Volume);
        else
            FSOUND_SetVolume(this->Channel, Volume);
        
        this->Volume = Volume;
    }
};
