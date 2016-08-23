#include "controlsSprites.h"

#include "graphics.h"

namespace {
	const int CTRLS_SPRITE_WIDTH = 247;
	const int CTRLS_SPRITE_HEIGHT = 354;
	const int SPACING = 17;
	const int CTRLS_X_POS = globals::SCREEN_WIDTH - CTRLS_SPRITE_WIDTH - SPACING;
	const int CTRLS_Y_POS = 87;
}

ControlsSprites::ControlsSprites() {}

ControlsSprites::ControlsSprites(Graphics & graphics, float posX, float posY) {
	this->_roundInProgress = false;

																	//int sourceX, int sourceY, int width, int height, float posX, float posY
	this->_heading = Sprite(graphics, "content/sprites/controls.png", 0, 0, CTRLS_SPRITE_WIDTH, 15, CTRLS_X_POS, CTRLS_Y_POS);
	this->_inRoundCtrls = Sprite(graphics, "content/sprites/controls.png", 0, 25, CTRLS_SPRITE_WIDTH, 121, CTRLS_X_POS,
		this->_heading.getY() + this->_heading.getSourceRect().h + 17);
	this->_endRoundCtrls = Sprite(graphics, "content/sprites/controls.png", 0, 146, CTRLS_SPRITE_WIDTH, 191, CTRLS_X_POS,
		this->_heading.getY() + this->_heading.getSourceRect().h + 17);
	this->_exitCtrl = Sprite(graphics, "content/sprites/controls.png", 0, 337, CTRLS_SPRITE_WIDTH, 17, CTRLS_X_POS,
		this->_endRoundCtrls.getY() + this->_endRoundCtrls.getSourceRect().h);
}

void ControlsSprites::update(bool roundInProgress) {
	this->_roundInProgress = roundInProgress;
}

void ControlsSprites::draw(Graphics & graphics) {
	this->_heading.draw(graphics, this->_heading.getX(), this->_heading.getY());
	if (this->_roundInProgress == true) {
		int exitCtrlYPos = this->_inRoundCtrls.getY() + this->_inRoundCtrls.getSourceRect().h;
		this->_inRoundCtrls.draw(graphics, this->_inRoundCtrls.getX(), this->_inRoundCtrls.getY());
		this->_exitCtrl.draw(graphics, this->_exitCtrl.getX(), exitCtrlYPos);
	}
	else {
		int exitCtrlYPos = this->_endRoundCtrls.getY() + this->_endRoundCtrls.getSourceRect().h;
		this->_endRoundCtrls.draw(graphics, this->_endRoundCtrls.getX(), this->_endRoundCtrls.getY());
		this->_exitCtrl.draw(graphics, this->_exitCtrl.getX(), exitCtrlYPos);
	}
}