#include "DoNotUse.h"
#include "SingleInstanceMacro.h"
#include "GameController.h"
#include "GameMap.h"
#include "GYAStarNavi.h"
#include "GameDrawer.h"


CDoNotUse::CDoNotUse()
{
  CREATE_SINGLE(CGameController)

  CREATE_SINGLE(CGameMap)

  CREATE_SINGLE(CGYAStarNavi)

  CREATE_SINGLE(CGameDrawer)


}


CDoNotUse::~CDoNotUse()
{
  RELEASE_SINGLE(CGameController)

  RELEASE_SINGLE(CGameMap)

  RELEASE_SINGLE(CGYAStarNavi)

  RELEASE_SINGLE(CGameDrawer)

}
