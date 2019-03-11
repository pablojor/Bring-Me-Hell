#include "FrameComponent.h"
#include "GameComponent.h"
#include "Camera.h"

void FrameComponent::setFilAndCol(int fil, int col)
{
	_fil = fil;
	_col = col;
}

void FrameComponent::render(Camera* c) const
{
	SDL_Rect destRect;
	destRect.w = (_texture->getW()/_texture->getNumCols()) * _transform->getScale().getX();
	destRect.h = (_texture->getH()/_texture->getNumFils()) * _transform->getScale().getY();
	destRect.x = _transform->getPosition().getX() - _transform->getAnchor().getX() * destRect.w - c->getCameraPosition().getX();
	destRect.y = _transform->getPosition().getY() - _transform->getAnchor().getY() * destRect.h - c->getCameraPosition().getY();

	SDL_Point anchor = { _transform->getAnchor().getX() * destRect.w, _transform->getAnchor().getY() * destRect.h };

	_texture->renderFrame(destRect, _fil, _col, _transform->getRotation(), anchor, (_flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}