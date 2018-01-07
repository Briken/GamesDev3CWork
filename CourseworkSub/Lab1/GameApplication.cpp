#include "GameApplication.h"
#include "Camera.h"
#include <iostream>
#include <string>
//#include <glm/ext.hpp>

unsigned int indices[] = { 0, 1, 2 };

Transform* transform = new Transform();
Transform* transform1 = new Transform();
Transform* transform2 = new Transform();
Transform* transform3 = new Transform();

GameApplication::GameApplication()
{
	m_CurrentState = GameState::PLAY;
	
	Screen* m_GameScreen = new Screen(); //new Screen
	Light* m_Light();
    Model* model = new Model();
	Model* model1 = new Model();
	Model* model2 = new Model();
	Model* model3 = new Model();
	sceneGraph = { transform, transform1, transform2, transform3 }; 
	modelGraph = { model, model1, model2, model3 };
	
}

GameApplication::~GameApplication()
{
}

void GameApplication::run()
{
	InitializeSystems(); 
	MainGameLoop();
}


void GameApplication::InitializeSystems()
{
		
	m_Light.initLight(glm::vec3(0.1f, 0.0f, 0.0f), glm::vec4(0.1f, 0.1f, 0.1f, 1.0f), glm::vec4(0.1f, 0.0f, 0.0f, 1.0f), glm::vec4(0.1f, 0.0f, 0.0f, 1.0f));
	m_GameScreen.InitializeScreen(); 
	
	shader = new Shader("../res/toon"); //new shader
	shader1 = new Shader("../res/toon");
	shader2 = new Shader("../res/shader");
	texture = new Texture("../res/bure4.png"); //load texture
	texture1 = new Texture("../res/water.jpg"); //load texture
	texture2 = new Texture("../res/bure5.png");
	model.LoadModel("../res/box4obj.obj");
	model1.LoadModel("../res/monkey3.obj");
	model2.LoadModel("../res/box5obj.obj");
	model3.LoadModel("../res/soccer_ball.obj");
	
	m_GameCamera.InitializeCamera(glm::vec3(0, 3, -40), 70.0f, (float)m_GameScreen.GetWidth()/m_GameScreen.GetHeight(), 0.01f, 1000.0f, false);
	m_SceneCamera.InitializeCamera(glm::vec3(0, 60, -45), 70.0f, (float)m_GameScreen.GetWidth() / m_GameScreen.GetHeight(), 0.01f, 1000.0f, true);
	m_SceneCamera.Pitch(0.25*4);
	m_ActiveCamera = m_SceneCamera;
	
	transform3->SetPos(m_GameCamera.GetPos());
	sceneGraph[0]->collider.colSize = 5;
	sceneGraph[1]->collider.colSize = 2;
	sceneGraph[2]->collider.colSize = 2;
	sceneGraph[3]->collider.colSize = 5;
	
	controllingModel = false;
	counter = 0.0f;
}


void GameApplication::MainGameLoop()
{
	audioManager.GetEngine()->play2D("../res/audio/bensound-extremeaction.wav", GL_TRUE);
	transform3->SetPos(m_GameCamera.GetPos());
	while (m_CurrentState != GameState::EXIT)
	{
		ManageInput();
		UpdateTransforms();
		CheckCollisions();
		RenderScene();
	}
}

