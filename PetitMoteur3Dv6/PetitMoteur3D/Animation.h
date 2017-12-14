#pragma once
#include <array>

#include "SpriteManager.h"
#include "BillBoardManager.h"

using namespace PM3D;

class Animation
{
public:
	using Time = float;
	using FrameSize = std::array<float, 2>;

public:
	Animation();

	virtual void 			setFrameSize(float mFrameSizeX, float mFrameSizeY) = 0;
	FrameSize		 		getFrameSize() const;

	void 					setNumFrames(std::size_t numFrames);
	std::size_t 			getNumFrames() const;

	void 					setDuration(Time duration);
	Time 					getDuration() const;

	void 					setRepeating(bool);
	bool 					isRepeating() const;

	void 					restart();
	bool 					isFinished() const;

	size_t GetFrame() { return mCurrentFrame; }

public:
	virtual void 			update(Time dt) = 0;
	virtual void 			Draw() const = 0;

protected:
	
	FrameSize 				mFrameSize;
	std::size_t 			mNumFrames;
	std::size_t 			mCurrentFrame;
	Time 					mDuration;
	Time					mElapsedTime;
	bool 					mRepeat;
};

class AnimationSprite : public Animation
{
public:
	AnimationSprite(TextureSprite* texture);
private:
	PM3D::TextureSprite*	pSprite;
public:
	void 					setFrameSize(float mFrameSizeX, float mFrameSizeY) override;
	void 					update(Time dt) override;
	void 					Draw() const override;
};

class AnimationBillBoard : public Animation
{
public:
	AnimationBillBoard(BillBoard* bb);
private:
	BillBoard*		pBillBoard;
public:
	void 					setFrameSize(float mFrameSizeX, float mFrameSizeY) override;
	void 					update(Time dt) override;
	void 					Draw() const override {}
};

