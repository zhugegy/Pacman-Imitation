#include "GameGhost.h"
#include "GameDrawer.h"
#include "SingleInstanceMacro.h"
#include "GameMap.h"
#include "math.h"

/*
CGameGhost::CGameGhost()
{
}*/


CGameGhost::~CGameGhost()
{
}


CGameGhost::CGameGhost(int nCoordX, int nCoordY, eGhostName eName)
  :CGameElement(nCoordX, nCoordY)
{
  //1. 设定Ghost自己的参数
  m_nGhostName = eName;
  m_nGhostMode = CGameGhost::CHASE;
  m_nDirection = CGameElement::LEFT;
  m_nOffsetWithinTile = 0;
  m_pGameController = GET_SINGLE(CGameController);
  m_pGYAStarNavi = GET_SINGLE(CGYAStarNavi);
  m_pGameMap = GET_SINGLE(CGameMap);
  m_PathStack.init_stack();
  m_nGhostHomeCoordX = nCoordX;
  m_nGhostHomeCoordY = nCoordY;

  //2. 重写一些Ghost特定参数（继承自GameElement类）
  m_bIsVisble = true;
  m_bIsActive = true;
  m_nMaxFrameNum = constnMaxFrameNumGhost;

  if (eName == CGameGhost::BLINKY || eName == PINKY)
  {
    m_nActPointGoal = constnActPointGoalGhostFast;
  }
  else
  {
    m_nActPointGoal = constnActPointGoalGhostNormal;
  }

  m_nDrawPointGoal = constnDrawPointGoalGhost;

  m_nDrawOffsetX = (constnElementDefaultDrawWidth - constnDrawSizeWidthGhost) / 2;
  m_nDrawOffsetY = (constnElementDefaultDrawHeight - constnDrawSizeHeightGhost) / 2;

  //3. 重写一些Ghost特定参数（继承自GameElement类）:重写画图handle
  switch (eName)
  {
  case CGameGhost::BLINKY:
    hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\blinky.bmp", IMAGE_BITMAP,
      constnDrawSizeWidthGhost * (constnMaxFrameNumGhost + 1),
      constnDrawSizeHeightGhost * 2 * 4 * 3, LR_LOADFROMFILE);
    break;
  case CGameGhost::PINKY:
    hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\pinky.bmp", IMAGE_BITMAP,
      constnDrawSizeWidthGhost * (constnMaxFrameNumGhost + 1),
      constnDrawSizeHeightGhost * 2 * 4 * 3, LR_LOADFROMFILE);
    break;
  case CGameGhost::INKY:
    hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\inky.bmp", IMAGE_BITMAP,
      constnDrawSizeWidthGhost * (constnMaxFrameNumGhost + 1),
      constnDrawSizeHeightGhost * 2 * 4 * 3, LR_LOADFROMFILE);
    break;
  case CGameGhost::CLYDE:
    hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\clyde.bmp", IMAGE_BITMAP,
      constnDrawSizeWidthGhost * (constnMaxFrameNumGhost + 1),
      constnDrawSizeHeightGhost * 2 * 4 * 3, LR_LOADFROMFILE);
    break;
  default:
    break;
  }

  refresh_path_stack();

}


