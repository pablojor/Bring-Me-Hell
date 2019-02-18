#include "PlayerControllerComponent.h"
#include "GameComponent.h"
#include "Player.h"

PlayerControllerComponent::PlayerControllerComponent(GameComponent* obj) : InputComponent(obj)
{
	_movement = obj->getComponent<MovingComponent>();
	if (_movement == nullptr)
		_movement = obj->addComponent<MovingComponent>();

	_anim = obj->getComponent<AnimatedSpriteComponent>();
	if (_anim == nullptr)
		_anim = obj->addComponent<AnimatedSpriteComponent>();

	_obj = obj;
}

void PlayerControllerComponent::handleInput(const SDL_Event& event)
{
	if (event.type == SDL_KEYDOWN)
	{
		if (event.key.keysym.sym == SDLK_a)
			_aPul = true;
		else if (event.key.keysym.sym == SDLK_d)
			_dPul = true;

		if (event.key.keysym.sym == SDLK_w)
			_wPul = true;

		if (event.key.keysym.sym == SDLK_s)
			_sPul = true;
	}

	if (event.type == SDL_KEYUP)
	{
		if (event.key.keysym.sym == SDLK_a)
			_aPul = false;
		else if (event.key.keysym.sym == SDLK_d)
			_dPul = false;

		if (event.key.keysym.sym == SDLK_w)
			_wPul = false;

		if (event.key.keysym.sym == SDLK_s)
			_sPul = false;
	}

	if (event.type == SDL_MOUSEBUTTONDOWN)
	{
		if (event.button.button == SDL_BUTTON_RIGHT)
		{
			_rightClickPul = true;
		}
	}

	if (event.type == SDL_MOUSEBUTTONUP)
	{
		if (event.button.button == SDL_BUTTON_RIGHT) //&& _isAttacking)
		{
			_rightClickPul = false;
		}
	}

	if (_rightClickPul) //&& !_isAttacking)
	{
		_movement->changeDir(0, 0);
		_isAttacking = true;
		//llamo a funci�n de melee
		dynamic_cast<Player*>(_obj)->setCurrentState(Player::Attacking);
		//_animArm->setActive(false);//desactivo brazo
		_anim->playAnim(AnimatedSpriteComponent::MeleeKnife);//llamo animacion del melee dependiendo del arma cuerpo a cuerpo
	}

	if ((_aPul &&_dPul) && !_isAttacking)
	{
		_movement->changeDir(0, 0); //Llamo a animacion idle
		_anim->playAnim(AnimatedSpriteComponent::Idle);
	}
	else if (_aPul && !_isAttacking)
	{
		_movement->changeDir(-1, 0); //Llamo a animacion de moverse y un flip
		if (!_anim->isFlipped())
			_anim->playAnim(AnimatedSpriteComponent::WalkBack);
		else
			_anim->playAnim(AnimatedSpriteComponent::Walk);
	}
	else if (_dPul && !_isAttacking)
	{
		_movement->changeDir(1, 0); //Llamo a animacion de moverse
		if (!_anim->isFlipped())
			_anim->playAnim(AnimatedSpriteComponent::Walk);
		else
			_anim->playAnim(AnimatedSpriteComponent::WalkBack);
	}
	else if (_wPul && !_isAttacking && jump)
	{
		_movement->changeDir(_movement->getDirX(), -1);
	}
	else if (!_isAttacking)
	{
		_movement->changeDir(0, 0); //Llamo a animacion idle
		_anim->playAnim(AnimatedSpriteComponent::Idle);
	}

	
	
	if (_sPul /*y estoy saltando*/)
	{
		//Llamo a componente de dash hacia abajo (culo)
	}
}

void PlayerControllerComponent::changeJump()
{
	jump = false;
	_movement->changeDir(_movement->getDirX(), 0);
}

void PlayerControllerComponent::ableJump()
{
	jump = true;
}
