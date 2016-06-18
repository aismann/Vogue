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

#pragma warning(disable: 4800) // Disable 'casting to bool' warning C4800

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
	m_pFacialHairModel = NULL;
	m_pNoseModel = NULL;
	m_pEarsModel = NULL;
	m_pBodyModel = NULL;
	m_pLegsModel = NULL;
	m_pRightHandModel = NULL;
	m_pLeftHandModel = NULL;
	m_pRightShoulderModel = NULL;
	m_pLeftShoulderModel = NULL;
	m_pRightFootModel = NULL;
	m_pLeftFootModel = NULL;

	m_pVoxelCharacter = new VoxelCharacter(m_pRenderer, m_pQubicleBinaryManager);
	m_pVoxelCharacter->UnloadCharacter();
	m_pVoxelCharacter->Reset();

	char characterBaseFolder[128];
	char qbFilename[128];
	char ms3dFilename[128];
	char animListFilename[128];
	char facesFilename[128];
	char characterFilename[128];
	sprintf(characterBaseFolder, "media/gamedata/models");
	sprintf(qbFilename, "media/gamedata/models/%s/%s.qb", "human", "base_human1");
	sprintf(ms3dFilename, "media/gamedata/models/%s/%s.ms3d", "human", "human");
	sprintf(animListFilename, "media/gamedata/models/%s/%s.animlist", "human", "human");
	sprintf(facesFilename, "media/gamedata/models/%s/%s.faces", "human", "base_human1");
	sprintf(characterFilename, "media/gamedata/models/%s/%s.character", "human", "base_human1");

	m_pVoxelCharacter->LoadVoxelCharacter("human", qbFilename, ms3dFilename, animListFilename, facesFilename, characterFilename, characterBaseFolder, false);
	m_pVoxelCharacter->SetBreathingAnimationEnabled(false);
	m_pVoxelCharacter->SetWinkAnimationEnabled(true);
	m_pVoxelCharacter->SetTalkingAnimationEnabled(false);
	m_pVoxelCharacter->SetRandomMouthSelection(false);
	m_pVoxelCharacter->SetRandomLookDirection(false);
	m_pVoxelCharacter->SetWireFrameRender(false);
	m_pVoxelCharacter->SetCharacterScale(0.08f);

	// Body parts indices
	m_headNum = 0;
	m_hairNum = 0;
	m_facialHairNum = 0;
	m_earsNum = 0;
	m_noseNum = 0;
	m_eyesNum = 0;
	m_bodyNum = 0;
	m_legsNum = 0;
	m_rightHandNum = 0;
	m_leftHandNum = 0;
	m_rightShoulderNum = 0;
	m_leftShoulderNum = 0;
	m_rightFootNum = 0;
	m_leftFootNum = 0;
	m_skinColourNum = 0;
	m_hairColourNum = 0;

	m_playerSex = ePlayerSex_Female;

	m_hairColourSwap = false;

	MAX_NUM_HEADS = 1;
	MAX_NUM_HAIRS_MALE = 22;
	MAX_NUM_HAIRS_FEMALE = 10;
	MAX_NUM_FACIAL_HAIRS = 9;
	MAX_NUM_NOSES = 6;
	MAX_NUM_EARS = 4;
	MAX_NUM_EYES = 7;
	MAX_NUM_BODY = 2;
	MAX_NUM_LEGS = 2;
	MAX_NUM_RIGHT_HAND = 1;
	MAX_NUM_LEFT_HAND = 1;
	MAX_NUM_RIGHT_SHOULDER = 2;
	MAX_NUM_LEFT_SHOULDER = 2;
	MAX_NUM_RIGHT_FOOT = 2;
	MAX_NUM_LEFT_FOOT = 2;

	LoadSkinColours();
	LoadHairColours();
	LoadEyesNames();

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
	ModifyFacialHair();
	ModifyNose();
	ModifyEars();
	ModifyEyes();
	ModifyBody();
	ModifyLegs();
	ModifyRightHand();
	ModifyLeftHand();
	ModifyRightShoulder();
	ModifyLeftShoulder();
	ModifyRightFoot();
	ModifyLeftFoot();
	ModifySkinColour();
	ModifyHairColour();
	UpdateDefaults();
	SetColourModifiers();
}

