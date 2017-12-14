#include "stdafx.h"
#include "BillBoardComponent.h"
#include "Animation.h"
#include "Moteur.h"
#include "MoteurWindows.h"

void PM3D::BillBoardComponent::Draw()
{
	if (anim != nullptr) {
		anim->update(CMoteur<CMoteurWindows, CDispositifD3D11>::GetInstance().GetTempsEcoule());
	}
	b->Draw(GetOwner(), index++, GetOwner()->GetBreakingSpeed(), theta);
	if (index == b->GetFramesAmount()) index = 0;
}
