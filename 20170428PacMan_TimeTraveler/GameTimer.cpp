#include "GameTimer.h"
#include "SingleInstanceMacro.h"


CGameTimer::CGameTimer()
  :CGameElement(0, 0)
{
  //1. 设定Timer自己的参数
  m_nCountForBackgroundReverse = 0;
  m_nCountForBrickThemeChange = 0;
  m_nCountForMakeGhostFrighented = 0;
  m_nCountForGameOverPause = 0;

  m_pGameController = GET_SINGLE(CGameController);

  //2. 重写一些Timer特定参数（继承自GameElement类）
  

  //3. 重写一些Timer特定参数（继承自GameElement类）:重写画图handle
  
}


CGameTimer::~CGameTimer()
{
}


int CGameTimer::update()
{
  //0. 检查自己状态（是否发生碰撞），只适用于player

  //1.action 执行动作（移动，看指令列表，变向，寻路等）

  //2.draw 画图

  //T. Timer特殊步骤
  
  /*specific_timer(m_nCountForBackgroundReverse, constnMaxCountForBackgroundReverse, BACKGROUND_REVERSE);
  specific_timer(m_nCountForBrickThemeChange, constnMaxCountForBrickThemeChange, BRICK_THEME_CHANGE);
  specific_timer(m_nCountForMakeGhostFrighented, constnMaxCountForMakeGhostFrighented, MAKE_GHOST_FRIGHENTED);
  specific_timer(m_nCountForGameOverPause, constnMaxCountForGameOverPause, GAME_OVER_PAUSE);*/

  if (m_nCountForBackgroundReverse != 0)
  {
    specific_timer(m_nCountForBackgroundReverse, constnMaxCountForBackgroundReverse, BACKGROUND_REVERSE);
  }

  if (m_nCountForBrickThemeChange != 0)
  {
    specific_timer(m_nCountForBrickThemeChange, constnMaxCountForBrickThemeChange, BRICK_THEME_CHANGE);

  }

  if (m_nCountForMakeGhostFrighented != 0)
  {
    specific_timer(m_nCountForMakeGhostFrighented, constnMaxCountForMakeGhostFrighented, MAKE_GHOST_FRIGHENTED);

  }

  if (m_nCountForGameOverPause != 0)
  {
    specific_timer(m_nCountForGameOverPause, constnMaxCountForGameOverPause, GAME_OVER_PAUSE);

  }

  return 0;
}


int CGameTimer::set_timer_triger(eTimerTriger eName)
{
  switch (eName)
  {
  case BACKGROUND_REVERSE:
    m_nCountForBackgroundReverse = 1;
    break;
  case BRICK_THEME_CHANGE:
    m_nCountForBrickThemeChange = 1;
    break;
  case MAKE_GHOST_FRIGHENTED:
    m_nCountForMakeGhostFrighented = 1;
    break;
  case GAME_OVER_PAUSE:
    m_nCountForGameOverPause = 1;
    break;
  default:
    break;
  }

  return 0;
}


int CGameTimer::specific_timer(int &nCounter, int nMax, eTimerTriger eName)
{
  /*if (nCounter != 0)
  {
    nCounter++;
  }*/
  nCounter++;

  if (nCounter > nMax)
  {
    nCounter = 0;
    m_pGameController->timer_finished(eName);
  }

  return 0;
}