Player::~Player()
{
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pHeadModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pHairModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pFacialHairModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pNoseModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pEarsModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pBodyModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLegsModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pRightHandModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLeftHandModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pRightShoulderModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLeftShoulderModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pRightFootModel);
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLeftFootModel);

	delete m_pVoxelCharacter;
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

void Player::LoadEyesNames()
{
	m_pEyesNames = new string[MAX_NUM_EYES];
	m_pEyesNames[0] = "eyes_brown";
	m_pEyesNames[1] = "eyes_blue";
	m_pEyesNames[2] = "eyes_gray";
	m_pEyesNames[3] = "eyes_green";
	m_pEyesNames[4] = "eyes_orange";
	m_pEyesNames[5] = "eyes_purple";
	m_pEyesNames[6] = "eyes_red";
}

void Player::ModifyHead()
{
	m_headNum++;
	if (m_headNum > MAX_NUM_HEADS)
	{
		m_headNum = 1;
	}

	ReplaceHead();
}

void Player::ModifyHair()
{
	m_hairNum++;
	if(m_playerSex == ePlayerSex_Male)
	{
		if (m_hairNum > MAX_NUM_HAIRS_MALE)
		{
			m_hairNum = 1;
		}
	}
	else
	{
		if (m_hairNum > MAX_NUM_HAIRS_FEMALE)
		{
			m_hairNum = 1;
		}
	}

	ReplaceHair();
}

void Player::ModifyFacialHair()
{
	m_facialHairNum++;
	if (m_facialHairNum > MAX_NUM_FACIAL_HAIRS)
	{
		m_facialHairNum = 1;
	}

	if (m_playerSex == ePlayerSex_Female) // Force females to have no facial hair
	{
		m_facialHairNum = 1;
	}

	ReplaceFacialHair();
}

void Player::ModifyNose()
{
	m_noseNum++;
	if (m_noseNum > MAX_NUM_NOSES)
	{
		m_noseNum = 1;
	}

	ReplaceNose();
}

void Player::ModifyEars()
{
	m_earsNum++;
	if (m_earsNum > MAX_NUM_EARS)
	{
		m_earsNum = 1;
	}

	ReplaceEars();
}

void Player::ModifyEyes()
{
	m_eyesNum++;
	if (m_eyesNum > MAX_NUM_EYES-1)
	{
		m_eyesNum = 0;
	}

	ReplaceEyes();
}

void Player::ModifyBody()
{
	m_bodyNum++;
	if (m_bodyNum > MAX_NUM_BODY)
	{
		m_bodyNum = 1;
	}

	ReplaceBody();
}

void Player::ModifyLegs()
{
	m_legsNum++;
	if (m_legsNum > MAX_NUM_LEGS)
	{
		m_legsNum = 1;
	}

	ReplaceLegs();
}
void Player::ModifyRightHand()
{
	m_rightHandNum++;
	if (m_rightHandNum > MAX_NUM_RIGHT_HAND)
	{
		m_rightHandNum = 1;
	}

	ReplaceRightHand();
}

void Player::ModifyLeftHand()
{
	m_leftHandNum++;
	if (m_leftHandNum > MAX_NUM_LEFT_HAND)
	{
		m_leftHandNum = 1;
	}

	ReplaceLeftHand();
}

void Player::ModifyRightShoulder()
{
	m_rightShoulderNum++;
	if (m_rightShoulderNum > MAX_NUM_RIGHT_SHOULDER)
	{
		m_rightShoulderNum = 1;
	}

	ReplaceRightShoulder();
}

void Player::ModifyLeftShoulder()
{
	m_leftShoulderNum++;
	if (m_leftShoulderNum > MAX_NUM_LEFT_SHOULDER)
	{
		m_leftShoulderNum = 1;
	}

	ReplaceLeftShoulder();
}

