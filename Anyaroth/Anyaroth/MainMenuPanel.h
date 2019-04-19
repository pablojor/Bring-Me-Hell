#pragma once
#include "PanelUI.h"
#include "ButtonUI.h"
#include "TextUI.h"

class MainMenuPanel : public PanelUI
{
	private:
		ButtonUI * _playButton;
		ButtonUI * _optionsButton;
		ButtonUI * _exitButton;

		TextUI * _playText;
		TextUI * _optionsText;
		TextUI * _exitText;

	public:
		MainMenuPanel(Game* g);
		~MainMenuPanel();

		void startGame(Game * g);
		void options(Game * g);
		void exitGame(Game * g);
};

