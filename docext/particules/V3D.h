/*
*	Exemple de moteur de particules avec gestion de la physique
*
*  dake/calodox
*  http://www.calodox.org
*
*  Classe de vecteur 3d avec opérations de base.
* 
*/

#ifndef _V3D_H_
#define _V3D_H_

class V3D {
public:

	float x,y,z;

	V3D()
	{
		Zero();
	}
	V3D(float x, float y, float z)
	{
		this->x = x; this->y = y; this->z = z;
	}

	friend V3D operator -(const V3D& v1, const V3D& v2)
	{
		return(V3D(v1.x - v2.x, v1.y - v2.y, v1.z - v2.z));
	}

	friend V3D operator +(const V3D& v1, const V3D& v2)
	{
		return(V3D(v1.x + v2.x, v1.y + v2.y, v1.z + v2.z));
	}

	friend V3D operator *(const V3D& v, float scalaire)
	{
		return(V3D(v.x * scalaire, v.y * scalaire, v.z * scalaire));
	}

	friend V3D operator /(const V3D& v, float scalaire)
	{
		return(V3D(v.x / scalaire, v.y / scalaire, v.z / scalaire));
	}

	V3D operator =(const V3D& v)
	{
		x = v.x; y = v.y; z = v.z; return (*this);
	}

	V3D operator +=(const V3D& v)
	{
		x += v.x; y += v.y; z += v.z; return (*this);
	}

	V3D operator -=(const V3D& v)
	{
		x -= v.x; y -= v.y; z -= v.z; return (*this);
	}

	V3D operator *=(const float scalaire)
	{
		x *= scalaire; y *= scalaire; z *= scalaire; return (*this);
	}

	V3D operator /=(const float scalaire)
	{
		x /= scalaire; y /= scalaire; z /= scalaire; return (*this);
	}


	void Zero()
	{
		x = y = z = 0.0f;
	}
	float CarreLongueur()
	{
		return (x * x + y * y + z * z);
	}
	float Longueur()
	{
		return sqrtf(CarreLongueur());
	}

	void Unitaire()
	{
		float lg = 1.0f / Longueur();
		x *= lg; y *= lg; z *= lg;
	}
};



#endif