void Player::ModifyRightFoot()
{
	m_rightFootNum++;
	if (m_rightFootNum > MAX_NUM_RIGHT_FOOT)
	{
		m_rightFootNum = 1;
	}

	ReplaceRightFoot();
}

void Player::ModifyLeftFoot()
{
	m_leftFootNum++;
	if (m_leftFootNum > MAX_NUM_LEFT_FOOT)
	{
		m_leftFootNum = 1;
	}

	ReplaceLeftFoot();
}

void Player::ReplaceHead()
{
	// Replace the head model on the player model
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pHeadModel);
	string qubicleFile = "media/gamedata/head/base_head" + to_string(m_headNum) + ".qb";
	m_pHeadModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pHeadMatrix = m_pHeadModel->GetQubicleMatrix("Head");
	pHeadMatrix->m_boneIndex = m_pVoxelCharacter->GetHeadBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pHeadMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceHair()
{
	// Replace the hair model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pHairModel);
	string qubicleFile;
	if (m_playerSex == ePlayerSex_Male)
	{
		qubicleFile = "media/gamedata/hair/male_hair" + to_string(m_hairNum) + ".qb";
	}
	else
	{
		qubicleFile = "media/gamedata/hair/female_hair" + to_string(m_hairNum) + ".qb";
	}
	m_pHairModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pHairMatrix = m_pHairModel->GetQubicleMatrix("hair");
	pHairMatrix->m_boneIndex = m_pVoxelCharacter->GetHeadBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pHairMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceFacialHair()
{
	// Replace the facial hair model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pFacialHairModel);
	string qubicleFile = "media/gamedata/facial_hair/facial_hair" + to_string(m_facialHairNum) + ".qb";
	m_pFacialHairModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pFacialHairMatrix = m_pFacialHairModel->GetQubicleMatrix("Facial_Hair");
	pFacialHairMatrix->m_boneIndex = m_pVoxelCharacter->GetHeadBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pFacialHairMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceNose()
{
	// Replace the nose model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pNoseModel);
	string qubicleFile = "media/gamedata/nose/nose" + to_string(m_noseNum) + ".qb";
	m_pNoseModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pNoseMatrix = m_pNoseModel->GetQubicleMatrix("nose");
	pNoseMatrix->m_boneIndex = m_pVoxelCharacter->GetHeadBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pNoseMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceEars()
{
	// Replace the ears model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pEarsModel);
	string qubicleFile = "media/gamedata/ears/ears" + to_string(m_earsNum) + ".qb";
	m_pEarsModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pEarsMatrix = m_pEarsModel->GetQubicleMatrix("ears");
	pEarsMatrix->m_boneIndex = m_pVoxelCharacter->GetHeadBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pEarsMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceEyes()
{
	m_pVoxelCharacter->ModifyEyesTextures("media/gamedata/models", "human", m_pEyesNames[m_eyesNum].c_str());
}

void Player::ReplaceBody()
{
	// Replace the body model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pBodyModel);
	string qubicleFile = "media/gamedata/body/male_body" + to_string(m_bodyNum) + ".qb";
	m_pBodyModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pBodyMatrix = m_pBodyModel->GetQubicleMatrix("Body");
	pBodyMatrix->m_boneIndex = m_pVoxelCharacter->GetBodyBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pBodyMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceLegs()
{
	// Replace the legs model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLegsModel);
	string qubicleFile = "media/gamedata/legs/male_legs" + to_string(m_legsNum) + ".qb";
	m_pLegsModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pLegsMatrix = m_pLegsModel->GetQubicleMatrix("Legs");
	pLegsMatrix->m_boneIndex = m_pVoxelCharacter->GetLegsBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pLegsMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceRightHand()
{
	// Replace the right hand model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pRightHandModel);
	string qubicleFile = "media/gamedata/right_hand/right_hand" + to_string(m_rightHandNum) + ".qb";
	m_pRightHandModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pRightHandMatrix = m_pRightHandModel->GetQubicleMatrix("Right_Hand");
	pRightHandMatrix->m_boneIndex = m_pVoxelCharacter->GetRightHandBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pRightHandMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceLeftHand()
{
	// Replace the left hand model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLeftHandModel);
	string qubicleFile = "media/gamedata/left_hand/left_hand" + to_string(m_leftHandNum) + ".qb";
	m_pLeftHandModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pLeftHandMatrix = m_pLeftHandModel->GetQubicleMatrix("Left_Hand");
	pLeftHandMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftHandBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pLeftHandMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceRightShoulder()
{
	// Replace the right shoulder model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pRightShoulderModel);
	string qubicleFile = "media/gamedata/right_shoulder/right_shoulder" + to_string(m_rightShoulderNum) + ".qb";
	m_pRightShoulderModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pRightShoulderMatrix = m_pRightShoulderModel->GetQubicleMatrix("Right_Shoulder");
	pRightShoulderMatrix->m_boneIndex = m_pVoxelCharacter->GetRightShoulderBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pRightShoulderMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceLeftShoulder()
{
	// Replace the left shoulder model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLeftShoulderModel);
	string qubicleFile = "media/gamedata/left_shoulder/left_shoulder" + to_string(m_leftShoulderNum) + ".qb";
	m_pLeftShoulderModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pLeftShoulderMatrix = m_pLeftShoulderModel->GetQubicleMatrix("Left_Shoulder");
	pLeftShoulderMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftShoulderBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pLeftShoulderMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceRightFoot()
{
	// Replace the right foot model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pRightFootModel);
	string qubicleFile = "media/gamedata/right_foot/right_foot" + to_string(m_rightFootNum) + ".qb";
	m_pRightFootModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pRightFootMatrix = m_pRightFootModel->GetQubicleMatrix("Right_Foot");
	pRightFootMatrix->m_boneIndex = m_pVoxelCharacter->GetRightFootBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pRightFootMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::ReplaceLeftFoot()
{
	// Replace the left foot model on the player
	m_pVoxelCharacter->GetQubicleModel()->SetNullLinkage(m_pLeftFootModel);
	string qubicleFile = "media/gamedata/left_foot/left_foot" + to_string(m_leftFootNum) + ".qb";
	m_pLeftFootModel = m_pQubicleBinaryManager->GetQubicleBinaryFile(qubicleFile.c_str(), true);
	QubicleMatrix* pLeftFootMatrix = m_pLeftFootModel->GetQubicleMatrix("Left_Foot");
	pLeftFootMatrix->m_boneIndex = m_pVoxelCharacter->GetLeftFootBoneIndex();
	m_pVoxelCharacter->AddQubicleMatrix(pLeftFootMatrix, false);
	m_pVoxelCharacter->SetupFacesBones(); // Need to resetup since the model matrix indiceswill have changed
}

void Player::RandomizeParts()
{
	m_headNum = GetRandomNumber(0, MAX_NUM_HEADS);
	m_hairNum = GetRandomNumber(0, (m_playerSex == ePlayerSex_Male) ? MAX_NUM_HAIRS_MALE : MAX_NUM_HAIRS_FEMALE);
	m_facialHairNum = GetRandomNumber(0, MAX_NUM_FACIAL_HAIRS);
	m_noseNum = GetRandomNumber(0, MAX_NUM_NOSES);
	m_earsNum = GetRandomNumber(0, MAX_NUM_EARS);
	m_eyesNum = GetRandomNumber(0, MAX_NUM_EYES);
	m_bodyNum = GetRandomNumber(0, MAX_NUM_BODY);
	m_legsNum = GetRandomNumber(0, MAX_NUM_LEGS);
	m_rightHandNum = GetRandomNumber(0, MAX_NUM_RIGHT_HAND);
	m_leftHandNum = GetRandomNumber(0, MAX_NUM_LEFT_HAND);
	m_rightShoulderNum = GetRandomNumber(0, MAX_NUM_RIGHT_SHOULDER);
	m_leftShoulderNum = GetRandomNumber(0, MAX_NUM_LEFT_SHOULDER);
	m_rightFootNum = GetRandomNumber(0, MAX_NUM_RIGHT_FOOT);
	m_leftFootNum = GetRandomNumber(0, MAX_NUM_LEFT_FOOT);
	m_skinColourNum = GetRandomNumber(0, MAX_NUM_SKIN_COLOURS-1);
	m_hairColourNum = GetRandomNumber(0, MAX_NUM_HAIR_COLOURS-1);
	m_hairColourSwap = (bool)GetRandomNumber(0, 1);

	ModifyHead();
	ModifyHair();
	ModifyFacialHair();
	ModifyNose();
	ModifyEars();
	ModifyEyes();
	ModifyBody();
	ModifyLegs();
	ModifyRightHand();
	ModifyLeftHand();
	ModifyRightShoulder();
	ModifyLeftShoulder();
	ModifyRightFoot();
	ModifyLeftFoot();
}

// Default scale and offsets
void Player::UpdateDefaults()
{
	string defaultFile = "";
	QubicleMatrix* pMatrix = NULL;
	for (int i = 0; i < 13; i++)
	{
		if (i == 0)
		{
			defaultFile = "media/gamedata/head/base_head" + to_string(m_headNum) + ".default";
			pMatrix = m_pHeadModel->GetQubicleMatrix("Head");
		}
		if (i == 1)
		{
			if (m_playerSex == ePlayerSex_Male)
			{
				defaultFile = "media/gamedata/hair/male_hair" + to_string(m_hairNum) + ".default";
			}
			else
			{
				defaultFile = "media/gamedata/hair/female_hair" + to_string(m_hairNum) + ".default";
			}
			pMatrix = m_pHairModel->GetQubicleMatrix("hair");
		}
		if (i == 2)
		{
			defaultFile = "media/gamedata/facial_hair/facial_hair" + to_string(m_facialHairNum) + ".default";
			pMatrix = m_pFacialHairModel->GetQubicleMatrix("Facial_Hair");
		}
		if (i == 3)
		{
			defaultFile = "media/gamedata/nose/nose" + to_string(m_noseNum) + ".default";
			pMatrix = m_pNoseModel->GetQubicleMatrix("nose");
		}
		if (i == 4)
		{
			defaultFile = "media/gamedata/ears/ears" + to_string(m_noseNum) + ".default";
			pMatrix = m_pEarsModel->GetQubicleMatrix("ears");
		}
		if (i == 5)
		{
			defaultFile = "media/gamedata/body/male_body" + to_string(m_bodyNum) + ".default";
			pMatrix = m_pBodyModel->GetQubicleMatrix("Body");
		}
		if (i == 6)
		{
			defaultFile = "media/gamedata/legs/male_legs" + to_string(m_legsNum) + ".default";
			pMatrix = m_pLegsModel->GetQubicleMatrix("Legs");
		}
		if (i == 7)
		{
			defaultFile = "media/gamedata/right_hand/right_hand" + to_string(m_rightHandNum) + ".default";
			pMatrix = m_pRightHandModel->GetQubicleMatrix("Right_Hand");
		}
		if (i == 8)
		{
			defaultFile = "media/gamedata/left_hand/left_hand" + to_string(m_leftHandNum) + ".default";
			pMatrix = m_pLeftHandModel->GetQubicleMatrix("Left_Hand");
		}
		if (i == 9)
		{
			defaultFile = "media/gamedata/right_shoulder/right_shoulder" + to_string(m_rightShoulderNum) + ".default";
			pMatrix = m_pRightShoulderModel->GetQubicleMatrix("Right_Shoulder");
		}
		if (i == 10)
		{
			defaultFile = "media/gamedata/left_shoulder/left_shoulder" + to_string(m_leftShoulderNum) + ".default";
			pMatrix = m_pLeftShoulderModel->GetQubicleMatrix("Left_Shoulder");
		}
		if (i == 11)
		{
			defaultFile = "media/gamedata/right_foot/right_foot" + to_string(m_rightFootNum) + ".default";
			pMatrix = m_pRightFootModel->GetQubicleMatrix("Right_Foot");
		}
		if (i == 12)
		{
			defaultFile = "media/gamedata/left_foot/left_foot" + to_string(m_leftFootNum) + ".default";
			pMatrix = m_pLeftFootModel->GetQubicleMatrix("Left_Foot");
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

// Colour modifiers
void Player::ModifySkinColour()
{
	ReplaceHead();
	ReplaceNose();
	ReplaceEars();
	ReplaceBody();
	ReplaceLegs();
	ReplaceRightHand();
	ReplaceLeftHand();
	ReplaceRightShoulder();
	ReplaceLeftShoulder();
	ReplaceRightFoot();
	ReplaceLeftFoot();

	m_skinColourNum++;
	if (m_skinColourNum > MAX_NUM_SKIN_COLOURS-1)
	{
		m_skinColourNum = 0;
	}
}

void Player::ModifyHairColour()
{
	ReplaceHair();
	ReplaceFacialHair();

	m_hairColourNum++;
	if (m_hairColourNum > MAX_NUM_HAIR_COLOURS-1)
	{
		m_hairColourNum = 0;
	}
}

void Player::SwapHairColours()
{
	ReplaceHair();
	ReplaceFacialHair();

	m_hairColourSwap = !m_hairColourSwap;
}

void Player::SetColourModifiers()
{
	// SKin colour
	m_colourModifierRed[eColourModifiers_Skin] = m_pSkinColours[m_skinColourNum].GetRed();
	m_colourModifierGreen[eColourModifiers_Skin] = m_pSkinColours[m_skinColourNum].GetGreen();
	m_colourModifierBlue[eColourModifiers_Skin] = m_pSkinColours[m_skinColourNum].GetBlue();

	// Hair colours
	int colour1 = eColourModifiers_Hair1;
	int colour2 = eColourModifiers_Hair2;
	if (m_hairColourSwap == true)
	{
		colour1 = eColourModifiers_Hair2;
		colour2 = eColourModifiers_Hair1;
	}
	m_colourModifierRed[colour1] = m_pHair1Colours[m_hairColourNum].GetRed();
	m_colourModifierGreen[colour1] = m_pHair1Colours[m_hairColourNum].GetGreen();
	m_colourModifierBlue[colour1] = m_pHair1Colours[m_hairColourNum].GetBlue();
	m_colourModifierRed[colour2] = m_pHair2Colours[m_hairColourNum].GetRed();
	m_colourModifierGreen[colour2] = m_pHair2Colours[m_hairColourNum].GetGreen();
	m_colourModifierBlue[colour2] = m_pHair2Colours[m_hairColourNum].GetBlue();

	for (int i = 0; i < eColourModifiers_NUM; i++)
	{
		m_pVoxelCharacter->ConvertMeshColour(m_colourModifierRed[i], m_colourModifierGreen[i], m_colourModifierBlue[i], m_colourIdentifierRed[i], m_colourIdentifierGreen[i], m_colourIdentifierBlue[i]);
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
	// Update the voxel model
	float animationSpeeds[AnimationSections_NUMSECTIONS] = { 1.0f, 1.0f, 1.0f, 1.0f, 1.0f };
	m_pVoxelCharacter->Update(dt, animationSpeeds);
}

// Render
void Player::Render()
{
	Colour OulineColour(1.0f, 1.0f, 0.0f, 1.0f);

	m_pRenderer->PushMatrix();
		m_pRenderer->MultiplyWorldMatrix(m_worldMatrix);

		m_pVoxelCharacter->Render(false, false, false, OulineColour, false);
		m_pVoxelCharacter->RenderWeapons(false, false, false, OulineColour);
	m_pRenderer->PopMatrix();
}

void Player::RenderFace()
{
	m_pRenderer->PushMatrix();
		m_pRenderer->MultiplyWorldMatrix(m_worldMatrix);

		m_pVoxelCharacter->RenderFace();
	m_pRenderer->PopMatrix();
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