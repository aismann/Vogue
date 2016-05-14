// ******************************************************************************
// Filename:    Room.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 13/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "Room.h"
#include "RoomManager.h"
#include "../utils/Random.h"


Room::Room(Renderer* pRenderer, RoomManager* pRoomManager)
{
	m_pRenderer = pRenderer;
	m_pRoomManager = pRoomManager;

	m_length = 0.5f;
	m_width = 0.5f;
	m_height = 0.5f;
}

Room::~Room()
{
	ClearDoors();
}

// Clearing
void Room::ClearDoors()
{
	for (unsigned int i = 0; i < m_vpDoorList.size(); i++)
	{
		delete m_vpDoorList[i];
		m_vpDoorList[i] = 0;
	}
	m_vpDoorList.clear();
}

// Accessors
void Room::SetPosition(vec3 pos)
{
	m_position = pos;
}

vec3 Room::GetPosition()
{
	return m_position;
}

void Room::SetDimensions(float length, float width, float height)
{
	m_length = length;
	m_width = width;
	m_height = height;
}

float Room::GetLength()
{
	return m_length;
}

float Room::GetWidth()
{
	return m_width;
}

float Room::GetHeight()
{
	return m_height;
}

// Generation
bool Room::CanCreateDoor(eDirection doorDirection)
{
	if (doorDirection == eDirection_NONE)
	{
		return false;
	}

	eDirection dontAllowDirection = eDirection_NONE;
	if (doorDirection == eDirection_Up)
	{
		dontAllowDirection = eDirection_Down;
	}
	else if (doorDirection == eDirection_Down)
	{
		dontAllowDirection = eDirection_Up;
	}
	else if (doorDirection == eDirection_Left)
	{
		dontAllowDirection = eDirection_Right;
	}
	else if (doorDirection == eDirection_Right)
	{
		dontAllowDirection = eDirection_Left;
	}

	for (unsigned int i = 0; i < m_vpDoorList.size(); i++)
	{
		Door* pDoor = m_vpDoorList[i];
		if (pDoor->GetDirection() == dontAllowDirection)
		{
			return false;
		}
	}

	return true;
}

void Room::CreateDoor(eDirection doorDirection)
{
	Door* pNewDoor = new Door(m_pRenderer);

	float doorLength = 0.5f;
	float doorWidth = 0.15f;
	float doorHeight = 1.0f;
	vec3 doorPosition;

	if (doorDirection == eDirection_Up)
	{
		doorLength = 0.5f;
		doorWidth = 0.15f;
		doorPosition = vec3(0.0f, 0.0f, -m_width);
	}
	if (doorDirection == eDirection_Down)
	{
		doorLength = 0.5f;
		doorWidth = 0.15f;
		doorPosition = vec3(0.0f, 0.0f, m_width);
	}
	if (doorDirection == eDirection_Left)
	{
		doorLength = 0.15f;
		doorWidth = 0.5f;
		doorPosition = vec3(-m_length, 0.0f, 0.0f);
	}
	if (doorDirection == eDirection_Right)
	{
		doorLength = 0.15f;
		doorWidth = 0.5f;
		doorPosition = vec3(m_length, 0.0f, 0.0f);
	}

	pNewDoor->SetDimensions(doorLength, doorWidth, doorHeight);
	pNewDoor->SetPosition(m_position + doorPosition);
	pNewDoor->SetDirection(doorDirection);

	m_vpDoorList.push_back(pNewDoor);
}

// Update
void Room::Update(float dt)
{
	for (unsigned int i = 0; i < m_vpDoorList.size(); i++)
	{
		Door* pDoor = m_vpDoorList[i];

		pDoor->Update(dt);
	}
}

// Render
void Room::Render()
{
	m_pRenderer->SetRenderMode(RM_WIREFRAME);
	m_pRenderer->SetCullMode(CM_NOCULL);
	m_pRenderer->SetLineWidth(1.0f);

	m_pRenderer->PushMatrix();
		m_pRenderer->TranslateWorldMatrix(m_position.x, m_position.y, m_position.z);
		m_pRenderer->ImmediateColourAlpha(1.0f, 1.0f, 1.0f, 1.0f);

		m_pRenderer->EnableImmediateMode(IM_QUADS);
			m_pRenderer->ImmediateNormal(0.0f, 0.0f, -1.0f);
			m_pRenderer->ImmediateVertex(m_length, -m_height, -m_width);
			m_pRenderer->ImmediateVertex(-m_length, -m_height, -m_width);
			m_pRenderer->ImmediateVertex(-m_length, m_height, -m_width);
			m_pRenderer->ImmediateVertex(m_length, m_height, -m_width);

			m_pRenderer->ImmediateNormal(0.0f, 0.0f, 1.0f);
			m_pRenderer->ImmediateVertex(-m_length, -m_height, m_width);
			m_pRenderer->ImmediateVertex(m_length, -m_height, m_width);
			m_pRenderer->ImmediateVertex(m_length, m_height, m_width);
			m_pRenderer->ImmediateVertex(-m_length, m_height, m_width);

			m_pRenderer->ImmediateNormal(1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(m_length, -m_height, m_width);
			m_pRenderer->ImmediateVertex(m_length, -m_height, -m_width);
			m_pRenderer->ImmediateVertex(m_length, m_height, -m_width);
			m_pRenderer->ImmediateVertex(m_length, m_height, m_width);

			m_pRenderer->ImmediateNormal(-1.0f, 0.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-m_length, -m_height, -m_width);
			m_pRenderer->ImmediateVertex(-m_length, -m_height, m_width);
			m_pRenderer->ImmediateVertex(-m_length, m_height, m_width);
			m_pRenderer->ImmediateVertex(-m_length, m_height, -m_width);

			m_pRenderer->ImmediateNormal(0.0f, -1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(-m_length, -m_height, -m_width);
			m_pRenderer->ImmediateVertex(m_length, -m_height, -m_width);
			m_pRenderer->ImmediateVertex(m_length, -m_height, m_width);
			m_pRenderer->ImmediateVertex(-m_length, -m_height, m_width);

			m_pRenderer->ImmediateNormal(0.0f, 1.0f, 0.0f);
			m_pRenderer->ImmediateVertex(m_length, m_height, -m_width);
			m_pRenderer->ImmediateVertex(-m_length, m_height, -m_width);
			m_pRenderer->ImmediateVertex(-m_length, m_height, m_width);
			m_pRenderer->ImmediateVertex(m_length, m_height, m_width);
		m_pRenderer->DisableImmediateMode();
	m_pRenderer->PopMatrix();

	m_pRenderer->SetCullMode(CM_BACK);

	// Render doors
	for (unsigned int i = 0; i < m_vpDoorList.size(); i++)
	{
		Door* pDoor = m_vpDoorList[i];

		pDoor->Render();
	}
}