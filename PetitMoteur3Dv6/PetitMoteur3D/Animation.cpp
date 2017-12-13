#include "stdafx.h"
#include "Animation.h"
#include <fstream>


Animation::Animation(TextureSprite* texture)
	: pSprite(texture)
	, mFrameSize()
	, mNumFrames(0)
	, mCurrentFrame(0)
	, mDuration{}
	, mElapsedTime{}
	, mRepeat(false)
{}

void Animation::setFrameSize(int mFrameSizeX, int mFrameSizeY)
{
	mFrameSize[0] = mFrameSizeX / pSprite->GetDimension()[0];
	mFrameSize[1] = mFrameSizeY / pSprite->GetDimension()[1];
}

Animation::FrameSize Animation::getFrameSize() const
{
	return mFrameSize;
}

void Animation::setNumFrames(std::size_t numFrames)
{
	mNumFrames = numFrames;
}

std::size_t Animation::getNumFrames() const
{
	return mNumFrames;
}

void Animation::setDuration(Time duration)
{
	mDuration = duration;
}

Animation::Time Animation::getDuration() const
{
	return mDuration;
}

void Animation::setRepeating(bool flag)
{
	mRepeat = flag;
}

bool Animation::isRepeating() const
{
	return mRepeat;
}

void Animation::restart()
{
	mCurrentFrame = 0;
}

bool Animation::isFinished() const
{
	return mCurrentFrame >= mNumFrames;
}


using namespace PM3D;
void Animation::update(Time dt)
{
	Time timePerFrame = mDuration / static_cast<float>(mNumFrames);
	mElapsedTime += dt;

	std::array<float, 2> textureSize = { 1.f,1.f };//pSprite->GetDimension();
	TextureSprite::TextureRectangle textureRect = pSprite->GetTextureRect();
	textureRect.width = mFrameSize[0];
	textureRect.height = mFrameSize[1];


	if (mCurrentFrame == 0)
		textureRect = { 0, 0, mFrameSize[0], mFrameSize[1] };

	// While we have a frame to process
	while (mElapsedTime >= timePerFrame && (mCurrentFrame <= mNumFrames || mRepeat))
	{
		// Move the texture rect left
		textureRect.left += textureRect.width;

		// If we reach the end of the texture
		if (textureRect.left + textureRect.width > textureSize[0])
		{
			// Move it down one line
			textureRect.left = 0;
			textureRect.top += textureRect.height;
		}

		// And progress to next frame
		mElapsedTime -= timePerFrame;
		if (mRepeat)
		{
			mCurrentFrame = (mCurrentFrame + 1) % mNumFrames;

			if (mCurrentFrame == 0)
				textureRect = { 0, 0, mFrameSize[0], mFrameSize[1] };
		}
		else
		{
			mCurrentFrame++;
		}
	}

	textureRect.width += textureRect.left;
	textureRect.height += textureRect.top;

	pSprite->SetTextureRect(textureRect);
}

void Animation::Draw() const
{
	pSprite->Draw();
}
