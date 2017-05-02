#include "GameBrick.h"
#include "GameMap.h"
#include "GameDrawer.h"

/*
CGameBrick::CGameBrick()
{
}*/


CGameBrick::~CGameBrick()
{
}


CGameBrick::CGameBrick(int nCoordX, int nCoordY)
  :CGameElement(nCoordX, nCoordY)
{
  //1. �趨Brick�Լ��Ĳ���
  m_nBrickTheme = NORMAL_WALL;

  //2. ��дһЩPean�ض��������̳���GameElement�ࣩ
  m_bIsVisble = true;
  m_nMaxFrameNum = constnMaxFrameNumBrick;
  m_nDrawOffsetX = (constnElementDefaultDrawWidth - constnDrawSizeWidthBrick) / 2;
  m_nDrawOffsetY = (constnElementDefaultDrawHeight - constnDrawSizeHeightBrick) / 2;
  
  //3. ��дһЩPean�ض��������̳���GameElement�ࣩ:��д��ͼhandle
  hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\brick.bmp", IMAGE_BITMAP,
    constnDrawSizeWidthBrick * constnMapWidth  * (constnMaxFrameNumBrick + 1),
    constnDrawSizeHeightBrick * constnMapHeight * 2, LR_LOADFROMFILE);
  
}


int CGameBrick::update()
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

    SelectObject(CGameDrawer::m_bufdc, hDrawHandel);
    if (m_nBrickTheme == NORMAL_WALL)
    {
      BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY, constnDrawSizeWidthBrick, constnDrawSizeHeightBrick, CGameDrawer::m_bufdc, m_nCoordX * constnDrawSizeWidthBrick, m_nCoordY * constnDrawSizeHeightBrick, SRCCOPY);
    }
    else
    {
      BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY, constnDrawSizeWidthBrick, constnDrawSizeHeightBrick, CGameDrawer::m_bufdc, m_nCoordX * constnDrawSizeWidthBrick, (m_nCoordY + constnMapHeight) * constnDrawSizeHeightBrick, SRCCOPY);
    }

  }

  return 0;
}


int CGameBrick::change_brick_theme(eBrickThemeName eName)
{
  m_nBrickTheme = eName;
  return 0;
}
