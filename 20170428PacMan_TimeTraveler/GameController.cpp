#include "GameController.h"
#include "GYAStarNavi.h"
#include "GamePean.h"
#include "GameDrawer.h"
#include <time.h>
#include "GameElement.h"
#include "GameBackground.h"
#include "GameBrick.h"
#include "GamePlayer.h"
#include "GameGhost.h"
#include "SingleInstanceMacro.h"
#include "GameTelepoint.h"

CGameController::CGameController()
{
  m_nStatus = CGameController::GAME_START;
  hDrawHandelForGameOverPause = (HBITMAP)LoadImage(NULL, L"game_resource\\GameOverPause.bmp", 
    IMAGE_BITMAP, constnWindowWidth, constnWindowHeight, LR_LOADFROMFILE);

  hDrawHandelForGameStart = (HBITMAP)LoadImage(NULL, L"game_resource\\GameStart.bmp",
    IMAGE_BITMAP, constnWindowWidth, constnWindowHeight, LR_LOADFROMFILE);

  m_pGamePlayer = NULL;
  m_pGameBackground = NULL;
  m_pGameTimer = NULL;
  m_pGameGhostBlinky = NULL;
  m_nAryGameBrickCount = 0;
  m_nAryGameGhostCount = 0;

  m_pGameDrawer = GET_SINGLE(CGameDrawer);

  for (int i = 0; i < constnMapHeight; i++)
  {
    for (int j = 0; j < constnMapWidth; j++)
    {
      m_pAryGamePean[i][j] = NULL;
    }
  }
}


CGameController::~CGameController()
{
}


int CGameController::init(HWND hwnd)
{
  srand((unsigned int)time(0));

  CGameDrawer *pGameDrawer = GET_SINGLE(CGameDrawer);
  pGameDrawer->init(hwnd);

  CGameMap *pGameMap = GET_SINGLE(CGameMap);
  pGameMap->generate_random_map();
  pGameMap->get_map((char *)m_chGameMap);

  CGYAStarNavi *pGYAStarNavi = GET_SINGLE(CGYAStarNavi);
  pGYAStarNavi->load_maze();

  m_pGameBackground = new CGameBackground;

  int nPeanCount = 0;
  CGamePean *pGamePean = NULL;
  CGameBrick *pGameBrick = NULL;

  for (int i = 0; i < constnMapHeight; i++)
  {
    for (int j = 0; j < constnMapWidth; j++)
    {
      if (m_chGameMap[i][j] == CGameMap::MAP_PATH)
      {
        nPeanCount++;
        if (nPeanCount % 28 == 0)
        {
          pGamePean = new CGamePean(j, i, CGamePean::ENERGIZER_PEAN);
        }
        else
        {
          pGamePean = new CGamePean(j, i, CGamePean::NORMAL_PEAN);
        }
        m_pAryGamePean[i][j] = pGamePean;
      }
      else
      {
        pGameBrick = new CGameBrick(j, i);
        m_pAryGameBrick[m_nAryGameBrickCount] = pGameBrick;
        m_nAryGameBrickCount++;
      }
    }
  }

  m_pGamePlayer = new CGamePlayer(1, 16);

  m_pGameGhostBlinky = new CGameGhost(constnMapWidth / 2, constnMapHeight / 2 - 1, CGameGhost::BLINKY);
  m_pAryGameGhost[m_nAryGameGhostCount++] = m_pGameGhostBlinky;

  CGameGhost *pTmpGhost = new CGameGhost(constnMapWidth / 2, constnMapHeight / 2, CGameGhost::PINKY);
  m_pAryGameGhost[m_nAryGameGhostCount++] = pTmpGhost;

  pTmpGhost = new CGameGhost(constnMapWidth / 2 - 1, constnMapHeight / 2, CGameGhost::INKY);
  m_pAryGameGhost[m_nAryGameGhostCount++] = pTmpGhost;

  pTmpGhost = new CGameGhost(constnMapWidth / 2 + 1, constnMapHeight / 2, CGameGhost::CLYDE);
  m_pAryGameGhost[m_nAryGameGhostCount++] = pTmpGhost;

  CGameTelepoint *pTelepoint = NULL;
  pTelepoint = new CGameTelepoint(0, 16, 31, 16);
  pTelepoint = new CGameTelepoint(31, 16, 0, 16);

  m_pGameTimer = new CGameTimer();

  return 0;
}

//不用了
/*
int CGameController::make_element_update()
{
  for (int i = 0; i < CGameElement::m_nGameElementCount; i++)
  {
    CGameElement::pGameElement[i]->update();
  }

  return 0;
}*/


int CGameController::loop()
{
  //用于检查Ghost之间的相互碰撞（并不是和player碰撞）
  //check_ghost_collision();

  if (m_nStatus == CGameController::GAME_RUNNING)
  {
    for (int i = 0; i < CGameElement::m_nGameElementCount; i++)
    {
      CGameElement::pGameElement[i]->update();
    }
  }
  else if (m_nStatus == CGameController::GAME_STATUS_PAUSE)
  {
    SelectObject(CGameDrawer::m_bufdc, hDrawHandelForGameOverPause);
    BitBlt(CGameDrawer::m_mdc, 0, 0, constnWindowWidth, constnWindowHeight,
      CGameDrawer::m_bufdc, 0, 0, SRCCOPY);
    m_pGameTimer->update();
  }
  else /*m_nStatus == CGameController::GAME_START*/
  {
    SelectObject(CGameDrawer::m_bufdc, hDrawHandelForGameStart);
    BitBlt(CGameDrawer::m_mdc, 0, 0, constnWindowWidth, constnWindowHeight,
      CGameDrawer::m_bufdc, 0, 0, SRCCOPY);
  }

  m_pGameDrawer->display();

  return 0;
}