int CGameGhost::update()
{
  //0. 检查自己状态（是否发生碰撞），只适用于player
  /*if (m_nOffsetWithinTile == 0)
  {
    m_pGameController->player_hit_pean(m_nCoordX, m_nCoordY);
  }*/

  //1.action 执行动作（移动，看指令列表，变向，寻路等）
  if (m_bIsActive == true)
  {
    m_nActPoint++;
    if (m_nActPoint >= m_nActPointGoal)
    {
      m_nActPoint = 0;

      if (m_nOffsetWithinTile == 0)
      {
        if (am_i_on_intersection() == true)
        {
          refresh_path_stack();
        }
        m_nDirection = decide_direction_via_path_stack_pop();

        //scatter chase模式切换，放在这里看上去不合适，暂时这么弄
        if (m_nGhostMode == SCATTER && 
          m_nCoordX == m_pGameMap->get_ghost_home_coordX() &&
          m_nCoordY == m_pGameMap->get_ghost_home_coordY())
        {
          m_nGhostMode = CHASE;
          switch (m_nGhostName)
          {
          case CGameGhost::BLINKY:
          case CGameGhost::PINKY:
            m_nActPointGoal = constnActPointGoalGhostFast;
            break;
          case CGameGhost::INKY:
          case CGameGhost::CLYDE:
            m_nActPointGoal = constnActPointGoalGhostNormal;
            break;
          default:
            break;
          }
        }

      }

      keep_moving();

      if (abs(m_nOffsetWithinTile) == constnElementDefaultDrawWidth / 2)
      {
        switch_tile();
        m_nOffsetWithinTile = m_nOffsetWithinTile * (-1);

        //有时会卡住（Blinky），（在此处暴力回位）
        if (m_PathStack.get_top() == -1)
        {
          refresh_path_stack();
        }
      }

    }
  }

  //2.draw 画图
  if (m_bIsVisble == true)
  {
    int nDisplayCoordX = CGameDrawer::coordX_to_display_transfer(m_nCoordX) + m_nDrawOffsetX;
    int nDisplayCoordY = CGameDrawer::coordY_to_display_transfer(m_nCoordY) + m_nDrawOffsetY;

    if (m_nDirection == LEFT || m_nDirection == RIGHT)
    {
      nDisplayCoordX += m_nOffsetWithinTile;
    }
    else
    {
      nDisplayCoordY += m_nOffsetWithinTile;
    }

    //按照目前的移动方向取出对应人物的连续走动图，并确定截取人物图的宽度与高度
    SelectObject(CGameDrawer::m_bufdc, hDrawHandel);
    BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY,
      constnDrawSizeWidthGhost, constnDrawSizeHeightGhost,
      CGameDrawer::m_bufdc, m_nCurrentFrame * constnDrawSizeWidthGhost,
      (8 * m_nGhostMode + 2 * m_nDirection + 1) * constnDrawSizeHeightGhost, SRCAND);
    BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY,
      constnDrawSizeWidthGhost, constnDrawSizeHeightGhost,
      CGameDrawer::m_bufdc, m_nCurrentFrame * constnDrawSizeWidthGhost,
      (8 * m_nGhostMode + 2 * m_nDirection) * constnDrawSizeHeightGhost, SRCPAINT);

    m_nDrawPoint++;
    if (m_nDrawPoint >= m_nDrawPointGoal)
    {
      m_nDrawPoint = 0;

      m_nCurrentFrame++;
      if (m_nCurrentFrame > m_nMaxFrameNum)
      {
        m_nCurrentFrame = 0;
      }
    }
  }

  return 0;
}


int CGameGhost::reverse_direction()
{
  switch (m_nDirection)
  {
  case CGameElement::UP:
    m_nDirection = CGameElement::DOWN;
    break;
  case CGameElement::LEFT:
    m_nDirection = CGameElement::RIGHT;
    break;
  case CGameElement::DOWN:
    m_nDirection = CGameElement::UP;
    break;
  case CGameElement::RIGHT:
    m_nDirection = CGameElement::LEFT;
    break;
  case CGameElement::NO_DIRECTION:
    break;
  default:
    break;
  }
  return 0;
}


