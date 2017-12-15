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
	GameObject* go = GetOwner();
	if (go->GetTypeId() == "MonsterTruckGo") {
		if (go->GetPlayNitro()) {
			if (b->GetName() == "nitro" || b->GetName() == "nitro2") {
				b->Draw(go, index++, go->GetBreakingSpeed(), theta);
			}
		}
		else {
			if (b->GetName() == "smoke" || b->GetName() == "smoke2") {
				b->Draw(go, index++, go->GetBreakingSpeed(), theta);
			}
		}
	}
	else {
		b->Draw(go, index++, go->GetBreakingSpeed(), theta);
	}

	if (index == b->GetFramesAmount()) index = 0;
}
