#include "PlayState.h"
#include "Game.h"
#include "Layer.h"
#include "Player.h"

PlayState::PlayState(Game* g) : GameState(g)
{
	stages.push_back(new Layer("Capa de Patrones 1", g->getTexture("tileset"), TILEMAP_PATH + "P2.json"));
	stages.push_back(new Layer("Capa de patrones 2", g->getTexture("tileset"), TILEMAP_PATH + "P2.json"));
	stages.push_back(new Layer("Capa de patrones 3", g->getTexture("tileset"), TILEMAP_PATH + "P2.json"));
  
	stages.push_back(new Player(g->getTexture("Mk")));
};

void PlayState::handleEvents(SDL_Event& e)
{
	GameState::handleEvents(e);
}

void PlayState::update()
{
	GameState::update();
}