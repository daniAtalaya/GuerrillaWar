#include "redSoldier.h"

#include "Application.h"
#include "ModuleCollisions.h"

redSoldier::redSoldier(int x, int y) : Enemy(x, y)
{
	front.PushBack({577,0,27,54});
	front.PushBack({607,1,30,54});
	front.PushBack({640,0,28,54});
	front.PushBack({674,0,25,54});
	front.loop = true;
	front.speed = 0.1f;
	currentAnim = &front;

	path.PushBack({ 0, -1.2f }, 150, &front);
	path.PushBack({ 0, 1.2f }, 150, &front);

	collider = App->collisions->AddCollider({ x, y, 28, 55 }, Collider::Type::ENEMY, (Module*)App->enemies);

	currentAnim = &back;
}

void redSoldier::Update()
{
	path.Update();
	positionenemy = spawnPos + path.GetRelativePosition();
	currentAnim = path.GetCurrentAnimation();

	// Call to the base class. It must be called at the end
	// It will update the collider depending on the position
	Enemy::Update();
}
