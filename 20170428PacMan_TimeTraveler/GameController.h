#pragma once
#include "SingleInstanceMacro.h"
#include "GameMap.h"
#include "GameElement.h"
#include "GamePlayer.h"
#include "GameTimer.h"
#include "GameBackground.h"
#include "GameBrick.h"
#include "GameDrawer.h"

const int constnMaxArray = 2000;  //��ʱ��

//�������ڹ�����Ϸ���̡�����ģʽ��
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
  //��Ϸ��ͼ
  char m_chGameMap[constnMapHeight][constnMapWidth];
  //������ҵ�ָ��
  CGamePlayer *m_pGamePlayer;
  //���涹�ӵ�ָ�루Ҳ���ǽ�һ����������ٲ�ѯ��
  CGamePean *m_pAryGamePean[constnMapHeight][constnMapWidth];
  //���汳����ָ��
  CGameBackground *m_pGameBackground;
  //�����ʱ����ָ��
  CGameTimer *m_pGameTimer;
  //����ǽ���ָ��
  CGameBrick *m_pAryGameBrick[constnMaxArray];
  int m_nAryGameBrickCount;
  //�����һ��Blinky��ָ�루����Inky��Ѱ·��
  CGameGhost *m_pGameGhostBlinky;
  CGameGhost *m_pAryGameGhost[constnMaxArray];
  int m_nAryGameGhostCount;
  //���滭ͼ���ָ��
  CGameDrawer *m_pGameDrawer;
  eGameControllerStatus m_nStatus;
  HBITMAP hDrawHandelForGameOverPause;
  HBITMAP hDrawHandelForGameStart;

  

public:
  //int make_element_update();  //������
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
