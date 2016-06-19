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
#include "utils/Random.h"


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
	m_pMainWindow->SetDimensions(15, 35, 320, 180);
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
	m_pInstanceRenderCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "InstanceRender");
	m_pInstanceRenderCheckBox->SetDimensions(10, 136, 14, 14);
	m_pDebugRenderCheckBox = new CheckBox(m_pRenderer, m_defaultGUIFont, "DebugRender");
	m_pDebugRenderCheckBox->SetDimensions(10, 154, 14, 14);

	m_pGenderMaleOptionBox = new OptionBox(m_pRenderer, m_defaultGUIFont, "Male");
	m_pGenderMaleOptionBox->SetDimensions(10, 50, 14, 14);
	m_pGenderFemaleOptionBox = new OptionBox(m_pRenderer, m_defaultGUIFont, "Female");
	m_pGenderFemaleOptionBox->SetDimensions(10, 30, 14, 14);
	m_pGenderBothOptionBox = new OptionBox(m_pRenderer, m_defaultGUIFont, "Both");
	m_pGenderBothOptionBox->SetDimensions(10, 10, 14, 14);
	m_pGenderOptionController = new OptionController(m_pRenderer, m_defaultGUIFont, "Gender");
	m_pGenderOptionController->SetDisplayLabel(true);
	m_pGenderOptionController->SetDisplayBorder(true);
	m_pGenderOptionController->SetDimensions(230, 55, 85, 70);
	m_pGenderOptionController->Add(m_pGenderMaleOptionBox);
	m_pGenderOptionController->Add(m_pGenderFemaleOptionBox);
	m_pGenderOptionController->Add(m_pGenderBothOptionBox);
	m_pGenderBothOptionBox->SetToggled(true);

	m_pRandomizeButton = new Button(m_pRenderer, m_defaultGUIFont, "Randomize");
	m_pRandomizeButton->SetDimensions(235, 25, 75, 25);
	m_pRandomizeButton->SetCallBackFunction(_RandomizeButtonClicked);
	m_pRandomizeButton->SetCallBackData(this);
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

	delete m_pGenderOptionController;
	delete m_pGenderMaleOptionBox;
	delete m_pGenderFemaleOptionBox;
	delete m_pGenderBothOptionBox;
	delete m_pRandomizeButton;
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

	m_pMainWindow->AddComponent(m_pGenderOptionController);
	m_pMainWindow->AddComponent(m_pRandomizeButton);

	m_pDeferredCheckBox->SetToggled(true);
	m_pDynamicLightingCheckBox->SetToggled(true);
	m_pSSAOCheckBox->SetToggled(true);
	m_pMSAACheckBox->SetToggled(true);
	m_pShadowsCheckBox->SetToggled(true);

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

// Game functionality
ePlayerSex VogueGUI::GetPlayerSex()
{
	if (m_pGenderMaleOptionBox->GetToggled())
	{
		return ePlayerSex_Male;
	}
	else if (m_pGenderFemaleOptionBox->GetToggled())
	{
		return ePlayerSex_Female;
	}

	return (ePlayerSex)GetRandomNumber(0, ePlayerSex_Female);
}

// GUI callback functionality
void VogueGUI::_RandomizeButtonClicked(void *apData)
{
	VogueGUI* lpVogueGUI = (VogueGUI*)apData;
	lpVogueGUI->RandomizeButtonClicked();
}

void VogueGUI::RandomizeButtonClicked()
{
	VogueGame::GetInstance()->GetPlayer()->RandomizeParts();
	VogueGame::GetInstance()->GetPlayer()->UpdateDefaults();
	VogueGame::GetInstance()->GetPlayer()->SetColourModifiers();
}