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

		pShader = m_pRenderer->GetShader(m_shadowShader);
		GLuint shadowMapUniform = glGetUniformLocationARB(pShader->GetProgramObject(), "ShadowMap");
		m_pRenderer->PrepareShaderTexture(7, shadowMapUniform);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDepthTextureFromFrameBuffer(m_shadowFrameBuffer));
		glUniform1iARB(glGetUniformLocationARB(pShader->GetProgramObject(), "renderShadow"), m_shadows);
		glUniform1iARB(glGetUniformLocationARB(pShader->GetProgramObject(), "alwaysShadow"), false);
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

		// Shadow rendering to the shadow frame buffer
		if (m_shadows)
		{
			RenderShadows();
		}

		// ---------------------------------------
		// Render 3d
		// ---------------------------------------
		m_pRenderer->PushMatrix();
			// Set the default projection mode
			m_pRenderer->SetProjectionMode(PM_PERSPECTIVE, m_defaultViewport);

			// Enable vector normalization, for scaling and lighting
			m_pRenderer->EnableVectorNormalize();

			// Set back culling as default
			m_pRenderer->SetCullMode(CM_BACK);

			// Set default depth test
			m_pRenderer->EnableDepthTest(DT_LESS);

			// Enable colour material
			m_pRenderer->EnableColourMaterial();

			// Set the lookat camera
			m_pGameCamera->Look();

			// Enable the lights
			if (m_dynamicLighting)
			{
				m_pRenderer->PushMatrix();
					m_pRenderer->EnableLight(m_defaultLight, 0);
				m_pRenderer->PopMatrix();
			}

			// Multisampling MSAA
			if (m_multiSampling)
			{
				m_pRenderer->EnableMultiSampling();
			}
			else
			{
				m_pRenderer->DisableMultiSampling();
			}

			// SSAO frame buffer rendering start
			if (m_deferredRendering)
			{
				m_pRenderer->StartRenderingToFrameBuffer(m_SSAOFrameBuffer);
			}

			m_pRenderer->SetClearColour(0.75f, 0.75f, 0.95f, 1.0f);
			m_pRenderer->ClearScene(true, true, true);

			// Render the lights (DEBUG)
			if (m_dynamicLighting)
			{
				m_pRenderer->PushMatrix();
					m_pRenderer->SetCullMode(CM_BACK);
					m_pRenderer->SetRenderMode(RM_SOLID);
					m_pRenderer->RenderLight(m_defaultLight);
				m_pRenderer->PopMatrix();
			}

			BeginShaderRender();
			{
				// Rooms
				//m_pRoomManager->Render();

				// Tile
				m_pTileManager->Render();

				// Instanced objects
				m_pInstanceManager->Render();

				// Player
				m_pPlayer->Render();
			}
			EndShaderRender();

			// SSAO frame buffer rendering stop
			if (m_deferredRendering)
			{
				m_pRenderer->StopRenderingToFrameBuffer(m_SSAOFrameBuffer);
			}
		m_pRenderer->PopMatrix();

		// ---------------------------------------
		// Render transparency
		// ---------------------------------------
		RenderTransparency();

		// Render our deferred textures from the frame buffers
		if (m_deferredRendering)
		{
			RenderSSAOTexture();

			if (m_multiSampling && m_fxaaShader != -1)
			{
				RenderFXAATexture();
			}
			
			if (m_blur)
			{
				RenderFirstPassFullScreen();
				RenderSecondPassFullScreen();
			}
		}

		// ---------------------------------------
		// Render 2d (on top of our deferred textures)
		// ---------------------------------------
		m_pRenderer->PushMatrix();
			// Disable multisampling for 2d gui and text
			m_pRenderer->DisableMultiSampling();

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

void VogueGame::RenderShadows()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->StartRenderingToFrameBuffer(m_shadowFrameBuffer);
		m_pRenderer->SetColourMask(false, false, false, false);

		float shadowRadius = 5.0f;
		m_pRenderer->SetupOrthographicProjection(-shadowRadius, shadowRadius, -shadowRadius, shadowRadius, 0.01f, 1000.0f);
		vec3 lightPos = m_defaultLightPosition + m_pPlayer->GetPosition(); // Make sure our light is always offset from the player
		m_pRenderer->SetLookAtCamera(vec3(lightPos.x, lightPos.y, lightPos.z), m_pPlayer->GetPosition(), vec3(0.0f, 1.0f, 0.0f));

		m_pRenderer->PushMatrix();
			m_pRenderer->SetCullMode(CM_FRONT);

			// Render the player
			m_pPlayer->Render();

			// Render the instanced objects
			if(m_instanceRender)
			{
				m_pInstanceManager->Render();
			}

			m_pRenderer->SetTextureMatrix();
			m_pRenderer->SetCullMode(CM_BACK);
		m_pRenderer->PopMatrix();

		m_pRenderer->SetColourMask(true, true, true, true);
		m_pRenderer->StopRenderingToFrameBuffer(m_shadowFrameBuffer);
	m_pRenderer->PopMatrix();
}

