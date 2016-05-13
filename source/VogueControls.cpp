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

	if (gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
	{
		// Keyboard camera movements
		if (m_bKeyboardForward)
		{
			m_pGameCamera->Fly(20.0f * dt, true);
		}
		if (m_bKeyboardBackward)
		{
			m_pGameCamera->Fly(-20.0f * dt, true);
		}
		if (m_bKeyboardStrafeLeft)
		{
			m_pGameCamera->Strafe(-20.0f * dt, true);
		}
		if (m_bKeyboardStrafeRight)
		{
			m_pGameCamera->Strafe(20.0f * dt, true);
		}
		if (m_bKeyboardUp)
		{
			m_pGameCamera->Levitate(20.0f * dt, true);
		}
		if (m_bKeyboardDown)
		{
			m_pGameCamera->Levitate(-20.0f * dt, true);
		}
	}
}

void VogueGame::UpdateMouseControls(float dt)
{
	GameMode gameMode = GetGameMode();

	if (gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
	{
		if (m_bCameraRotate)
		{
			MouseCameraRotate();
		}
	}
}

void VogueGame::UpdateGamePadControls(float dt)
{
	JoystickCameraZoom(dt);
}
