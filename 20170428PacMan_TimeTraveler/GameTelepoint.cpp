#include "GameTelepoint.h"

/*
CGameTelepoint::CGameTelepoint()
{
}*/
CGameTelepoint::CGameTelepoint(int nCoordX, int nCoordY, int nTeleDestCoordX, int nTeleDestCoordY)
  :CGameElement(nCoordX, nCoordY)
{
  //1. �趨Telepoint�Լ��Ĳ���
  m_nTeleDestCoordX = nTeleDestCoordX;
  m_nTeleDestCoordY = nTeleDestCoordY;
  
  //2. ��дһЩTelepoint�ض��������̳���GameElement�ࣩ

  //3. ��дһЩTelepoint�ض��������̳���GameElement�ࣩ:��д��ͼhandle

}

CGameTelepoint::~CGameTelepoint()
{
}

int CGameTelepoint::update()
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

  }
  return 0;
}


int CGameTelepoint::get_tele_dest_coordX()
{
  return m_nTeleDestCoordX;
}


int CGameTelepoint::get_tele_dest_coordY()
{
  return m_nTeleDestCoordY;
}
