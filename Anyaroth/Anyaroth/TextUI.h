#pragma once
#include "UIElement.h"
#include <string>
#include "Vector2D.h"
#include "Texture.h"

class TextUI : public UIElement
{
	private:
		SDL_Rect _destRect;

	protected:
		Font* _font = nullptr;
		uint _fontSize;
		string _text = "";
		SDL_Color _color;

	public:
		TextUI() : UIElement(nullptr) {};
		TextUI(Game* game);
		TextUI(Game* game, string text, Font* font, uint fontSize = 12, int xPos = 0, int yPos = 0, SDL_Color color = { 0, 0, 0, 255});
		~TextUI() {};

		virtual void render() const;
		virtual void update() {};
		virtual void handleEvent(const SDL_Event& event) {};

		inline string getText() { return _text; }
		void setText(string text);

		void setPosition(int x, int y);
		void setSize(int w, int h);
		Vector2D getPosition() { return Vector2D(_destRect.x, _destRect.y); }
		Vector2D getSize() { return Vector2D(_destRect.w, _destRect.h); }

		void setFont(string fontName, uint fontSize);
		
		inline void setColor(const SDL_Color& color) { _color = color; }
		inline SDL_Color getColor() { return _color; }

		inline int getW() { return _destRect.w; };
		inline int getH() { return _destRect.h; };
		inline int getX() { return _destRect.x; };
		inline int getY() { return _destRect.y; };
};