bool CGameGhost::am_i_on_intersection()
{
  int nCountForPathTile = 0;

  if (m_nCoordY - 1 >= 0 
    && m_pGameController->query_map_teil_status(m_nCoordX, m_nCoordY - 1) ==
    CGameMap::MAP_PATH)
  {
    nCountForPathTile++;
  }

  if (m_nCoordY + 1 < constnMapHeight
    && m_pGameController->query_map_teil_status(m_nCoordX, m_nCoordY + 1) ==
    CGameMap::MAP_PATH)
  {
    nCountForPathTile++;
  }

  if (m_nCoordX - 1 >= 0
    && m_pGameController->query_map_teil_status(m_nCoordX - 1, m_nCoordY) ==
    CGameMap::MAP_PATH)
  {
    nCountForPathTile++;
  }

  if (m_nCoordX + 1 < constnMapWidth
    && m_pGameController->query_map_teil_status(m_nCoordX + 1, m_nCoordY) ==
    CGameMap::MAP_PATH)
  {
    nCountForPathTile++;
  }

  if (nCountForPathTile > 2)
  {
    return true;
  }

  return false;
}


int CGameGhost::refresh_path_stack()
{
  int nDestCoordX = 0;
  int nDestCoordY = 0;

  m_PathStack.init_stack();
  get_dest_coord(nDestCoordX, nDestCoordY);

  //碰撞，暂时缓冲一下，否则容易飞。debug用，正式游戏中由于player首先提出碰撞，所以永远不会来这里
  if (nDestCoordX == m_nCoordX && nDestCoordY == m_nCoordY)
  {
    nDestCoordX = 0;
    nDestCoordY = 0;
  }

  m_pGYAStarNavi->get_path(m_nCoordX, m_nCoordY, nDestCoordX, nDestCoordY, m_PathStack);

  return 0;
}


int CGameGhost::get_dest_coord(int & nDestCoordX, int & nDestCoordY)
{
  CGamePlayer *pGamePlayer = m_pGameController->get_GamePlayer();
  int nPlayerCoordX = pGamePlayer->GetCoordX();
  int nPlayerCoordY = pGamePlayer->GetCoordY();
  CGameElement::eGameDirection ePlayerDirection = pGamePlayer->get_direction();

  switch (m_nGhostMode)
  {
  case CGameGhost::CHASE:
  case CGameGhost::FRIGHTENED:

    switch (m_nGhostName)
    {
    case CGameGhost::BLINKY:
      nDestCoordX = nPlayerCoordX;
      nDestCoordY = nPlayerCoordY;
      break;
    case CGameGhost::PINKY:
      find_dest_for_pinky(nPlayerCoordX, nPlayerCoordY, ePlayerDirection,
        nDestCoordX, nDestCoordY);
      break;
    case CGameGhost::INKY:
      find_dest_for_inky(nPlayerCoordX, nPlayerCoordY, ePlayerDirection,
        nDestCoordX, nDestCoordY);
      break;
    case CGameGhost::CLYDE:
      find_dest_for_clyde(nPlayerCoordX, nPlayerCoordY, ePlayerDirection,
        nDestCoordX, nDestCoordY);
      break;
    default:
      break;
    }

    break;
  case CGameGhost::SCATTER:
    nDestCoordX = m_pGameMap->get_ghost_home_coordX();
    nDestCoordY = m_pGameMap->get_ghost_home_coordY();
    break;
  default:
    break;
  }

  return 0;
}


int CGameGhost::find_dest_for_pinky(int nPlayerCoordX, int nPlayerCoordY, 
  CGameElement::eGameDirection ePlayerDirection, 
  int & nDestCoordiX, int & nDestCoordiY)
{
  nDestCoordiX = nPlayerCoordX;
  nDestCoordiY = nPlayerCoordY;

  switch (ePlayerDirection)
  {
  case CGameElement::UP:
    nDestCoordiY -= 4;
    adjust_coordY(nDestCoordiY);
    break;
  case CGameElement::LEFT:
    nDestCoordiX -= 4;
    adjust_coordX(nDestCoordiX);
    break;
  case CGameElement::DOWN:
    nDestCoordiY += 4;
    adjust_coordY(nDestCoordiY);
    break;
  case CGameElement::RIGHT:
    nDestCoordiX += 4;
    adjust_coordX(nDestCoordiX);
    break;
  case CGameElement::NO_DIRECTION:
    break;
  default:
    break;
  }
  return 0;
}


