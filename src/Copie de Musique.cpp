/*
    Copyright (C) 2003  Arana Sébastien, Arab Gawen
    
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
	/*
	=============================================
	JukeBox
	=============================================
	Cette classe est censée gérer toutes les
	musiques chargées dans le moteur. les musiques
	sont representés par des numero, de 1 à 255.
	*/
    class _JukeBox JukeBox;
	/*---CONSTRUCTEUR---*/
	/*******************/
    _JukeBox::_JukeBox(void)
	{
		unsigned char index=0;
		this->Looping = 0;
		this->Paused = 0;
		for(index;index < 255;index++) this->MusicTab[index] = NULL;
	}

	/*---DESTRUCTEUR---*/
	/*******************/
	_JukeBox::~_JukeBox(void)
	{
		unsigned char index=0;
		for(index;index < 255;index++) this->MusicTab[index] = NULL;
	}

	/*---STOP TOUTES LES MUSIQUES---*/
	/*******************/
	void _JukeBox::StopAll(void)
	{
		FMUSIC_StopAllSongs();
	}

	/*---CHARGE UNE MUSIQUE EN MEMOIRE---*/
	/*******************/
	Music _JukeBox::AddMusique(const char *Fichier)
	{
		//on va chercher la prochaine case de libre
		unsigned char index=0;
		while(this->MusicTab[index] != NULL && index < 255)	index++;
		if(index < 255)
		{
			if((MusicTab[index] = FMUSIC_LoadSong(Fichier)) == NULL)
			{
				#ifdef __DEBUG__
				fprintf(Debug.Fp,"Echec - chargement de musique %s : erreur n° %d\n",Fichier,FSOUND_GetError());
				#endif
				return 0;
			}
			#ifdef __DEBUG__
			fprintf(Debug.Fp,"Reussi - chargement de musique %s\n",Fichier);
			#endif
			return (index+1);
		}
		else
		{
			#ifdef __DEBUG__
			fprintf(Debug.Fp,"Echec - chargement de musique %s, Tampon Plein.\n",Fichier);
			#endif
			return 0;
		}
	}

	/*---SUPPRIME UNE MUSIQUE DE LA MEMOIRE---*/
	/******************************************/
	bool _JukeBox::RemoveMusique(unsigned char index)
	{
		//on supprime la musique de la mémoire et on libere la place
		if(FMUSIC_FreeSong(this->MusicTab[index-1]))
		{
			this->MusicTab[index-1] = NULL;
			return true;
		}
		return false;
	}

	/*---JOUE UNE MUSIQUE---*/
	/************************/
	bool _JukeBox::Play(unsigned char index)
	{
		if(FMUSIC_PlaySong(this->MusicTab[index-1]))
			return true;
		return false;
	}

	/*---AUTORISE OU NON LE LOOP DE LA MUSIQUE---*/
	/*********************************************/
	bool _JukeBox::ToggleLoop(unsigned char index)
	{
		this->Looping = !this->Looping;
		if(FMUSIC_SetLooping(this->MusicTab[index-1],(signed char)this->Looping))
			return true;
		return false;
	}

	/*---MODIFIE LE PITCH DE LA MUSIQUE---*/
	/**************************************/
	bool _JukeBox::Pitch(unsigned char index,float percent)
	{
		double speed;
		speed = percent/100.0;
		if(speed > 10.0) speed = 10.0;
		if(speed < 0.0) speed = 1.0;
		if(FMUSIC_SetMasterSpeed(this->MusicTab[index-1],(float)speed))
			return true;
		return false;
	}

	/*---MODIFIE LE VOLUME DE LA MUSIQUE---*/
	/***************************************/
	bool _JukeBox::Volume(unsigned char index,float percent)
	{
		double volume;
		volume = 2.56*percent;
		if(volume > 256.0) volume = 256.0;
		if(volume < 0.0) volume = 0.0;
		if(FMUSIC_SetMasterVolume(this->MusicTab[index-1],(int)volume))
			return true;
		return false;
	}

	/*---MODIFIE LE PAN SEPARATION STEREO FULL <-DEGRADE-> MONO---*/
	/**************************************************************/
	bool _JukeBox::Pan(unsigned char index,float percent)
	{
		double pansep;
		pansep = percent/100.0;
		if(pansep > 1.0) pansep = 1.0;
		if(pansep < 0.0) pansep = 0.0;
		if(FMUSIC_SetPanSeperation(this->MusicTab[index-1],(float)pansep))
			return true;
		return false;
	}


	/*---TOGGLE LA PAUSE---*/
	/***********************/
	bool _JukeBox::TogglePause(unsigned char index)
	{
		this->Paused = !this->Paused;
		if(FMUSIC_SetPaused(this->MusicTab[index-1],(signed char)this->Paused))
			return true;
		return false;
	}

	
	/*---ARRETE LA MUSIQUE---*/
	/*************************/
	bool _JukeBox::Stop(unsigned char index)
	{
		if(FMUSIC_StopSong(this->MusicTab[index-1]))
			return true;
		return false;
	}



};