void GameApplication::ManageInput()
{
	SDL_Event evnt;

	while (SDL_PollEvent(&evnt)) //get and process events
	{
		switch (evnt.type)
		{
		case SDL_QUIT:
			QuitGame();
			break;
		case SDL_MOUSEBUTTONDOWN:
			switch (evnt.button.button)
			{
			case SDL_BUTTON_LEFT:
				std::cout << "mouse pressed";
				break;
			}
			break;
		case SDL_KEYDOWN:
			switch (evnt.key.keysym.sym)
			{
			case SDLK_RIGHT:
				if (controllingModel == false)
				{
					m_GameCamera.MoveLeftRight(-0.5);
					transform3->SetPos(glm::vec3(m_GameCamera.GetPos()));
				}
				else
				{
					transform3->SetPos(glm::vec3(transform3->GetPosition()->x - 0.5f, transform3->GetPosition()->y, transform3->GetPosition()->z));
				}
				break;
			case SDLK_LEFT:
				if (controllingModel == false)
				{
					m_GameCamera.MoveLeftRight(0.5);
					transform3->SetPos(glm::vec3(m_GameCamera.GetPos()));
				}
				else
				{
					transform3->SetPos(glm::vec3(transform3->GetPosition()->x + 0.5f, transform3->GetPosition()->y, transform3->GetPosition()->z));
				}
				break;
			case SDLK_UP:
				if (controllingModel == false)
				{
					m_GameCamera.MoveForwardBack(0.5);
					transform3->SetPos(glm::vec3(m_GameCamera.GetPos()));
				}
				else
				{
					transform1->SetPos(glm::vec3(transform1->GetPosition()->x, transform1->GetPosition()->y, transform1->GetPosition()->z + 0.5f));
				}
				break;
			case SDLK_DOWN:
				if (controllingModel == false)
				{
					m_GameCamera.MoveForwardBack (-0.5);
					transform3->SetPos(glm::vec3(m_GameCamera.GetPos()));
				}
				else
				{
					transform1->SetPos(glm::vec3(transform1->GetPosition()->x, transform1->GetPosition()->y, transform1->GetPosition()->z - 0.5f));
				}
				break;
			case SDLK_w:
				if (controllingModel == false)
				{
					m_ActiveCamera.Pitch(-0.25f);
					transform1->SetPos(m_GameCamera.GetForward());
					
				}
				else
				{
					transform1->SetRot(glm::vec3(transform1->GetRot()->x + 0.5, transform1->GetRot()->y, transform1->GetRot()->z));
				}
				break;
			case SDLK_s:
				if (controllingModel == false)
				{
					m_GameCamera.Pitch(0.25f);
					transform1->SetPos(m_GameCamera.GetForward());

				}
				else
				{
					transform->SetRot(glm::vec3(transform1->GetRot()->x - 0.5, transform1->GetRot()->y, transform1->GetRot()->z));
				}
				break;
			case SDLK_d:
				if (controllingModel == false)
				{
					m_GameCamera.RotateY(-0.25);
				}
				else
				{
					transform1->SetRot(glm::vec3(transform1->GetRot()->x, transform1->GetRot()->y - 0.5, transform1->GetRot()->z));
				}
				break;
			case SDLK_a:
				if (controllingModel == false)
				{
					m_GameCamera.RotateY(0.25);
				}
				else
				{
					transform1->SetRot(glm::vec3(transform1->GetRot()->x, transform1->GetRot()->y + 0.5, transform1->GetRot()->z));
				}
				break;
			case SDLK_r:
				Reload();
				break;
			case SDLK_EQUALS:
				audioManager.volume = audioManager.GetEngine()->getSoundVolume();
				audioManager.GetEngine()->setSoundVolume(audioManager.volume += 0.1);
				break;
			case SDLK_MINUS:
				audioManager.volume = audioManager.GetEngine()->getSoundVolume();
				audioManager.GetEngine()->setSoundVolume(audioManager.volume -= 0.1);
				break;
			case SDLK_SPACE:
				SwitchControlMode();
				break;
			case SDLK_0:
				m_ActiveCamera = m_SceneCamera;
				m_SceneCamera.isActive = true;
				m_GameCamera.isActive = false;
				break;
			case SDLK_1:
				m_ActiveCamera = m_GameCamera;
				m_GameCamera.isActive = true;
				m_SceneCamera.isActive = false;
				break;
			case SDLK_ESCAPE:
				QuitGame();
				break;
			}
			break;
		}
	}

}


