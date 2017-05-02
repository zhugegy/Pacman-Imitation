#pragma once
#include <windows.h>

//用于多态遍历所有GameElement，本来可以用链表，动态生成，但是太复杂，暂时用数组存放
const int constMaxElementNum = 3000;   

//元素默认画图尺寸，必须是偶数，而且宽度和高度要相同（关系到player和ghost的移动判断）
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

