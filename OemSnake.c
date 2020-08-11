#include "Commoninit.h"
#include "Background.h"
#include "Exit.h"
#include "KeyMap.h"
#include "Snake.h"
#include "ScreenLayer.h"
#include "OemSnake.h"

OEM_SNAKE_INIT_LIST OemSnakeInitList[] = {CommonInit, ScreenLayerInit , BackgroundInit, KepMapInit ,SnakeInit, ExitInit, NULL};
OEM_SNAKE_END_LIST  OemSnakeEndList[]  = {SnakeEnd, BackgroundEnd, ScreenLayerExit, KepMapEnd, CommonEnd, NULL};

EFI_STATUS
EFIAPI
OemSnakeEntry (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS      Status = EFI_SUCCESS;
    UINT8           i = 0;

    // Misc init
    while(OemSnakeInitList[i] != NULL){
        Status = OemSnakeInitList[i++]();
        if(EFI_ERROR(Status)){
            Print(L"OemSnakeInitList[%d] : %r \r\n", i, Status);
            DEBUG ((DEBUG_ERROR, "%a %d OemSnakeInitList[%d] : %r \n", __FUNCTION__, __LINE__, i, Status));
            return Status;
        }
    }

    while (1)
    {
        CpuSleep();//For reduce power consumption
    }
    
    return Status;
}