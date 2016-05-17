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

	float roomLength = GetRandomNumber(30, 80, 2) * 0.1f;
	float roomWidth = GetRandomNumber(30, 80, 2) * 0.1f;
	float roomHeight = 1.0f;

	pNewRoom->SetDimensions(roomLength, roomWidth, roomHeight);

	eDirection dontAllowDirection = eDirection_NONE;

	// If we are connected to a room, set our position
	if (pRoomConnection != NULL)
	{
		vec3 newRoomPosition = pRoomConnection->GetPosition();
		if (connectedDirection == eDirection_Up)
		{
			newRoomPosition -= vec3(0.0f, 0.0f, pRoomConnection->GetWidth() + pRoomConnection->GetCorridorLength(connectedDirection)*2.0f + roomWidth);
			dontAllowDirection = eDirection_Down;
		}
		if (connectedDirection == eDirection_Down)
		{
			newRoomPosition += vec3(0.0f, 0.0f, pRoomConnection->GetWidth() + pRoomConnection->GetCorridorLength(connectedDirection)*2.0f + roomWidth);
			dontAllowDirection = eDirection_Up;
		}
		if (connectedDirection == eDirection_Left)
		{
			newRoomPosition -= vec3(pRoomConnection->GetLength() + pRoomConnection->GetCorridorLength(connectedDirection)*2.0f + roomLength, 0.0f, 0.0f);
			dontAllowDirection = eDirection_Right;
		}
		if (connectedDirection == eDirection_Right)
		{
			newRoomPosition += vec3(pRoomConnection->GetLength() + pRoomConnection->GetCorridorLength(connectedDirection)*2.0f + roomLength, 0.0f, 0.0f);
			dontAllowDirection = eDirection_Left;
		}

		pNewRoom->SetPosition(newRoomPosition);

		// Create a door back to the room we just connected to
		pNewRoom->CreateDoor(dontAllowDirection);
	}

	// Create a door and corridor for the connected room
	if (roomDepth < 0)
	{
		// Don't allow to create doors back to the previously connected room (if we have one)
		// Keep getting a new direction, until we don't match the connected direction.
		eDirection direction = dontAllowDirection;
		while(direction == dontAllowDirection)
		{
			direction = (eDirection)GetRandomNumber(0, 3);
		}

		// Create the door object
		pNewRoom->CreateDoor(direction);

		// Create the corridor object
		pNewRoom->CreateCorridor(direction);

		// Create a new room, that connects to this one
		CreateRandomRoom(pNewRoom, direction, roomDepth+1);
	}

	m_vpRoomList.push_back(pNewRoom);
}

void RoomManager::CreateConnectedRoom()
{
	int randomRoomIndex = GetRandomNumber(0, (int)m_vpRoomList.size()-1);

	Room* pRoom = m_vpRoomList[randomRoomIndex];

	bool canCreateRoomFromDirection = false;
	int numDirctionTries = 0;
	while (canCreateRoomFromDirection == false && numDirctionTries < 10)
	{
		eDirection direction = (eDirection)GetRandomNumber(0, 3);

		if (pRoom->CanCreateConnection(direction))
		{
			canCreateRoomFromDirection = true;

			// Create the door object
			pRoom->CreateDoor(direction);

			// Create the corridor object
			pRoom->CreateCorridor(direction);

			// Create a new room, that connects to this one
			CreateRandomRoom(pRoom, direction, 0);
		}

		numDirctionTries++;
	}
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