#include "stdafx.h"
#include "PauseState.h"
#include "SoundManager.h"
#include "BlackBoard.h"

using namespace PM3D;

PauseState::PauseState(StateStack& stack, Context context)
	: State(stack, context)
	, background{ TextureSprite{ "pause.dds", -400, -78, 1851, 925, context.pDispositif } }
	, score { SpriteManager::GetInstance().GetFontTitle(), 577,380 , 180, 70, context.pDispositif}
	, time { SpriteManager::GetInstance().GetFontTitle(),302 ,380 , 160, 70, context.pDispositif }
{
	EcrireScore();
	EcrireTime();
	SoundManager::GetInstance().PauseAllGameSounds();
}

void PauseState::draw()
{
	mContext.pDispositif->DesactiverZBuffer();
	mContext.pDispositif->DesactiverCulling();
	background.Draw();
	score.Draw();
	time.Draw();
	/*mContext.pDispositif->ActiverZBuffer();
	mContext.pDispositif->ActiverCulling();*/
}

bool PauseState::update(Time)
{
	return false;
}

void PauseState::ProcessInput()
{
	auto pGestionnaireDeSaisie = InputManager::GetInstance().GetDIManipulateur();

	//Retour Game
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_ESCAPE)) {
		SoundManager::GetInstance().ContinueAllGameSounds();
		requestStackPop();
	}

	//Quit Game
	if (pGestionnaireDeSaisie->ToucheAppuyee(DIK_Q)) {
		//CMoteur<CMoteurWindows, CDispositifD3D11>::GetInstance().StopGame();
		PostQuitMessage(0);
	}
}


void PauseState::EcrireScore()
{
	int scoreVal = BlackBoard::GetInstance().GetScore();
	string s = to_string(scoreVal / 10000 % 10) + to_string(scoreVal / 1000 % 10) + to_string(scoreVal / 100 % 10) + to_string(scoreVal / 10 % 10) + to_string(scoreVal % 10);
	score.Ecrire({ s.begin(), s.end() }, 55);
}

void PauseState::EcrireTime()
{
	float timeVal = BlackBoard::GetInstance().GetChrono();
	int unit = static_cast<int>(timeVal) / 60;
	int decimal1 = static_cast<int>(timeVal) % 60 / 10;
	int decimal2 = static_cast<int>(timeVal) % 60 % 10;

	string s = to_string(unit) + ":" + to_string(decimal1) + to_string(decimal2);
	time.Ecrire({ s.begin(), s.end() }, 55);
}
