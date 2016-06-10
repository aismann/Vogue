// ******************************************************************************
// Filename:    Player.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 25/05/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "Player.h"
#include "../utils/Random.h"

#include <fstream>
#include <ostream>
#include <iostream>
#include <string>
#include <algorithm>
#include <vector>
using namespace std;


Player::Player(Renderer* pRenderer, QubicleBinaryManager* pQubicleBinaryManager)
{
	m_pRenderer = pRenderer;
	m_pQubicleBinaryManager = pQubicleBinaryManager;

	m_forward = vec3(0.0f, 0.0f, 1.0f);
	m_right = vec3(1.0f, 0.0f, 0.0f);
	m_up = vec3(0.0f, 1.0f, 0.0f);

	m_position = vec3(0.0f, 0.0f, 0.0f);
	m_gravityDirection = vec3(0.0f, -1.0f, 0.0f);

	// Body parts indices
	m_headNum = 0;
	m_hairNum = 0;
	m_noseNum = 0;
	m_skinRed = 0.7686274509803922f;
	m_skinBlue = 0.6509803921568627f;
	m_skinGreen = 0.4156862745098039f;

	m_pPlayerModel = new QubicleBinary(m_pRenderer);

	ModifyHead();
	ModifyHair();
	ModifyNose();
	UpdateDefaults();
}

Player::~Player()
{
	m_pPlayerModel->SetNullLinkage(m_pHeadModel);
	m_pPlayerModel->SetNullLinkage(m_pHairModel);
	m_pPlayerModel->SetNullLinkage(m_pNoseModel);
}

static int const MAX_NUM_HEADS = 1;
static int const MAX_NUM_HAIRS = 20;
static int const MAX_NUM_NOSES = 4;

void Player::ModifyHead()
{
	m_headNum++;
	if (m_headNum > MAX_NUM_HEADS)
	{
		m_headNum = 1;
	}

	string qubicleFile = "media/gamedata/head/base_head" + to_string(m_headNum) + ".qb";
	m_pHeadModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), false);
	QubicleMatrix* pHeadMatrix = m_pHeadModel->GetQubicleMatrix("head");
	m_pPlayerModel->AddQubicleMatrix(pHeadMatrix, false);
}

void Player::ModifyHair()
{
	m_hairNum++;
	if (m_hairNum > MAX_NUM_HAIRS)
	{
		m_hairNum = 1;
	}

	string qubicleFile = "media/gamedata/hair/male_hair" + to_string(m_hairNum) + ".qb";
	m_pHairModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), false);
	QubicleMatrix* pHairMatrix = m_pHairModel->GetQubicleMatrix("hair");
	m_pPlayerModel->AddQubicleMatrix(pHairMatrix, false);
}

void Player::ModifyNose()
{
	m_noseNum++;
	if (m_noseNum > MAX_NUM_NOSES)
	{
		m_noseNum = 1;
	}

	string qubicleFile = "media/gamedata/nose/nose" + to_string(m_noseNum) + ".qb";
	m_pNoseModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), false);
	QubicleMatrix* pNoseMatrix = m_pNoseModel->GetQubicleMatrix("nose");
	m_pPlayerModel->AddQubicleMatrix(pNoseMatrix, false);
}

void Player::UpdateDefaults()
{
	string defaultFile = "";
	QubicleMatrix* pMatrix = NULL;
	for (int i = 0; i < 3; i++)
	{
		if (i == 0)
		{
			defaultFile = "media/gamedata/head/base_head" + to_string(m_headNum) + ".default";
			pMatrix = m_pHeadModel->GetQubicleMatrix("head");
		}
		if (i == 1)
		{
			defaultFile = "media/gamedata/hair/male_hair" + to_string(m_hairNum) + ".default";
			pMatrix = m_pHairModel->GetQubicleMatrix("hair");
		}
		if (i == 2)
		{
			defaultFile = "media/gamedata/nose/nose" + to_string(m_noseNum) + ".default";
			pMatrix = m_pNoseModel->GetQubicleMatrix("nose");
		}

		if (pMatrix != NULL)
		{
			ifstream importFile;
			importFile.open(defaultFile.c_str(), ios::in);
			float scale;
			float offsetX;
			float offsetY;
			float offsetZ;

			if (importFile.is_open())
			{
				string tempString;

				importFile >> tempString >> scale;
				importFile >> tempString >> offsetX;
				importFile >> tempString >> offsetY;
				importFile >> tempString >> offsetZ;

				pMatrix->m_scale = scale;
				pMatrix->m_offsetX = offsetX;
				pMatrix->m_offsetY = offsetY;
				pMatrix->m_offsetZ = offsetZ;

				importFile.close();
			}
		}
	}
}

