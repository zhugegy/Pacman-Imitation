#include "GameMap.h"

#include <stdlib.h>
#include <memory.h>


CGameMap::CGameMap()
  :m_nMapWidth(constnMapWidth), m_nMapHeight(constnMapHeight)
{
  m_fPathDensity = constfPathDensity;

  // 设定地图起点
  m_nMapEntranceCoordX = 1;
  m_nMapEntranceCoordY = m_nMapHeight / 2;

  // 设定地图终点
  m_nMapExitCoordX = m_nMapWidth - 2;
  m_nMapExitCoordY = m_nMapEntranceCoordY;

  // 设定传送点A
  m_nMapTeleportPointACoordX = 0;
  m_nMapTeleportPointACoordY = m_nMapEntranceCoordY;

  //设定传送点B
  m_nMapTeleportPointBCoordX = m_nMapWidth - 1;
  m_nMapTeleportPointBCoordY = m_nMapExitCoordY;

  //设定鬼的老窝
  m_nMapGhostHomeCoordX = m_nMapWidth / 2;
  m_nMapGhostHomeCoordY = m_nMapHeight / 2;

  // 设定地图生成容忍度
  m_nPathGenerationTolerance = constnPathGenerationTolerance;

  //设定最大循环次数（退出死循环）
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

  //先全部初始化为墙
  //生成起点到终点的通路，起点到鬼老窝的通路
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

  //debug //print_maze(m_chMaze); //在贴图版本，debug似乎没用处了，可以debug到文件

  //生成随机路径
  while (get_path_density() < m_fPathDensity)
  {
    get_a_random_path_coordinate(&nTempStartCoorX, &nTempStartCoorY);

    nTempEndCoorX = (rand() % (m_nMapWidth - 2)) + 1;
    nTempEndCoorY = (rand() % (m_nMapHeight - 2)) + 1;

    generate_path(nTempStartCoorX, nTempStartCoorY, nTempEndCoorX,
      nTempEndCoorY);

    //debug //print_maze(m_chMaze);
  }

  //打通出入口点（确保）
  m_chMap[m_nMapEntranceCoordY][m_nMapEntranceCoordX] = MAP_PATH;
  m_chMap[m_nMapExitCoordY][m_nMapExitCoordX] = MAP_PATH;

  //打通传送点A、B
  m_chMap[m_nMapTeleportPointACoordY][m_nMapTeleportPointACoordX] = MAP_PATH;
  m_chMap[m_nMapTeleportPointBCoordY][m_nMapTeleportPointBCoordX] = MAP_PATH;

  //打通鬼的老家（4个）
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
  /*nTolerance用于检测不得不生成宽度为2的路的情况（需要避免此情况，如果过多，则说明路
  径无解，返回函数）*/
  int nTolerance = 0;

  /*nWhileLoopCount用于应对发生死循环的情况，此计数器超过一定值则强制跳出循环。*/
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
    case 0:   //向上
              //如果整体趋势是要向下，则忽略
      if (nStartCoorY < nEndCoorY)
      {
        continue;
      }

      //如果到达迷宫边界，则忽略
      if (nCurrentCoorY <= 1)
      {
        continue;
      }

      //如果左侧形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorX >= 1 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX - 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //如果右侧形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorX <= m_nMapWidth - 2 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX + 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //合法情况
      m_chMap[nCurrentCoorY - 1][nCurrentCoorX] = MAP_PATH;
      nCurrentCoorY--;
      nTolerance = 0;

      break;
    case 1:   //向下
              //如果整体趋势是要向上，则忽略
      if (nStartCoorY > nEndCoorY)
      {
        continue;
      }

      //如果到达迷宫边界，则忽略
      if (nCurrentCoorY >= m_nMapHeight - 2)
      {
        continue;
      }

      //如果左侧形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorX >= 1 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX - 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //如果右侧形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorX <= m_nMapWidth - 2 &&
        m_chMap[nCurrentCoorY][nCurrentCoorX + 1] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //合法情况
      m_chMap[nCurrentCoorY + 1][nCurrentCoorX] = MAP_PATH;
      nCurrentCoorY++;
      nTolerance = 0;

      break;
    case 2:   //向左
              //如果整体趋势是要向右，则忽略
      if (nStartCoorX < nEndCoorX)
      {
        continue;
      }

      //如果到达迷宫边界，则忽略
      if (nCurrentCoorX <= 1)
      {
        continue;
      }

      //如果上方形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorY >= 1 &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //如果下方形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorY <= m_nMapHeight - 2 &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX - 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //合法情况
      m_chMap[nCurrentCoorY][nCurrentCoorX - 1] = MAP_PATH;
      nCurrentCoorX--;
      nTolerance = 0;

      break;
    case 3:   //向右
              //如果整体趋势是要向左，则忽略
      if (nStartCoorX > nEndCoorX)
      {
        continue;
      }

      //如果到达迷宫边界，则忽略
      if (nCurrentCoorX >= m_nMapWidth - 2)
      {
        continue;
      }

      //如果上方形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorY >= 1 &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY - 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //如果下方形成宽度为2的路，则增加nTolerance，忽略
      if (nCurrentCoorY <= m_nMapHeight - 2 &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX] == MAP_PATH &&
        m_chMap[nCurrentCoorY + 1][nCurrentCoorX + 1] == MAP_PATH)
      {
        nTolerance++;
        continue;
      }

      //合法情况
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
