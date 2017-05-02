#pragma once
#include "GameElement.h"
#include "SingleInstanceMacro.h"
#include "GameController.h"

const int constnMaxFrameNumPlayer = 8;
const int constnActPointGoalPlayer = 2;
const int constnDrawPointGoalPlayer = 20;

const int constnDrawSizeWidthPlayer = 36;
const int constnDrawSizeHeightPlayer = 42;

class CGameController;

class CGamePlayer :
  public CGameElement
{
public:
  //CGamePlayer();
  virtual ~CGamePlayer();
  CGamePlayer(int nCoordX, int nCoordY);

private:
  eGameDirection m_nDirection;
  int m_nOffsetWithinTile;
  CGameController *m_pGameController;
  eGameDirection m_nDirectionAttempt;
public:
  virtual int update();
private:
  bool is_movable(eGameDirection nDirection);
  int keep_moving();
  int switch_tile();
public:
  int give_change_direction_attemption(eGameDirection nDirection);
  CGameElement::eGameDirection get_direction();
  int set_coordX(int nCoordX);
  int set_coordY(int nCoordY);
  int set_offset_within_tile(int nOffset);
};

