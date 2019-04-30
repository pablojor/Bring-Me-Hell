#include "PlayState.h"
#include "Game.h"
#include "PauseState.h"
#include "ParallaxLayer.h"
#include "WeaponManager.h"
#include "GameManager.h"
#include "CutScene.h"
#include "checkML.h"
#include <json.hpp>

using namespace nlohmann;

PlayState::PlayState(Game* g) : GameState(g)
{
}

PlayState::~PlayState()
{
	if (_cutScene != nullptr)
		delete _cutScene;
}

void PlayState::start()
{
	GameManager::init();
	WeaponManager::init();

	//HUD
	_playHud = new PlayStateHUD(_gameptr);
	setCanvas(_playHud);

	//Player
	_player = new Player(_gameptr, 100, 200);
	_playerBulletPool = new BulletPool(_gameptr);
	_player->setPlayerBulletPool(_playerBulletPool);
	_player->setPlayerPanel(_playHud->getPlayerPanel());

	_playHud->getShop()->setPlayer(_player);
	_playHud->getShop()->setVisible(false);

	//Enemy Pool
	auto enemyPool = new BulletPool(_gameptr);

	//Levels
	GameManager::getInstance()->setCurrentLevel(LevelManager::BossDemo);
	_level = new GameObject(_gameptr);
	_levelManager = LevelManager(_gameptr, _player, _level, _mainCamera, _playHud, enemyPool);
	_levelManager.setLevel(GameManager::getInstance()->getCurrentLevel());
	_mainCamera->setWorldBounds(_levelManager.getCurrentLevel(GameManager::getInstance()->getCurrentLevel())->getWidth(), _levelManager.getCurrentLevel(GameManager::getInstance()->getCurrentLevel())->getHeight());

	//Background
	_parallaxZone1 = new ParallaxBackGround(_mainCamera);
	_parallaxZone1->addLayer(new ParallaxLayer(_gameptr->getTexture("BgZ1L1"), _mainCamera, 0.25));
	_parallaxZone1->addLayer(new ParallaxLayer(_gameptr->getTexture("BgZ1L2"), _mainCamera, 0.5));
	_parallaxZone1->addLayer(new ParallaxLayer(_gameptr->getTexture("BgZ1L3"), _mainCamera, 0.75));
	_controls = new ParallaxLayer(_gameptr->getTexture("ControlsBG"), _mainCamera, 0);
	_parallaxZone1->addLayer(_controls);

	//Cursor
	_cursor = new Cursor(_gameptr);
	SDL_ShowCursor(false);

	//Camera
	_mainCamera->fixCameraToObject(_player);
	_mainCamera->setBackGround(_parallaxZone1);

	//Collisions and debugger
	_gameptr->getWorld()->SetContactListener(&_colManager);
	_gameptr->getWorld()->SetDebugDraw(&_debugger);

	//World
	_debugger.getRenderer(_gameptr->getRenderer());
	_debugger.getTexture(_gameptr->getTexture("Box"));
	_debugger.SetFlags(b2Draw::e_shapeBit);
	_debugger.getCamera(_mainCamera);

	//Gestion de colisiones
	_gameptr->getWorld()->SetContactListener(&_colManager);
	_gameptr->getWorld()->SetDebugDraw(&_debugger);

	//Particulas
	_particles = new ParticlePull(_gameptr);
	_particleManager = ParticleManager::GetParticleManager();
	_particleManager->setParticlePull(_particles);

	//----AÑADIR A LOS OBJETOS----//

	_stages.push_back(_level);
	_stages.push_back(_player);
	_stages.push_back(_cursor);
	_stages.push_back(_playerBulletPool);
	_stages.push_back(enemyPool);
	_stages.push_back(_particles);

	getMainCamera()->fadeIn(3000);
	getMainCamera()->fitCamera({ (double)_levelManager.getCurrentLevel(GameManager::getInstance()->getCurrentLevel())->getWidth(),
		(double)_levelManager.getCurrentLevel(GameManager::getInstance()->getCurrentLevel())->getHeight() }, false);
}

bool PlayState::handleEvent(const SDL_Event& event)
{
	GameState::handleEvent(event);

	bool handled = false;
	if (((event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_ESCAPE) || (event.type == SDL_CONTROLLERBUTTONDOWN && event.cbutton.button == SDL_CONTROLLER_BUTTON_START))
		&& !GameManager::getInstance()->getOnDialogue() && !GameManager::getInstance()->getOnShop())
	{
		_gameptr->setTimestep(0);
		_gameptr->pushState(new PauseState(_gameptr));
		handled = true;
	}
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_0) //Boton de prueba para reiniciar el nivel
		_levelManager.resetLevel();
	else if (event.type == SDL_KEYDOWN && event.key.keysym.sym == SDLK_1) //Boton de prueba para reiniciar la municion
	{
		_player->getCurrentGun()->resetAmmo();
		_playHud->getPlayerPanel()->updateAmmoViewer(_player->getCurrentGun()->getClip(), _player->getCurrentGun()->getMagazine());
	}

	return handled;
}

