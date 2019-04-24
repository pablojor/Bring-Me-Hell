#pragma once
#include "GameObject.h"
#include "Player.h"
#include "TransformComponent.h"
#include "BodyComponent.h"
#include "MovingComponent.h"
#include "CustomAnimatedSpriteComponent.h"
#include "Life.h"

class Enemy : public GameObject
{
protected:
	TransformComponent* _transform = nullptr;
	BodyComponent* _body = nullptr;
	MovingComponent* _movement = nullptr;
	CustomAnimatedSpriteComponent* _anim = nullptr;

	Player* _player = nullptr;
	Vector2D _playerDistance;

	Life _life;
	bool _attacking = false, _drop = true, _dropMelee = false, _stunned = false;
	int _vision, _attackRangeX, _attackRangeY, _attackTime, _damage;
	double _time;
	float32 _speed;
	
	int _coinValue = 10;

public:
	Enemy(Game* g, Player* player, Vector2D pos, Texture* texture);
	virtual ~Enemy() {}

	virtual void beginCollision(GameObject* other, b2Contact* contact);
	virtual void update(const double& deltaTime);

	inline void stopAttacking() { _attacking = false; }
	inline int random(int low, int high) const { return low + (rand() % abs(high - low)); }

	virtual void die();
	virtual void drop();

	virtual void subLife(int damage);
	inline Life getLife() const { return _life; }

	inline bool isStunned() { return _stunned; }
	inline void setStunned(bool value) { _stunned = value; }

	bool inCamera();
};