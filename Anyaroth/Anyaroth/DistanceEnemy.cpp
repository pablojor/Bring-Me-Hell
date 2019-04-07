#include "DistanceEnemy.h"
#include "Game.h"
#include "BasicPistol.h"

DistanceEnemy::DistanceEnemy(Game* g, Player* player, Vector2D pos, Texture* texture, BulletPool* pool) : Enemy(g, player, pos, texture)
{
	_arm = new EnemyArm(g, this, player, this, { 10, 12 });
	_arm->setTexture(g->getTexture("ArmPistol"));
	_arm->setAnimations(PlayerArmType);
	addChild(_arm);

	_myGun = new BasicPistol(g);
	_myBulletPool = pool;
}

void DistanceEnemy::raycast()
{
	b2Vec2 enemyPos = _body->getBody()->GetPosition();
	b2Vec2 targetPos = _player->getComponent<BodyComponent>()->getBody()->GetPosition();

	b2RayCastInput rayInput;

	rayInput.maxFraction = 1;
	rayInput.p1 = { (float32)(enemyPos.x), (float32)(enemyPos.y - _body->getH()) };
	rayInput.p2 = { (float32)(targetPos.x), (float32)(targetPos.y) };

	b2RayCastOutput rayOutput;

	_armVision = true;

	for (b2Body* b = getWorld()->GetBodyList(); b && _armVision; b = b->GetNext())
		for (b2Fixture* f = b->GetFixtureList(); f && _armVision; f = f->GetNext())
			if (b->GetType() == b2_staticBody && f->RayCast(&rayOutput, rayInput, 0))
				_armVision = false;
}

void DistanceEnemy::shoot()
{
	if (_armVision) //Si vemos al jugador
	{
		if (_playerDistance.getX() > 0) //Derecha
			_anim->unFlip();
		else if (_playerDistance.getX() < 0) //Izquierda
			_anim->flip();

		_arm->shoot();
		_myGun->enemyShoot(_myBulletPool, _arm->getPosition(), !_anim->isFlipped() ? _arm->getAngle() + random(-_fail, _fail) : _arm->getAngle() + 180 + random(-_fail, _fail), "EnemyBullet");
	}
}

void DistanceEnemy::update(const double& deltaTime)
{
	Enemy::update(deltaTime);
	_myGun->refreshGunCadence(deltaTime);
}