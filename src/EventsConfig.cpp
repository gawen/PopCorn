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
    class _EventsConfig Events;

    extern void Init(void);
    
    _EventsConfig::_EventsConfig(void)
    {
    }
    
    bool _EventsConfig::Init(void)
    {
        if(Scene != NULL && Scene->Initialization != NULL)
            Scene->Initialization();
        
        #ifdef __DEBUG__
		fprintf(Debug.Fp, "\n\n***Initialisation évènements [OK]\n\n");
		#endif
        return true;
    }
    
    void _EventsConfig::Flush(void)
    {
        static SDL_Event event;
        static bool MouseLeft = false;
        static bool MouseMid = false;
        static bool MouseRight = false;
        static bool MouseChange = false;

        if(Scene == NULL) return;
        
        while(SDL_PollEvent(&event))
        {
            switch(event.type)
            {
                case SDL_KEYDOWN:
                case SDL_KEYUP:
                    if(Scene->Keyboard != NULL)
                        Scene->Keyboard(event.type == SDL_KEYDOWN, event.key.keysym.sym);
                    break;
                
                case SDL_MOUSEMOTION:
                    if(Scene->MouseMotion != NULL)
                        Scene->MouseMotion(event.motion.x, event.motion.y,
                                           MouseLeft, MouseMid, MouseRight
                        );
                    break;
                
                case SDL_MOUSEBUTTONDOWN:
                case SDL_MOUSEBUTTONUP:
                    if(Scene->MouseClick != NULL)
                        if(event.button.button == SDL_BUTTON_LEFT && MouseLeft != (event.type == SDL_MOUSEBUTTONDOWN))
                        {
                            MouseChange = true;
                            MouseLeft = (event.type == SDL_MOUSEBUTTONDOWN);
                        }
                        if(event.button.button == SDL_BUTTON_MIDDLE && MouseMid != (event.type == SDL_MOUSEBUTTONDOWN))
                        {
                            MouseChange = true;
                            MouseMid = (event.type == SDL_MOUSEBUTTONDOWN);
                        }
                        if(event.button.button == SDL_BUTTON_RIGHT && MouseRight != (event.type == SDL_MOUSEBUTTONDOWN))
                        {
                            MouseChange = true;
                            MouseRight = (event.type == SDL_MOUSEBUTTONDOWN);
                        }
                        
                        if(MouseChange)
                        {
                            Scene->MouseClick(event.type == SDL_MOUSEBUTTONDOWN,
                                              event.button.x, event.button.y,
                                              MouseLeft, MouseMid, MouseRight
                            );
                            MouseChange = false;
                        }
                    break;
                
                case SDL_VIDEORESIZE:
                    Video.Width = event.resize.w;
                    Video.Height = event.resize.h;
                    Init();
                    break;
                
                case SDL_QUIT:
                    if(Scene->Exit != NULL)
                        Scene->Exit();
                    else
                        exit(0);
                    break;
            }
        }
    }
};

