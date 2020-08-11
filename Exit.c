#include "Exit.h"
#include "KeyMap.h"

extern EFI_HANDLE      GlobalKeyHandle;

typedef EFI_STATUS (EFIAPI *OEM_SNAKE_END_LIST) (VOID);
extern OEM_SNAKE_END_LIST OemSnakeEndList[];

EFI_STATUS
EFIAPI
KeyExitCallback(EFI_KEY_DATA *KeyData)
{
    EFI_STATUS Status = EFI_SUCCESS;
    UINT8           i = 0;

    // Misc end
    while(OemSnakeEndList[i] != NULL){
        Status = OemSnakeEndList[i++]();
        if(EFI_ERROR(Status)){
            Print(L"OemSnakeEndList[%d] : %r \r\n", i, Status);
            DEBUG ((DEBUG_ERROR, "%a %d OemSnakeEndList[%d] : %r \n", __FUNCTION__, __LINE__, i, Status));
            // return Status;
        }
    }

    if(SimpleOut != NULL)
    {
        Status = SimpleOut->ClearScreen(SimpleOut);
        if(EFI_ERROR(Status)){
            DEBUG ((DEBUG_ERROR, "%a %d ClearScreen Status: %r \n", __FUNCTION__, __LINE__, Status));
            // return Status;
        }
    }

    gBS->Exit(gImageHandle, Status, 0, NULL);
    return Status;
}

EFI_STATUS
EFIAPI
ExitInit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    OEM_SNAKE_KEYMAP *Newkeymap = NULL;
    Newkeymap = AllocateZeroPool (sizeof (OEM_SNAKE_KEYMAP_LIST));
    
    Newkeymap->KeyData = (EFI_KEY_DATA){{SCAN_ESC,0},{0,0}};
    Newkeymap->Callback = KeyExitCallback;

    KeyMapAddKeyMap(Newkeymap, 0);

    return Status;
}

EFI_STATUS
EFIAPI
ExitEnd(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;


    return Status;
}