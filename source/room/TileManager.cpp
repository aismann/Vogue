// ******************************************************************************
// Filename:    TileManager.cpp
// Project:     Vogue
// Author:      Steven Ball
//
// Revision History:
//   Initial Revision - 22/04/16
//
// Copyright (c) 2005-2016, Steven Ball
// ******************************************************************************

#include "TileManager.h"
#include "../utils/Random.h"

#include <vector>
#include <algorithm>
using namespace std;


TileManager::TileManager(Renderer* pRenderer)
{
	m_pRenderer = pRenderer;
}

TileManager::~TileManager()
{
	ClearTiles();
}

// Deletion
void TileManager::ClearTiles()
{
	for (unsigned int i = 0; i < m_vpTileList.size(); i++)
	{
		delete m_vpTileList[i];
		m_vpTileList[i] = 0;
	}
	m_vpTileList.clear();
}

// Creation
Tile* TileManager::CreateTile(vec3 position)
{
	Tile* pNewTile = new Tile(m_pRenderer);
	pNewTile->SetPosition(position);

	m_vpTileList.push_back(pNewTile);

	return pNewTile;
}

// Update
void TileManager::Update(float dt)
{
	for (unsigned int i = 0; i < m_vpTileList.size(); i++)
	{
		Tile *pTile = m_vpTileList[i];

		pTile->Update(dt);
	}
}

// Render
void TileManager::Render()
{
	m_pRenderer->PushMatrix();
		for (unsigned int i = 0; i < m_vpTileList.size(); i++)
		{
			Tile *pTile = m_vpTileList[i];

			pTile->Render();
		}
	m_pRenderer->PopMatrix();
}