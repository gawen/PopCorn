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

#ifdef __DEBUG__
namespace PopCorn
{

    /*********************************************
	Classe _Debug
    **********************************************/
   	_Debug Debug("debug.txt");
    _Debug::_Debug(const char *fichier)
	{
		_unlink(fichier);
//		_unlink("log.txt");
//		freopen("log.txt", "w", stdout);
//		freopen("log.txt", "w", stderr);
		this->Fp = fopen(fichier,"w");
	}

	_Debug::~_Debug(void)
	{
		fclose(this->Fp);
	}
	
}
#endif
