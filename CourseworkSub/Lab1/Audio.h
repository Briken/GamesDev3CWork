#pragma once
#include <irrKlang\irrKlang.h>


using namespace irrklang;

class Audio
{
public:
	Audio(); //default constructor creates the engine
	~Audio();//destructor deletes the engine

	ISoundEngine* GetEngine(); //returns the sound engine managing the sound
	float volume; //volume for the engine
	



	private:
	ISoundEngine* soundEngine; // sound engine used to manage soune

};

