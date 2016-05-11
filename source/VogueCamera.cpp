// ******************************************************************************
// Filename:    VogueCamera.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 27/10/15
//
// Copyright (c) 2005-2015, Steven Ball
// ******************************************************************************

#include "VogueGame.h"

// Camera controls
void VogueGame::UpdateCamera(float dt)
{
}

void VogueGame::UpdateCameraModeSwitching()
{
}

void VogueGame::InitializeCameraRotation()
{
}

void VogueGame::UpdateCameraAutoCamera(float dt, bool updateCameraPosition)
{
}

void VogueGame::UpdateCameraFirstPerson(float dt)
{
}

void VogueGame::UpdateCameraNPCDialog(float dt)
{
}

void VogueGame::UpdateCameraEnemyTarget(float dt)
{
}

void VogueGame::UpdateCameraClipping(float dt)
{
}

void VogueGame::UpdateCameraZoom(float dt)
{
}

bool VogueGame::ShouldRestorePreviousCameraMode()
{
	return m_shouldRestorePreviousCameraMode;
}

void VogueGame::SavePreviousCameraMode()
{
	m_previousCameraMode = m_cameraMode;
}

void VogueGame::RestorePreviousCameraMode()
{
	SetCameraMode(m_previousCameraMode);
	m_shouldRestorePreviousCameraMode = false;
}