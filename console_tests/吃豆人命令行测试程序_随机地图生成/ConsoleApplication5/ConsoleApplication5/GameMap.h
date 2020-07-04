#pragma once
#include "SingleInstanceMacro.h"

//�������ڵ�ͼ��ع����������ͼ���ɣ���ͼ���ݴ��䡣����ģʽ��

//ָ���ո�·�����ܶȡ������ԣ�0.36�Ƚ��ʺϡ�
const float constfPathDensity = 0.36;

//�趨��ͼ��С����һ�������������Σ�
const int constnMapWidth = 32;
const int constnMapHeight = 32;

//�趨·���������̶�(���ڴ�ֵ������˴�·������)
const int constnPathGenerationTolerance = 5;

//�趨���ѭ���������˳���ѭ����
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
  int get_map(char * pchMap);   //���ݴ���ӿ�

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
  const int m_nMapWidth;  //�����ڴ˴�����Ϊ�˷���������ʣ�ͨ��get_map_width()������
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

  //��ͼ
  char m_chMap[constnMapWidth][constnMapWidth];
};
