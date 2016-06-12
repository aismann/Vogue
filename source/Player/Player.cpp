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
	m_noseNum = 0;

	// Colour modifiers
	m_colourIdentifierRed[eColourModifiers_Skin] = 0.7686274509803922f;
	m_colourIdentifierGreen[eColourModifiers_Skin] = 0.6509803921568627f;
	m_colourIdentifierBlue[eColourModifiers_Skin] = 0.4156862745098039f;
	m_colourIdentifierRed[eColourModifiers_Hair1] = 0.0f;
	m_colourIdentifierGreen[eColourModifiers_Hair1] = 1.0f;
	m_colourIdentifierBlue[eColourModifiers_Hair1] = 0.0f;
	m_colourIdentifierRed[eColourModifiers_Hair2] = 0.0f;
	m_colourIdentifierGreen[eColourModifiers_Hair2] = 0.0f;
	m_colourIdentifierBlue[eColourModifiers_Hair2] = 1.0f;

	// Default colours
	m_colourModifierRed[eColourModifiers_Skin] = 0.7686274509803922f;
	m_colourModifierGreen[eColourModifiers_Skin] = 0.6509803921568627f;
	m_colourModifierBlue[eColourModifiers_Skin] = 0.4156862745098039f;
	m_colourModifierRed[eColourModifiers_Hair1] = 0.2470588235294118f;
	m_colourModifierGreen[eColourModifiers_Hair1] = 0.1450980392156863f;
	m_colourModifierBlue[eColourModifiers_Hair1] = 0.0f;
	m_colourModifierRed[eColourModifiers_Hair2] = 0.3490196078431373f;
	m_colourModifierGreen[eColourModifiers_Hair2] = 0.207843137254902f;
	m_colourModifierBlue[eColourModifiers_Hair2] = 0.007843137254902f;

	m_pHeadModel = NULL;
	m_pHairModel = NULL;
	m_pNoseModel = NULL;
	m_pEarsModel = NULL;
	m_pPlayerModel = new QubicleBinary(m_pRenderer);

	ModifyHead();
	ModifyHair();
	ModifyNose();
	ModifyEars();
	UpdateDefaults();
	SetColourModifiers();
}

Player::~Player()
{
	m_pPlayerModel->SetNullLinkage(m_pHeadModel);
	m_pPlayerModel->SetNullLinkage(m_pHairModel);
	m_pPlayerModel->SetNullLinkage(m_pNoseModel);
	m_pPlayerModel->SetNullLinkage(m_pEarsModel);
}

static int const MAX_NUM_HEADS = 1;
static int const MAX_NUM_HAIRS = 21;
static int const MAX_NUM_NOSES = 5;
static int const MAX_NUM_EARS = 4;

void Player::ModifyHead()
{
	m_headNum++;
	if (m_headNum > MAX_NUM_HEADS)
	{
		m_headNum = 1;
	}

	m_pPlayerModel->SetNullLinkage(m_pHeadModel);
	string qubicleFile = "media/gamedata/head/base_head" + to_string(m_headNum) + ".qb";
	m_pHeadModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
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

	m_pPlayerModel->SetNullLinkage(m_pHairModel);
	string qubicleFile = "media/gamedata/hair/male_hair" + to_string(m_hairNum) + ".qb";
	m_pHairModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
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

	m_pPlayerModel->SetNullLinkage(m_pNoseModel);
	string qubicleFile = "media/gamedata/nose/nose" + to_string(m_noseNum) + ".qb";
	m_pNoseModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pNoseMatrix = m_pNoseModel->GetQubicleMatrix("nose");
	m_pPlayerModel->AddQubicleMatrix(pNoseMatrix, false);
}

void Player::ModifyEars()
{
	m_earsNum++;
	if (m_earsNum > MAX_NUM_EARS)
	{
		m_earsNum = 1;
	}

	m_pPlayerModel->SetNullLinkage(m_pEarsModel);
	string qubicleFile = "media/gamedata/ears/ears" + to_string(m_earsNum) + ".qb";
	m_pEarsModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pEarsMatrix = m_pEarsModel->GetQubicleMatrix("ears");
	m_pPlayerModel->AddQubicleMatrix(pEarsMatrix, false);
}

