#include "stdafx.h"
#include "BillBoardComponentManager.h"
#include "BillBoardComponent.h"

void PM3D::BillBoardComponentManager::Draw()
{
	std::for_each(std::begin(bComponents), std::end(bComponents), [](BillBoardComponent* bc)
	{
		bc->Draw();
	});
}