void GameApplication::RenderScene()
{
	m_GameScreen.ClearScreen(1.0f, 1.0f, 1.0f, 1.0f);

	
	//textureGraph = {texture, texture1, texture2};

	if (transform->isActive)
	{
		shader->BindShader();
		shader->Update(transform, m_ActiveCamera);
		texture->BindTexture(0);
		model.DrawMesh();
	}

	if (transform1->isActive)
	{
		shader1->BindShader();
		shader1->SetDirection(m_SceneCamera);
		shader1->Update(transform1, m_ActiveCamera);
		texture1->BindTexture(0);
		model1.DrawMesh();
	}
	
	if (transform2->isActive)
	{
		shader2->BindShader();
		shader2->Update(transform2, m_ActiveCamera);
		texture2->BindTexture(0);
		model2.DrawMesh();
	}

		shader->BindShader();
		shader->Update(transform3, m_ActiveCamera);
		texture2->BindTexture(0);
		model3.DrawMesh();



	counter = counter + 0.01f;
				
	glEnableClientState(GL_COLOR_ARRAY); 
	glEnd();

	m_GameScreen.SwapBuffer();
}

void GameApplication::SwitchControlMode()
{
	controllingModel = !controllingModel;
}

void GameApplication::QuitGame()
{
	m_CurrentState = GameState::EXIT;
}

void GameApplication::UpdateTransforms()
{
	if (m_GameCamera.isActive)
	{
		m_ActiveCamera = m_GameCamera;
	}
	transform->SetPos(glm::vec3(10.0, 0.0, 0.0));
	transform->SetRot(glm::vec3(0.0, counter * 5, 0.0f));
	transform->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));

	transform1->SetScale(glm::vec3(2.0f, 2.0f, 2.0f));
			  
	transform2->SetPos(glm::vec3(-10, 0, 0));
	transform2->SetRot(glm::vec3(0.0, counter * -5, 0.0));
	transform2->SetScale(glm::vec3(1.0f, 1.0f, 1.0f));
	//sceneGraph = { transform, transform1, transform2, transform3 };
	//for (int i = 0; i < sceneGraph.size(); i++)
	//{
		
		//}
}

void GameApplication::CheckCollisions()
{
	//int itMax = sceneGraph.size();
	for (int i = 0; i < sceneGraph.size(); i++)
	{
		Transform colliding1;
		Transform colliding2;
		for (int j = 0; j < sceneGraph.size(); j++)
		{
			//check distance between the edges of two colliders, if that distance is less than 0 then they are colliding
			//do not check if the object is colliding with itself
			if (i != j && sceneGraph[i]->isActive && sceneGraph[j]->isActive)
			{

				
				
				float xDist = sceneGraph[i]->collider.position.x - sceneGraph[j]->collider.position.x;
				
				float yDist = sceneGraph[i]->collider.position.y - sceneGraph[j]->collider.position.y;
				
				float zDist = sceneGraph[i]->collider.position.z - sceneGraph[j]->collider.position.z;
				float totalDist = glm::pow(xDist, 2) + glm::pow(yDist, 2) + glm::pow(zDist, 2);
				
				float dist = glm::sqrt(totalDist);
				float radSum = sceneGraph[i]->collider.colSize + sceneGraph[j]->collider.colSize;

				if (dist < radSum)
				{
					if (i == 2)
					{
						i = 2;
					}
					if (sceneGraph[i]->isActive && sceneGraph[j]->isActive)
					{
						std::cout << "objects " << i << " and " << j << " are colliding" << endl;
						if (i != 3)
						{
							sceneGraph[i]->isActive = false;
							audioManager.GetEngine()->play2D("../res/audio/dead.wav", GL_FALSE);
						}
						if (j != 3)
						{
							sceneGraph[j]->isActive = false;
							audioManager.GetEngine()->play2D("../res/audio/dead.wav", GL_FALSE);
						}
					}
				}
			}
		}
		
	}
}

void GameApplication::Reload()
{
	for (int i = 0; i < sceneGraph.size(); i++)
	{
		sceneGraph[i]->isActive = true;
	}
	m_GameCamera.SetPos(glm::vec3(0, 3, -40));
	transform3->SetPos(m_GameCamera.GetPos());
}