void VogueGame::RenderTransparency()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_PERSPECTIVE, m_defaultViewport);
		m_pRenderer->SetCullMode(CM_BACK);

		// Set the lookat camera
		m_pGameCamera->Look();

		if (m_deferredRendering)
		{
			m_pRenderer->StartRenderingToFrameBuffer(m_transparencyFrameBuffer);
		}

		m_pPlayer->RenderFace();

		if (m_deferredRendering)
		{
			m_pRenderer->StopRenderingToFrameBuffer(m_transparencyFrameBuffer);
		}
	m_pRenderer->PopMatrix();
}

void VogueGame::RenderSSAOTexture()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		if (m_multiSampling)
		{
			m_pRenderer->StartRenderingToFrameBuffer(m_FXAAFrameBuffer);
		}
		else if (m_blur)
		{
			m_pRenderer->StartRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		}

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

		pShader->setUniform1i("lighting_enabled", m_dynamicLighting);
		pShader->setUniform1i("ssao_enabled", m_ssao);

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

		if (m_multiSampling)
		{
			m_pRenderer->StopRenderingToFrameBuffer(m_FXAAFrameBuffer);
		}
		else if (m_blur)
		{
			m_pRenderer->StopRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		}
	m_pRenderer->PopMatrix();
}

void VogueGame::RenderFXAATexture()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		if (m_blur)
		{
			m_pRenderer->StartRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		}

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

		if (m_blur)
		{
			m_pRenderer->StopRenderingToFrameBuffer(m_firstPassFullscreenBuffer);
		}
	m_pRenderer->PopMatrix();
}

void VogueGame::RenderFirstPassFullScreen()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		// Blur first pass (Horizontal)
		m_pRenderer->StartRenderingToFrameBuffer(m_secondPassFullscreenBuffer);
		m_pRenderer->BeginGLSLShader(m_blurHorizontalShader);
		glShader* pShader = m_pRenderer->GetShader(m_blurHorizontalShader);

		unsigned int textureId0 = glGetUniformLocationARB(pShader->GetProgramObject(), "texture");
		m_pRenderer->PrepareShaderTexture(0, textureId0);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDiffuseTextureFromFrameBuffer(m_firstPassFullscreenBuffer));

		float blurSize = 0.0015f;

		// Override any blur amount if we have global blur set
		if (m_globalBlurAmount > 0.0f)
		{
			blurSize = m_globalBlurAmount;
		}
		
		pShader->setUniform1f("blurSize", blurSize);

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

		m_pRenderer->EndGLSLShader(m_blurHorizontalShader);
		m_pRenderer->StopRenderingToFrameBuffer(m_secondPassFullscreenBuffer);
	m_pRenderer->PopMatrix();
}

void VogueGame::RenderSecondPassFullScreen()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->SetProjectionMode(PM_2D, m_defaultViewport);
		m_pRenderer->SetLookAtCamera(vec3(0.0f, 0.0f, 250.0f), vec3(0.0f, 0.0f, 0.0f), vec3(0.0f, 1.0f, 0.0f));

		// Blur second pass (Vertical)
		m_pRenderer->BeginGLSLShader(m_blurVerticalShader);
		glShader* pShader = m_pRenderer->GetShader(m_blurVerticalShader);

		unsigned int textureId0 = glGetUniformLocationARB(pShader->GetProgramObject(), "texture");
		m_pRenderer->PrepareShaderTexture(0, textureId0);
		m_pRenderer->BindRawTextureId(m_pRenderer->GetDiffuseTextureFromFrameBuffer(m_secondPassFullscreenBuffer));

		float blurSize = 0.0015f;
		bool applyBlueTint = false;

		// Override any blur amount if we have global blur set
		if (m_globalBlurAmount > 0.0f)
		{
			blurSize = m_globalBlurAmount;
		}
		
		pShader->setUniform1f("blurSize", blurSize);

		glUniform1iARB(glGetUniformLocationARB(pShader->GetProgramObject(), "applyBlueTint"), applyBlueTint);

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

		m_pRenderer->EndGLSLShader(m_blurVerticalShader);
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
	sprintf(lDrawingBuff, "Vertices: %i, Faces: %i", m_pRenderer->GetNumRenderedVertices(), m_pRenderer->GetNumRenderedFaces());

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