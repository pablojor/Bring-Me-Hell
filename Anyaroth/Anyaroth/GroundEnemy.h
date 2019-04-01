#pragma once
#include "Enemy.h"

class GroundEnemy : virtual public Enemy
{
private:
	bool collidingR = false, collidingL = false;
protected:
	Vector2D _dir = Vector2D();

public:
	GroundEnemy(Game* g, Player* player, Vector2D pos, Texture* texture);
	virtual ~GroundEnemy() {}

	virtual void idle();
	virtual void moving(Vector2D& dir);
	virtual void attack();
	virtual void attacking(const double& deltaTime) = 0;
	virtual void addSensors();
	virtual void beginCollision(GameObject* other, b2Contact* contact);
	virtual void endCollision(GameObject* other, b2Contact* contact);
};