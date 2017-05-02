#pragma once
#include "GameElement.h"

const int constnMaxFrameNumBrick = 0; //取决于砖墙背景图的数量

//（整面砖墙）Brick的画图尺寸768*768

//单块砖墙24*24
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

