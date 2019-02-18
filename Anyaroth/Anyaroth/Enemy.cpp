#include "Enemy.h"
#include "TransformComponent.h"
#include "BodyComponent.h"
#include "AnimatedSpriteComponent.h"
#include "MovingComponent.h"
#include "Game.h"
#include "Player.h"

Enemy::Enemy(Player* player, Game* g, PlayState* play,Texture* texture, Vector2D posIni) : _playerTransform(player->getComponent<TransformComponent>()), _play(play), GameComponent(g)
{
	addComponent<Texture>(texture);

	_transform = addComponent<TransformComponent>();
	_transform->setPosition(posIni.getX(), posIni.getY());

	_body = addComponent<BodyComponent>();
	_body->getBody()->SetType(b2_dynamicBody);
	_body->getBody()->SetFixedRotation(true);

	_anim = addComponent<AnimatedSpriteComponent>();
	_anim->addAnim(AnimatedSpriteComponent::Idle, 16, true);
	_anim->addAnim(AnimatedSpriteComponent::Walk, 10, true);
	_anim->addAnim(AnimatedSpriteComponent::WalkBack, 10, true); //esta en realidad es opcional
	_anim->addAnim(AnimatedSpriteComponent::MeleeKnife, 6, false);

	_anim->playAnim(AnimatedSpriteComponent::Idle);

	_movement = addComponent<MovingComponent>();
}
void Enemy::setItList(list<GameObject*>::iterator itFR)
{
	_itList = itFR;
}
void Enemy::update()
{
	GameComponent::update();
}