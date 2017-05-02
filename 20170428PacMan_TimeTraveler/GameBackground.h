#pragma once
#include "GameElement.h"
#include "GameDrawer.h"

const int constnMaxFrameNumBackground = 38;

const int constnDrawPointGoalBackground = 28;
const int constnDrawPointGoalBackgroundFast = 5;


//peanµÄ»­Í¼³ß´ç24*24 =¡· 1024*768
const int constnDrawSizeWidthBackground = constnWindowWidth;
const int constnDrawSizeHeightBackground = constnWindowHeight;

class CGameBackground :
  public CGameElement
{
public:
  enum eBackGroundState
  {
    NORMAL = 0,
    REVERSE = 1
  };

  CGameBackground();
  virtual ~CGameBackground();

private:
  eBackGroundState m_eState;
public:
  virtual int update();
private:
  int cal_source_draw_coordX(int nFrameNum, int &nDrawCoordX, int &nDrawCoordY);
public:
  int change_state(eBackGroundState eName);
  int turn_speed_fast(bool bBool);
};

