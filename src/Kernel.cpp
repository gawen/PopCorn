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
#include <time.h>

namespace PopCorn
{
    class _TimerFPS TimerFPS;
    extern void Picking(void);
    Uint32 TicksError;
    
    bool Go(void)
    {
        // Initialisation de la Video
        if(!Video.Init())
            return false;
        
        if(!Audio.Init())
            return false;
        
        atexit(Stop);
        
        // Maintenant ca devient serieux ! ;)
        // Initialisation
        
        #ifdef __DEBUG__
        fprintf(Debug.Fp, "\nPopCorn a pris la main ...\n");
        #endif
        
        srand( (unsigned)time( NULL ) );
        
        // Initialisation des évènements
        if(!Events.Init())
            return false;
            
        Video.FPS.Start();
                
        // La boucle commence
        int i=0;
        TicksError = 0;
#ifdef __DEBUG2__
        DebugPointer("Loop's starting");
#endif
        for( ; ; )
        {
            // Gestion du FPS
            Video.FPS.Refresh();
            if(Scene != NULL)
            {            
                if(TicksError == 0) TicksError=SDL_GetTicks();
                Scene->Draw();
                
                // Flush
                Video.Flush();
                Audio.Flush();
                Events.Flush();
            }
#ifdef __DEBUG2__
            DebugPointer("Loop's end");
#endif
        }
        
        
        return true;
    }
    
    void Stop(void)
    {
            #ifdef __DEBUG__
            fprintf(Debug.Fp, "Appel de la fonction de fermeture\n");
            fprintf(Debug.Fp, "Rendu des données de FPS :\n\t- FPS instantanné : %u\n\t- FPS minimum : %u\n\t- FPS maximum : %u\n",Video.FPS.FPS, Video.FPS.FPSmin, Video.FPS.FPSmax);
            #endif
        // Pour l'instant rien
    }
};

