#pragma once
#include "SingleInstanceMacro.h"

//此类用于地图相关工作：随机地图生成，地图数据传输。单例模式。

//指定空格（路径）密度。经测试，0.36比较适合。
const float constfPathDensity = 0.36;

//设定地图大小（不一定必须是正方形）
const int constnMapWidth = 32;
const int constnMapHeight = 32;

//设定路径生成容忍度(大于此值则放弃此次路径生成)
const int constnPathGenerationTolerance = 5;

//设定最大循环次数（退出死循环）
const int constnMaxLoopTime = 600;

class CGameMap
{
public:

  enum eGameMapIdentifier
  {
    MAP_PATH = 0,
    MAP_BRICK = 1
  };

  SINGLE_INSTANCE(CGameMap)

  int generate_random_map();
  int get_map_width();
  int get_map_height();
  int get_map(char * pchMap);   //数据传输接口

private:
  CGameMap();
  virtual ~CGameMap();

  int init_map_with_0();
  int generate_map();
  int generate_path(int nStartCoorX, int nStartCoorY, int nEndCoorX, int nEndCoorY);
  float get_path_density();
  int get_a_random_path_coordinate(int * pnCoorX, int * pnCoorY);

private:
  float m_fPathDensity;
  const int m_nMapWidth;  //定义在此处，是为了方便类外访问（通过get_map_width()函数）
  const int m_nMapHeight;

  int m_nMapEntranceCoordX;
  int m_nMapEntranceCoordY;
  int m_nMapExitCoordX;
  int m_nMapExitCoordY;

  int m_nMapTeleportPointACoordX;
  int m_nMapTeleportPointACoordY;
  int m_nMapTeleportPointBCoordX;
  int m_nMapTeleportPointBCoordY;

  int m_nMapGhostHomeCoordX;
  int m_nMapGhostHomeCoordY;

  int m_nPathGenerationTolerance;
  int m_nMaxLoopTime;

  //地图
  char m_chMap[constnMapWidth][constnMapWidth];
};
