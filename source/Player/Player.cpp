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

	m_pPlayerModel = m_pQubicleBinaryManager->GetQubicleBinaryFile("media/gamedata/heads/base_head1.qb", false);

	LoadDefaultsFile();
}

Player::~Player()
{
	m_pPlayerModel->SetNullLinkage(m_pHairModel);
}

static int fileNumber = 1;
static int const MAX_NUM_HAIRS = 13;

void Player::LoadDefaultsFile()
{
	fileNumber++;
	if (fileNumber > MAX_NUM_HAIRS)
	{
		fileNumber = 1;
	}

	string qubicleFile = "media/gamedata/hair/male_hair" + to_string(fileNumber) + ".qb";

	m_pHairModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), false);
	QubicleMatrix* pHairMatrix = m_pHairModel->GetQubicleMatrix("hair");
	m_pPlayerModel->AddQubicleMatrix(pHairMatrix, false);

	UpdateDefaults();
}

void Player::UpdateDefaults()
{
	string defaultFile = "media/gamedata/hair/male_hair" + to_string(fileNumber) + ".default";

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

		QubicleMatrix* pHairMatrix = m_pHairModel->GetQubicleMatrix("hair");

		pHairMatrix->m_scale = scale;
		pHairMatrix->m_offsetX = offsetX;
		pHairMatrix->m_offsetY = offsetY;
		pHairMatrix->m_offsetZ = offsetZ;

		importFile.close();
	}
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