int CGameController::terminate(HWND hwnd)
{
  CGameDrawer *pGameDrawer = GET_SINGLE(CGameDrawer);
  pGameDrawer->clean_dc(hwnd);

  return 0;
}


char CGameController::query_map_teil_status(int nCoordX, int nCoordY)
{
  return m_chGameMap[nCoordY][nCoordX];
}


CGamePlayer * CGameController::get_GamePlayer()
{
  return m_pGamePlayer;
}

//玩家吃豆子处理
int CGameController::player_hit_pean(int nCoordX, int nCoordY)
{
  CGamePean *pTmp = m_pAryGamePean[nCoordY][nCoordX];

  if (pTmp->is_visible() == true)
  {
    pTmp->change_visible(false);
    if (pTmp->get_state() == CGamePean::ENERGIZER_PEAN)
    {
      when_player_hit_energized_pean();
    }
  }

  return 0;
}


int CGameController::timer_finished(int eName)
{
  switch (eName)
  {
  case CGameTimer::BACKGROUND_REVERSE:
    m_pGameBackground->change_state(CGameBackground::NORMAL);
    m_pGameBackground->turn_speed_fast(false);
    break;
  case CGameTimer::BRICK_THEME_CHANGE:
    for (int i = 0; i < m_nAryGameBrickCount; i++)
    {
      m_pAryGameBrick[i]->change_brick_theme(CGameBrick::NORMAL_WALL);
    }
  case CGameTimer::MAKE_GHOST_FRIGHENTED:
    for (int i = 0; i < m_nAryGameGhostCount; i++)
    {
      m_pAryGameGhost[i]->make_ghost_not_frighented();
    }
    break;
  case CGameTimer::GAME_OVER_PAUSE:
    m_nStatus = CGameController::GAME_RUNNING;
    break;
  default:
    break;
  }

  return 0;
}


int CGameController::when_player_hit_energized_pean()
{
  m_pGameBackground->change_state(CGameBackground::REVERSE);
  m_pGameBackground->turn_speed_fast(true);
  m_pGameTimer->set_timer_triger(CGameTimer::BACKGROUND_REVERSE);

  for (int i = 0; i < m_nAryGameBrickCount; i++)
  {
    m_pAryGameBrick[i]->change_brick_theme(CGameBrick::UNIVERSE);
  }
  m_pGameTimer->set_timer_triger(CGameTimer::BRICK_THEME_CHANGE);

  for (int i = 0; i < m_nAryGameGhostCount; i++)
  {
    m_pAryGameGhost[i]->make_ghost_frightened();
  }
  m_pGameTimer->set_timer_triger(CGameTimer::MAKE_GHOST_FRIGHENTED);

  return 0;
}


CGameGhost * CGameController::get_blinky()
{
  return m_pGameGhostBlinky;
}

//用于检查Ghost之间的相互碰撞（并不是和player碰撞）
int CGameController::check_ghost_collision()
{
  for (int i = 0; i < m_nAryGameGhostCount; i++)
  {
    for (int j = 0; j < m_nAryGameGhostCount; j++)
    {
      if (i != j && 
        m_pAryGameGhost[i]->GetCoordX() == m_pAryGameGhost[j]->GetCoordX() &&
        m_pAryGameGhost[i]->GetCoordY() == m_pAryGameGhost[j]->GetCoordY())
      {
        m_pAryGameGhost[i]->reverse_direction();
      }
    }
  }
  return 0;
}


int CGameController::player_hit_ghost()
{
  for (int i = 0; i < m_nAryGameGhostCount; i++)
  {
    if (m_pGamePlayer->GetCoordX() == m_pAryGameGhost[i]->GetCoordX() &&
        m_pGamePlayer->GetCoordY() == m_pAryGameGhost[i]->GetCoordY())
    {
      //发生碰撞
      if (m_pAryGameGhost[i]->get_ghost_mode() == CGameGhost::CHASE)
      {
        //game over
        for (int g = 0; g < m_nAryGameGhostCount; g++)
        {
          m_pAryGameGhost[g]->forcely_set_ghost_to_init();
        }

        m_nStatus = CGameController::GAME_STATUS_PAUSE;
        m_pGameTimer->set_timer_triger(CGameTimer::GAME_OVER_PAUSE);

        return 0;
      }

      if (m_pAryGameGhost[i]->get_ghost_mode() == CGameGhost::FRIGHTENED)
      {
        m_pAryGameGhost[i]->make_ghost_scatter();
      }
    }
  }
  return 0;
}


int CGameController::run_game()
{
  if (m_nStatus == GAME_START)
  {
    m_nStatus = GAME_RUNNING;
  }
  return 0;
}


int CGameController::player_hit_telepoint(int nCoordX, int nCoordY)
{
  /*简单地处理，因为只有两个固定的传送点（标准做法是遍历CGameTelepoint对象的数组（链表
  ））。*/
  if (nCoordX == 0 && nCoordY == 16)
  {
    m_pGamePlayer->set_coordX(31);
    m_pGamePlayer->set_offset_within_tile(-1);
    player_hit_pean(31, 16);
  }

  if (nCoordX == 31 && nCoordY == 16)
  {
    m_pGamePlayer->set_coordX(0);
    m_pGamePlayer->set_offset_within_tile(1);
    player_hit_pean(0, 16);


  }

  return 0;
}
