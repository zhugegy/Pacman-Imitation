#pragma once
#include "GameElement.h"

//´«ËÍµã
class CGameTelepoint :
  public CGameElement
{
public:
  //CGameTelepoint();
  virtual ~CGameTelepoint();

private:
  int m_nTeleDestCoordX;
  int m_nTeleDestCoordY;
public:
  CGameTelepoint(int nCoordX, int nCoordY, int nTeleDestCoordX, int nTeleDestCoordY);
  virtual int update();
  int get_tele_dest_coordX();
  int get_tele_dest_coordY();
};

