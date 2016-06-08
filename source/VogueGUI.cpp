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

	// Create GUI fonts
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

	m_pWireframeCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "Wireframe");
	m_pWireframeCheckBox->SetDimensions(10, 10, 14, 14);
	m_pShadowsCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "Shadows");
	m_pShadowsCheckBox->SetDimensions(10, 28, 14, 14);
	m_pMSAACheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "Anti-Aliasing");
	m_pMSAACheckBox->SetDimensions(10, 46, 14, 14);
	m_pDynamicLightingCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "Lighting");
	m_pDynamicLightingCheckBox->SetDimensions(10, 64, 14, 14);
	m_pSSAOCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "SSAO");
	m_pSSAOCheckBox->SetDimensions(10, 82, 14, 14);
	m_pBlurCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "Blur");
	m_pBlurCheckBox->SetDimensions(10, 100, 14, 14);
	m_pDeferredCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "Deferred");
	m_pDeferredCheckBox->SetDimensions(10, 118, 14, 14);
	m_pDebugRenderCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "DebugRender");
	m_pDebugRenderCheckBox->SetDimensions(110, 10, 14, 14);
	m_pInstanceRenderCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "InstanceRender");
	m_pInstanceRenderCheckBox->SetDimensions(110, 28, 14, 14);
}

// Destruction
void VogueGUI::DestroyGUI()
{
	delete m_pMainWindow;
	delete m_pShadowsCheckBox;
	delete m_pSSAOCheckBox;
	delete m_pBlurCheckBox;
	delete m_pDynamicLightingCheckBox;
	delete m_pWireframeCheckBox;
	delete m_pMSAACheckBox;
	delete m_pDeferredCheckBox;
	delete m_pDebugRenderCheckBox;
	delete m_pInstanceRenderCheckBox;
}

// Setup
void VogueGUI::SetupGUI()
{
	m_pMainWindow->AddComponent(m_pShadowsCheckBox);
	m_pMainWindow->AddComponent(m_pSSAOCheckBox);
	m_pMainWindow->AddComponent(m_pBlurCheckBox);
	m_pMainWindow->AddComponent(m_pDynamicLightingCheckBox);
	m_pMainWindow->AddComponent(m_pWireframeCheckBox);
	m_pMainWindow->AddComponent(m_pMSAACheckBox);
	m_pMainWindow->AddComponent(m_pDeferredCheckBox);
	m_pMainWindow->AddComponent(m_pDebugRenderCheckBox);
	m_pMainWindow->AddComponent(m_pInstanceRenderCheckBox);

	m_pDeferredCheckBox->SetToggled(true);
	m_pSSAOCheckBox->SetToggled(true);
	m_pMSAACheckBox->SetToggled(true);

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