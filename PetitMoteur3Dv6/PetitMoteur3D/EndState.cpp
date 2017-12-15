#include "stdafx.h"
#include "EndState.h"

#include "BlackBoard.h"


using namespace PM3D;

EndState::EndState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "end.dds", -400, -78, 1851, 925, context.pDispositif } }
	, score{ SpriteManager::GetInstance().GetFontTitle(), 405,275 , 230, 80, context.pDispositif }
	, rank1{ SpriteManager::GetInstance().GetFontTitle(), 500,402 , 180, 70, context.pDispositif }
	, rank2{ SpriteManager::GetInstance().GetFontTitle(), 500,442 , 180, 70, context.pDispositif }
	, rank3{ SpriteManager::GetInstance().GetFontTitle(), 500,482 , 180, 70, context.pDispositif }
{
	EcrireScore();
	EcrireRank();
	SoundManager::GetInstance().PauseSound(SoundManager::ENGINE);
}

void EndState::draw()
{
	mContext.pDispositif->DesactiverZBuffer();
	mContext.pDispositif->DesactiverCulling();
	background.Draw();
	score.Draw();
	rank1.Draw();
	rank2.Draw();
	rank3.Draw();
}

bool EndState::update(Time)
{
	return false;
}

void EndState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_Q)) {
		PostQuitMessage(0);
	}
}

void EndState::EcrireScore()
{
	int scoreVal = BlackBoard::GetInstance().GetScore();
	string s = to_string(scoreVal / 10000 % 10) + to_string(scoreVal / 1000 % 10) + to_string(scoreVal / 100 % 10) + to_string(scoreVal / 10 % 10) + to_string(scoreVal % 10);
	score.Ecrire({ s.begin(), s.end() }, 70);
}

void EndState::EcrireRank()
{
	int rank = BlackBoard::GetInstance().GetRank1();
	string s = to_string(rank / 10 % 10) + to_string(rank % 10);
	rank1.Ecrire({ s.begin(), s.end() }, 26);

	rank = BlackBoard::GetInstance().GetRank2();
	s = to_string(rank / 10 % 10) + to_string(rank % 10);
	rank2.Ecrire({ s.begin(), s.end() }, 26);

	rank = BlackBoard::GetInstance().GetRank3();
	s = to_string(rank / 10 % 10) + to_string(rank % 10);
	rank3.Ecrire({ s.begin(), s.end() }, 26);
}
