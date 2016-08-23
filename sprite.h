#ifndef SPRITE_H
#define SPRITE_H

#include <SDL.h>

#include <string>

#include "globals.h"

class Graphics;

/* Sprite class
* Holds all information for individual sprites
*/

class Sprite {
public:
	Sprite();
	Sprite(Graphics &graphics, const std::string &filePath, int sourceX, int sourceY, int width, int height,
		float posX = 0, float posY = 0);
	//Sprite(const Sprite & sprite);
	virtual ~Sprite();
	virtual void update();
	void draw(Graphics &graphics, int x, int y);

	const inline float getX() const { return this->_x; }
	const inline float getY() const { return this->_y; }
	const inline SDL_Rect getSourceRect() const { return this->_sourceRect; }

	void setSourceRectX(int value);
	void setSourceRectY(int value);
	void setSourceRectW(int value);
	void setSourceRectH(int value);
	void setX(float x);
	void setY(float y);

protected:
	SDL_Rect _sourceRect;
	SDL_Texture* _spriteSheet;

	float _x, _y;
	std::string _filePath;

private:
};


#endif
