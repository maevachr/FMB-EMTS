#pragma once
#include <array>

#include "SpriteManager.h"

using namespace PM3D;

class Animation
{
public:
	using Time = float;
	using FrameSize = std::array<float, 2>;

public:
	 						Animation(TextureSprite* texture);

	void 					setFrameSize(int mFrameSizeX, int mFrameSizeY);
	FrameSize		 		getFrameSize() const;

	void 					setNumFrames(std::size_t numFrames);
	std::size_t 			getNumFrames() const;

	void 					setDuration(Time duration);
	Time 					getDuration() const;

	void 					setRepeating(bool);
	bool 					isRepeating() const;

	void 					restart();
	bool 					isFinished() const;

	void 					update(Time dt);


private:
	void 					Draw() const;


private:
	PM3D::TextureSprite*	pSprite;
	FrameSize 				mFrameSize;
	std::size_t 			mNumFrames;
	std::size_t 			mCurrentFrame;
	Time 					mDuration;
	Time					mElapsedTime;
	bool 					mRepeat;
};