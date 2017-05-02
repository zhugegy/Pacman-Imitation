#include "GameBackground.h"


CGameBackground::CGameBackground()
  :CGameElement(0, 0)
{
  //1. 设定Backgound自己的参数
  m_eState = NORMAL;

  //2. 重写一些Pean特定参数（继承自GameElement类）
  m_bIsVisble = true;
  m_nMaxFrameNum = constnMaxFrameNumBackground;
  m_nDrawPointGoal = constnDrawPointGoalBackground;

  //3. 重写一些Pean特定参数（继承自GameElement类）:重写画图handle
  hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\bg.bmp", IMAGE_BITMAP,
    constnDrawSizeWidthBackground * 13,
    constnDrawSizeHeightBackground * 3, LR_LOADFROMFILE);
}


CGameBackground::~CGameBackground()
{
}


int CGameBackground::update()
{
  //0. 检查自己状态（是否发生碰撞），只适用于player

  //1.action 执行动作（移动，看指令列表，变向，寻路等）
  if (m_bIsActive == true)
  {
    m_nActPoint++;
    if (m_nActPoint >= m_nActPointGoal)
    {
      m_nActPoint = 0;
    }
  }

  //2.draw 画图
  if (m_bIsVisble == true)
  {
    int nDisplayCoordX = 0;
    int nDisplayCoordY = 0;

    int nDrawSourceCoordX = 0;
    int nDrawSourceCoordY = 0;

    cal_source_draw_coordX(m_nCurrentFrame, nDrawSourceCoordX, nDrawSourceCoordY);

    //按照目前的移动方向取出对应人物的连续走动图，并确定截取人物图的宽度与高度
    SelectObject(CGameDrawer::m_bufdc, hDrawHandel);
    BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY, constnDrawSizeWidthBackground, constnDrawSizeHeightBackground, CGameDrawer::m_bufdc, nDrawSourceCoordX, nDrawSourceCoordY, SRCCOPY);

    m_nDrawPoint++;
    if (m_nDrawPoint >= m_nDrawPointGoal)
    {
      m_nDrawPoint = 0;

      if (m_eState == NORMAL)
      {
        m_nCurrentFrame++;
      }

      if (m_eState == REVERSE)
      {
        m_nCurrentFrame--;
      }

      if (m_nCurrentFrame > m_nMaxFrameNum)
      {
        m_nCurrentFrame = 0;
      }

      if (m_nCurrentFrame < 0)
      {
        m_nCurrentFrame = m_nMaxFrameNum;
      }

    }
  }

  return 0;
}

//13*3的排布
int CGameBackground::cal_source_draw_coordX(int nFrameNum, int &nDrawCoordX, int &nDrawCoordY)
{
  int nTmp = nFrameNum / 13;
  nDrawCoordY = nTmp * constnDrawSizeHeightBackground;
  nDrawCoordX = (nFrameNum - nTmp * 13) * constnDrawSizeWidthBackground;

  return 0;
}


int CGameBackground::change_state(eBackGroundState eName)
{
  m_eState = eName;
  return 0;
}


int CGameBackground::turn_speed_fast(bool bBool)
{
  if (bBool == true)
  {
    m_nDrawPointGoal = constnDrawPointGoalBackgroundFast;
  }
  else
  {
    m_nDrawPointGoal = constnDrawPointGoalBackground;
  }

  return 0;
}
