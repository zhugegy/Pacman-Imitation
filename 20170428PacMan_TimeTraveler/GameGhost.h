#pragma once
#include "GameElement.h"
#include "GameController.h"
#include "GYEasyStack.h"
#include "GYAStarNavi.h"
#include "GameMap.h"

const int constnMaxFrameNumGhost = 15;

const int constnActPointGoalGhostRealFast = 0;
const int constnActPointGoalGhostFast = 2;
const int constnActPointGoalGhostNormal = 3;

const int constnDrawPointGoalGhost = 10;  //10

const int constnDrawSizeWidthGhost = 40;
const int constnDrawSizeHeightGhost = 40;

class CGameController;
class CGYAStarNavi;
class CGameMap;

class CGameGhost :
  public CGameElement
{
public:
  enum eGhostName
  {
    BLINKY,
    PINKY,
    INKY,
    CLYDE
  };

  enum eGhostMode
  {
    CHASE = 0,
    FRIGHTENED = 1,
    SCATTER = 2
  };

  //CGameGhost();
  virtual ~CGameGhost();
  CGameGhost(int nCoordX, int nCoordY, eGhostName eName);

private:
  eGhostName m_nGhostName;
  eGhostMode m_nGhostMode;
  eGameDirection m_nDirection;
  int m_nOffsetWithinTile;
  CGYEasyStack m_PathStack;
  CGameController *m_pGameController;
  CGYAStarNavi *m_pGYAStarNavi;
  int m_nGhostHomeCoordX;
  int m_nGhostHomeCoordY;
  CGameMap *m_pGameMap;

public:
  int update();
  int reverse_direction();
private:
  bool am_i_on_intersection();
  int refresh_path_stack();
  int get_dest_coord(int & nDestCoordX, int & nDestCoordY);
  int find_dest_for_pinky(int nPlayerCoordX, int nPlayerCoordY, 
    CGameElement::eGameDirection ePlayerDirection, 
    int & nDestCoordiX, int & nDestCoordiY);
  int find_dest_for_inky(int nPlayerCoordX, int nPlayerCoordY,
    CGameElement::eGameDirection ePlayerDirection,
    int & nDestCoordiX, int & nDestCoordiY);
  int adjust_coordX(int & nCoordX);
  int adjust_coordY(int & nCoordY);
  int find_dest_for_clyde(int nPlayerCoordX, int nPlayerCoordY,
    CGameElement::eGameDirection ePlayerDirection,
    int & nDestCoordiX, int & nDestCoordiY);
  CGameElement::eGameDirection decide_direction_via_path_stack_pop();
  int keep_moving();
  int switch_tile();

public:
  int make_ghost_frightened();
  int make_ghost_not_frighented();
  CGameGhost::eGhostMode get_ghost_mode();
  int make_ghost_scatter();
  int forcely_set_ghost_to_init();
};