int CGameGhost::find_dest_for_inky(int nPlayerCoordX, int nPlayerCoordY,
  CGameElement::eGameDirection ePlayerDirection,
  int & nDestCoordiX, int & nDestCoordiY)
{
  CGameGhost *pGhostBlinky = m_pGameController->get_blinky();
  int nBlinkyCoordX = pGhostBlinky->GetCoordX();
  int nBlinkyCoordY = pGhostBlinky->GetCoordY();

  int nIntermidateCoordX = nPlayerCoordX;
  int nIntermidateCoordY = nPlayerCoordY;

  switch (ePlayerDirection)
  {
  case CGameElement::UP:
    nIntermidateCoordY -= 2;
    adjust_coordY(nIntermidateCoordY);
    break;
  case CGameElement::LEFT:
    nIntermidateCoordX -= 2;
    adjust_coordX(nIntermidateCoordX);
    break;
  case CGameElement::DOWN:
    nIntermidateCoordY += 2;
    adjust_coordY(nIntermidateCoordY);
    break;
  case CGameElement::RIGHT:
    nIntermidateCoordX += 2;
    adjust_coordX(nIntermidateCoordX);
    break;
  case CGameElement::NO_DIRECTION:
    break;
  default:
    break;
  }

  /*destX -- intermidateX -- BlinkyX
  (destX + BlinkyX) / 2 = intermidateX
  destX = intermidateX * 2 - BlinkyX
  */

  nDestCoordiX = nIntermidateCoordX * 2 - nBlinkyCoordX;
  adjust_coordX(nDestCoordiX);

  nDestCoordiY = nIntermidateCoordY * 2 - nBlinkyCoordY;
  adjust_coordY(nDestCoordiY);

  return 0;
}


int CGameGhost::adjust_coordX(int & nCoordX)
{
  if (nCoordX < 0)
  {
    nCoordX = 0;
  }

  if (nCoordX > constnMapWidth - 1)
  {
    nCoordX = constnMapWidth - 1;
  }

  return 0;
}


int CGameGhost::adjust_coordY(int & nCoordY)
{
  if (nCoordY < 0)
  {
    nCoordY = 0;
  }

  if (nCoordY > constnMapHeight - 1)
  {
    nCoordY = constnMapHeight - 1;
  }

  return 0;
}


int CGameGhost::find_dest_for_clyde(int nPlayerCoordX, int nPlayerCoordY,
  CGameElement::eGameDirection ePlayerDirection,
  int & nDestCoordiX, int & nDestCoordiY)
{
  int nAryPossibleCoordX[4] = {0, constnMapWidth - 1, 0,                   constnMapWidth - 1};
  int nAryPossibleCoordY[4] = {0, 0,                  constnMapHeight - 1, constnMapHeight - 1};
  int nArySerial = 0;

  if (abs(m_nCoordX - nPlayerCoordX) + abs(m_nCoordY - nPlayerCoordY) < 8)
  {
    nDestCoordiX = nPlayerCoordX;
    nDestCoordiY = nPlayerCoordY;
  }
  else
  {
    nArySerial = rand() % 4;
    nDestCoordiX = nAryPossibleCoordX[nArySerial];
    nDestCoordiY = nAryPossibleCoordY[nArySerial];
  }

  return 0;
}


