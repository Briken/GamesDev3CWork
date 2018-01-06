#include "Audio.h"
#include <iostream>


Audio::Audio()
{
	soundEngine = createIrrKlangDevice();
}


Audio::~Audio()
{
	delete soundEngine;
}

ISoundEngine * Audio::GetEngine()
{
	if (soundEngine == nullptr)
	{
		std::cout << "sound engine not found" << std::endl;
		return nullptr;
	}
	if (soundEngine != nullptr)
	{ 
		return soundEngine;
	}
}
