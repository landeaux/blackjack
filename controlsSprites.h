#ifndef CONTROLSSPRITES_H
#define CONTROLSSPRITES_H

#include "sprite.h"

class Graphics;

class ControlsSprites {
public:
	ControlsSprites();
	ControlsSprites(Graphics & graphics, float posX = 0, float posY = 0);
	void update(bool roundInProgress);
	void draw(Graphics & graphics);

private:

	// Health Sprites
	Sprite _heading;
	Sprite _inRoundCtrls;
	Sprite _endRoundCtrls;
	Sprite _exitCtrl;

	bool _roundInProgress;
};

#endif