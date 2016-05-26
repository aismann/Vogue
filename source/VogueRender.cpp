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
	m_pPlayer->CalculateWorldTransformMatrix();
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

		// ---------------------------------------
		// Render 3d
		// ---------------------------------------
		m_pRenderer->PushMatrix();
			// Set the default projection mode
			m_pRenderer->SetProjectionMode(PM_PERSPECTIVE, m_defaultViewport);

			// Set back culling as default
			m_pRenderer->SetCullMode(CM_BACK);

			// Set default depth test
			m_pRenderer->EnableDepthTest(DT_LESS);

			// Enable colour material
			m_pRenderer->EnableColourMaterial();

			// Set the lookat camera
			m_pGameCamera->Look();

			// Enable the lights
			m_pRenderer->PushMatrix();
				m_pRenderer->EnableLight(m_defaultLight, 0);
			m_pRenderer->PopMatrix();

			// SSAO frame buffer rendering start
			//if (m_deferredRendering) // TODO
			{
				m_pRenderer->StartRenderingToFrameBuffer(m_SSAOFrameBuffer);
			}

			m_pRenderer->SetClearColour(0.0f, 0.0f, 0.0f, 1.0f);
			m_pRenderer->ClearScene(true, true, true);

			// Render the lights (DEBUG)
			m_pRenderer->PushMatrix();
				m_pRenderer->SetCullMode(CM_BACK);
				m_pRenderer->SetRenderMode(RM_SOLID);
				m_pRenderer->RenderLight(m_defaultLight);
			m_pRenderer->PopMatrix();

			// Rooms
			m_pRoomManager->Render();

			// Tile
			m_pTileManager->Render();

			// Instanced objects
			m_pInstanceManager->Render();

			// Player
			m_pPlayer->Render();

			// SSAO frame buffer rendering stop
			//if (m_deferredRendering) // TODO
			{
				m_pRenderer->StopRenderingToFrameBuffer(m_SSAOFrameBuffer);
			}
		m_pRenderer->PopMatrix();

		// Render our deferred textures from the frame buffers
		//if (m_deferredRendering) // TODO
		{
			RenderSSAOTexture();

			//if (m_multiSampling && m_fxaaShader != -1) // TODO
			{
				//RenderFXAATexture();
			}
		}

		// ---------------------------------------
		// Render 2d (on top of our deferred textures)
		// ---------------------------------------
		m_pRenderer->PushMatrix();
			// Render the GUI
			RenderGUI();

			// Render debug information and text
			RenderDebugInformation();
		m_pRenderer->PopMatrix();

	// End rendering
	m_pRenderer->EndScene();


	// Pass render call to the window class, allow to swap buffers
	m_pVogueWindow->Render();
}

