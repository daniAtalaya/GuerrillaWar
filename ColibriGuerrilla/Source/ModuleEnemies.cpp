#include "ModuleEnemies.h"

#include "Application.h"

#include "ModuleRender.h"
#include "ModuleTextures.h"
#include "ModuleAudio.h"
#include "ModulePlayer.h"
#include "Collider.h"

#include "Enemy.h"
#include "redSoldier.h"
#include "greenSoldier.h"
#include "Boss_F1.h"
#include "Boss_F2.h"
#include "Boss_Soldiers.h"
#include "greenSoldiergranada.h"

#define SPAWN_MARGIN 200


ModuleEnemies::ModuleEnemies(bool startEnabled) : Module(startEnabled)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
		enemies[i] = nullptr;
}

ModuleEnemies::~ModuleEnemies()
{

}

bool ModuleEnemies::Start()
{
	texture = App->textures->Load("Assets/Sprites/EnemiesGW.png");
	enemyDestroyedFx = App->audio->LoadFx("Assets/Fx/enemydead.wav");

	return true;
}


Update_Status ModuleEnemies::PreUpdate()
{
	// Remove all enemies scheduled for deletion
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr && enemies[i]->pendingToDelete)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleEnemies::Update()
{
	HandleEnemiesSpawn();

	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
			enemies[i]->Update();
	}

	HandleEnemiesDespawn();

	return Update_Status::UPDATE_CONTINUE;
}

Update_Status ModuleEnemies::PostUpdate()
{
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
			enemies[i]->Draw();
	}

	return Update_Status::UPDATE_CONTINUE;
}

// Called before quitting
bool ModuleEnemies::CleanUp()
{
	LOG("Freeing all enemies");

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if(enemies[i] != nullptr)
		{
			delete enemies[i];
			enemies[i] = nullptr;
		}
	}

	return true;
}

bool ModuleEnemies::AddEnemy(Enemy_Type type, int x, int y)
{
	bool ret = false;

	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
			if (spawnQueue[i].type == Enemy_Type::NO_TYPE)
			{
				spawnQueue[i].type = type;
				spawnQueue[i].x = x;
				spawnQueue[i].y = y;
				ret = true;
				break;
			}
	}

	return ret;
}

void ModuleEnemies::HandleEnemiesSpawn()
{
	// Iterate all the enemies queue
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (spawnQueue[i].type != Enemy_Type::NO_TYPE)
		{
			// Spawn a new enemy if the screen has reached a spawn position
			if (spawnQueue[i].x * SCREEN_SIZE < App->player->cameraGameplay.x + (App->player->cameraGameplay.w * SCREEN_SIZE) + SPAWN_MARGIN)
			{
				LOG("Spawning enemy at %d", spawnQueue[i].x * SCREEN_SIZE);

				SpawnEnemy(spawnQueue[i]);
				spawnQueue[i].type = Enemy_Type::NO_TYPE; // Removing the newly spawned enemy from the queue
			}
		}
	}
}

void ModuleEnemies::HandleEnemiesDespawn()
{
	// Iterate existing enemies
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] != nullptr)
		{
			// Delete the enemy when it has reached the end of the screen
			if (enemies[i]->positionenemy.x * SCREEN_SIZE < (App->player->cameraGameplay.x) - SPAWN_MARGIN)
			{
				LOG("DeSpawning enemy at %d", enemies[i]->positionenemy.x * SCREEN_SIZE);

				enemies[i]->SetToDelete();
			}
		}
	}
}

void ModuleEnemies::SpawnEnemy(const EnemySpawnpoint& info)
{
	// Find an empty slot in the enemies array
	for (uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if (enemies[i] == nullptr)
		{
			switch (info.type)
			{
				case Enemy_Type::GREENSOLDIER:
					enemies[i] = new greenSoldier(info.x, info.y);
					break;
				case Enemy_Type::GREENSOLDIERGRAN:
					enemies[i] = new greenSoldiergranada(info.x, info.y);
					break;
				case Enemy_Type::REDSOLDIER:
					enemies[i] = new redSoldier(info.x, info.y);
					break;
				case Enemy_Type::BOSSF1:
					enemies[i] = new Boss_F1(info.x, info.y);
					break;
				case Enemy_Type::BOSSF2:
					enemies[i] = new Boss_F2(info.x, info.y);
					break;
				case Enemy_Type::SOLDIERSBOSS:
					enemies[i] = new Boss_Soldiers(info.x, info.y);
					break;
			}
			enemies[i]->texture = texture;
			enemies[i]->destroyedFx = enemyDestroyedFx;
			break;
		}
	}
}

void ModuleEnemies::OnCollision(Collider* c1, Collider* c2)
{
	for(uint i = 0; i < MAX_ENEMIES; ++i)
	{
		if ((c2->type == Collider::Type::PLAYER_SHOT || c2->type == Collider::Type::EXPLOSION || c2->type == Collider::Type::PLAYER))
		{
			if (enemies[i] != nullptr && enemies[i]->GetCollider() == c1)
			{
				enemies[i]->OnCollision(c2); //Notify the enemy of a collision
				App->player->score += 100;
				App->audio->PlayFx(enemyDestroyedFx);

				break;
			}
		}
	}
}