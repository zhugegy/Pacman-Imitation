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
  //1. �趨Pean�Լ��Ĳ���
  m_nState = eState;

  //2. ��дһЩPean�ض��������̳���GameElement�ࣩ
  m_nMaxFrameNum = constnMaxFrameNumPean;
  m_nDrawPointGoal = constnDrawPointGoalPean;
  m_nDrawOffsetX = (constnElementDefaultDrawWidth - constnDrawSizeWidthPean) / 2;
  m_nDrawOffsetY = (constnElementDefaultDrawHeight - constnDrawSizeHeightPean) / 2;
  m_bIsVisble = true;
  
  //3. ��дһЩPean�ض��������̳���GameElement�ࣩ:��д��ͼhandle
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
  //0. ����Լ�״̬���Ƿ�����ײ����ֻ������player

  //1.action ִ�ж������ƶ�����ָ���б�����Ѱ·�ȣ�
  if (m_bIsActive == true)
  {
    m_nActPoint++;
    if (m_nActPoint >= m_nActPointGoal)
    {
      m_nActPoint = 0;
    }
  }
  
  //2.draw ��ͼ
  if (m_bIsVisble == true)
  {
    int nDisplayCoordX = CGameDrawer::coordX_to_display_transfer(m_nCoordX) + m_nDrawOffsetX;
    int nDisplayCoordY = CGameDrawer::coordY_to_display_transfer(m_nCoordY) + m_nDrawOffsetY;

    //����Ŀǰ���ƶ�����ȡ����Ӧ����������߶�ͼ����ȷ����ȡ����ͼ�Ŀ����߶�
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
