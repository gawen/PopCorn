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
#include "stdarg.h"
#include <windows.h>

namespace PopCorn
{
    // Pour voir si PopCorn est utilisé dans un projet
    const char* _security = ":*/*: PopCorn, The SilverCoders's 3d game engine - Pointer :*/*:";
    
#ifdef __DEBUG__
    static unsigned short _dbgPointerVar=0;

    void _dbgPointer(const char* file, int line, const char* comment)
    {
        printf("%u : %s:%u (%s)\n",++_dbgPointerVar, file, line, comment);
    }
    
    void _dbgPointerFile(unsigned int f, const char* file, int line, const char* comment)
    {
        static char strf[25];
        sprintf(strf, "debug%u.txt", f);
        FILE* _f = fopen(strf, "w");
        
        if(_f != NULL)
        {
            fprintf(_f,"%u> %s:%u (%s)\n",++_dbgPointerVar, file, line, comment);
            fclose(_f);
        } else {
            fprintf(stderr, "Can't open %s", strf);
        }
    }
    
#ifdef WIN32
    void _dbgMsgBox(const char* msg, ...)
    {
        static char str[512];
        
        va_list list;
        va_start(list, msg);
        vsprintf(str, msg, list);
        va_end(list);
        
        MessageBox(HWND_DESKTOP, str, "PopCorn Debug Mode", MB_OK);
    }

	void _PopCornError(const char* Str)
	{
        MessageBox(HWND_DESKTOP, Str, "Error", MB_OK);
		fprintf(stderr, Str);
		exit(-1);
	}
#else
	void _PopCornError(const char* Str)
	{
        fprintf(stderr, Str);
		exit(-1);
	}
#endif
#endif
};

