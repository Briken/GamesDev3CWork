#pragma once
#include <irrKlang\irrKlang.h>


using namespace irrklang;

class Audio
{
public:
	Audio();
	~Audio();

	ISoundEngine* GetEngine();
	float volume; 
	



	private:
	ISoundEngine* soundEngine;

};

