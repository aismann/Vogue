// ******************************************************************************
// Filename:    VogueCamera.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueGame.h"

// Camera controls
void VogueGame::UpdateCamera(float dt)
{
	m_pGameCamera->SetPosition(m_pGameCamera->GetFakePosition());
}

void VogueGame::UpdateCameraZoom(float dt)
{
	// Make sure we gradually move inwards/outwards
	float camDiff = fabs(m_cameraDistance - m_maxCameraDistance);
	float changeAmount = 0.0f;
	if (m_cameraDistance < m_maxCameraDistance)
	{
		changeAmount = camDiff * dt;
	}
	else if (m_cameraDistance >= m_maxCameraDistance)
	{
		changeAmount = -camDiff * dt;
	}

	m_cameraDistance += changeAmount;
	m_pGameCamera->Zoom(changeAmount, true);
}
