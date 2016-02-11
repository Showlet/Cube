#ifndef MUSIC_H__
#define MUSIC_H__
#include "parametre.h"
#include "define.h"
#include "vector3.h"
class Music
{
public:
	static Music& GetInstance();
	// M�thodes
	void PlayNext();
	void setVolume(int volume);
private:
	// M�thodes
	void getMusic();
	
	// Champs
	sf::Music m_music;
	int m_index = 0;
	Parametre& m_settings = Parametre::GetInstance();

	// Contruscteur Singleton
	Music& operator= (const Music&) {}
	Music(const Music&) {}

	static Music m_instance;
	Music();
	~Music();

};
#endif // !MUSIC_H__
