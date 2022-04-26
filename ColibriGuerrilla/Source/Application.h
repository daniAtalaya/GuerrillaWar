#ifndef __APPLICATION_H__
#define __APPLICATION_H__
#include "Globals.h"
#include "ModuleRender.h"
#include "ModuleWindow.h"
#include "ModuleInput.h"
#include "ModuleScene.h"
#include "ModuleImatges.h"
#include "ModulePlayer.h"
#include "ModuleMusica.h"
#include "ModuleText.h"
#include "ModuleParticles.h"
#include "ModuleCollisions.h"
#include "Module.h"
#include "ModuleState.h"
#include "ModuleEnemies.h"
class Application {
	public:
		Application();
		~Application();
		bool Init();
		update_status Update();
		bool CleanUp();
		Module* modules[NUM_MODULES];	
		ModuleWindow* window = nullptr;
		ModuleInput* input = nullptr;
		ModuleImatges* imatges = nullptr;
		ModulePlayer* player = nullptr;
		ModuleScene* scene = nullptr;
		ModuleParticles* particles = nullptr;
		ModuleRender* render = nullptr;
		ModuleText* text = nullptr;
		ModuleCollisions* collisions = nullptr;
		ModuleMusica* audio = nullptr;
		ModuleState* state = nullptr;
		ModuleEnemies* enemies = nullptr;

};
extern Application* App;
#endif // __APPLICATION_H__