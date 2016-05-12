// ******************************************************************************
// Filename:    VogueControls.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueGame.h"

// Controls
void VogueGame::UpdateControls(float dt)
{
	if (m_gamepadMovement == false)
	{
		UpdateKeyboardControls(dt);
		UpdateMouseControls(dt);
	}

	if (m_keyboardMovement == false)
	{
		if (m_pVogueWindow->IsJoyStickConnected(0))
		{
			UpdateGamePadControls(dt);
		}
	}
}

void VogueGame::UpdateKeyboardControls(float dt)
{
	GameMode gameMode = GetGameMode();
}

void VogueGame::UpdateMouseControls(float dt)
{
	GameMode gameMode = GetGameMode();
}

void VogueGame::UpdateGamePadControls(float dt)
{
	JoystickCameraZoom(dt);
}
