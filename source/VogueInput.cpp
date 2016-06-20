// ******************************************************************************
// Filename:    VogueInput.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
using namespace std;

#include "utils/Random.h"
#include "VogueGame.h"


// Input callbacks
void KeyCallback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			VogueGame::GetInstance()->KeyPressed(key, scancode, mods);
			
			break;
		}
		case GLFW_RELEASE:
		{
			VogueGame::GetInstance()->KeyReleased(key, scancode, mods);
			break;
		}
		case GLFW_REPEAT:
		{
			break;
		}
	}
}

void CharacterCallback(GLFWwindow* window, unsigned int keyCode)
{
	VogueGame::GetInstance()->CharacterEntered(keyCode);
}

void MouseButtonCallback(GLFWwindow* window, int button, int action, int mods)
{
	switch (action)
	{
		case GLFW_PRESS:
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
				VogueGame::GetInstance()->MouseLeftPressed();
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
				VogueGame::GetInstance()->MouseRightPressed();
			if (button == GLFW_MOUSE_BUTTON_MIDDLE)
				VogueGame::GetInstance()->MouseMiddlePressed();

			break;
		}
		case GLFW_RELEASE:
		{
			if (button == GLFW_MOUSE_BUTTON_LEFT)
				VogueGame::GetInstance()->MouseLeftReleased();
			if (button == GLFW_MOUSE_BUTTON_RIGHT)
				VogueGame::GetInstance()->MouseRightReleased();
			if (button == GLFW_MOUSE_BUTTON_MIDDLE)
				VogueGame::GetInstance()->MouseMiddleReleased();

			break;
		}
	}
}

void MouseScrollCallback(GLFWwindow* window, double x, double y)
{
	VogueGame::GetInstance()->MouseScroll(x, y);
}

// Input
void VogueGame::KeyPressed(int key, int scancode, int mods)
{
	m_pGUI->KeyPressed(key, mods);

	if (m_pGUI->IsKeyboardInteractingWithGUIComponent() && key != GLFW_KEY_ESCAPE)
	{
		return;  // For textbox entry
	}

	switch (key)
	{
		// Movement
		case GLFW_KEY_W:
		{
			m_bKeyboardForward = true;
			break;
		}
		case GLFW_KEY_S:
		{
			m_bKeyboardBackward = true;
			break;
		}
		case GLFW_KEY_A:
		{
			m_bKeyboardLeft = true;
			m_bKeyboardStrafeLeft = true;
			break;
		}
		case GLFW_KEY_D:
		{
			m_bKeyboardRight = true;
			m_bKeyboardStrafeRight = true;
			break;
		}
		case GLFW_KEY_F:
		{
			m_bKeyboardUp = true;
			break;
		}
		case GLFW_KEY_V:
		{
			m_bKeyboardDown = true;
			break;
		}
		case GLFW_KEY_SPACE:
		{
			m_bKeyboardSpace = true;
			break;
		}
		case GLFW_KEY_ESCAPE:
		{
			m_bKeyboardMenu = true;
			break;
		}		
	}
}

