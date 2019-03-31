#include "DepotPanel.h"
#include "Player.h"
#include "ShopMenu.h"

DepotPanel::DepotPanel(Game* game, Player* player) : _player(player), PanelUI(game)
{
	//----MARCOS----//

	_depotFrame = new ImageUI(game, game->getTexture("ShopPanel"), 5, 5);
	_depotFrame->setSize(_depotFrame->getW() * 2, _depotFrame->getH());
	_equipmentFrame = new ImageUI(game, game->getTexture("ShopPanel"), _depotFrame->getX() + _depotFrame->getW() + 10, 5);
	_equipmentFrame->setSize(_equipmentFrame->getW() * 0.75, _equipmentFrame->getH() * 0.75);

	addChild(_depotFrame);
	addChild(_equipmentFrame);

	//----BOTON DE SALIR----//

	_exitButton = new ButtonUI(game, game->getTexture("Button"), ShopMenu::closeDepotPanel, { 0,1,2,3 });
	_exitButton->setPosition(_depotFrame->getX(), _depotFrame->getY() + _depotFrame->getH() + 2);
	_exitButton->setSize(_depotFrame->getW(), _exitButton->getH() + 4);

	addChild(_exitButton);

	//----EQUIPAMIENTO----//

	int distanceBetweenSlots = 10;
	int itemWidth = (_equipmentFrame->getW() / 2) - (distanceBetweenSlots + distanceBetweenSlots / 2);
	int itemHeight = itemWidth;

	_firstWeapon = new ShopItem(game, game->getTexture("InfoIcon"));
	_firstWeapon->setSize(itemWidth, itemHeight);
	_firstWeapon->setPosition(_equipmentFrame->getX() + distanceBetweenSlots, 
								_equipmentFrame->getY() + _equipmentFrame->getH() / 4 - distanceBetweenSlots / 2);

	_secondWeapon = new ShopItem(game, game->getTexture("InfoIcon"));
	_secondWeapon->setSize(itemWidth, itemHeight);
	_secondWeapon->setPosition(_firstWeapon->getX() + _firstWeapon->getW() + distanceBetweenSlots, 
								_firstWeapon->getY());

	_meleeWeapon = new ShopItem(game, game->getTexture("InfoIcon"));
	_meleeWeapon->setSize(itemWidth, itemHeight);
	_meleeWeapon->setPosition(_equipmentFrame->getX() + _equipmentFrame->getW() / 2 - itemWidth / 2, 
								_equipmentFrame->getY() + (_equipmentFrame->getH() / 4) * 2 + distanceBetweenSlots / 2);

	addChild(_firstWeapon);
	addChild(_secondWeapon);
	addChild(_meleeWeapon);

	//----ALMACEN----//
}
