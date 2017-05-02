#include "GamePlayer.h"
#include "GameDrawer.h"
#include <math.h>


/*
CGamePlayer::CGamePlayer()
{
}*/


CGamePlayer::~CGamePlayer()
{
}


CGamePlayer::CGamePlayer(int nCoordX, int nCoordY)
  :CGameElement(nCoordX, nCoordY)
{
  //1. �趨player�Լ��Ĳ���
  m_nDirection = UP;
  m_nOffsetWithinTile = 0;
  m_pGameController = GET_SINGLE(CGameController);
  m_nDirectionAttempt = NO_DIRECTION;

  //2. ��дһЩPean�ض��������̳���GameElement�ࣩ
  m_bIsVisble = true;
  m_bIsActive = true;
  m_nMaxFrameNum = constnMaxFrameNumPlayer;
  m_nActPointGoal = constnActPointGoalPlayer;
  m_nDrawPointGoal = constnDrawPointGoalPlayer;

  m_nDrawOffsetX = (constnElementDefaultDrawWidth - constnDrawSizeWidthPlayer) / 2;
  m_nDrawOffsetY = constnElementDefaultDrawHeight - constnDrawSizeHeightPlayer;

  //3. ��дһЩPean�ض��������̳���GameElement�ࣩ:��д��ͼhandle
  hDrawHandel = (HBITMAP)LoadImage(NULL, L"game_resource\\player.bmp", IMAGE_BITMAP,
    constnDrawSizeWidthPlayer * (constnMaxFrameNumPlayer + 1),
    constnDrawSizeHeightPlayer * 4 * 2, LR_LOADFROMFILE);

}


int CGamePlayer::update()
{
  //0. ����Լ�״̬���Ƿ�����ײ����ֻ������player
  m_pGameController->player_hit_ghost();

  if (m_nOffsetWithinTile == 0)
  {
    m_pGameController->player_hit_pean(m_nCoordX, m_nCoordY);
    m_pGameController->player_hit_telepoint(m_nCoordX, m_nCoordY);
  }

  //���ײghost


  //1.action ִ�ж������ƶ�����ָ���б�����Ѱ·�ȣ�
  if (m_bIsActive == true)
  {
    m_nActPoint++;
    if (m_nActPoint >= m_nActPointGoal)
    {
      m_nActPoint = 0;

      //������ҵ�ת����ͼ
      if (m_nOffsetWithinTile == 0 && m_nDirectionAttempt != NO_DIRECTION &&
        is_movable(m_nDirectionAttempt) == true)
      {
        m_nDirection = m_nDirectionAttempt;
        m_nDirectionAttempt = NO_DIRECTION;
      }

      if (m_nOffsetWithinTile == 0 && is_movable(m_nDirection) == false)
      {
        //ֹͣ�ƶ�
      }
      else
      {
        //�����ƶ�
        keep_moving();
      }

      if (abs(m_nOffsetWithinTile) == constnElementDefaultDrawWidth / 2)
      {
        switch_tile();
        m_nOffsetWithinTile = m_nOffsetWithinTile * (-1);
      }
    }
       
  }

  //2.draw ��ͼ
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

    //����Ŀǰ���ƶ�����ȡ����Ӧ����������߶�ͼ����ȷ����ȡ����ͼ�Ŀ����߶�
    SelectObject(CGameDrawer::m_bufdc, hDrawHandel);
    BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY,
      constnDrawSizeWidthPlayer, constnDrawSizeHeightPlayer,
      CGameDrawer::m_bufdc, m_nCurrentFrame * constnDrawSizeWidthPlayer,
      (4 + m_nDirection) * constnDrawSizeHeightPlayer, SRCAND);
    BitBlt(CGameDrawer::m_mdc, nDisplayCoordX, nDisplayCoordY,
      constnDrawSizeWidthPlayer, constnDrawSizeHeightPlayer,
      CGameDrawer::m_bufdc, m_nCurrentFrame * constnDrawSizeWidthPlayer,
      m_nDirection * constnDrawSizeHeightPlayer, SRCPAINT);

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


bool CGamePlayer::is_movable(eGameDirection nDirection)
{
  char chDestContent = CGameMap::MAP_BRICK;

  switch (nDirection)
  {
  case CGameElement::UP:
    if (m_nCoordY - 1 >= 0)
    {
      chDestContent = m_pGameController->query_map_teil_status(
        m_nCoordX, m_nCoordY - 1);
    }
    break;
  case CGameElement::LEFT:
    if (m_nCoordX - 1 >= 0)
    {
      chDestContent = m_pGameController->query_map_teil_status(
        m_nCoordX - 1, m_nCoordY);
    }
    break;
  case CGameElement::DOWN:
    if (m_nCoordY + 1 < constnMapHeight)
    {
      chDestContent = m_pGameController->query_map_teil_status(
        m_nCoordX, m_nCoordY + 1);
    }
    break;
  case CGameElement::RIGHT:
    if (m_nCoordX + 1 < constnMapWidth)
    {
      chDestContent = m_pGameController->query_map_teil_status(
        m_nCoordX + 1, m_nCoordY);
    }
    break;
  default:
    break;
  }

  if (chDestContent == CGameMap::MAP_PATH)
  {
    return true;
  }

  return false;
}


int CGamePlayer::keep_moving()
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


int CGamePlayer::switch_tile()
{
  switch (m_nDirection)
  {
  case CGameElement::UP:
    m_nCoordY--;
    break;
  case CGameElement::LEFT:
    m_nCoordX--;
    break;
  case CGameElement::DOWN:
    m_nCoordY++;
    break;
  case CGameElement::RIGHT:
    m_nCoordX++;
    break;
  default:
    break;
  }

  return 0;
}


int CGamePlayer::give_change_direction_attemption(eGameDirection nDirection)
{
  m_nDirectionAttempt = nDirection;
  return 0;
}

CGameElement::eGameDirection CGamePlayer::get_direction()
{
  return m_nDirection;
}


int CGamePlayer::set_coordX(int nCoordX)
{
  m_nCoordX = nCoordX;
  return 0;
}


int CGamePlayer::set_coordY(int nCoordY)
{
  m_nCoordY = nCoordY;
  return 0;
}


int CGamePlayer::set_offset_within_tile(int nOffset)
{
  m_nOffsetWithinTile = nOffset;
  return 0;
}
