#pragma once
#include <list>
#include "GameComponent.h"

class Player;
class PlayState;

class AnimatedSpriteComponent;
class MovingComponent;
class TransformComponent;
class BodyComponent;


class Enemy : public GameComponent
{
	protected:
		AnimatedSpriteComponent* _anim = nullptr;
		MovingComponent* _movement = nullptr;
		TransformComponent* _transform = nullptr;
		Player* _player = nullptr;
		BodyComponent* _body = nullptr;

		PlayState* _play;
		list<GameObject*>::iterator _itList;

		bool _attacking = false;
		double _time;
		int _vision, _attackRange, _attackTime, _life, _damage;

	public:
		Enemy(Player* player, Game* g, PlayState* play, Texture* texture, Vector2D posIni, string tag);
		virtual ~Enemy() {}

		virtual void setLife(double amount);
		virtual void addLife(double amount);
		virtual void subLife(double amount);
		virtual void die();

		virtual void beginCollision(GameComponent* other, b2Contact* contact);

		void setItList(list<GameObject*>::iterator itFR);

		virtual inline void noLongerAttacking() { _attacking = false; }

		virtual void update();
};