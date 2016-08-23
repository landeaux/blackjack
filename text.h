#ifndef TEXT_H
#define TEXT_H

#include <SDL.h>
#include <SDL_ttf.h>

#include <string>

#include "globals.h"

class Graphics;

/* Text class
* Holds all information for individual Texts
*/

class Text {
public:
	Text();
	Text(Graphics &graphics, float posX, float posY, const std::string & message, 
		int fontSize = 24, Uint8 r = 0, Uint8 g = 0, Uint8 b = 0, const std::string & fontStyle = "content/fonts/OpenSans-Regular.ttf", 
		bool wrapped = false, int breakWidth = globals::SCREEN_WIDTH, int width = 0, int height = 0);
	virtual ~Text();
	void draw(Graphics & graphics);
	void draw(Graphics & graphics, int x, int y);
	void update(Graphics & graphics, const std::string & message);
	void update(Graphics & graphics, const std::string & message, SDL_Color color);
	void reset();

	const inline float getX() const { return this->_x; }
	const inline float getY() const { return this->_y; }
	const inline SDL_Rect getMessageRect() const { return this->_messageRect; }

	void setX(float x);
	void setY(float y);
	void setMessage(const std::string & message);
	void setMessage(const std::string & message, SDL_Color color);

protected:
	TTF_Font* _font;
	SDL_Color _color;
	SDL_Rect _messageRect;
	SDL_Surface* _surfaceMessage;
	SDL_Texture* _message;

	float _x, _y;
	bool _wrapped;
	int _breakWidth, _width, _height;

private:
};


#endif