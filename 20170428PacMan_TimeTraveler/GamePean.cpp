#include "GamePean.h"
#include "GameDrawer.h"


/*
CGamePean::CGamePean()
{
}*/


CGamePean::~CGamePean()
{
}


CGamePean::CGamePean(int nCoordX, int nCoordY, ePeanState eState)
  :CGameElement(nCoordX, nCoordY)
{
  //1. 设定Pean自己的参数
  m_nState = eState;

  //2. 重写一些Pean特定参数（继承自GameElement类）
  m_nMaxFrameNum = constnMaxFrameNumPean;
  m_nDrawPointGoal = constnDrawPointGoalPean;
  m_nDrawOffsetX = (constnElementDefaultDrawWidth - constnDrawSizeWidthPean) / 2;
  m_nDrawOffsetY = (constnElementDefaultDrawHeight - constnDrawSizeHeightPean) / 2;
  m_bIsVisble = true;
  
  //3. 重写一些Pean特定参数（继承自GameElement类）:重写画图handle
  switch (eState)
  {
  case NORMAL_PEAN:
    hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\pean0.bmp", IMAGE_BITMAP, 
      constnDrawSizeWidthPean * (constnMaxFrameNumPean + 1), 
      constnDrawSizeHeightPean * 2, LR_LOADFROMFILE);
    break;
  case ENERGIZER_PEAN:
    hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\pean1.bmp", IMAGE_BITMAP, 
      constnDrawSizeWidthPean * (constnMaxFrameNumPean + 1), 
      constnDrawSizeHeightPean * 2, LR_LOADFROMFILE);
    break;
  default:
    break;
  }

}


int CGamePean::update()
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
    int nDisplayCoordX = CGameDrawer::coordX_to_display_transfer(m_nCoordX) + m_nDrawOffsetX;
    int nDisplayCoordY = CGameDrawer::coordY_to_display_transfer(m_nCoordY) + m_nDrawOffsetY;

    //按照目前的移动方向取出对应人物的连续走动图，并确定截取人物图的宽度与高度
    SelectObject(CGameDrawer::m_bufdc, hDrawHandel);
    BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY, 
      constnDrawSizeWidthPean, constnDrawSizeHeightPean,
      CGameDrawer::m_bufdc, m_nCurrentFrame * constnDrawSizeWidthPean, 
      constnDrawSizeHeightPean, SRCAND);
    BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY, 
      constnDrawSizeWidthPean, constnDrawSizeHeightPean,
      CGameDrawer::m_bufdc, m_nCurrentFrame * constnDrawSizeWidthPean, 
      0, SRCPAINT);

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


bool CGamePean::is_visible()
{
  return m_bIsVisble;
}


int CGamePean::change_visible(bool bStatus)
{
  m_bIsVisble = bStatus;
  return 0;
}


int CGamePean::get_state()
{
  return m_nState;
}
