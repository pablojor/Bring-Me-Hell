#include "PlayState.h"
#include "PauseState.h"
#include "Game.h"
#include "BodyComponent.h"
#include "FollowingComponent.h"
#include "checkML.h"
#include "ObjectLayer.h"
#include "Coin.h"
#include "ParallaxBackGround.h"
#include "ParallaxLayer.h"
#include "PlayStateHUD.h"

PlayState::PlayState(Game* g) : GameState(g)
{
	_game = g;
	_world = g->getWorld();

	//Tilemap
	_layer = new Layer("Mapa", g->getTexture("tileset"), TILEMAP_PATH + "Nivel1.json", g, "Mapa");
	_stages.push_back(_layer);
	_colisionLayer = new Layer("Suelo", g->getTexture("tileset"), TILEMAP_PATH + "Nivel1.json", g, "Suelo");
	_colisionLayer->addComponent<BodyComponent>();
	_stages.push_back(_colisionLayer);

	//Pools balas
	_basicBulletPool = new BulletPool<100>(g, g->getTexture("PistolBullet"), 100, 10, 1000);
	_stages.push_back(_basicBulletPool);
	_pools.push_back(_basicBulletPool);

	_basicShotgunBulletPool = new BulletPool<100>(g, g->getTexture("PistolBullet"), 100, 25, 60);
	_stages.push_back(_basicShotgunBulletPool);
	_pools.push_back(_basicShotgunBulletPool);

	_enemyPool = new BulletPool<200>(g, g->getTexture("PistolBullet"), 100, 10, 1000);
	_stages.push_back(_enemyPool);
	_pools.push_back(_enemyPool);

	//Player
	_player = new Player(g->getTexture("Mk"), g, this, "Player");
	_stages.push_back(_player);

	//Camera
	_mainCamera->fixCameraToObject(_player);
	
	//Enemies
	 auto oL= new ObjectLayer(TILEMAP_PATH + "Nivel1.json", "Enemigos");
	 vector <Vector2D> enemiesPos = oL->getObjectsPositions();
	 delete oL;

	 for (int i = 0; i < enemiesPos.size(); i++)
	 {
		 _enemy = new MartyrEnemy(_player, g, this, g->getTexture("EnemyMelee"), Vector2D(enemiesPos[i].getX(), enemiesPos[i].getY() - TILES_SIZE * 2), "Enemy");
		_stages.push_back(_enemy);
		auto itFR = --(_stages.end());
		_enemy->setItList(itFR);
	 }

	//Coins
	 oL = new ObjectLayer(TILEMAP_PATH + "Nivel1.json", "Monedas");
	 vector <Vector2D> coinsPos = oL->getObjectsPositions();
	 delete oL;
	 for (int i = 0; i < coinsPos.size(); i++)
	 {
		 _coin = new Coin(this, g, g->getTexture("Coin"), Vector2D(coinsPos[i].getX(), coinsPos[i].getY() - TILES_SIZE), 20);
		 _stages.push_back(_coin);

		 auto itFR = --(_stages.end());
		 _coin->setItList(itFR);
	 }

	 //World
	 _debugger.getRenderer(g->getRenderer());
	 _debugger.getTexture(g->getTexture("body"));
	 _debugger.SetFlags(b2Draw::e_shapeBit);
	 _debugger.getCamera(_mainCamera);

	 //Gestion de colisiones
	 g->getWorld()->SetContactListener(&_colManager);
	 g->getWorld()->SetDebugDraw(&_debugger);
	
	//Camera BackGound
	ParallaxBackGround* a = new ParallaxBackGround(_mainCamera);
	a->addLayer(new ParallaxLayer(g->getTexture("BgZ1L1"), _mainCamera,0.5));
	a->addLayer(new ParallaxLayer(g->getTexture("BgZ1L2"), _mainCamera,1));
	a->addLayer(new ParallaxLayer(g->getTexture("BgZ1L3"), _mainCamera,1.5));
	_mainCamera->setBackGround(a);

	//HUD
	auto b = new PlayStateHUD(g);
	setCanvas(b);
	//Asignacion de paneles a sus controladores
	_player->setPlayerPanel(b->getPlayerPanel());
}

void PlayState::KillObject(list<GameObject*>::iterator itList)
{
	items_ToDelete.push_back(itList);
}

void PlayState::render()
{
	GameState::render();
	_world->DrawDebugData();
}

bool PlayState::handleEvents(SDL_Event& e)
{
	GameState::handleEvents(e);

	bool handled = false;
	if (e.type == SDL_KEYDOWN && e.key.keysym.sym == SDLK_ESCAPE) 
	{
		_game->pushState(new PauseState(_game));
		handled = true;
	}
	return handled;
}

void PlayState::update()
{
	GameState::update();
	_world->Step(1 / 20.0, 8, 3);

	int i = items_ToDelete.size() - 1;
	while (i >= 0)
	{
		delete *items_ToDelete[i];
		_stages.erase(items_ToDelete[i]);
		items_ToDelete.pop_back();
		i--;
	}
}