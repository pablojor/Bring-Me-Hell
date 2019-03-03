#pragma once
#include "UIElement.h"
#include "Texture.h"

class ImageUI :	public UIElement
{
	protected:
		Texture* _image;
		SDL_Rect _destRect;
		SDL_Rect _clipArea;
	public:
		ImageUI() : UIElement(nullptr) {};
		ImageUI(Game* game, Texture* image, int xPos = 0, int yPos = 0);
		~ImageUI() {};

		virtual void render() const;
		virtual void update() {}
		virtual void handleEvent(const SDL_Event& event) {}

		inline void setImage(Texture* image) { _image = image; };
		inline Texture* getImage() const { return _image; }

		void setPosition(int x, int y);
		void setSize(int w, int h);
		void setClippingAreaSize(float w, float h);		//Solo el alto y ancho porque la posicion tiene que ser la misma (Valor de 0 a 1)

		inline int getW() const { return _destRect.w; };
		inline int getH() const { return _destRect.h; };
		inline int getX() const { return _destRect.x; };
		inline int getY() const { return _destRect.y; };
};

