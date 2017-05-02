#include "GameMap.h"

#include <stdlib.h>
#include <memory.h>


CGameMap::CGameMap()
  :m_nMapWidth(constnMapWidth), m_nMapHeight(constnMapHeight)
{
  m_fPathDensity = constfPathDensity;

  // �趨��ͼ���
  m_nMapEntranceCoordX = 1;
  m_nMapEntranceCoordY = m_nMapHeight / 2;

  // �趨��ͼ�յ�
  m_nMapExitCoordX = m_nMapWidth - 2;
  m_nMapExitCoordY = m_nMapEntranceCoordY;

  // �趨���͵�A
  m_nMapTeleportPointACoordX = 0;
  m_nMapTeleportPointACoordY = m_nMapEntranceCoordY;

  //�趨���͵�B
  m_nMapTeleportPointBCoordX = m_nMapWidth - 1;
  m_nMapTeleportPointBCoordY = m_nMapExitCoordY;

  //�趨�������
  m_nMapGhostHomeCoordX = m_nMapWidth / 2;
  m_nMapGhostHomeCoordY = m_nMapHeight / 2;

  // �趨��ͼ�������̶�
  m_nPathGenerationTolerance = constnPathGenerationTolerance;

  //�趨���ѭ���������˳���ѭ����
  m_nMaxLoopTime = constnMaxLoopTime;

  init_map_with_0();
}


CGameMap::~CGameMap()
{
}


int CGameMap::generate_random_map()
{
  generate_map();

  return 0;
}


int CGameMap::get_map_width()
{
  return m_nMapWidth;
}


int CGameMap::get_map_height()
{
  return m_nMapHeight;
}


int CGameMap::init_map_with_0()
{
  for (int i = 0; i < m_nMapWidth; i++)
  {
    for (int j = 0; j < m_nMapHeight; j++)
    {
      m_chMap[j][i] = 0;
    }
  }

  return 0;
}


int CGameMap::generate_map()
{
  int i = 0;
  int j = 0;

  int nTempStartCoorX = 0;
  int nTempStartCoorY = 0;
  int nTempEndCoorX = 0;
  int nTempEndCoorY = 0;

  //��ȫ����ʼ��Ϊǽ
  //������㵽�յ��ͨ·����㵽�����ѵ�ͨ·
  int nGeneResult1 = 0;
  int nGeneResult2 = 0;

  while (true)
  {
    for (i = 0; i < m_nMapHeight; i++)
    {
      for (j = 0; j < m_nMapWidth; j++)
      {
        m_chMap[i][j] = MAP_BRICK;
      }
    }

    nGeneResult1 = generate_path(m_nMapEntranceCoordX, m_nMapEntranceCoordY,
      m_nMapExitCoordX, m_nMapExitCoordY);
    nGeneResult2 = generate_path(m_nMapEntranceCoordX, m_nMapEntranceCoordY,
      m_nMapGhostHomeCoordX, m_nMapGhostHomeCoordY);

    if (nGeneResult1 == 0 && nGeneResult2 == 0)
    {
      break;
    }
  }

  //debug //print_maze(m_chMaze); //����ͼ�汾��debug�ƺ�û�ô��ˣ�����debug���ļ�

  //�������·��
  while (get_path_density() < m_fPathDensity)
  {
    get_a_random_path_coordinate(&nTempStartCoorX, &nTempStartCoorY);

    nTempEndCoorX = (rand() % (m_nMapWidth - 2)) + 1;
    nTempEndCoorY = (rand() % (m_nMapHeight - 2)) + 1;

    generate_path(nTempStartCoorX, nTempStartCoorY, nTempEndCoorX,
      nTempEndCoorY);

    //debug //print_maze(m_chMaze);
  }

  //��ͨ����ڵ㣨ȷ����
  m_chMap[m_nMapEntranceCoordY][m_nMapEntranceCoordX] = MAP_PATH;
  m_chMap[m_nMapExitCoordY][m_nMapExitCoordX] = MAP_PATH;

  //��ͨ���͵�A��B
  m_chMap[m_nMapTeleportPointACoordY][m_nMapTeleportPointACoordX] = MAP_PATH;
  m_chMap[m_nMapTeleportPointBCoordY][m_nMapTeleportPointBCoordX] = MAP_PATH;

  //��ͨ����ϼң�4����
  m_chMap[m_nMapGhostHomeCoordY][m_nMapGhostHomeCoordX] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY - 1][m_nMapGhostHomeCoordX] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY][m_nMapGhostHomeCoordX - 1] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY][m_nMapGhostHomeCoordX + 1] = MAP_PATH;

  m_chMap[m_nMapGhostHomeCoordY - 1][m_nMapGhostHomeCoordX + 1] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY - 1][m_nMapGhostHomeCoordX - 1] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY + 1][m_nMapGhostHomeCoordX - 1] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY + 1][m_nMapGhostHomeCoordX + 1] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY + 1][m_nMapGhostHomeCoordX] = MAP_PATH;
  m_chMap[m_nMapGhostHomeCoordY - 2][m_nMapGhostHomeCoordX] = MAP_PATH;



  //debug
  /*m_chMap[m_nMapGhostHomeCoordY][m_nMapGhostHomeCoordX] = 'g';
  m_chMap[m_nMapGhostHomeCoordY - 1][m_nMapGhostHomeCoordX] = 'g';
  m_chMap[m_nMapGhostHomeCoordY][m_nMapGhostHomeCoordX - 1] = 'g';
  m_chMap[m_nMapGhostHomeCoordY][m_nMapGhostHomeCoordX + 1] = 'g';*/

  return 0;
}


