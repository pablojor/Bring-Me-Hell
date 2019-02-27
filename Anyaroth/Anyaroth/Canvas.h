#pragma once
#include "UIElement.h"

class Canvas
{
	private:
		std::vector<UIElement*> _elements;
	public:
		Canvas() {};
		virtual ~Canvas() {};

		virtual void render() const;
		virtual void update();
		virtual void handleEvent(const SDL_Event& event);
	
		inline virtual void addUIElement(UIElement* elem) { _elements.push_back(elem); }
};
