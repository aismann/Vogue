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
	
	GenerateNewLayout();
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
	CreateRandomRoom();
}

void RoomManager::CreateRandomRoom()
{
	Room* pNewRoom = new Room(m_pRenderer);

	float length = GetRandomNumber(30, 80, 2) * 0.1f;
	float width = GetRandomNumber(30, 80, 2) * 0.1f;
	float height = 1.0f;

	pNewRoom->SetDimensions(length, width, height);
	pNewRoom->CreateRoom();

	m_vpRoomList.push_back(pNewRoom);
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