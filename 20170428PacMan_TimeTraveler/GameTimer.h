#pragma once
#include "GameElement.h"
#include "GameController.h"

const int constnMaxCountForBackgroundReverse = 1200;
const int constnMaxCountForBrickThemeChange = 1200;
const int constnMaxCountForMakeGhostFrighented = 1200;
const int constnMaxCountForGameOverPause = 900;




class CGameController;

//������Ϸ�еĸ��ֵ���ʱ�������õ���ʱ��������ɱ�����ֶΣ�������ʱ���ʺϣ���Ϊ�������ж����õ������飬����������
class CGameTimer :
  public CGameElement
{
public:

  enum eTimerTriger
  {
    BACKGROUND_REVERSE = 0,
    BRICK_THEME_CHANGE = 1,
    MAKE_GHOST_FRIGHENTED = 2,
    GAME_OVER_PAUSE = 3,
  };

  CGameTimer();
  virtual ~CGameTimer();
private:
  int m_nCountForBackgroundReverse;
  int m_nCountForBrickThemeChange;
  int m_nCountForMakeGhostFrighented;
  int m_nCountForGameOverPause;


  CGameController *m_pGameController;

public:
  virtual int update();
  int set_timer_triger(eTimerTriger eName);
private:
  int specific_timer(int &nCounter, int nMax, eTimerTriger eName);
};

