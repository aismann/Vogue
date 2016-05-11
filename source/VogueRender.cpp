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

	// End rendering
	m_pRenderer->EndScene();


	// Pass render call to the window class, allow to swap buffers
	m_pVogueWindow->Render();
}