// ******************************************************************************
// Filename:    RoomManager.cpp
// Project:     Vogue
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

// Accessors
int RoomManager::GetNumRooms()
{
	return (int)m_vpRoomList.size();
}

// Validation
bool RoomManager::DoesOverlap(vec3 position1, float length1, float width1, float height1, vec3 position2, float length2, float width2, float height2)
{
	Plane3D planes[6];
	planes[0] = Plane3D(vec3(-1.0f, 0.0f, 0.0f), vec3(length1, 0.0f, 0.0f));
	planes[1] = Plane3D(vec3(1.0f, 0.0f, 0.0f), vec3(-length1, 0.0f, 0.0f));
	planes[2] = Plane3D(vec3(0.0f, -1.0f, 0.0f), vec3(0.0f, height1, 0.0f));
	planes[3] = Plane3D(vec3(0.0f, 1.0f, 0.0f), vec3(0.0f, -height1, 0.0f));
	planes[4] = Plane3D(vec3(0.0f, 0.0f, -1.0f), vec3(0.0f, 0.0f, width1));
	planes[5] = Plane3D(vec3(0.0f, 0.0f, 1.0f), vec3(0.0f, 0.0f, -width1));

	for (int i = 0; i < 8; i++)
	{
		// Check all corners and mid points along the walls
		vec3 point;
		if (i == 0)
		{
			point = position2 + vec3(length2, 0.0f, width2);
		}
		else if (i == 1)
		{
			point = position2 + vec3(-length2, 0.0f, width2);
		}
		else if (i == 2)
		{
			point = position2 + vec3(length2, 0.0f, -width2);
		}
		else if (i == 3)
		{
			point = position2 + vec3(-length2, 0.0f, -width2);
		}
		else if (i == 4)
		{
			point = position2 + vec3(length2, 0.0f, 0.0f);
		}
		else if (i == 5)
		{
			point = position2 + vec3(-length2, 0.0f, 0.0f);
		}
		else if (i == 6)
		{
			point = position2 + vec3(0.0f, 0.0f, width2);
		}
		else if (i == 7)
		{
			point = position2 + vec3(0.0f, 0.0f, -width2);
		}

		float distance;
		int outside = 0;
		int inside = 0;

		for (int i = 0; i < 6; i++)
		{
			distance = planes[i].GetPointDistance(point - position1);

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
	}

	return false;
}

bool RoomManager::DoesRoomOverlap(vec3 position, float length, float width, float height)
{
	for (unsigned int i = 0; i < m_vpRoomList.size(); i++)
	{
		Room* pCheckRoom = m_vpRoomList[i];

		// Check each corner and each mid point along the walls
		vec3 topLeft = position + vec3(length, 0.0f, width);
		vec3 topRight = position + vec3(-length, 0.0f, width);
		vec3 bottomLeft = position + vec3(length, 0.0f, -width);
		vec3 bottomRight = position + vec3(-length, 0.0f, -width);
		vec3 midLeft = position + vec3(length, 0.0f, 0.0f);
		vec3 midRight = position + vec3(-length, 0.0f, 0.0f);
		vec3 midTop = position + vec3(0.0f, 0.0f, width);
		vec3 midBottom = position + vec3(0.0f, 0.0f, -width);

		if (pCheckRoom->IsPointInsideRoom(topLeft))
		{
			return true;
		}
		if (pCheckRoom->IsPointInsideRoom(topRight))
		{
			return true;
		}
		if (pCheckRoom->IsPointInsideRoom(bottomLeft))
		{
			return true;
		}
		if (pCheckRoom->IsPointInsideRoom(bottomRight))
		{
			return true;
		}
		if (pCheckRoom->IsPointInsideRoom(midLeft))
		{
			return true;
		}
		if (pCheckRoom->IsPointInsideRoom(midRight))
		{
			return true;
		}
		if (pCheckRoom->IsPointInsideRoom(midTop))
		{
			return true;
		}
		if (pCheckRoom->IsPointInsideRoom(midBottom))
		{
			return true;
		}

		// Also make sure that our existing room is not overlapping with the new room
		if (DoesOverlap(position, length, width, height, pCheckRoom->GetPosition(), pCheckRoom->GetLength(), pCheckRoom->GetWidth(), pCheckRoom->GetHeight()))
		{
			return true;
		}
	}

	return false;
}

// Generation
void RoomManager::GenerateNewLayout()
{
	// First clear all existing rooms
	ClearRooms();

	// Generate the starting room
	Room* pCreatedRoom = CreateRandomRoom(NULL, eDirection_NONE, 0.0f, 0);
}

Room* RoomManager::CreateRandomRoom(Room* pRoomConnection, eDirection connectedDirection, float corridorLengthAmount, int roomDepth)
{
	float roomLength = 0.0f;
	float roomWidth = 0.0f;
	float roomHeight = 0.0f;

	eDirection dontAllowDirection = eDirection_NONE;
	vec3 newRoomPosition;

	bool overlapsExistingRoom = true;
	int numRoomTries = 0;
	while(overlapsExistingRoom == true && numRoomTries < 10)
	{
		roomLength = GetRandomNumber(30, 140, 2) * 0.1f;
		roomWidth = GetRandomNumber(30, 140, 2) * 0.1f;
		roomHeight = 1.0f;

		// If we are connected to a room, set our position
		if (pRoomConnection != NULL)
		{
			newRoomPosition = pRoomConnection->GetPosition();
			if (connectedDirection == eDirection_Up)
			{
				newRoomPosition -= vec3(0.0f, 0.0f, pRoomConnection->GetWidth() + corridorLengthAmount + roomWidth);
				dontAllowDirection = eDirection_Down;
			}
			if (connectedDirection == eDirection_Down)
			{
				newRoomPosition += vec3(0.0f, 0.0f, pRoomConnection->GetWidth() + corridorLengthAmount + roomWidth);
				dontAllowDirection = eDirection_Up;
			}
			if (connectedDirection == eDirection_Left)
			{
				newRoomPosition -= vec3(pRoomConnection->GetLength() + corridorLengthAmount + roomLength, 0.0f, 0.0f);
				dontAllowDirection = eDirection_Right;
			}
			if (connectedDirection == eDirection_Right)
			{
				newRoomPosition += vec3(pRoomConnection->GetLength() + corridorLengthAmount + roomLength, 0.0f, 0.0f);
				dontAllowDirection = eDirection_Left;
			}
		}

		overlapsExistingRoom = DoesRoomOverlap(newRoomPosition, roomLength, roomWidth, roomHeight);

		numRoomTries++;
	}

	Room* pNewRoom = NULL;
	if (overlapsExistingRoom == false)
	{
		pNewRoom = new Room(m_pRenderer, this);
		pNewRoom->SetDimensions(roomLength, roomWidth, roomHeight);
		pNewRoom->SetPosition(newRoomPosition);
		pNewRoom->SetRoomDepth(roomDepth);
		if (pRoomConnection != NULL)
		{
			// Create a door back to the room we just connected to
			pNewRoom->CreateDoor(dontAllowDirection);
		}

		m_vpRoomList.push_back(pNewRoom);
	}

	return pNewRoom;
}

void RoomManager::CreateConnectedRoom()
{
	Room* pRoom = NULL;
	bool canCreateRoomConnection = false;
	int numRoomTries = 0;
	while (canCreateRoomConnection == false && numRoomTries < 10)
	{
		if ((int)m_vpRoomList.size() > 0)
		{
			int randomRoomIndex = GetRandomNumber(0, (int)m_vpRoomList.size() - 1);
			pRoom = m_vpRoomList[randomRoomIndex];
		}

		if (pRoom != NULL && pRoom->IsRoomFullOfDoors() == false && pRoom->IsRoomAbleToCreateMoreConnections() == true)
		{
			bool canCreateRoomFromDirection = false;
			int numDirctionTries = 0;
			while (canCreateRoomFromDirection == false && numDirctionTries < 10)
			{
				eDirection direction = (eDirection)GetRandomNumber(0, 3);

				if (pRoom->CanCreateConnection(direction))
				{
					float randomCorridorAmount = GetRandomNumber(10, 40, 2) * 0.2f;

					// Create a new room, that connects to this one
					Room* pCreatedRoom = CreateRandomRoom(pRoom, direction, randomCorridorAmount, pRoom->GetRoomDepth() + 1);

					if (pCreatedRoom != NULL)
					{
						canCreateRoomFromDirection = true;
						canCreateRoomConnection = true;

						// Create the door object
						pRoom->CreateDoor(direction);

						// Create the corridor object
						pRoom->CreateCorridor(direction, randomCorridorAmount);
					}
				}

				numDirctionTries++;

				if (numDirctionTries == 10 && canCreateRoomFromDirection == false)
				{
					pRoom->SetRoomAbleToCreateMoreConnections(false);
				}
			}
		}

		numRoomTries++;
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