#pragma once
#include "SingleInstanceMacro.h"
#include "GameMap.h"
#include "GameElement.h"
#include "GamePlayer.h"
#include "GameTimer.h"
#include "GameBackground.h"
#include "GameBrick.h"
#include "GameDrawer.h"

const int constnMaxArray = 2000;  //临时的

//此类用于管理游戏进程。单例模式。
class CGamePlayer;
class CGamePean;
class CGameBackground;
class CGameTimer;
class CGameBrick;
class CGameGhost;

class CGameDrawer;

class CGameController
{
public:

  enum eGameControllerStatus
  {
    GAME_START,
    GAME_RUNNING,
    GAME_STATUS_PAUSE
  };

  SINGLE_INSTANCE(CGameController)

private:
  CGameController();
  virtual ~CGameController();
public:
  int init(HWND hwnd);

private:
  //游戏地图
  char m_chGameMap[constnMapHeight][constnMapWidth];
  //保存玩家的指针
  CGamePlayer *m_pGamePlayer;
  //保存豆子的指针（也就是建一个表，方便快速查询）
  CGamePean *m_pAryGamePean[constnMapHeight][constnMapWidth];
  //保存背景的指针
  CGameBackground *m_pGameBackground;
  //保存计时器的指针
  CGameTimer *m_pGameTimer;
  //保存墙体的指针
  CGameBrick *m_pAryGameBrick[constnMaxArray];
  int m_nAryGameBrickCount;
  //保存第一个Blinky的指针（用于Inky的寻路）
  CGameGhost *m_pGameGhostBlinky;
  CGameGhost *m_pAryGameGhost[constnMaxArray];
  int m_nAryGameGhostCount;
  //保存画图类的指针
  CGameDrawer *m_pGameDrawer;
  eGameControllerStatus m_nStatus;
  HBITMAP hDrawHandelForGameOverPause;
  HBITMAP hDrawHandelForGameStart;

  

public:
  //int make_element_update();  //不用了
  int loop();
  int terminate(HWND hwnd);
  char query_map_teil_status(int nCoordX, int nCoordY);

  CGamePlayer* get_GamePlayer();
  int player_hit_pean(int nCoordX, int nCoordY);
  int timer_finished(int eName);
private:
  int when_player_hit_energized_pean();
public:
  CGameGhost * get_blinky();
private:
  int check_ghost_collision();
public:
  int player_hit_ghost();
  int run_game();
  int player_hit_telepoint(int nCoordX, int nCoordY);
};
