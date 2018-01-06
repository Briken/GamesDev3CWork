#pragma once
#include <SDL/SDL.h>
#include <GL\glew.h>
#include <iostream>
#include <string>
using namespace std;


class Screen
{
public:
	Screen();
	~Screen();
	void InitializeScreen(); // initialises the screen and sets necessary variables to defauls
	void SwapBuffer();
	void ClearScreen(float r, float g, float b, float a);
	float GetWidth();
	float GetHeight();

private:

	void ReturnError(std::string errorString);
	
	

	SDL_GLContext m_GameContext;	//global variable to hold the context
	SDL_Window* m_GameWindow;		//holds pointer to the window
	float m_ScreenWidth;			//holds the screen width
	float m_ScreenHeight;			//holds the screen height
};

