#include "Screen.h"


Screen::Screen()
{
	m_GameWindow = nullptr; //initialise to generate null access violation for debugging. 
	m_ScreenWidth = 1024.0f;
	m_ScreenHeight = 768.0f; 
}

Screen::~Screen()
{
	SDL_GL_DeleteContext(m_GameContext); // delete context
	SDL_DestroyWindow(m_GameWindow); // detete window (make sure to delete the context and the window in the opposite order of creation in InitializeScreen())
	SDL_Quit();
}

float Screen::GetWidth() { return m_ScreenWidth; } //getters
float Screen::GetHeight() { return m_ScreenHeight; }

void Screen::ReturnError(std::string errorString)
{
	std::cout << errorString << std::endl;
	std::cout << "press any  key to quit...";
	int in;
	std::cin >> in;
	SDL_Quit();
}


void Screen::SwapBuffer()
{
	SDL_GL_SwapWindow(m_GameWindow); //swap buffers
}

void Screen::ClearScreen(float r, float g, float b, float a)
{
	glClearColor(r, g, b, a);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT); // clear colour and depth buffer - set colour to colour defined in glClearColor
}

void Screen::InitializeScreen()
{
	SDL_Init(SDL_INIT_EVERYTHING); //initalise everything

	SDL_GL_SetAttribute(SDL_GL_RED_SIZE, 8); //Min no of bits used to diplay colour
	SDL_GL_SetAttribute(SDL_GL_GREEN_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_BLUE_SIZE, 8);
	SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 16);// set up z-buffer
	SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1); // set up double buffer   

	m_GameWindow = SDL_CreateWindow("Game Window", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, (int)m_ScreenWidth, (int)m_ScreenHeight, SDL_WINDOW_OPENGL); // create window

	if (m_GameWindow == nullptr)
	{
		ReturnError("window failed to create");
	}

	m_GameContext = SDL_GL_CreateContext(m_GameWindow);

	if (m_GameContext == nullptr)
	{
		ReturnError("SDL_GL context failed to create");
	}

	GLenum error = glewInit();
	if (error != GLEW_OK)
	{
		ReturnError("GLEW failed to initialise");
	}

	glEnable(GL_DEPTH_TEST); //enable z-buffering 
	glEnable(GL_CULL_FACE); //dont draw faces that are not pointing at the camera
	

	glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
}