void Player::ModifySkinColor()
{
	m_pPlayerModel->SetNullLinkage(m_pHeadModel);
	m_pPlayerModel->SetNullLinkage(m_pNoseModel);

	string qubicleFile = "media/gamedata/head/base_head" + to_string(m_headNum) + ".qb";
	QubicleBinary* pHeadModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pHeadMatrix = pHeadModel->GetQubicleMatrix("head");
	m_pPlayerModel->AddQubicleMatrix(pHeadMatrix, false);

	qubicleFile = "media/gamedata/nose/nose" + to_string(m_noseNum) + ".qb";
	QubicleBinary* pNoseModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pNoseMatrix = pNoseModel->GetQubicleMatrix("nose");
	m_pPlayerModel->AddQubicleMatrix(pNoseMatrix, false);

	m_skinRed = GetRandomNumber(0, 1, 2);
	m_skinBlue = GetRandomNumber(0, 1, 2);
	m_skinGreen = GetRandomNumber(0, 1, 2);
}

void Player::SetSkinColor()
{
	m_pPlayerModel->ConvertMeshColour(m_skinRed, m_skinBlue, m_skinGreen, 0.7686274509803922f, 0.6509803921568627f, 0.4156862745098039f);
}

// Rendering Helpers
void Player::CalculateWorldTransformMatrix()
{
	m_right = normalize(cross(m_up, m_forward));
	m_forward = normalize(cross(m_right, m_up));

	float lMatrix[16] =
	{
		m_right.x, m_right.y, m_right.z, 0.0f,
		m_up.x, m_up.y, m_up.z, 0.0f,
		m_forward.x, m_forward.y, m_forward.z, 0.0f,
		m_position.x, m_position.y, m_position.z, 1.0f
	};

	m_worldMatrix.SetValues(lMatrix);
}

// Update
void Player::Update(float dt)
{
}

// Render
void Player::Render()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->TranslateWorldMatrix(m_position.x, m_position.y, m_position.z);

		// Scale down
		m_pRenderer->ScaleWorldMatrix(0.1f, 0.1f, 0.1f);

		Colour OulineColour(1.0f, 1.0f, 0.0f, 1.0f);
		m_pPlayerModel->Render(false, false, false, OulineColour);
	m_pRenderer->PopMatrix();

	RenderDebug();
}

void Player::RenderDebug()
{
	m_pRenderer->SetRenderMode(RM_WIREFRAME);
	m_pRenderer->SetCullMode(CM_NOCULL);
	m_pRenderer->SetLineWidth(1.0f);

	float length = 0.25f;
	float height = 0.5f;
	float width = 0.25f;
	m_pRenderer->PushMatrix();
		m_pRenderer->MultiplyWorldMatrix(m_worldMatrix);
		m_pRenderer->ImmediateColourAlpha(1.0f, 1.0f, 0.0f, 1.0f);

		m_pRenderer->EnableImmediateMode(IM_QUADS);
			m_pRenderer->ImmediateNormal(0.0f, 0.0f, -1.0f);
			m_pRenderer->ImmediateVertex(length, -height, -width);
			m_pRenderer->ImmediateVertex(-length, -height, -width);
			m_pRenderer->ImmediateVertex(-length, height, -width);
			m_pRenderer->ImmediateVertex(length, height, -width);

			m_pRenderer->ImmediateNormal(0.0f, 0.0f, 1.0f);
			m_pRenderer->ImmediateVertex(-length, -height, width);
			m_pRenderer->ImmediateVertex(length, -height, width);
			m_pRenderer->ImmediateVertex(length, height, width);
			m_pRenderer->ImmediateVertex(-length, height, width);

			m_pRenderer->ImmediateNormal(1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(length, -height, width);
			m_pRenderer->ImmediateVertex(length, -height, -width);
			m_pRenderer->ImmediateVertex(length, height, -width);
			m_pRenderer->ImmediateVertex(length, height, width);

			m_pRenderer->ImmediateNormal(-1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-length, -height, -width);
			m_pRenderer->ImmediateVertex(-length, -height, width);
			m_pRenderer->ImmediateVertex(-length, height, width);
			m_pRenderer->ImmediateVertex(-length, height, -width);

			m_pRenderer->ImmediateNormal(0.0f, -1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-length, -height, -width);
			m_pRenderer->ImmediateVertex(length, -height, -width);
			m_pRenderer->ImmediateVertex(length, -height, width);
			m_pRenderer->ImmediateVertex(-length, -height, width);

			m_pRenderer->ImmediateNormal(0.0f, 1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(length, height, -width);
			m_pRenderer->ImmediateVertex(-length, height, -width);
			m_pRenderer->ImmediateVertex(-length, height, width);
			m_pRenderer->ImmediateVertex(length, height, width);
		m_pRenderer->DisableImmediateMode();
	m_pRenderer->PopMatrix();

	m_pRenderer->SetCullMode(CM_BACK);
}