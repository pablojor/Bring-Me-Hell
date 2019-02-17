#pragma once

#include <list>
#include <vector>
#include <map>

#include "Texture.h"
#include "GameStateMachine.h"
#include "PlayState.h"
#include "DebugDraw.h"
#include "CollisionManager.h"
#include "Gun.h"
#include "Shooter.h"


const int NUM_VARIABLES = 4;
enum TypeVariable
{
	WIN_WIDTH, WIN_HEIGHT, FRAME_RATE, TIME_PER_FRAME	
};

const int NUM_STATES = 1;
enum StateName
{
	Play
};

const string INFO_PATH = "..\\files\\info\\";
const string FONTS_PATH = "..\\font\\";
const string FILES_PATH = "..\\files\\levels\\";
const string SAVES_PATH = "..\\files\\saves\\";
const string SPRITE_PATH = "..\\assets\\sprites\\";
const string TILEMAP_PATH = "..\\files\\tilemaps\\";


const int NUM_TEXTURES = 7;
const int NUM_FONTS = 0;

const int NUM_LEVELS = 1;
const string LEVELS[NUM_LEVELS] =
{

};

const double TILES_W = 16;
const double TILES_H = 16;

const double M_TO_PIXEL = 8;

//ARMAS
const int NUM_GUNS = 2; //N�mero de armas en el juego

struct GunAttributes
{
	Shooter shooter;
	string name;
	int maxAmmo;
	int maxClip;
};

//********************************************************************************
//********************************************************************************
//********************************************************************************
//********************************************************************************

class Game
{
	private:
		SDL_Window* window = nullptr;
		SDL_Renderer* renderer = nullptr;
		map <string, Texture*> textures;
		GameState* states[NUM_STATES];
		GameStateMachine* stateMachine = new GameStateMachine();
		vector<string> texturesName;
		b2World* _world = nullptr;
		CollisionManager colManager;
		DebugDraw debugger;
		bool exit = false;

	public:
		vector<int> var;

		//Las armas que hay en el juego
		enum GameGun
		{
			BasicGun,
			BasicShotgun
		};
		GunAttributes gameGuns[NUM_GUNS] = 
		{
			{Shooter(),"Pistola",60,12},
			{ Shooter(),"Escopeta",30,2 }
		};

		//Metodos
		void createVariables();
		void createTextures();
		void pushState(StateName);
		void changeState(StateName);
		Texture* getTexture(string nameText);
		SDL_Renderer* getRenderer() { return renderer; }
		b2World* getWorld() { return _world; }
		void newGame();
		void load();
		void save();
		//*************************
		Game();
		~Game();
		void run();
		void update(Uint32 time);
		void render(Uint32 time) const;
		void handleEvents(Uint32 time);
};