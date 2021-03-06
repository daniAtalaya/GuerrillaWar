#pragma once

#ifndef __MODULE_PICKUP_H__
#define __MODULE_PICKUP_H__

#include "Module.h"

#define MAX_PICKS 100

enum class Pick_Type {
	NO_TYPE,
	FUSIL,
	LANZALLAMAS,
	RECLUSO,
};

struct PickSpawnpoint
{
		Pick_Type type = Pick_Type::NO_TYPE;
		int x, y;
};
	
class pickUp;
struct SDL_Texture;

class ModulePickUp : public Module
	{
	public:
		// Constructor
		ModulePickUp(bool startEnabled);

		// Destructor
		~ModulePickUp();

		// Called when the module is activated
		// Loads the necessary textures for the enemies
		bool Start() override;

		// Called at the beginning of the application loop
		// Removes all enemies pending to delete
		Update_Status PreUpdate() override;

		// Called at the middle of the application loop
		// Handles all enemies logic and spawning/despawning
		Update_Status Update() override;

		// Called at the end of the application loop
		// Iterates all the enemies and draws them
		Update_Status PostUpdate() override;

		// Called on application exit
		// Destroys all active enemies left in the array
		bool CleanUp() override;

		// Called when an enemi collider hits another collider
		// The enemy is destroyed and an explosion particle is fired
		void OnCollision(Collider* c1, Collider* c2) override;

		// Add an enemy into the queue to be spawned later
		bool AddPick(Pick_Type type, int x, int y);

		// Iterates the queue and checks for camera position
		void HandlePickUpSpawn();

		// Destroys any enemies that have moved outside the camera limits
		void HandlePickUpDespawn();

	private:
		// Spawns a new enemy using the data from the queue
		void SpawnpickUp(const PickSpawnpoint& info);

	private:
		// A queue with all spawn points information
		PickSpawnpoint spawnQueue[MAX_PICKS];

		// All spawned enemies in the scene
		pickUp* pickUps[MAX_PICKS] = { nullptr };

		// The enemies sprite sheet
		SDL_Texture* texture = nullptr;

		// The audio fx for destroying an enemy
		int enemyDestroyedFx = 0;

	};

#endif // __MODULE_PICKUP_H__

