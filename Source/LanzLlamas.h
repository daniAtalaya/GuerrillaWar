
#ifndef __LANZALLAMAS_H__
#define __LANZALLAMAS_H__
#include "pickUp.h"
#include "Path.h"

class LanzLlamas : public pickUp
{

public:

	// Constructor (x y coordinates in the world)
	// Creates animation data and the collider
	LanzLlamas(int x, int y);

	// The enemy is going to perform a sinusoidal movement
	void Update() override;

private:

	// The enemy animation
	Animation fusilaso;

	Path path;

};
#endif // __FUSIL_H__