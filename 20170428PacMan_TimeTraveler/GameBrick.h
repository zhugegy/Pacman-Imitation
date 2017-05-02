#pragma once
#include "GameElement.h"

const int constnMaxFrameNumBrick = 0; //ȡ����שǽ����ͼ������

//������שǽ��Brick�Ļ�ͼ�ߴ�768*768

//����שǽ24*24
const int constnDrawSizeWidthBrick = 24;
const int constnDrawSizeHeightBrick = 24;

class CGameBrick :
  public CGameElement
{
public:
  enum eBrickThemeName
  {
    NORMAL_WALL = 0,
    UNIVERSE = 1
  };

  //CGameBrick();
  virtual ~CGameBrick();
  CGameBrick(int nCoordX, int nCoordY);
  virtual int update();
private:
  int m_nBrickTheme;
public:
  int change_brick_theme(eBrickThemeName eName);
};

