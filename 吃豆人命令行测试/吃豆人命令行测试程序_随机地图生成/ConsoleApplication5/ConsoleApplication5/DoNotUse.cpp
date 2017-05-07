#include "stdafx.h"
#include "DoNotUse.h"
#include "SingleInstanceMacro.h"
//#include "GameController.h"
#include "GameMap.h"


CDoNotUse::CDoNotUse()
{
  //CREATE_SINGLE(CGameController)

  CREATE_SINGLE(CGameMap)

}


CDoNotUse::~CDoNotUse()
{
  //RELEASE_SINGLE(CGameController)

  RELEASE_SINGLE(CGameMap)
}
