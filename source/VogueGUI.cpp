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

VogueGUI::VogueGUI(Renderer* pRenderer, OpenGLGUI* pGUI, int width, int height)
{
	m_pRenderer = pRenderer;
	m_pGUI = pGUI;

	m_windowWidth = width;
	m_windowHeight = height;

	m_pRenderer->CreateFreeTypeFont("media/fonts/arial.ttf", 12, &m_defaultGUIFont);
}

VogueGUI::~VogueGUI()
{
	DestroyGUI();
}

// Creation
void VogueGUI::CreateGUI()
{
	m_pMainWindow = new GUIWindow(m_pRenderer, m_defaultGUIFont, "Main");
	m_pMainWindow->AllowMoving(true);
	m_pMainWindow->AllowClosing(false);
	m_pMainWindow->AllowMinimizing(true);
	m_pMainWindow->AllowScrolling(true);
	m_pMainWindow->SetRenderTitleBar(true);
	m_pMainWindow->SetRenderWindowBackground(true);
	m_pMainWindow->SetOutlineRender(true);
	m_pMainWindow->SetDimensions(15, 35, 320, 140);
	m_pMainWindow->SetApplicationDimensions(m_windowWidth, m_windowHeight);
}

// Destruction
void VogueGUI::DestroyGUI()
{
	delete m_pMainWindow;
}

// Setup
void VogueGUI::SetupGUI()
{
	m_pGUI->AddWindow(m_pMainWindow);
}

// Resize
void VogueGUI::SignalResize(int width, int height)
{
	m_windowWidth = width;
	m_windowHeight = height;
}

// Skinning
void VogueGUI::SkinGUI()
{

}

void VogueGUI::UnSkinGUI()
{

}

// GUI Functionality
void VogueGUI::GUITurnOffCursor()
{

}

void VogueGUI::ShowGUI()
{

}

void VogueGUI::HideGUI()
{

}

// Update
void VogueGUI::UpdateGUI(float dt)
{

}