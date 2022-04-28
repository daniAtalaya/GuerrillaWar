#include "ModuleParticles.h"

#include "Application.h"

#include "ModuleImatges.h"
#include "ModuleRender.h"
#include "ModuleCollisions.h"

#include "SDL/include/SDL_timer.h"

ModuleParticles::ModuleParticles()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
		particles[i] = nullptr;
}

ModuleParticles::~ModuleParticles()
{

}

bool ModuleParticles::Start()
{
	bool ret = false;
	LOG("Loading particles");
	ret = App->imatges->get("shoots", texture);

	// Explosion particle
	explosion.anim.PushBack({ 274, 296, 33, 30 });
	explosion.anim.PushBack({ 313, 296, 33, 30 });
	explosion.anim.PushBack({ 346, 296, 33, 30 });
	explosion.anim.PushBack({ 382, 296, 33, 30 });
	explosion.anim.PushBack({ 419, 296, 33, 30 });
	explosion.anim.PushBack({ 457, 296, 33, 30 });
	explosion.anim.loop = false;
	explosion.anim.speed = 0.3f;

	//Normal bullet anims
	bulletN.anim.PushBack({ 7, 180, 7, 10 });
	bulletN.anim.PushBack({ 23, 179, 7, 11 });
	bulletN.speed.y = -5;
	bulletN.lifetime = 180;
	bulletN.anim.speed = 0.2f;

	bulletNE.anim.PushBack({ 36, 181, 8, 11 });
	bulletNE.anim.PushBack({ 52, 181, 8, 11 });
	bulletNE.speed.y = -5;
	bulletNE.speed.x = 5;
	bulletNE.lifetime = 180;
	bulletNE.anim.speed = 0.2f;

	bulletE.anim.PushBack({ 67, 183, 9, 9 });
	bulletE.anim.PushBack({ 84, 183, 9, 9 });
	bulletE.speed.x = 5;
	bulletE.lifetime = 180;
	bulletE.anim.speed = 0.2f;

	bulletSE.anim.PushBack({ 99, 180, 8, 12 });
	bulletSE.anim.PushBack({ 115, 180, 8, 12 });
	bulletSE.speed.x = 5;
	bulletSE.speed.y = 5;
	bulletSE.lifetime = 180;
	bulletSE.anim.speed = 0.2f;

	bulletS.anim.PushBack({ 134, 179, 7, 12 });
	bulletS.anim.PushBack({ 150, 180, 7, 11 });
	bulletS.speed.y = 5;
	bulletS.lifetime = 180;
	bulletS.anim.speed = 0.2f;

	bulletSW.anim.PushBack({ 165, 180, 7, 12 });
	bulletSW.anim.PushBack({ 181, 180, 7, 12 });
	bulletSW.speed.x = -5;
	bulletSW.speed.y = 5;
	bulletSW.lifetime = 180;
	bulletSW.anim.speed = 0.2f;

	bulletW.anim.PushBack({ 189, 182, 9, 8 });
	bulletW.anim.PushBack({ 195, 182, 10, 8 });
	bulletW.speed.x = -5;
	bulletW.lifetime = 180;
	bulletW.anim.speed = 0.2f;

	bulletNW.anim.PushBack({ 228, 181, 8, 11 });
	bulletNW.anim.PushBack({ 245, 181, 8, 11 });
	bulletNW.speed.x = -5;
	bulletNW.speed.y = - 5;
	bulletNW.lifetime = 180;
	bulletNW.anim.speed = 0.2f;

	return true;
}

bool ModuleParticles::CleanUp()
{
	LOG("Unloading particles");

	// Delete all remaining active particles on application exit 
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		if (particles[i] != nullptr)
		{
			delete particles[i];
			particles[i] = nullptr;
		}
	}

	return true;
}

// TODO 6: Make so every time a particle hits a wall it triggers an explosion particle
void ModuleParticles::OnCollision(Collider* c1, Collider* c2) {
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i) {
		// Always destroy particles that collide
		if (particles[i] != nullptr && particles[i]->collider == c1) {
			AddParticle(explosion, c1->rect.x, c1->rect.y - c1->rect.h);
			delete particles[i];
			particles[i] = nullptr;
			break;
		}
	}
}

update_status ModuleParticles::Update()
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle == nullptr)	continue;

		// Call particle Update. If it has reached its lifetime, destroy it
		if (particle->Update() == false)
		{
			delete particle;
			particles[i] = nullptr;
		}
	}

	return update_status::UPDATE_CONTINUE;
}

update_status ModuleParticles::PostUpdate()
{
	//Iterating all particle array and drawing any active particles
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		Particle* particle = particles[i];

		if (particle != nullptr && particle->isAlive)
		{
			App->render->Blit(texture, particle->position.x, particle->position.y, &(particle->anim.GetCurrentFrame()));
		}
	}

	return update_status::UPDATE_CONTINUE;
}

void ModuleParticles::AddParticle(const Particle& particle, int x, int y, Collider::Type colliderType, uint delay)
{
	for (uint i = 0; i < MAX_ACTIVE_PARTICLES; ++i)
	{
		//Finding an empty slot for a new particle
		if (particles[i] == nullptr)
		{
			Particle* p = new Particle(particle);

			p->frameCount = -(int)delay;			// We start the frameCount as the negative delay
			p->position.x = x;						// so when frameCount reaches 0 the particle will be activated
			p->position.y = y;

			//Adding the particle's collider
			if (colliderType != Collider::Type::NONE)
				p->collider = App->collisions->AddCollider(p->anim.GetCurrentFrame(), colliderType, this);

			particles[i] = p;
			break;
		}
	}
}