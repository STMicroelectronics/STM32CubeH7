#include <gui/model/Model.hpp>
#include <gui/model/ModelListener.hpp>
#include "bsp.h"

Model::Model() : modelListener(0)
{
}

void Model::tick()
{
}

void Model::moduleSelected(int moduleIndex)
{
int SubDemoAddress = 0x0;
  /* Store the address of the Sub Demo binary */
  switch (moduleIndex)
  {
  case 1:
    SubDemoAddress = 0x08100000; /* Audio player jump address */
    break;
  case 2:
    SubDemoAddress = 0x08020000; /* Touch GFX jump address */
    break;
  case 3:
    SubDemoAddress = 0x081A0000; /* Clock & Weather jump address */
    break;
  case 4:
    SubDemoAddress = 0x08060000; /*  STemWin jump address */
    break;
  case 5:
    SubDemoAddress = 0x080A0000; /* Embedded Wizard jump address */
    break;
  default:
    break;
  }
  if(SubDemoAddress != 0x0)
  {
    BSP_SetSubDemoAddress(SubDemoAddress);
  }

}

void Model::setFirmwareVersion(const char* fwVersion)
{
    modelListener->setFirmwareVersion(fwVersion);
}
