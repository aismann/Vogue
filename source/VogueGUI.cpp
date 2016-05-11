// ******************************************************************************
// Filename:    VogueGUI.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueGame.h"
#include "utils/FileUtils.h"

void VogueGame::CreateGUI()
{
	m_GUICreated = true;
}

void VogueGame::SetupGUI()
{
}

void VogueGame::SkinGUI()
{
}

void VogueGame::UnSkinGUI()
{
}

void VogueGame::DestroyGUI()
{
}

void VogueGame::UpdateGUI(float dt)
{
}

void VogueGame::GUITurnOffCursor()
{
	m_pGUI->ResetSelectionManager();
}

void VogueGame::ShowGUI()
{
}

void VogueGame::HideGUI()
{
}