void Player::RandomizeParts()
{
	m_headNum = GetRandomNumber(0, MAX_NUM_HEADS);
	m_hairNum = GetRandomNumber(0, MAX_NUM_HAIRS);
	m_noseNum = GetRandomNumber(0, MAX_NUM_NOSES);
	m_earsNum = GetRandomNumber(0, MAX_NUM_EARS);
	ModifyHead();
	ModifyHair();
	ModifyNose();
	ModifyEars();
}

void Player::UpdateDefaults()
{
	string defaultFile = "";
	QubicleMatrix* pMatrix = NULL;
	for (int i = 0; i < 4; i++)
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
		if (i == 3)
		{
			defaultFile = "media/gamedata/ears/ears" + to_string(m_noseNum) + ".default";
			pMatrix = m_pEarsModel->GetQubicleMatrix("ears");
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

void Player::ModifySkinColour()
{
	m_pPlayerModel->SetNullLinkage(m_pHeadModel);
	m_pPlayerModel->SetNullLinkage(m_pNoseModel);
	m_pPlayerModel->SetNullLinkage(m_pEarsModel);

	string qubicleFile = "media/gamedata/head/base_head" + to_string(m_headNum) + ".qb";
	QubicleBinary* pHeadModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pHeadMatrix = pHeadModel->GetQubicleMatrix("head");
	m_pPlayerModel->AddQubicleMatrix(pHeadMatrix, false);

	qubicleFile = "media/gamedata/nose/nose" + to_string(m_noseNum) + ".qb";
	QubicleBinary* pNoseModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pNoseMatrix = pNoseModel->GetQubicleMatrix("nose");
	m_pPlayerModel->AddQubicleMatrix(pNoseMatrix, false);

	qubicleFile = "media/gamedata/ears/ears" + to_string(m_earsNum) + ".qb";
	m_pEarsModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pEarsMatrix = m_pEarsModel->GetQubicleMatrix("ears");
	m_pPlayerModel->AddQubicleMatrix(pEarsMatrix, false);

	m_colourModifierRed[eColourModifiers_Skin] = GetRandomNumber(0, 1, 2);
	m_colourModifierGreen[eColourModifiers_Skin] = GetRandomNumber(0, 1, 2);
	m_colourModifierBlue[eColourModifiers_Skin] = GetRandomNumber(0, 1, 2);
}

void Player::ModifyHairColour()
{
	m_pPlayerModel->SetNullLinkage(m_pHairModel);

	string qubicleFile = "media/gamedata/hair/male_hair" + to_string(m_hairNum) + ".qb";
	m_pHairModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pHairMatrix = m_pHairModel->GetQubicleMatrix("hair");
	m_pPlayerModel->AddQubicleMatrix(pHairMatrix, false);

	m_colourModifierRed[eColourModifiers_Hair1] = GetRandomNumber(0, 1, 2);
	m_colourModifierGreen[eColourModifiers_Hair1] = GetRandomNumber(0, 1, 2);
	m_colourModifierBlue[eColourModifiers_Hair1] = GetRandomNumber(0, 1, 2);
	m_colourModifierRed[eColourModifiers_Hair2] = GetRandomNumber(0, 1, 2);
	m_colourModifierGreen[eColourModifiers_Hair2] = GetRandomNumber(0, 1, 2);
	m_colourModifierBlue[eColourModifiers_Hair2] = GetRandomNumber(0, 1, 2);
}

void Player::SetColourModifiers()
{
	for (int i = 0; i < eColourModifiers_NUM; i++)
	{
		m_pPlayerModel->ConvertMeshColour(m_colourModifierRed[i], m_colourModifierGreen[i], m_colourModifierBlue[i], m_colourIdentifierRed[i], m_colourIdentifierGreen[i], m_colourIdentifierBlue[i]);
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