int CGameMap::generate_path(int nStartCoorX, int nStartCoorY, int nEndCoorX, int nEndCoorY)
{
  /*nTolerance���ڼ�ⲻ�ò����ɿ��Ϊ2��·���������Ҫ����������������࣬��˵��·
  ���޽⣬���غ�����*/
  int nTolerance = 0;

  /*nWhileLoopCount����Ӧ�Է�����ѭ����������˼���������һ��ֵ��ǿ������ѭ����*/
  int nWhileLoopCount = 0;

  int nDirection = -1;
  int nCurrentCoorX = nStartCoorX;
  int nCurrentCoorY = nStartCoorY;

  while (true)
  {
    if (nWhileLoopCount++ >= m_nMaxLoopTime)
    {
      return -2;
    }

    if (nTolerance > m_nPathGenerationTolerance)
    {
      return -1;
    }

    if (nCurrentCoorX == nEndCoorX && nCurrentCoorY == nEndCoorY)
    {
      return 0;
    }

    nDirection = rand() % 4;

    switch (nDirection)
    {
    case 0:   //����
              //�������������Ҫ���£������
      if (nStartCoorY < nEndCoorY)
      {
        continue;
      }

      //��������Թ��߽磬�����
      if (nCurrentCoorY <= 1)
      {
        continue;
      }

      //�������γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorX >= 1 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX - 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //����Ҳ��γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorX <= m_nMapWidth - 2 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX + 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //�Ϸ����
      m_chMap[nCurrentCoorY - 1][nCurrentCoorX] = MAP_PATH;
      nCurrentCoorY--;
      nTolerance = 0;

      break;
    case 1:   //����
              //�������������Ҫ���ϣ������
      if (nStartCoorY > nEndCoorY)
      {
        continue;
      }

      //��������Թ��߽磬�����
      if (nCurrentCoorY >= m_nMapHeight - 2)
      {
        continue;
      }

      //�������γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorX >= 1 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX - 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //����Ҳ��γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorX <= m_nMapWidth - 2 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX + 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //�Ϸ����
      m_chMap[nCurrentCoorY + 1][nCurrentCoorX] = MAP_PATH;
      nCurrentCoorY++;
      nTolerance = 0;

      break;
    case 2:   //����
              //�������������Ҫ���ң������
      if (nStartCoorX < nEndCoorX)
      {
        continue;
      }

      //��������Թ��߽磬�����
      if (nCurrentCoorX <= 1)
      {
        continue;
      }

      //����Ϸ��γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorY >= 1 &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //����·��γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorY <= m_nMapHeight - 2 &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //�Ϸ����
      m_chMap[nCurrentCoorY][nCurrentCoorX - 1] = MAP_PATH;
      nCurrentCoorX--;
      nTolerance = 0;

      break;
    case 3:   //����
              //�������������Ҫ���������
      if (nStartCoorX > nEndCoorX)
      {
        continue;
      }

      //��������Թ��߽磬�����
      if (nCurrentCoorX >= m_nMapWidth - 2)
      {
        continue;
      }

      //����Ϸ��γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorY >= 1 &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //����·��γɿ��Ϊ2��·��������nTolerance������
      if (nCurrentCoorY <= m_nMapHeight - 2 &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //�Ϸ����
      m_chMap[nCurrentCoorY][nCurrentCoorX + 1] = MAP_PATH;
      nCurrentCoorX++;
      nTolerance = 0;

      break;
    default:
      break;
    }
  }

  return -1;
}


float CGameMap::get_path_density()
{
  int nPathCount = 0;
  int i = 0;
  int j = 0;
  float fPathDensity = 0.0f;

  for (i = 0; i < m_nMapHeight; i++)
  {
    for (j = 0; j < m_nMapWidth; j++)
    {
      if (m_chMap[i][j] == MAP_PATH)
      {
        nPathCount++;
      }
    }
  }

  fPathDensity = (float)(((float)nPathCount) / (m_nMapWidth * m_nMapHeight));

  return fPathDensity;
}


int CGameMap::get_a_random_path_coordinate(int *pnCoorX, int *pnCoorY)
{
  int nRandomizedCoorX = 0;
  int nRandomizedCoorY = 0;

  while (true)
  {
    nRandomizedCoorX = (rand() % (m_nMapWidth - 2)) + 1;
    nRandomizedCoorY = (rand() & (m_nMapHeight - 2)) + 1;

    if (m_chMap[nRandomizedCoorY][nRandomizedCoorX] == MAP_PATH)
    {
      *pnCoorX = nRandomizedCoorX;
      *pnCoorY = nRandomizedCoorY;
      return 0;
    }
  }

  return -1;
}


int CGameMap::get_map(char *pchMap)
{
  memcpy(pchMap, m_chMap, m_nMapWidth * m_nMapHeight);

  return 0;
}


int CGameMap::get_ghost_home_coordX()
{
  return m_nMapGhostHomeCoordX;
}


int CGameMap::get_ghost_home_coordY()
{
  return m_nMapGhostHomeCoordY;
}
