#include <Demeter/Terrain.h>
#include <map>

class SampleTextureFactory:public Demeter::TextureFactory
{
      public:
	SampleTextureFactory(Demeter::Terrain * pTerrain);
	virtual ~SampleTextureFactory();
	virtual Demeter::Texture * GetTexture(int index, float originX, float originY, float width, float height);
	virtual void UnloadTexture(int index);
      private:
	Demeter::Terrain * m_pTerrain;
	std::map < int, Demeter::Texture * >m_Textures;
};