void VogueGame::RenderSSAOTexture()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		//if (m_multiSampling) // TODO
		//{
		//	m_pRenderer->StartRenderingToFrameBuffer(m_FXAAFrameBuffer);
		//}
		//else if (m_blur || m_pChunkManager->IsUnderWater(m_pGameCamera->GetPosition()))
		//{
		//	m_pRenderer->StartRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		//}

		// SSAO shader
		m_pRenderer->BeginGLSLShader(m_SSAOShader);
		glShader* pShader = m_pRenderer->GetShader(m_SSAOShader);

		unsigned int textureId0 = glGetUniformLocationARB(pShader->GetProgramObject(), "bgl_DepthTexture");
		m_pRenderer->PrepareShaderTexture(0, textureId0);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDepthTextureFromFrameBuffer(m_SSAOFrameBuffer));

		unsigned int textureId1 = glGetUniformLocationARB(pShader->GetProgramObject(), "bgl_RenderedTexture");
		m_pRenderer->PrepareShaderTexture(1, textureId1);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDiffuseTextureFromFrameBuffer(m_SSAOFrameBuffer));

		unsigned int textureId2 = glGetUniformLocationARB(pShader->GetProgramObject(), "light");
		m_pRenderer->PrepareShaderTexture(2, textureId2);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDiffuseTextureFromFrameBuffer(m_lightingFrameBuffer));

		unsigned int textureId3 = glGetUniformLocationARB(pShader->GetProgramObject(), "bgl_TransparentTexture");
		m_pRenderer->PrepareShaderTexture(3, textureId3);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDiffuseTextureFromFrameBuffer(m_transparencyFrameBuffer));

		unsigned int textureId4 = glGetUniformLocationARB(pShader->GetProgramObject(), "bgl_TransparentDepthTexture");
		m_pRenderer->PrepareShaderTexture(4, textureId4);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDepthTextureFromFrameBuffer(m_transparencyFrameBuffer));

		pShader->setUniform1i("screenWidth", m_windowWidth);
		pShader->setUniform1i("screenHeight", m_windowHeight);
		pShader->setUniform1f("nearZ", 0.01f);
		pShader->setUniform1f("farZ", 1000.0f);

		pShader->setUniform1f("samplingMultiplier", 1.5f);

		pShader->setUniform1i("lighting_enabled", true); // TODO
		pShader->setUniform1i("ssao_enabled", true); // TODO

		m_pRenderer->SetRenderMode(RM_TEXTURED);
		m_pRenderer->EnableImmediateMode(IM_QUADS);
			m_pRenderer->ImmediateTextureCoordinate(0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(0.0f, 0.0f, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(1.0f, 0.0f);
			m_pRenderer->ImmediateVertex((float)m_windowWidth, 0.0f, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(1.0f, 1.0f);
			m_pRenderer->ImmediateVertex((float)m_windowWidth, (float)m_windowHeight, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(0.0f, 1.0f);
			m_pRenderer->ImmediateVertex(0.0f, (float)m_windowHeight, 1.0f);
		m_pRenderer->DisableImmediateMode();

		m_pRenderer->EmptyTextureIndex(4);
		m_pRenderer->EmptyTextureIndex(3);
		m_pRenderer->EmptyTextureIndex(2);
		m_pRenderer->EmptyTextureIndex(1);
		m_pRenderer->EmptyTextureIndex(0);

		m_pRenderer->EndGLSLShader(m_SSAOShader);

		//if (m_multiSampling) // TODO
		//{
		//	m_pRenderer->StopRenderingToFrameBuffer(m_FXAAFrameBuffer);
		//}
		//else if (m_blur || m_pChunkManager->IsUnderWater(m_pGameCamera->GetPosition()))
		//{
		//	m_pRenderer->StopRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		//}
	m_pRenderer->PopMatrix();
}

void VogueGame::RenderFXAATexture()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		// TODO
		//if (m_blur || m_pChunkManager->IsUnderWater(m_pGameCamera->GetPosition()))
		//{
		//	m_pRenderer->StartRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		//}

		m_pRenderer->BeginGLSLShader(m_fxaaShader);
		glShader* pShader = m_pRenderer->GetShader(m_fxaaShader);

		pShader->setUniform1i("screenWidth", m_windowWidth);
		pShader->setUniform1i("screenHeight", m_windowHeight);

		unsigned int textureId0 = glGetUniformLocationARB(pShader->GetProgramObject(), "texture");
		m_pRenderer->PrepareShaderTexture(0, textureId0);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDiffuseTextureFromFrameBuffer(m_FXAAFrameBuffer));

		m_pRenderer->SetRenderMode(RM_TEXTURED);
		m_pRenderer->EnableImmediateMode(IM_QUADS);
			m_pRenderer->ImmediateTextureCoordinate(0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(0.0f, 0.0f, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(1.0f, 0.0f);
			m_pRenderer->ImmediateVertex((float)m_windowWidth, 0.0f, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(1.0f, 1.0f);
			m_pRenderer->ImmediateVertex((float)m_windowWidth, (float)m_windowHeight, 1.0f);
			m_pRenderer->ImmediateTextureCoordinate(0.0f, 1.0f);
			m_pRenderer->ImmediateVertex(0.0f, (float)m_windowHeight, 1.0f);
		m_pRenderer->DisableImmediateMode();

		m_pRenderer->EmptyTextureIndex(0);

		m_pRenderer->EndGLSLShader(m_fxaaShader);

		// TODO
		//if (m_blur || m_pChunkManager->IsUnderWater(m_pGameCamera->GetPosition()))
		//{
		//	m_pRenderer->StopRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		//}
	m_pRenderer->PopMatrix();
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

	char lRoomsBuff[256];
	sprintf(lRoomsBuff, "Rooms: %i, ConnectionList: %i, Item: %i (%i), Boss: %i (%i)", m_pRoomManager->GetNumRooms(), m_pRoomManager->GetNumConnectionRoomsPossible(),
		m_pRoomManager->GetNumItemRooms(), m_pRoomManager->GetNumItemRoomsPossible(), m_pRoomManager->GetNumBossRooms(), m_pRoomManager->GetNumBossRoomsPossible());
	
	char lInstancesBuff[256];
	sprintf(lInstancesBuff, "Instance Parents: %i, Instance Objects: %i, Instance Render: %i", m_pInstanceManager->GetNumInstanceParents(), m_pInstanceManager->GetTotalNumInstanceObjects(), m_pInstanceManager->GetTotalNumInstanceRenderObjects());

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
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, m_windowHeight - (l_nTextHeight * 2) - 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lDrawingBuff);
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, m_windowHeight - (l_nTextHeight * 3) - 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lRoomsBuff);
			m_pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, m_windowHeight - (l_nTextHeight * 4) - 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lInstancesBuff);
		}

		m_pRenderer->RenderFreeTypeText(m_defaultFont, m_windowWidth-fpsWidthOffset, 10.0f, 1.0f, Colour(1.0f, 1.0f, 1.0f), 1.0f, lFPSBuff);
		m_pRenderer->RenderFreeTypeText(m_defaultFont, 10.0f, 10.0f, 1.0f, Colour(0.75f, 0.75f, 0.75f), 1.0f, lBuildInfo);

	m_pRenderer->PopMatrix();
}