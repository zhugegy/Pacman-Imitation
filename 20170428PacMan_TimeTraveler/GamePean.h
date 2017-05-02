#pragma once
#include "GameElement.h"
#include <windows.h>

const int constnMaxFrameNumPean = 15;
const int constnDrawPointGoalPean = 30;

//peanµÄ»­Í¼³ß´ç24*24 =¡· 20*20
const int constnDrawSizeWidthPean = 6;
const int constnDrawSizeHeightPean = 6;

class CGamePean :
  public CGameElement
{
public:

  enum ePeanState
  {
    NORMAL_PEAN,
    ENERGIZER_PEAN
  };

  //CGamePean();
  CGamePean(int nCoordX, int nCoordY, ePeanState eState);

  virtual ~CGamePean();

private:
  int m_nState;
public:
  virtual int update();
  bool is_visible();
  int change_visible(bool bStatus);
  int get_state();
};
