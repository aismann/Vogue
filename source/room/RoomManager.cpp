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

#include <vector>
#include <algorithm>
using namespace std;

const int RoomManager::MAX_ROOM_DEPTH = 3;


RoomManager::RoomManager(Renderer* pRenderer, TileManager* pTileManager)
{
	m_pRenderer = pRenderer;
	m_pTileManager = pTileManager;
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

	m_vpConnectionRoomList.clear();
	m_vpCanBeItemRoomList.clear();
	m_vpCanBeBossRoomList.clear();

	m_numItemRooms = 0;
	m_numBossRooms = 0;
}

// Accessors
int RoomManager::GetNumRooms()
{
	return (int)m_vpRoomList.size();
}

int RoomManager::GetNumConnectionRoomsPossible()
{
	return (int)m_vpConnectionRoomList.size();
}

int RoomManager::GetNumItemRooms()
{
	return m_numItemRooms;
}

int RoomManager::GetNumItemRoomsPossible()
{
	return (int)m_vpCanBeItemRoomList.size();
}

int RoomManager::GetNumBossRooms()
{
	return m_numBossRooms;
}

int RoomManager::GetNumBossRoomsPossible()
{
	return (int)m_vpCanBeBossRoomList.size();
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

	for (int i = 0; i < 9; i++)
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
		else if (i == 8) // Center
		{
			point = position2;
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
		vec3 center = position;

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
		if (pCheckRoom->IsPointInsideRoom(center))
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
	float randomLengthOffset;
	Room* pCreatedRoom = CreateRandomRoom(NULL, eDirection_NONE, 0.0f, &randomLengthOffset, 0);
}

Room* RoomManager::CreateRandomRoom(Room* pRoomConnection, eDirection connectedDirection, float corridorLengthAmount, float *randomLengthOffset, int roomDepth)
{
	float roomLength = 0.0f;
	float roomWidth = 0.0f;
	float roomHeight = 0.0f;

	eDirection dontAllowDirection = eDirection_NONE;
	vec3 newRoomPosition;
	float randomRoomOffset;

	bool overlapsExistingRoom = true;
	int numRoomTries = 0;
	while(overlapsExistingRoom == true && numRoomTries < 1)
	{
		roomLength = GetRandomNumber(50, 140, 2) * 0.1f;
		roomWidth = GetRandomNumber(50, 140, 2) * 0.1f;
		roomHeight = 1.0f;

		*randomLengthOffset = GetRandomNumber(-100, 100, 2) * 0.01f;
		// If we are connected to a room, set our position
		if (pRoomConnection != NULL)
		{
			newRoomPosition = pRoomConnection->GetPosition();
			if (connectedDirection == eDirection_Up)
			{
				randomRoomOffset = (GetRandomNumber(-100, 100, 2) * 0.01f) * (roomLength - 0.5f);
				newRoomPosition -= vec3(randomRoomOffset + (*randomLengthOffset * (pRoomConnection->GetLength() - 0.5f)), 0.0f, pRoomConnection->GetWidth() + corridorLengthAmount + roomWidth);
				dontAllowDirection = eDirection_Down;
			}
			if (connectedDirection == eDirection_Down)
			{
				randomRoomOffset = (GetRandomNumber(-100, 100, 2) * 0.01f) * (roomLength - 0.5f);
				newRoomPosition += vec3(randomRoomOffset + (*randomLengthOffset * (pRoomConnection->GetLength() - 0.5f)), 0.0f, pRoomConnection->GetWidth() + corridorLengthAmount + roomWidth);
				dontAllowDirection = eDirection_Up;
			}
			if (connectedDirection == eDirection_Left)
			{
				randomRoomOffset = (GetRandomNumber(-100, 100, 2) * 0.01f) * (roomWidth - 0.5f);
				newRoomPosition -= vec3(pRoomConnection->GetLength() + corridorLengthAmount + roomLength, 0.0f, randomRoomOffset + (*randomLengthOffset * (pRoomConnection->GetWidth() - 0.5f)));
				dontAllowDirection = eDirection_Right;
			}
			if (connectedDirection == eDirection_Right)
			{
				randomRoomOffset = (GetRandomNumber(-100, 100, 2) * 0.01f) * (roomWidth - 0.5f);
				newRoomPosition += vec3(pRoomConnection->GetLength() + corridorLengthAmount + roomLength, 0.0f, randomRoomOffset + (*randomLengthOffset * (pRoomConnection->GetWidth() - 0.5f)));
				dontAllowDirection = eDirection_Left;
			}
		}

		overlapsExistingRoom = DoesRoomOverlap(newRoomPosition, roomLength, roomWidth, roomHeight);

		numRoomTries++;
	}

	Room* pNewRoom = NULL;
	if (overlapsExistingRoom == false)
	{
		pNewRoom = new Room(m_pRenderer, m_pTileManager, this);
		pNewRoom->SetDimensions(roomLength, roomWidth, roomHeight);
		pNewRoom->SetPosition(newRoomPosition);
		pNewRoom->SetRoomDepth(roomDepth);
		if (pRoomConnection != NULL)
		{
			// Create a door back to the room we just connected to
			pNewRoom->CreateDoor(dontAllowDirection, randomRoomOffset);
		}

		m_vpRoomList.push_back(pNewRoom);

		if (roomDepth != 0)
		{
			m_vpCanBeItemRoomList.push_back(pNewRoom);
		}
		if (roomDepth < MAX_ROOM_DEPTH)
		{
			m_vpConnectionRoomList.push_back(pNewRoom);
		}
		else
		{
			m_vpCanBeBossRoomList.push_back(pNewRoom);
		}
	}

	return pNewRoom;
}

void RoomManager::CreateConnectedRoom()
{
	Room* pRoom = NULL;
	bool canCreateRoomConnection = false;
	int numRoomTries = 0;
	while (canCreateRoomConnection == false && numRoomTries < 1)
	{
		if ((int)m_vpConnectionRoomList.size() > 0)
		{
			int randomRoomIndex = GetRandomNumber(0, (int)m_vpConnectionRoomList.size() - 1);
			pRoom = m_vpConnectionRoomList[randomRoomIndex];
		}

		if (pRoom != NULL && pRoom->IsRoomFullOfDoors() == false && pRoom->IsRoomAbleToCreateMoreConnections() == true && pRoom->GetRoomDepth() < MAX_ROOM_DEPTH)
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
					float randomRoomOffset;
					Room* pCreatedRoom = CreateRandomRoom(pRoom, direction, randomCorridorAmount, &randomRoomOffset, pRoom->GetRoomDepth() + 1);

					if (pCreatedRoom != NULL)
					{
						canCreateRoomFromDirection = true;
						canCreateRoomConnection = true;
						float randomLengthOffset;

						// Create the door object
						if (direction == eDirection_Up || direction == eDirection_Down)
						{
							randomLengthOffset = randomRoomOffset * (pRoom->GetLength() - 0.5f);
						}
						else if (direction == eDirection_Left || direction == eDirection_Right)
						{
							randomLengthOffset = randomRoomOffset * (pRoom->GetWidth() - 0.5f);
						}
						pRoom->CreateDoor(direction, randomLengthOffset);

						// Create the corridor object
						pRoom->CreateCorridor(direction, randomCorridorAmount, randomLengthOffset);

						// Remove this room from the connection list if we become full of doors
						if (pRoom->IsRoomFullOfDoors())
						{
							RemoveRoomFromConnectionList(pRoom);
						}
					}
				}

				numDirctionTries++;

				if (numDirctionTries == 10 && canCreateRoomFromDirection == false)
				{
					// Set room unable to create more connections and remove from connection list
					pRoom->SetRoomAbleToCreateMoreConnections(false);
					RemoveRoomFromConnectionList(pRoom);
				}
			}
		}

		numRoomTries++;
	}
}

