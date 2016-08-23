#include "text.h"
#include "graphics.h"
#include "globals.h"

#include <stdio.h>

Text::Text() {}

Text::Text(Graphics &graphics, float posX, float posY, const std::string & message,	
	int fontSize, Uint8 r, Uint8 g, Uint8 b, const std::string & fontStyle,	bool wrapped, int breakWidth, int width, int height) :
	_x(posX),
	_y(posY),
	_wrapped(wrapped),
	_breakWidth(breakWidth),
	_width(width),
	_height(height)
{
	this->_color = { r, g, b };
	this->_messageRect.x = posX;
	this->_messageRect.y = posY;

	if ((this->_font = TTF_OpenFont(fontStyle.c_str(), fontSize)) == NULL) {
		printf("\nError: Calling TTF_OpenFont ... %s\n", TTF_GetError());
	}

	if (wrapped == true) {
		if ((this->_surfaceMessage = TTF_RenderText_Blended_Wrapped(this->_font, message.c_str(), this->_color, breakWidth)) == NULL) {
			printf("\nError: Calling TTF_RenderText_Blended_Wrapped ... %s\n", SDL_GetError());
		}
		this->_messageRect.w = width;
		this->_messageRect.h = height;
	}
	else {
		if ((this->_surfaceMessage = TTF_RenderText_Blended(this->_font, message.c_str(), this->_color)) == NULL) {
			printf("\nError: Calling TTF_RenderText_Blended ... %s\n", SDL_GetError());
		}
		if (TTF_SizeText(this->_font, message.c_str(), &this->_messageRect.w, &this->_messageRect.h)) {
			// perhaps print the current TTF_GetError(), the string can't be rendered...
			printf("\nError: Calling TTF_SizeText ... %s\n", TTF_GetError());
		}
	}

	if ((this->_message = SDL_CreateTextureFromSurface(graphics.getRenderer(), _surfaceMessage)) == NULL) {
		printf("\nError: Calling SDL_CreateTextureFromSurface ... %s\n", SDL_GetError());
	}
}

Text::~Text() {}

void Text::draw(Graphics &graphics) {
	//TTF_SizeText(this->_font, this->_msg, &this->_messageRect.w, &this->_messageRect.h);
	graphics.blitSurface(this->_message, NULL, &this->_messageRect);
}

void Text::draw(Graphics &graphics, int x, int y) {
	SDL_Rect destinationRectangle = { x, y, this->_messageRect.w, this->_messageRect.h };

	if (this->_message == NULL) {
		printf("\nERROR: Message is null\n");
		exit(1);
	}

	graphics.blitSurface(this->_message, NULL, &destinationRectangle);
}

void Text::update(Graphics & graphics, const std::string & message) {
	this->setMessage(message, this->_color);

	if (this->_message != NULL) {
		SDL_DestroyTexture(this->_message);
		this->_message = NULL;
	}

	if ((this->_message = SDL_CreateTextureFromSurface(graphics.getRenderer(), _surfaceMessage)) == NULL) {
		printf("\nError: Calling SDL_CreateTextureFromSurface ... %s\n", SDL_GetError());
	}

	if (this->_wrapped != true) {
		if (TTF_SizeText(this->_font, message.c_str(), &this->_messageRect.w, &this->_messageRect.h)) {
			printf("\nError: Calling TTF_SizeText ... %s\n", TTF_GetError());
		}
	}
}

void Text::update(Graphics & graphics, const std::string & message, SDL_Color color) {
	this->_color = color;
	this->update(graphics, message);
}

void Text::reset() {
	this->setMessage(" ");
}

void Text::setX(float x) {
	this->_x = x;
}

void Text::setY(float y) {
	this->_y = y;
}

void Text::setMessage(const std::string & message) {
	if (this->_surfaceMessage != NULL) {
		SDL_FreeSurface(this->_surfaceMessage);
		this->_surfaceMessage = NULL;
	}

	if (this->_wrapped != true) {
		if ((this->_surfaceMessage = TTF_RenderText_Blended(this->_font, (message == "" ? message + " " : message).c_str(), this->_color)) == NULL) {
			printf("\nError: Calling TTF_RenderText_Blended ... %s\n", SDL_GetError());
		}
	}
	else {
		if ((this->_surfaceMessage = TTF_RenderText_Blended_Wrapped(this->_font, (message == "" ? message + " " : message).c_str(), this->_color, this->_breakWidth)) == NULL) {
			printf("\nError: Calling TTF_RenderText_Blended_Wrapped ... %s\n", SDL_GetError());
		}
	}
}

void Text::setMessage(const std::string & message, SDL_Color color) {
	this->_color = color;
	this->setMessage(message);
}