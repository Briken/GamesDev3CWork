#pragma once

#include <string>
#include <ostream>
#include <glm\glm.hpp>
#include <glm\gtx\transform.hpp>
#include <glm\ext.hpp>
#include <glm\gtc\matrix_transform.hpp>
#include <SDL\SDL.h>
#include <GL/glew.h>
#include <Windows.h>
#include "Screen.h" 
#include "Shader.h"
#include "Light.h"
#include "Model.h"
#include "Texture.h"
#include "Transform.h"
#include "Audio.h"

enum class GameState{PLAY, EXIT};

class GameApplication
{
public:
	GameApplication();
	~GameApplication();

	void run();

private:

	void InitializeSystems();//initialise game systems
	void ManageInput();//manage keypresses
	void MainGameLoop();//run the core loop
	void RenderScene();//renders current scene
	void SwitchControlMode();//switches between control of the camera and an object in the scene
	void QuitGame();//ends processes
	void UpdateTransforms();//updates the transforms in the scene
	void CheckCollisions();//checks for collisions in the scene
	
	void Reload(); // resets the level
	

	Screen m_GameScreen;
	Light m_Light;
	GameState m_CurrentState;

	//a collection of models, shaders, textures and cameras
	Model model;
	Model model1;
	Model model2;
	Model model3;
	
	Shader* shader;
	Shader* shader1;
	Shader* shader2;
	
	Texture* texture;
	Texture* texture1;
	Texture* texture2;
	
	Camera m_GameCamera;//first person camera, used to move the ball about the scene
	Camera m_SceneCamera;//default camera for the scene
	Camera m_ActiveCamera;//used to determine which camera is to be used

	bool controllingModel;//used to determine the if a model is being controlled or if the camera is being controlled
	float counter;
	
	Audio audioManager;//manager for scene audio
	
	//graphs to hold relevant classes
	std::vector<Model*> modelGraph;
	std::vector<Transform*> sceneGraph;
	std::vector<Texture> textureGraph;
	
	
};

