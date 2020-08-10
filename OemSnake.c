#include "Commoninit.h"
#include "Background.h"
#include "Exit.h"
#include "KeyMap.h"
#include "Snake.h"
#include "ScreenLayer.h"
#include "OemSnake.h"

OEM_SNAKE_INIT_LIST OemSnakeInitList[] = {CommonInit, ScreenLayerInit , BackgroundInit, SnakeInit, NULL};
OEM_SNAKE_END_LIST  OemSnakeEndList[]  = {SnakeEnd, BackgroundEnd, ScreenLayerExit, CommonEnd, NULL};

//Need implement Link list to dynamic add or remove keys.
OEM_SNAKE_KEYMAP    OemSnakeKeymap[] = {
    //Key                      KeyState
    //Scancode  UnicodeChar    KeyShiftState   KeyToggleState
    { {{SCAN_ESC,        0},        {0,               0}}, onExit},//Exit
    // { {{SCAN_UP,         0},        {0,               0}}, onArrowUp},// Up
    // { {{SCAN_DOWN,       0},        {0,               0}}, onArrowDown},// Down
    // { {{SCAN_LEFT,       0},        {0,               0}}, onArrowLeft},// Left
    // { {{SCAN_RIGHT,      0},        {0,               0}}, onArrowRight},// Right
    // { {{SCAN_F5,         0},        {0,               0}}, SnakeStart},// Enter GameStart
    { {{0,               0},        {0,               0}}, NULL}// MapEnd
};

EFI_HANDLE      GlobalKeyHandle = NULL;

EFI_STATUS
EFIAPI
KeyBindingFunction(
    IN EFI_KEY_DATA *KeyData
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT8      i      = 0;
    //Match hotkeys
    while (OemSnakeKeymap[i].KeyData.Key.ScanCode ||
            OemSnakeKeymap[i].KeyData.Key.UnicodeChar ||
            OemSnakeKeymap[i].KeyData.KeyState.KeyShiftState ||
            OemSnakeKeymap[i].KeyData.KeyState.KeyToggleState 
    )
    {
        if(OemSnakeKeymap[i].KeyData.Key.ScanCode)
        {
            if(OemSnakeKeymap[i].KeyData.Key.ScanCode != KeyData->Key.ScanCode)
            {
                i++;
                continue;
            }
        }
        if(OemSnakeKeymap[i].KeyData.Key.UnicodeChar)
        {
            if(OemSnakeKeymap[i].KeyData.Key.UnicodeChar != KeyData->Key.UnicodeChar)
            {
                i++;
                continue;
            }
        }
        if(OemSnakeKeymap[i].KeyData.KeyState.KeyShiftState)
        {
            if(OemSnakeKeymap[i].KeyData.KeyState.KeyShiftState != KeyData->KeyState.KeyShiftState)
            {
                i++;
                continue;
            }
        }
        if(OemSnakeKeymap[i].KeyData.KeyState.KeyToggleState)
        {
            if(OemSnakeKeymap[i].KeyData.KeyState.KeyToggleState != KeyData->KeyState.KeyToggleState)
            {
                i++;
                continue;
            }
        }

        Status = OemSnakeKeymap[i].Callback(KeyData);
        if(EFI_ERROR(Status)){
            Print(L"OemSnakeKeymap[%d].Callback : %r \r\n", Status);
            DEBUG ((DEBUG_ERROR, "%a %d OemSnakeInitList[%d] : %r \n", __FUNCTION__, __LINE__, i, Status));
            return Status;
        }
        i++;
    }

    return Status;
}

EFI_STATUS
EFIAPI
OemSnakeEntry (
    IN EFI_HANDLE        ImageHandle,
    IN EFI_SYSTEM_TABLE  *SystemTable
)
{
    EFI_STATUS      Status = EFI_SUCCESS;
    UINT8           i = 0;
    EFI_KEY_DATA	KeyData = {{0,0},{0,0}};//Used to globalKeyBinding

    // Misc init
    while(OemSnakeInitList[i] != NULL){
        Status = OemSnakeInitList[i++]();
        if(EFI_ERROR(Status)){
            Print(L"OemSnakeInitList[%d] : %r \r\n", i, Status);
            DEBUG ((DEBUG_ERROR, "%a %d OemSnakeInitList[%d] : %r \n", __FUNCTION__, __LINE__, i, Status));
            return Status;
        }
    }

    if(SimpleEx != NULL){
        Status = SimpleEx->RegisterKeyNotify (
            SimpleEx,
            &KeyData,
            KeyBindingFunction,
            &GlobalKeyHandle
        );
        if(EFI_ERROR(Status)){
            DEBUG ((DEBUG_ERROR, "%a %d RegisterKeyNotify Status: %r \n", __FUNCTION__, __LINE__, Status));
            return Status;
        }
    }
    else
    {
        Print(L"SimpleEx == NULL\r\n");
    }

    while (1)
    {
        CpuSleep();//For reduce power consumption
    }
    
    return Status;
}