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
    /*********************************************
	Classe _Timer
    **********************************************/
    class _TimerFPS FPS;
	Timer::Timer(void)
	{
		this->MiliStart = 0;
	}
	void Timer::Start(void)
	{
		this->MiliStart = SDL_GetTicks();
	}
	void Timer::Reset(void)
	{
		this->MiliStart = SDL_GetTicks();
	}

	long Timer::Get(void)
	{
		return SDL_GetTicks() - this->MiliStart;
	}


    /*********************************************
	Classe _TimerFPS
    **********************************************/
	_TimerFPS::_TimerFPS(void)
	{
		this->FPS = 0;
		this->FPSmin = (unsigned int)-1;
		this->FPSmax = 0;
		this->QueryFPS = 0;
	}
 
    void _TimerFPS::Start(void)
	{
		this->MiliStart = SDL_GetTicks();
	}
	
    void _TimerFPS::Refresh(void)
	{
		this->compteur++;
		if((SDL_GetTicks()-this->MiliStart) > 1000)
		{
			this->FPS = this->compteur;
#           ifdef __DEBUG2__
            fprintf(Debug.Fp, "FPS : %u\n", this->FPS);
#           endif
			if(this->FPS < this->FPSmin) this->FPSmin = this->FPS;
			if(this->FPS > this->FPSmax) this->FPSmax = this->FPS;
			this->compteur = 0;
			this->MiliStart = SDL_GetTicks();
		}
		
		// Gère le précalcul
        if(this->QueryFPS > 0 && Scene != NULL)
		{
		    // Précalcul activé
		    unsigned int Delay = (1000 / this->QueryFPS) + SDL_GetTicks();
		    
		    while(SDL_GetTicks() < Delay)
		    {
		        if(Scene->Idle != NULL) Scene->Idle();
		    }
		}
	}


	/*********************************************
	Classe TimerEvent
    **********************************************/
    TimerEvent::TimerEvent(void)
    {
        this->Delay = 0;
        this->TimerFunc = NULL;
    }
    
    void TimerEvent::Enable(void)
    {
        this->BePersistant(CRN_ALWOBJS);
        this->timer.Start();
    }
    
    void TimerEvent::Disable(void)
    {
        this->BeNotPersistant(CRN_ALWOBJS);
    }
    
    void TimerEvent::_draw(GLERROR* Position, double Advance)
    {
        _PopCornSetInfoFile(Position);
        
        if(this->timer.Get() >= this->Delay) {
            this->timer.Reset();
            if(this->TimerFunc != NULL)
                this->TimerFunc();
        }
    }
}
