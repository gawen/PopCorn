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

#include "popcorn.hpp"

namespace PopCorn
{
//Constructeur
template <class T> CLst<T>::CLst()
{
	m_pDebut = NULL;
	m_nNbElem = 0;
}

//Destructeur
template <class T> CLst<T>::~CLst()
{
	if (m_pDebut != NULL)
		delete[] m_pDebut;
}

//Ajouter un élément à la suite de la liste
template <class T> int CLst<T>::AddElem(T val)
{
	LISTE* pT = new LISTE;

	pT->val = val;

	if(m_pDebut == NULL)
	{//Si c'est le premier élément qu'on insère
		m_pDebut = pT;
		m_pDebut->pSuivant = pT;
	}
	else
		m_pFin->pSuivant = pT;

	m_pFin = pT;
	m_pFin->pSuivant = NULL;

	m_nNbElem++;
	
	return m_nNbElem - 1;
}

//Retourne la valeur se trouvant à une position donnée
template <class T> T CLst<T>::operator [] (int nPos)
{
	LISTE* pT = (LISTE*)GetPtrAt(nPos);
	return pT->val;
}

//Insérer un élément supplémentaire à la position nPos
//Tous les autres éléments seront décalé
//retourne false en cas de position invalide
template <class T> bool CLst<T>::InsertElem(int nPos,T val)
{
	LISTE* pV = new LISTE;
	LISTE* pT = NULL;

	pV->val = val;

	if(nPos == 0)
	{
		pV->pSuivant = m_pDebut;
		m_pDebut = pV;
	}
	else
	{
		pT = (LISTE*)GetPtrAt(nPos-1);
		if(pT == NULL)
		{
			delete pV;
			return false;
		}

		pV->pSuivant = pT->pSuivant;
		pT->pSuivant = pV;
	}

	m_nNbElem++;

	return true;
}

//Remplacer la valeur de l'élément se trouvant à la position nPos par val
//retourne false en cas de position invalide
template <class T> bool CLst<T>::SetAt(int nPos, T val)
{
	LISTE* pT = (LISTE*)GetPtrAt(nPos);

	if(pT == NULL)
		return false;

	pT->val = val;

	return true;
}

//Détruire l'item se trouvant à la position nPos
//retourne false en cas de position invalide
template <class T> bool CLst<T>::DeleteElem(int nPos)
{
	LISTE* pD;
	LISTE* pT;

	if(nPos == 0)
	{
		pT = m_pDebut;
		if(pT == NULL)
			return false;

		pD = pT;
		pT = pT->pSuivant;
		m_pDebut = pT;
	}
	else
	{
		pT = (LISTE*)GetPtrAt(nPos -1);

		if(pT == NULL || pT->pSuivant == NULL)
			return false;

		pD = pT;
		pD = pD->pSuivant;
		pT->pSuivant = pD->pSuivant;
	}

	delete pD;
	m_nNbElem--;

	return true;
}

//Retourne l'adresse de la position nPos
template <class T> LPVOID CLst<T>::GetPtrAt(int nPos)
{
	LISTE* pT = m_pDebut;
	int i = 0;

	if(nPos < 0 || nPos >= m_nNbElem)
		return NULL;

	while(i < nPos && pT != NULL)
	{
		pT = pT->pSuivant;
		i++;
	}

	return pT;
}

//Détruire la liste
template <class T> void CLst<T>::Delete()
{
	if (m_pDebut != NULL)
	{
		delete[] m_pDebut;
		m_pDebut = NULL;
		m_nNbElem = 0;
	}
}

template <class T> int CLst<T>::GetNbElem() { return m_nNbElem; }

template <class T> int CLst<T>::Find(T val)
{
	LISTE* pT = this->m_pDebut;
	int i = 0;

	while(i < nPos && pT != NULL)
	{
		if(pT->val == val)
		    return i;
        pT = pT->pSuivant;
		i++;
	}

	return -1;
}

template <class T> bool CLst<T>::DeleteElem(T val)
{
   int i = this->Find(val);
   if(i != -1)
       return this->DeleteElem(i);
   else
      return false;
}

};

