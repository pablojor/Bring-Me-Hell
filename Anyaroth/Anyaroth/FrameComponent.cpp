#include "FrameComponent.h"
#include "GameComponent.h"


FrameComponent::FrameComponent(GameComponent* obj) : RenderComponent(obj)
{
	_texture = obj->getComponent<Texture>();
	_transform = obj->getComponent<TransformComponent>();
	if (_transform == nullptr) _transform = obj->addComponent<TransformComponent>();
}

void FrameComponent::setFilAndCol(int fil, int col)
{
	_fil = fil;
	_col = col;
}

void FrameComponent::render() const
{
	SDL_Rect destRect;
	destRect.w = (_texture->getW()/_texture->getNumCols()) * _transform->getScale().getX();
	destRect.h = (_texture->getH()/_texture->getNumFils()) * _transform->getScale().getY();
	destRect.x = _transform->getPosition().getX() - _transform->getAnchor().getX() * destRect.w;
	destRect.y = _transform->getPosition().getY() - _transform->getAnchor().getY() * destRect.h;

	SDL_Point anchor = { _transform->getAnchor().getX() * destRect.w, _transform->getAnchor().getY() * destRect.h };

	_texture->renderFrame(destRect, _fil, _col, _transform->getRotation(), anchor, (_flip ? SDL_FLIP_HORIZONTAL : SDL_FLIP_NONE));
}

void FrameComponent::flip()
{
	_flip = true;
}

void FrameComponent::unFlip()
{
	_flip = false;
}