void RoomManager::RemoveRoomFromConnectionList(Room* pRoom)
{
	RoomList::iterator iter = find(m_vpConnectionRoomList.begin(), m_vpConnectionRoomList.end(), pRoom);
	if (iter != m_vpConnectionRoomList.end())
	{
		m_vpConnectionRoomList.erase(iter);
	}
}

void RoomManager::RemoveRoomFromItemList(Room* pRoom)
{
	RoomList::iterator iter = find(m_vpCanBeItemRoomList.begin(), m_vpCanBeItemRoomList.end(), pRoom);
	if (iter != m_vpCanBeItemRoomList.end())
	{
		m_vpCanBeItemRoomList.erase(iter);
	}
}

void RoomManager::RemoveRoomFromBossList(Room* pRoom)
{
	RoomList::iterator iter = find(m_vpCanBeBossRoomList.begin(), m_vpCanBeBossRoomList.end(), pRoom);
	if (iter != m_vpCanBeBossRoomList.end())
	{
		m_vpCanBeBossRoomList.erase(iter);
	}
}

void RoomManager::CreateBossRoom()
{
	Room* pRoom = NULL;
	bool createdBossRoom = false;
	int numRoomTries = 0;
	while (createdBossRoom == false && numRoomTries < 1)
	{
		if ((int)m_vpCanBeBossRoomList.size() > 0)
		{
			int randomRoomIndex = GetRandomNumber(0, (int)m_vpCanBeBossRoomList.size() - 1);
			pRoom = m_vpCanBeBossRoomList[randomRoomIndex];
		}

		if (pRoom != NULL && pRoom->IsItemRoom() == false && pRoom->IsBossRoom() == false && pRoom->GetRoomDepth() != 0)
		{
			pRoom->SetBossRoom(true);

			RemoveRoomFromItemList(pRoom);
			RemoveRoomFromBossList(pRoom);

			createdBossRoom = true;
		}

		numRoomTries++;
	}
}

void RoomManager::CreateItemRoom()
{
	Room* pRoom = NULL;
	bool createdItemRoom = false;
	int numRoomTries = 0;
	while (createdItemRoom == false && numRoomTries < 1)
	{
		if ((int)m_vpCanBeItemRoomList.size() > 0)
		{
			int randomRoomIndex = GetRandomNumber(0, (int)m_vpCanBeItemRoomList.size() - 1);
			pRoom = m_vpCanBeItemRoomList[randomRoomIndex];
		}

		if (pRoom != NULL && pRoom->IsItemRoom() == false && pRoom->IsBossRoom() == false && pRoom->GetRoomDepth() != 0)
		{
			pRoom->SetItemRoom(true);

			RemoveRoomFromItemList(pRoom);
			RemoveRoomFromBossList(pRoom);

			createdItemRoom = true;
		}
		
		numRoomTries++;
	}
}

// Update
void RoomManager::Update(float dt)
{
	m_numItemRooms = 0;
	m_numBossRooms = 0;
	for (unsigned int i = 0; i < m_vpRoomList.size(); i++)
	{
		Room *pRoom = m_vpRoomList[i];

		pRoom->Update(dt);

		if (pRoom->IsItemRoom())
		{
			m_numItemRooms++;
		}
		if (pRoom->IsBossRoom())
		{
			m_numBossRooms++;
		}
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