#include "PausePanel.h"
#include "Game.h"


PausePanel::PausePanel(Game* g)
{
	//----BOTONES----//

	_playButton = new ButtonUI(g, g->getTexture("MenuButtons"), [this](Game* game) { continueGame(game); }, { 0, 1, 2, 2, 2 });
	int buttonH = _playButton->getH(),
		buttonW = _playButton->getW();
	_playButton->setPosition(CAMERA_RESOLUTION_X / 2 - buttonW / 2, CAMERA_RESOLUTION_Y / 3 - buttonH + 50);

	_optionsButton = new ButtonUI(g, g->getTexture("MenuButtons"), [this](Game* game) { options(game); }, { 0, 1, 2, 2, 2 });
	_optionsButton->setPosition(CAMERA_RESOLUTION_X / 2 - buttonW / 2, CAMERA_RESOLUTION_Y / 3 + buttonH + 50);

	_menuButton = new ButtonUI(g, g->getTexture("MenuButtons"), [this](Game* game) { returnMenu(game); }, { 0, 1, 2, 2, 2 });
	_menuButton->setPosition(CAMERA_RESOLUTION_X / 2 - buttonW / 2, CAMERA_RESOLUTION_Y / 3 + buttonH + 100);

	//----TEXTOS----//

	_playText = new TextUI(g, "Continue", g->getFont("ARIAL12"), 12);
	_playText->setPosition(CAMERA_RESOLUTION_X / 2 - _playText->getW() / 2,
							_playButton->getY() + buttonH / 2 - _playText->getH() / 2);

	_optionsText = new TextUI(g, "Options", g->getFont("ARIAL12"), 12);
	_optionsText->setPosition(_optionsButton->getX() + buttonW / 2 - _optionsText->getW() / 2,
								_optionsButton->getY() + buttonH / 2 - _optionsText->getH() / 2);

	_menuText = new TextUI(g, "Menu", g->getFont("ARIAL12"), 12);
	_menuText->setPosition(_menuButton->getX() + buttonW / 2 - _menuText->getW() / 2,
							_menuButton->getY() + buttonH / 2 - _menuText->getH() / 2);

	addChild(_playButton);
	addChild(_optionsButton);
	addChild(_menuButton);
	addChild(_playText);
	addChild(_optionsText);
	addChild(_menuText);
}


PausePanel::~PausePanel()
{
}


void PausePanel::continueGame(Game * g)
{
	g->setTimestep(1 / 60.0);
	g->popState();
}

void PausePanel::options(Game * g)
{
	_visible = !_visible; 
	g->getCurrentState()->getMainCamera()->setBackGround(new BackGround(g->getTexture("BgOptionsMenu"), g->getCurrentState()->getMainCamera()));
	g->getCurrentState()->getPauseHUD()->getOptionsPanel()->setVisible(true);
}

void PausePanel::returnMenu(Game * g)
{
	g->setTimestep(1 / 60.0);
	g->popState();
	g->changeState(new MenuState(g));
}