void PlayState::saveGame()
{
	ofstream output;
	output.open(SAVES_PATH + "save.json");

	if (output.is_open())
	{
		json j;
		j["level"] = GameManager::getInstance()->getCurrentLevel();
		j["Bank"] = _player->getBank();
		j["currentGun"] = _player->getCurrentGun()->getGunID();
		j["otherGun"] = _player->getOtherGun()->getGunID();

		auto items = _playHud->getShop()->getItems();
		for (ShopItem* i : items)
			j[i->getItemInfo()._name] = i->getItemInfo()._sold;
		output << j;
	}
}

void PlayState::loadGame()
{
	ifstream input;
	input.open(SAVES_PATH + "save.json");

	if (input.is_open())
	{
		json j;
		input >> j;
		_player->setBank(j["Bank"]);
		_levelManager.changeLevel(j["level"]);
		_player->changeCurrentGun(WeaponManager::getWeapon(_gameptr, j["currentGun"]));
		_player->changeOtherGun(WeaponManager::getWeapon(_gameptr, j["otherGun"]));

		auto items = _playHud->getShop()->getItems();
		for (ShopItem* i : items)
		{
			i->setItemSell(j[i->getItemInfo()._name]);
			i->setItemEquiped(false);
		}
		_playHud->getShop()->setPlayer(_player);
	}
}


void PlayState::update(const double& deltaTime)
{
	if (_player->changeLevel())
	{
		GameManager* gManager = GameManager::getInstance();
		_player->setChangeLevel(false);

		if (!_player->isDead())
		{
			if ((gManager->getCurrentLevel() + 1) % 2 == 0) //Si el proximo nivel no es una safe zone guarda el juego
				saveGame();
			else
				_player->getMoney()->storeWallet();

			_player->setInputFreezed(true);
			getMainCamera()->fadeOut(1000);
			getMainCamera()->onFadeComplete([this, gManager](Game* game)
			{
				_player->revive();
				gManager->setCurrentLevel(gManager->getCurrentLevel() + 1);
				_levelManager.changeLevel(gManager->getCurrentLevel());
				_player->setInputFreezed(false);

				if (_cutScene != nullptr)
					_cutScene->play();

				if (_controls != nullptr) {
					_parallaxZone1->removeLayer(_controls);
					delete _controls;
					_controls = nullptr;
				}

				int l = gManager->getCurrentLevel();
				if (l == LevelManager::Boss1 || l == LevelManager::Boss2 || l == LevelManager::Boss3 || l == LevelManager::BossDemo)
				{
					game->getCurrentState()->getMainCamera()->fadeIn(1000);
					game->getCurrentState()->getMainCamera()->onFadeComplete([this, gManager, l](Game* game)
					{
						game->getCurrentState()->getMainCamera()->fitCamera({ (float)_levelManager.getCurrentLevel(l)->getWidth(), (float)_levelManager.getCurrentLevel(l)->getHeight() }, true);
					});
				}
			});
		}
		else if (!getMainCamera()->isFading())
		{
			_playerBulletPool->stopBullets();

			_player->setInputFreezed(true);
			getMainCamera()->fadeOut(1000);
			getMainCamera()->onFadeComplete([this, gManager](Game* game)
			{
				_player->revive();

				gManager->setCurrentLevel(gManager->getCurrentLevel() - 1);
				_levelManager.changeLevel(gManager->getCurrentLevel());

				if (gManager->getCurrentLevel() == LevelManager::SafeDemo)
				{
					_controls = new ParallaxLayer(game->getTexture("ControlsBG"), _mainCamera, 0);
					_parallaxZone1->addLayer(_controls);
					getMainCamera()->fitCamera({ (double)_levelManager.getCurrentLevel(1)->getWidth(), (double)_levelManager.getCurrentLevel(1)->getHeight() }, false);

				}

				_player->setChangeLevel(false);
				_player->setInputFreezed(false);
			});
		}
	}
	else
	{
		if (_cutScene != nullptr)
		{
			if (_cutScene->isPlaying())
				_cutScene->update(deltaTime);
			else
			{
				delete _cutScene;
				_cutScene = nullptr;
			}
		}
	}

	GameState::update(deltaTime);
    _particleManager->updateManager(deltaTime);
}