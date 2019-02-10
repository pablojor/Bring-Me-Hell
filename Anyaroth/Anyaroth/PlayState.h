#pragma once
#include "GameState.h"
#include "Layer.h"
#include "Player.h"

class PlayState : public GameState
{
	private:
		Player* _player = nullptr;
		Layer* _colLayer = nullptr;
	public:
		
		PlayState(Game* g);
		virtual void update();
		virtual void handleEvents(SDL_Event& e);

};