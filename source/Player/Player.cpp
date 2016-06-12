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

	m_pHeadModel = NULL;
	m_pHairModel = NULL;
	m_pNoseModel = NULL;
	m_pEarsModel = NULL;
	m_pPlayerModel = new QubicleBinary(m_pRenderer);

	// Body parts indices
	m_headNum = 0;
	m_hairNum = 0;
	m_noseNum = 0;
	m_noseNum = 0;
	m_skinColourNum = 0;
	m_hairColourNum = 0;

	MAX_NUM_HEADS = 1;
	MAX_NUM_HAIRS = 21;
	MAX_NUM_NOSES = 5;
	MAX_NUM_EARS = 4;

	LoadSkinColours();
	LoadHairColours();

	// Colour modifiers
	m_colourIdentifierRed[eColourModifiers_Skin]	= 0.76862f;
	m_colourIdentifierGreen[eColourModifiers_Skin]	= 0.65098f;
	m_colourIdentifierBlue[eColourModifiers_Skin]	= 0.41568f;
	m_colourIdentifierRed[eColourModifiers_Hair1]	= 0.00000f;
	m_colourIdentifierGreen[eColourModifiers_Hair1]	= 1.00000f;
	m_colourIdentifierBlue[eColourModifiers_Hair1]	= 0.00000f;
	m_colourIdentifierRed[eColourModifiers_Hair2]	= 0.00000f;
	m_colourIdentifierGreen[eColourModifiers_Hair2]	= 0.00000f;
	m_colourIdentifierBlue[eColourModifiers_Hair2]	= 1.00000f;

	ModifyHead();
	ModifyHair();
	ModifyNose();
	ModifyEars();
	ModifySkinColour();
	ModifyHairColour();
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
	m_skinColourNum = GetRandomNumber(0, MAX_NUM_SKIN_COLOURS-1);
	m_hairColourNum = GetRandomNumber(0, MAX_NUM_HAIR_COLOURS-1);
	ModifyHead();
	ModifyHair();
	ModifyNose();
	ModifyEars();
	ModifySkinColour();
	ModifyHairColour();
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

void Player::LoadSkinColours()
{
	ifstream importFile;
	importFile.open("media/gamedata/colours/skin_colours.txt", ios::in);

	if (importFile.is_open())
	{
		string tempString;
		importFile >> tempString >> MAX_NUM_SKIN_COLOURS;

		m_pSkinColours = new Colour[MAX_NUM_SKIN_COLOURS];
		for (int i = 0; i < MAX_NUM_SKIN_COLOURS; i++)
		{
			float r, g, b;
			importFile >> tempString >> r >> g >> b;

			m_pSkinColours[i].SetRed(r);
			m_pSkinColours[i].SetGreen(g);
			m_pSkinColours[i].SetBlue(b);
		}
	}
}

void Player::LoadHairColours()
{
	ifstream importFile;
	importFile.open("media/gamedata/colours/hair_colours.txt", ios::in);

	if (importFile.is_open())
	{
		string tempString;
		importFile >> tempString >> MAX_NUM_HAIR_COLOURS;

		m_pHair1Colours = new Colour[MAX_NUM_HAIR_COLOURS];
		m_pHair2Colours = new Colour[MAX_NUM_HAIR_COLOURS];
		for (int i = 0; i < MAX_NUM_HAIR_COLOURS; i++)
		{
			float r, g, b;
			importFile >> tempString >> r >> g >> b;

			m_pHair1Colours[i].SetRed(r);
			m_pHair1Colours[i].SetGreen(g);
			m_pHair1Colours[i].SetBlue(b);

			importFile >> tempString >> r >> g >> b;

			m_pHair2Colours[i].SetRed(r);
			m_pHair2Colours[i].SetGreen(g);
			m_pHair2Colours[i].SetBlue(b);
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

	m_skinColourNum++;
	if (m_skinColourNum > MAX_NUM_SKIN_COLOURS-1)
	{
		m_skinColourNum = 0;
	}

	m_colourModifierRed[eColourModifiers_Skin] = m_pSkinColours[m_skinColourNum].GetRed();
	m_colourModifierGreen[eColourModifiers_Skin] = m_pSkinColours[m_skinColourNum].GetGreen();
	m_colourModifierBlue[eColourModifiers_Skin] = m_pSkinColours[m_skinColourNum].GetBlue();
}

void Player::ModifyHairColour()
{
	m_pPlayerModel->SetNullLinkage(m_pHairModel);

	string qubicleFile = "media/gamedata/hair/male_hair" + to_string(m_hairNum) + ".qb";
	m_pHairModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pHairMatrix = m_pHairModel->GetQubicleMatrix("hair");
	m_pPlayerModel->AddQubicleMatrix(pHairMatrix, false);

	m_hairColourNum++;
	if (m_hairColourNum > MAX_NUM_HAIR_COLOURS-1)
	{
		m_hairColourNum = 0;
	}

	m_colourModifierRed[eColourModifiers_Hair1] = m_pHair1Colours[m_hairColourNum].GetRed();
	m_colourModifierGreen[eColourModifiers_Hair1] = m_pHair1Colours[m_hairColourNum].GetGreen();
	m_colourModifierBlue[eColourModifiers_Hair1] = m_pHair1Colours[m_hairColourNum].GetBlue();
	m_colourModifierRed[eColourModifiers_Hair2] = m_pHair2Colours[m_hairColourNum].GetRed();
	m_colourModifierGreen[eColourModifiers_Hair2] = m_pHair2Colours[m_hairColourNum].GetGreen();
	m_colourModifierBlue[eColourModifiers_Hair2] = m_pHair2Colours[m_hairColourNum].GetBlue();
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