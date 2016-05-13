// ******************************************************************************
// Filename:    VogueRender.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 11/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "VogueGame.h"

#include <glm/detail/func_geometric.hpp>


// Rendering
void VogueGame::PreRender()
{
	// Update matrices for game objects
}

void VogueGame::BeginShaderRender()
{
	glShader* pShader = NULL;

	if (m_shadows)
	{
		m_pRenderer->BeginGLSLShader(m_shadowShader);
	}
	else
	{
		m_pRenderer->BeginGLSLShader(m_defaultShader);
	}
}

void VogueGame::EndShaderRender()
{
	glDisable(GL_FOG);

	if (m_shadows)
	{
		m_pRenderer->EndGLSLShader(m_shadowShader);
	}
	else
	{
		m_pRenderer->EndGLSLShader(m_defaultShader);
	}
}

void VogueGame::Render()
{
	if (m_pVogueWindow->GetMinimized())
	{
		// Don't call any render functions if minimized
		return;
	}

	// Begin rendering
	m_pRenderer->BeginScene(true, true, true);

		// Render the GUI
		RenderGUI();

		// Render debug information and text
		RenderDebugInformation();

	// End rendering
	m_pRenderer->EndScene();


	// Pass render call to the window class, allow to swap buffers
	m_pVogueWindow->Render();
}

void VogueGame::RenderGUI()
{
	m_pRenderer->EmptyTextureIndex(0);

	// Render the GUI
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetCullMode(CM_BACK);

		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		m_pGUI->Render();
	m_pRenderer->PopMatrix();
}

void VogueGame::RenderDebugInformation()
{
	char lCameraBuff[256];
	sprintf(lCameraBuff, "Pos(%.2f, %.2f, %.2f), Facing(%.2f, %.2f, %.2f) = %.2f, Up(%.2f, %.2f, %.2f) = %.2f, Right(%.2f, %.2f, %.2f) = %.2f, View(%.2f, %.2f, %.2f), Zoom=%.2f",
		m_pGameCamera->GetPosition().x, m_pGameCamera->GetPosition().y, m_pGameCamera->GetPosition().z,
		m_pGameCamera->GetFacing().x, m_pGameCamera->GetFacing().y, m_pGameCamera->GetFacing().z, length(m_pGameCamera->GetFacing()),
		m_pGameCamera->GetUp().x, m_pGameCamera->GetUp().y, m_pGameCamera->GetUp().z, length(m_pGameCamera->GetUp()),
		m_pGameCamera->GetRight().x, m_pGameCamera->GetRight().y, m_pGameCamera->GetRight().z, length(m_pGameCamera->GetRight()),
		m_pGameCamera->GetView().x, m_pGameCamera->GetView().y, m_pGameCamera->GetView().z,
		m_pGameCamera->GetZoomAmount());

	char lDrawingBuff[256];
	sprintf(lDrawingBuff, "Vertices: %i, Faces: %i", 0, 0); // TODO : Debug rendering Metrics

	char lFPSBuff[128];
	float fpsWidthOffset = 65.0f;
	if (m_debugRender)
	{
		sprintf(lFPSBuff, "Delta: %.4f  FPS: %.0f", m_deltaTime, m_fps);
		fpsWidthOffset = 135.0f;
	}
	else
	{
		sprintf(lFPSBuff, "FPS: %.0f", m_fps);
	}

	char lBuildInfo[128];
#if defined(_DEBUG) || defined(NDEBUG)
	sprintf(lBuildInfo, "DEV %s", m_pVogueSettings->m_version.c_str());
#else
	sprintf(lBuildInfo, "RELEASE %s", m_pVogueSettings->m_version.c_str());
#endif //defined(_DEBUG) || defined(NDEBUG)

	int l_nTextHeight = m_pRenderer->GetFreeTypeTextHeight(m_defaultFont, "a");

	m_pRenderer->PushMatrix();
		m_pRenderer->EmptyTextureIndex(0);

		m_pRenderer->SetRenderMode(RM_SOLID);
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));
		if (m_debugRender)
		{
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, m_windowHeight - (l_nTextHeight * 1) - 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lCameraBuff);
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, m_windowHeight - (l_nTextHeight*2) - 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lDrawingBuff);
		}

		m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth-fpsWidthOffset, 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lFPSBuff);
		m_pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, 10.0f, 1.0f, Colour(0.75f, 0.75f, 0.75f), 1.0f, lBuildInfo);

	m_pRenderer->PopMatrix();
}