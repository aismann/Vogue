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

	m_roomDepth = 0;

	UpdateRoomPlanes();
}

Room::~Room()
{
	ClearDoors();
	ClearCorridors();
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

void Room::ClearCorridors()
{
	for (unsigned int i = 0; i < m_vpCorridorList.size(); i++)
	{
		delete m_vpCorridorList[i];
		m_vpCorridorList[i] = 0;
	}
	m_vpCorridorList.clear();
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

	UpdateRoomPlanes();
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

int Room::GetRoomDepth()
{
	return m_roomDepth;
}

void Room::SetRoomDepth(int depth)
{
	m_roomDepth = depth;
}

float Room::GetCorridorLength(eDirection direction)
{
	for (unsigned int i = 0; i < m_vpCorridorList.size(); i++)
	{
		Corridor *pCorridor = m_vpCorridorList[i];

		if (pCorridor->GetDirection() == direction)
		{
			if (direction == eDirection_Up || direction == eDirection_Down)
			{
				return pCorridor->GetWidth();
			}
			if (direction == eDirection_Left || direction == eDirection_Right)
			{
				return pCorridor->GetLength();
			}
		}
	}

	return 0.0f;
}

// Validation
bool Room::IsPointInsideRoom(vec3 point)
{
	float distance;
	int outside = 0;
	int inside = 0;

	for (int i = 0; i < 6; i++)
	{
		distance = m_planes[i].GetPointDistance(point - m_position);

		if (distance < 0.0f)
		{
			// Outside...
			outside++;
		}
		else
		{
			// Inside...
			inside++;
		}
	}

	if (outside == 0)
	{
		return true;
	}

	return false;
}

// Generation
bool Room::CanCreateConnection(eDirection direction)
{
	if (direction == eDirection_NONE)
	{
		return false;
	}
	for (unsigned int i = 0; i < m_vpDoorList.size(); i++)
	{
		Door* pDoor = m_vpDoorList[i];
		if (pDoor->GetDirection() == direction)
		{
			return false;
		}
	}

	return true;
}

void Room::CreateDoor(eDirection direction)
{
	Door* pNewDoor = new Door(m_pRenderer);

	float doorLength = 0.5f;
	float doorWidth = 0.15f;
	float doorHeight = 1.0f;
	vec3 doorPosition;

	if (direction == eDirection_Up)
	{
		doorLength = 0.5f;
		doorWidth = 0.15f;
		doorPosition = vec3(0.0f, 0.0f, -m_width);
	}
	if (direction == eDirection_Down)
	{
		doorLength = 0.5f;
		doorWidth = 0.15f;
		doorPosition = vec3(0.0f, 0.0f, m_width);
	}
	if (direction == eDirection_Left)
	{
		doorLength = 0.15f;
		doorWidth = 0.5f;
		doorPosition = vec3(-m_length, 0.0f, 0.0f);
	}
	if (direction == eDirection_Right)
	{
		doorLength = 0.15f;
		doorWidth = 0.5f;
		doorPosition = vec3(m_length, 0.0f, 0.0f);
	}

	pNewDoor->SetDimensions(doorLength, doorWidth, doorHeight);
	pNewDoor->SetPosition(m_position + doorPosition);
	pNewDoor->SetDirection(direction);

	m_vpDoorList.push_back(pNewDoor);
}

void Room::CreateCorridor(eDirection direction, float corridorLengthAmount)
{
	Corridor* pNewCorrider = new Corridor(m_pRenderer);
	float corridorLength;
	float corridorWidth;
	float corridorHeight = m_height;
	vec3 corridorPosition = GetPosition();
	float constantCorridorWidth = 0.5f;
	if (direction == eDirection_Up || direction == eDirection_Down)
	{
		corridorLength = constantCorridorWidth;
		corridorWidth = corridorLengthAmount * 0.5f;
		if (direction == eDirection_Up)
		{
			corridorPosition += vec3(0.0f, 0.0f, -m_width + -corridorWidth);
		}
		else if (direction == eDirection_Down)
		{
			corridorPosition += vec3(0.0f, 0.0f, m_width + corridorWidth);
		}
	}
	if (direction == eDirection_Left || direction == eDirection_Right)
	{
		corridorLength = corridorLengthAmount * 0.5f;
		corridorWidth = constantCorridorWidth;
		if (direction == eDirection_Left)
		{
			corridorPosition += vec3(-m_length + -corridorLength, 0.0f, 0.0f);
		}
		else if (direction == eDirection_Right)
		{
			corridorPosition += vec3(m_length + corridorLength, 0.0f, 0.0f);
		}
	}
	pNewCorrider->SetDimensions(corridorLength, corridorWidth, corridorHeight);
	pNewCorrider->SetPosition(corridorPosition);
	pNewCorrider->SetDirection(direction);

	m_vpCorridorList.push_back(pNewCorrider);
}

// Update
void Room::Update(float dt)
{
	// Update doors
	for (unsigned int i = 0; i < m_vpDoorList.size(); i++)
	{
		Door* pDoor = m_vpDoorList[i];

		pDoor->Update(dt);
	}

	// Update corridors
	for (unsigned int i = 0; i < m_vpCorridorList.size(); i++)
	{
		Corridor *pCorridor = m_vpCorridorList[i];

		pCorridor->Update(dt);
	}
}

void Room::UpdateRoomPlanes()
{
	m_planes[0] = Plane3D(vec3(-1.0f, 0.0f, 0.0f), vec3(m_length, 0.0f, 0.0f));
	m_planes[1] = Plane3D(vec3(1.0f, 0.0f, 0.0f), vec3(-m_length, 0.0f, 0.0f));
	m_planes[2] = Plane3D(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, m_height, 0.0f));
	m_planes[3] = Plane3D(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, -m_height, 0.0f));
	m_planes[4] = Plane3D(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, m_width));
	m_planes[5] = Plane3D(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -m_width));
}

// Render
void Room::Render()
{
	// Render room
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

	// Rnder corridors
	for (unsigned int i = 0; i < m_vpCorridorList.size(); i++)
	{
		Corridor *pCorridor = m_vpCorridorList[i];

		pCorridor->Render();
	}
}