CGameElement::eGameDirection CGameGhost::decide_direction_via_path_stack_pop()
{
  int nNextTileCoordX = 0;
  int nNextTileCoordY = 0;

  //和入栈顺序相反（参考CGYAStarNavi类的入栈方式）
  nNextTileCoordY = m_PathStack.pop();
  nNextTileCoordX = m_PathStack.pop();

  if (nNextTileCoordY < m_nCoordY)
  {
    return CGameElement::UP;
  }

  if (nNextTileCoordX < m_nCoordX)
  {
    return CGameElement::LEFT;
  }
  
  if (nNextTileCoordY > m_nCoordY)
  {
    return CGameElement::DOWN;
  }

  if (nNextTileCoordX > m_nCoordX)
  {
    return CGameElement::RIGHT;
  }

  //return CGameElement::NO_DIRECTION;    //debug(仅设想)
  return CGameElement::UP;
}


int CGameGhost::keep_moving()
{
  if (m_nDirection == RIGHT || m_nDirection == DOWN)
  {
    m_nOffsetWithinTile++;
  }
  else
  {
    m_nOffsetWithinTile--;
  }

  return 0;
}


int CGameGhost::switch_tile()
{
  switch (m_nDirection)
  {
  case CGameElement::UP:
    if (m_pGameController->query_map_teil_status(m_nCoordX, m_nCoordY - 1) == CGameMap::MAP_PATH)
    {
      m_nCoordY--;
    }
    else
    {
      //reverse_direction();
      m_nOffsetWithinTile = 2;
      refresh_path_stack();
    }
    break;
  case CGameElement::LEFT:
    if (m_pGameController->query_map_teil_status(m_nCoordX - 1, m_nCoordY) == CGameMap::MAP_PATH)
    {
      m_nCoordX--;
    }
    else
    {
      //reverse_direction();
      m_nOffsetWithinTile = 2;
      refresh_path_stack();

    }
    break;
  case CGameElement::DOWN:
    if (m_pGameController->query_map_teil_status(m_nCoordX, m_nCoordY + 1) == CGameMap::MAP_PATH)
    {
      m_nCoordY++;
    }
    else
    {
      //reverse_direction();
      m_nOffsetWithinTile = 2;
      refresh_path_stack();

    }
    break;
  case CGameElement::RIGHT:
    if (m_pGameController->query_map_teil_status(m_nCoordX + 1, m_nCoordY) == CGameMap::MAP_PATH)
    {
      m_nCoordX++;
    }
    else
    {
      //reverse_direction();
      m_nOffsetWithinTile = 2;
      refresh_path_stack();
    }
    break;
  default:
    break;
  }

  return 0;
}


int CGameGhost::make_ghost_frightened()
{
  if (m_nGhostMode == CGameGhost::CHASE)
  {
    m_nGhostMode = CGameGhost::FRIGHTENED;
  }
  //reverse_direction(); 不好用，容易飞，暂时不让ghost强制变向

  return 0;
}


int CGameGhost::make_ghost_not_frighented()
{
  if (m_nGhostMode == CGameGhost::FRIGHTENED)
  {
    m_nGhostMode = CGameGhost::CHASE;
  }
  return 0;
}


CGameGhost::eGhostMode CGameGhost::get_ghost_mode()
{
  return m_nGhostMode;
}


int CGameGhost::make_ghost_scatter()
{
  if (m_nGhostMode == CGameGhost::FRIGHTENED)
  {
    m_nGhostMode = CGameGhost::SCATTER;
    m_nActPointGoal = constnActPointGoalGhostRealFast;
  }
  return 0;
}



int CGameGhost::forcely_set_ghost_to_init()
{
  m_nCoordX = m_nGhostHomeCoordX;
  m_nCoordY = m_nGhostHomeCoordY;
  m_nGhostMode = CGameGhost::CHASE;

  switch (m_nGhostName)
  {
  case CGameGhost::BLINKY:
  case CGameGhost::PINKY:
    m_nActPointGoal = constnActPointGoalGhostFast;
    break;
  case CGameGhost::INKY:
  case CGameGhost::CLYDE:
    m_nActPointGoal = constnActPointGoalGhostNormal;
    break;
  default:
    break;
  }

  refresh_path_stack();

  return 0;
}
