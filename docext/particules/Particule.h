/*
*	Exemple de moteur de particules avec gestion de la physique
*
*  dake/calodox
*  http://www.calodox.org
*
*/

#ifndef _PARTICULE_H_
#define _PARTICULE_H_

#include "v3d.h"

class Particule {
public:
	Particule()
	{
		position.Zero(); vitesse.Zero(); force.Zero(); masse = 1.0f; couleur.x = couleur.y = couleur.z = 1.0f;
	}

	Particule(V3D position, V3D vitesse, V3D couleur, float masse)
	{
		this->position = position; 
		this->vitesse = vitesse; 
		this->masse = masse; 
		this->couleur = couleur;
		this->force.Zero();
	}

	void Euler(double dt);

	V3D position;
	V3D vitesse;
	V3D force;
	V3D couleur;
	float masse;
};

#endif