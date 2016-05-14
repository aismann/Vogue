// ******************************************************************************
// Filename:    RoomManager.cpp
// Project:     Vox
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 13/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "RoomManager.h"
#include "../utils/Random.h"


RoomManager::RoomManager(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

RoomManager::~RoomManager()
{
	ClearRooms();
}

// Clearing
void RoomManager::ClearRooms()
{
	for (unsigned int i = 0; i < m_vpRoomList.size(); i++)
	{
		delete m_vpRoomList[i];
		m_vpRoomList[i] = 0;
	}
	m_vpRoomList.clear();
}

// Generation
void RoomManager::GenerateNewLayout()
{
	ClearRooms();
	CreateRandomRoom(NULL, eDirection_NONE, 0);
}

void RoomManager::CreateRandomRoom(Room* pRoomConnection, eDirection connectedDirection, int roomDepth)
{
	Room* pNewRoom = new Room(m_pRenderer, this);

	float length = GetRandomNumber(30, 80, 2) * 0.1f;
	float width = GetRandomNumber(30, 80, 2) * 0.1f;
	float height = 1.0f;

	pNewRoom->SetDimensions(length, width, height);

	if (pRoomConnection != NULL)
	{
		vec3 newRoomPosition = pRoomConnection->GetPosition();
		if (connectedDirection == eDirection_Up)
		{
			newRoomPosition -= vec3(0.0f, 0.0f, width + pRoomConnection->GetWidth());
		}
		if (connectedDirection == eDirection_Down)
		{
			newRoomPosition += vec3(0.0f, 0.0f, width + pRoomConnection->GetWidth());
		}
		if (connectedDirection == eDirection_Left)
		{
			newRoomPosition -= vec3(length + pRoomConnection->GetLength(), 0.0f, 0.0f);
		}
		if (connectedDirection == eDirection_Right)
		{
			newRoomPosition += vec3(length + pRoomConnection->GetLength(), 0.0f, 0.0f);
		}

		pNewRoom->SetPosition(newRoomPosition);
	}

	if (roomDepth < 2)
	{
		eDirection dontAllowDirection = eDirection_NONE;
		if (connectedDirection == eDirection_Up)
		{
			dontAllowDirection = eDirection_Down;
		}
		else if (connectedDirection == eDirection_Down)
		{
			dontAllowDirection = eDirection_Up;
		}
		else if (connectedDirection == eDirection_Left)
		{
			dontAllowDirection = eDirection_Right;
		}
		else if (connectedDirection == eDirection_Right)
		{
			dontAllowDirection = eDirection_Left;
		}
		eDirection doorDirection = dontAllowDirection;
		while(doorDirection == dontAllowDirection)
		{
			doorDirection = (eDirection)GetRandomNumber(0, 3);
		}
		pNewRoom->CreateDoor(doorDirection);

		CreateRandomRoom(pNewRoom, doorDirection, roomDepth+1);
	}

	m_vpRoomList.push_back(pNewRoom);
}

void RoomManager::CreateConnectedRoom()
{
	int randomRoomIndex = GetRandomNumber(0, (int)m_vpRoomList.size());

	Room* pRoom = m_vpRoomList[randomRoomIndex];
}

// Update
void RoomManager::Update(float dt)
{
	for (unsigned int i = 0; i < m_vpRoomList.size(); i++)
	{
		Room *pRoom = m_vpRoomList[i];

		pRoom->Update(dt);
	}
}

// Render
void RoomManager::Render()
{
	m_pRenderer->PushMatrix();
		for (unsigned int i = 0; i < m_vpRoomList.size(); i++)
		{
			Room *pRoom = m_vpRoomList[i];

			pRoom->Render();
		}
	m_pRenderer->PopMatrix();
}