// ******************************************************************************
// Filename:    VogueUpdate.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueGame.h"

#include "utils/Interpolator.h"
#include "utils/TimeManager.h"

#ifdef __linux__
#include <sys/time.h>
#endif //__linux__


// Updating
void VogueGame::Update()
{
	// FPS
#ifdef _WIN32
	QueryPerformanceCounter(&m_fpsCurrentTicks);
	m_deltaTime = ((float)(m_fpsCurrentTicks.QuadPart - m_fpsPreviousTicks.QuadPart) / (float)m_fpsTicksPerSecond.QuadPart);
#else
	struct timeval tm;
	gettimeofday(&tm, NULL);
	m_fpsCurrentTicks = (double)tm.tv_sec + (double)tm.tv_usec / 1000000.0;
	m_deltaTime = (m_fpsCurrentTicks - m_fpsPreviousTicks);
#endif //_WIN32
	m_fps = 1.0f / m_deltaTime;
	m_fpsPreviousTicks = m_fpsCurrentTicks;

	float maxDeltaTime = 0.25f;
	if (m_deltaTime > maxDeltaTime)
	{
		cout << "Warning: DeltaTime exceeded sensible value, switching dt from " << m_deltaTime << " to " << maxDeltaTime << ".\n";
		m_deltaTime = maxDeltaTime;
	}

	// Update interpolator singleton
	Interpolator::GetInstance()->Update(m_deltaTime);

	// Pause the interpolator we are are paused.
	Interpolator::GetInstance()->SetPaused(m_bPaused);

	// Update the time manager (countdowntimers);
	TimeManager::GetInstance()->Update(m_deltaTime);

	// Update the initial wait timer and variables, so we dont do gameplay updates straight away
	if (m_initialStartWait == true)
	{
		if (m_initialWaitTimer > m_initialWaitTime)
		{
			m_initialStartWait = false;
		}
		else
		{
			m_initialWaitTimer += m_deltaTime;
			m_initialStartWait = true;
		}
	}

	// Update the GUI
	int x = m_pVogueWindow->GetCursorX();
	int y = m_pVogueWindow->GetCursorY();
	m_pGUI->Update(m_deltaTime);
	if (IsCursorOn())
	{
		m_pGUI->ImportMouseMotion(x, m_windowHeight - y);
	}
	UpdateGameGUI(m_deltaTime);

	// Main components update
	if (m_bPaused == false && m_initialStartWait == false)
	{
		m_pInstanceManager->Update(m_deltaTime);

		m_pRoomManager->Update(m_deltaTime);
		m_pTileManager->Update(m_deltaTime);

		m_pPlayer->Update(m_deltaTime);
	}

	// Update controls
	UpdateControls(m_deltaTime);

	// Update the camera
	UpdateCamera(m_deltaTime);

	// Update the application and window
	m_pVogueWindow->Update(m_deltaTime);
}

void VogueGame::UpdateNamePicking()
{
	POINT lMouse = { VogueGame::GetInstance()->GetWindowCursorX(), (m_windowHeight - VogueGame::GetInstance()->GetWindowCursorY()) };

	// Push attribs
	glPushAttrib(GL_LIST_BIT | GL_CURRENT_BIT | GL_ENABLE_BIT | GL_TRANSFORM_BIT);
	glColorMask(GL_FALSE, GL_FALSE, GL_FALSE, GL_FALSE);  // Disabled rendering

	// Set the projection mode
	m_pRenderer->SetProjectionMode(PM_PERSPECTIVE, m_defaultViewport);

	// Init name picking
	m_pRenderer->InitNameStack();

	// Start name picking
	m_pRenderer->StartNamePicking(m_defaultViewport, lMouse.x, lMouse.y);

	// Set the lookat camera
	m_pGameCamera->Look();

	// Different sub-systems render name picking
	{
	}

	// End the name picking
	m_pickedObject = m_pRenderer->GetPickedObject();

	// Pop attribs
	glColorMask(GL_TRUE, GL_TRUE, GL_TRUE, GL_TRUE);   // Enable rendering
	glPopAttrib();

	if (m_pickedObject != -1)
	{
		m_bNamePickingSelected = true;
	}
	else
	{
		m_bNamePickingSelected = false;
	}
}

void VogueGame::UpdateLights(float dt)
{
	m_pRenderer->EditLightPosition(m_defaultLight, m_defaultLightPosition);
}

void VogueGame::UpdateGameGUI(float dt)
{
	m_pVogueGUI->UpdateGUI(dt);

	m_shadows = m_pVogueGUI->m_pShadowsCheckBox->GetToggled();
	m_ssao = m_pVogueGUI->m_pSSAOCheckBox->GetToggled();
	m_blur = m_pVogueGUI->m_pBlurCheckBox->GetToggled();
	m_dynamicLighting = m_pVogueGUI->m_pDynamicLightingCheckBox->GetToggled();
	m_multiSampling = m_pVogueGUI->m_pMSAACheckBox->GetToggled();
	m_deferredRendering = m_pVogueGUI->m_pDeferredCheckBox->GetToggled();
	m_debugRender = m_pVogueGUI->m_pDebugRenderCheckBox->GetToggled();
	m_wireframeRender = m_pVogueGUI->m_pWireframeCheckBox->GetToggled();
	m_instanceRender = m_pVogueGUI->m_pInstanceRenderCheckBox->GetToggled();

	m_pInstanceManager->SetWireFrameRender(m_wireframeRender);
}