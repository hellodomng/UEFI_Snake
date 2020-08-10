#include "Exit.h"

extern EFI_HANDLE      GlobalKeyHandle;

typedef EFI_STATUS (EFIAPI *OEM_SNAKE_END_LIST) (VOID);
extern OEM_SNAKE_END_LIST OemSnakeEndList[];

EFI_STATUS
EFIAPI
onExit(EFI_KEY_DATA *KeyData)
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

    if(SimpleEx != NULL){
        Status = SimpleEx->UnregisterKeyNotify (SimpleEx,GlobalKeyHandle);
        if(EFI_ERROR(Status)){
            DEBUG ((DEBUG_ERROR, "%a %d RegisterKeyNotify Status: %r \n", __FUNCTION__, __LINE__, Status));
            // return Status;
        }
    }
    else{
        Print(L"SimpleEx == NULL\r\n");
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