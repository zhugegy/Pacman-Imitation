#pragma once
#include <windows.h>

//���ڶ�̬��������GameElement������������������̬���ɣ�����̫���ӣ���ʱ��������
const int constMaxElementNum = 3000;   

//Ԫ��Ĭ�ϻ�ͼ�ߴ磬������ż�������ҿ�Ⱥ͸߶�Ҫ��ͬ����ϵ��player��ghost���ƶ��жϣ�
const int constnElementDefaultDrawWidth = 24;
const int constnElementDefaultDrawHeight = 24;


class CGameElement
{
public:
  enum eGameDirection
  {
    UP = 0,
    LEFT = 1,
    DOWN = 2,
    RIGHT = 3,
    NO_DIRECTION = 4
  };

  //CGameElement();
  CGameElement(int nCoordX, int nCoordY);
  virtual ~CGameElement();

  static int m_nGameElementCount;
  static CGameElement* pGameElement[constMaxElementNum];

  int GetCoordX();
  int GetCoordY();
  virtual int update();

protected:
  int m_nCoordX;
  int m_nCoordY;
  bool m_bIsVisble;
  bool m_bIsActive;
  int m_nCurrentFrame;
  int m_nMaxFrameNum;
  int m_nActPoint;
  int m_nActPointGoal;
  int m_nDrawPoint;
  int m_nDrawPointGoal;

  int m_nDrawOffsetX;
  int m_nDrawOffsetY;

  HBITMAP hDrawHandel;
};