void VogueGame::KeyReleased(int key, int scancode, int mods)
{
	m_pGUI->KeyReleased(key, mods);

	if (m_pGUI->IsKeyboardInteractingWithGUIComponent() && key != GLFW_KEY_ESCAPE)
	{
		return;  // For textbox entry
	}

	switch (key)
	{
		// Movement
		case GLFW_KEY_W:
		{
			m_bKeyboardForward = false;
			break;
		}
		case GLFW_KEY_S:
		{
			m_bKeyboardBackward = false;
			break;
		}
		case GLFW_KEY_A:
		{
			m_bKeyboardLeft = false;
			m_bKeyboardStrafeLeft = false;
			break;
		}
		case GLFW_KEY_D:
		{
			m_bKeyboardRight = false;
			m_bKeyboardStrafeRight = false;
			break;
		}
		case GLFW_KEY_F:
		{
			m_bKeyboardUp = false;
			break;
		}
		case GLFW_KEY_V:
		{
			m_bKeyboardDown = false;
			break;
		}
		case GLFW_KEY_SPACE:
		{
			m_bKeyboardSpace = false;
			break;
		}
		case GLFW_KEY_ESCAPE:
		{
			m_bKeyboardMenu = false;
			break;
		}

		case GLFW_KEY_L:
		{
			long randomSeed = GetRandomNumber(0, INT_MAX);
			m_pPlayer->RandomizeParts(randomSeed);
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_U:
		{
			m_pPlayer->ModifyHair();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_I:
		{
			m_pPlayer->ModifyNose();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_O:
		{
			m_pPlayer->ModifyEars();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_H:
		{
			m_pPlayer->ModifyHead();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_J:
		{
			m_pPlayer->ModifyFacialHair();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_K:
		{
			m_pPlayer->ModifyEyes();
			break;
		}
		case GLFW_KEY_B:
		{
			m_pPlayer->ModifySkinColour();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_M:
		{
			m_pPlayer->SwapHairColours();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
		case GLFW_KEY_N:
		{
			m_pPlayer->ModifyHairColour();
			m_pPlayer->UpdateDefaults();
			m_pPlayer->SetColourModifiers();
			break;
		}
	}
}

void VogueGame::CharacterEntered(int keyCode)
{
	m_pGUI->CharacterEntered(keyCode);
}

void VogueGame::MouseLeftPressed()
{
	if (IsCursorOn())
	{
		m_pGUI->MousePressed(MOUSE_BUTTON1);
	}

	if (IsCursorOn() == false || !m_pGUI->IsMouseInteractingWithGUIComponent(false))
	{
		m_currentX = m_pVogueWindow->GetCursorX();
		m_currentY = m_pVogueWindow->GetCursorY();
		m_pressedX = m_currentX;
		m_pressedY = m_currentY;

		if (m_gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
		{
			// Turn cursor off
			if (IsCursorOn() == true)
			{
				TurnCursorOff(false);
			}

			m_bCameraRotate = true;
		}
	}
}

void VogueGame::MouseLeftReleased()
{
	if (IsCursorOn())
	{
		m_pGUI->MouseReleased(MOUSE_BUTTON1);
	}

	if (m_gameMode == GameMode_Debug || m_cameraMode == CameraMode_Debug)
	{
		if (!m_pGUI->IsMouseInteractingWithGUIComponent(false))
		{
			// Turn cursor on
			if (IsCursorOn() == false)
			{
				TurnCursorOn(true, false);
			}
		}

		m_bCameraRotate = false;
	}
}

void VogueGame::MouseRightPressed()
{
	if (IsCursorOn())
	{
		m_pGUI->MousePressed(MOUSE_BUTTON2);
	}

	if (IsCursorOn() == false || !m_pGUI->IsMouseInteractingWithGUIComponent(false))
	{
		m_currentX = m_pVogueWindow->GetCursorX();
		m_currentY = m_pVogueWindow->GetCursorY();
		m_pressedX = m_currentX;
		m_pressedY = m_currentY;
	}
}

void VogueGame::MouseRightReleased()
{
	if (IsCursorOn())
	{
		m_pGUI->MouseReleased(MOUSE_BUTTON2);
	}
}

void VogueGame::MouseMiddlePressed()
{
	if (IsCursorOn())
	{
		m_pGUI->MousePressed(MOUSE_BUTTON3);
	}
}

void VogueGame::MouseMiddleReleased()
{
	if (IsCursorOn())
	{
		m_pGUI->MouseReleased(MOUSE_BUTTON3);
	}
}

void VogueGame::MouseScroll(double x, double y)
{
	GameMode gameMode = GetGameMode();
}

void VogueGame::WrapCameraZoomValue()
{
	float minAmount = 0.5f;
	float maxAmount = 15.0f;

	// Camera rotation modes
	if (m_gameMode == GameMode_Game && m_cameraMode == CameraMode_MouseRotate)
	{
		minAmount = 1.0f;
		maxAmount = 15.0f;
	}

	if (m_maxCameraDistance <= minAmount)
	{
		m_maxCameraDistance = minAmount;
	}

	if (m_maxCameraDistance >= maxAmount)
	{
		m_maxCameraDistance = maxAmount;
	}
}

// Mouse controls
void VogueGame::MouseCameraRotate()
{
	int x = m_pVogueWindow->GetCursorX();
	int y = m_pVogueWindow->GetCursorY();

	float changeX;
	float changeY;

	// The mouse hasn't moved so just return
	if ((m_currentX == x) && (m_currentY == y))
	{
		return;
	}

	// Calculate and scale down the change in position
	changeX = (x - m_currentX) / 5.0f;
	changeY = (y - m_currentY) / 5.0f;

	// Upside down
	if (m_pGameCamera->GetUp().y < 0.0f)
	{
		changeX = -changeX;
	}

	// Limit the rotation, so we can't go 'over' or 'under' the player with our rotations
	vec3 cameraFacing = m_pGameCamera->GetFacing();
	float dotResult = acos(dot(cameraFacing, vec3(0.0f, 1.0f, 0.0f)));
	float rotationDegrees = RadToDeg(dotResult) - 90.0f;
	float limitAngle = 75.0f;
	if ((rotationDegrees > limitAngle && changeY < 0.0f) || (rotationDegrees < -limitAngle && changeY > 0.0f))
	{
		changeY = 0.0f;
	}

	m_pGameCamera->RotateAroundPoint(changeY*0.75f, 0.0f, 0.0f, true);
	m_pGameCamera->RotateAroundPointY(-changeX*0.75f, true);

	m_currentX = x;
	m_currentY = y;
}

// Joystick controls
void VogueGame::JoystickCameraMove(float dt)
{
	float axisX = m_pVogueWindow->GetJoystickAxisValue(0, 0);
	float axisY = m_pVogueWindow->GetJoystickAxisValue(0, 1);

	// Dead zones
	if (fabs(axisX) < m_pVogueWindow->GetJoystickAnalogDeadZone())
	{
		axisX = 0.0f;
	}
	if (fabs(axisY) < m_pVogueWindow->GetJoystickAnalogDeadZone())
	{
		axisY = 0.0f;
	}

	float changeX = axisX * 10.0f * dt;
	float changeY = axisY * 10.0f * dt;

	m_pGameCamera->Fly(-changeY);
	m_pGameCamera->Strafe(changeX);
}

void VogueGame::JoystickCameraRotate(float dt)
{
	float axisX = m_pVogueWindow->GetJoystickAxisValue(0, 4);
	float axisY = m_pVogueWindow->GetJoystickAxisValue(0, 3);

	// Dead zones
	if (fabs(axisX) < m_pVogueWindow->GetJoystickAnalogDeadZone())
	{
		axisX = 0.0f;
	}
	if (fabs(axisY) < m_pVogueWindow->GetJoystickAnalogDeadZone())
	{
		axisY = 0.0f;
	}

	float changeX = axisX * 150.0f * dt;
	float changeY = axisY * 150.0f * dt;

	// Upside down
	if (m_pGameCamera->GetUp().y < 0.0f)
	{
		changeX = -changeX;
	}

	// Limit the rotation, so we can't go 'over' or 'under' the player with our rotations
	vec3 cameraFacing = m_pGameCamera->GetFacing();
	float dotResult = acos(dot(cameraFacing, vec3(0.0f, 1.0f, 0.0f)));
	float rotationDegrees = RadToDeg(dotResult) - 90.0f;
	float limitAngle = 75.0f;
	if ((rotationDegrees > limitAngle && changeY < 0.0f) || (rotationDegrees < -limitAngle && changeY > 0.0f))
	{
		changeY = 0.0f;
	}

	m_pGameCamera->RotateAroundPoint(changeY, 0.0f, 0.0f, true);
	m_pGameCamera->RotateAroundPointY(-changeX, true);
}

void VogueGame::JoystickCameraZoom(float dt)
{
	bool zoomOut = m_pVogueWindow->GetJoystickButton(0, 4);
	bool zoomIn = m_pVogueWindow->GetJoystickButton(0, 5);

	float zoomAmount = 0.0f;
	if (zoomIn)
	{
		zoomAmount = 10.0f;
	}
	if (zoomOut)
	{
		zoomAmount = -10.0f;
	}

	float changeY = zoomAmount * dt;

	if (changeY < 0.0f)
	{
		m_cameraDistance = 2.0f;
		m_maxCameraDistance = 2.0f;
	}

	WrapCameraZoomValue();
}