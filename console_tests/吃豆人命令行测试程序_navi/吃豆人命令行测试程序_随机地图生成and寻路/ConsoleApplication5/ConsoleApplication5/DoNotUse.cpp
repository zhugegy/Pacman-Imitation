#include "stdafx.h"
#include "DoNotUse.h"
#include "SingleInstanceMacro.h"
//#include "GameController.h"
#include "GameMap.h"
#include "GYAStarNavi.h"


CDoNotUse::CDoNotUse()
{
  //CREATE_SINGLE(CGameController)

  CREATE_SINGLE(CGameMap)
  CREATE_SINGLE(CGYAStarNavi)

}


CDoNotUse::~CDoNotUse()
{
  //RELEASE_SINGLE(CGameController)

  RELEASE_SINGLE(CGameMap)

  RELEASE_SINGLE(CGYAStarNavi)

}
