/*
*	Exemple de moteur de particules avec gestion de la physique
*
*  dake/calodox
*  http://www.calodox.org
*
*/

#ifndef _MOTEUR_H_
#define _MOTEUR_H_

#include "particule.h"
class MoteurParticules {
public:
	MoteurParticules();
	virtual ~MoteurParticules()
	{

	};

	void Genere_Explosion_Particules(V3D centre, float rayon, int nb_particules);
	void Afficher();

private:
	void Mise_A_Jour();
	void Calcul_Camera();
	void Calcul_Forces();
	void Ajoute_Force_Trou_Noir(Particule* p, V3D position, float masse_trou_noir);

	vector < Particule> pList;

	LARGE_INTEGER TimerFreq;	
	LARGE_INTEGER TimeStart;	
	LARGE_INTEGER TimeCur;	

	float time;
	float old_time;
	float dt;
};


#endif