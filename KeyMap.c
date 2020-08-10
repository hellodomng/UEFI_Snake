#include "KeyMap.h"


EFI_HANDLE              KeyMapHandle = NULL;
OEM_SNAKE_KEYMAP_LIST   *KeymapTop = NULL;
OEM_SNAKE_KEYMAP_LIST   *KeymapBottom = NULL;
static UINT32           KeymapId = 0;

EFI_STATUS
EFIAPI
KeyMapAddKeyMap(OEM_SNAKE_KEYMAP *Newkeymap,UINT32 KeymapId)
{
    EFI_STATUS Status = EFI_SUCCESS;

    OEM_SNAKE_KEYMAP_LIST *CurrentKeyMap = NULL;

    CurrentKeyMap= AllocateZeroPool (sizeof (OEM_SNAKE_KEYMAP_LIST));
    if(CurrentKeyMap == NULL){
        Print(L"AllocateZeroPool CurrentKeyMap out of memory \r\n");
        DEBUG ((DEBUG_ERROR, "%a %d AllocateZeroPool CurrentKeyMap out of memory \n", __FUNCTION__, __LINE__));
        Status = EFI_OUT_OF_RESOURCES;
        goto Error;
    }

    CurrentKeyMap->Id       = KeymapId;
    CurrentKeyMap->Keymap   = Newkeymap;
    CurrentKeyMap->Next     = NULL;

    if(KeymapTop == NULL)
    {
        KeymapTop = KeymapBottom = CurrentKeyMap;
        return Status;
    }

    KeymapBottom->Next = CurrentKeyMap;
    return Status;

Error:
    if(CurrentKeyMap)
    {
        FreePool(CurrentKeyMap);
    }

    return Status;
}

EFI_STATUS
EFIAPI
KeyMapDeleteKeyMap(UINT32 KeymapId)
{
    EFI_STATUS              Status          = EFI_SUCCESS;
    OEM_SNAKE_KEYMAP_LIST   *CurrentKeyMap  = KeymapTop;
    OEM_SNAKE_KEYMAP_LIST   *PrevKeyMap     = NULL;

    while (CurrentKeyMap)
    {
        if(CurrentKeyMap->Id == KeymapId)
        {
            if(PrevKeyMap)
            {
                PrevKeyMap->Next = CurrentKeyMap->Next;
            }
            FreePool(CurrentKeyMap);
        }
        PrevKeyMap = CurrentKeyMap;
        CurrentKeyMap = CurrentKeyMap->Next;
    }
    return Status;
}



EFI_STATUS
EFIAPI
KeyBindingHandler(
    IN EFI_KEY_DATA *KeyData
)
{
    EFI_STATUS              Status = EFI_SUCCESS;
    OEM_SNAKE_KEYMAP_LIST   *CurrentKeyMap  = KeymapTop;

    //Match hotkeys
    while (CurrentKeyMap)
    {
        if(CurrentKeyMap->Keymap->KeyData.Key.ScanCode)
        {
            if(CurrentKeyMap->Keymap->KeyData.Key.ScanCode != KeyData->Key.ScanCode)
            {
                CurrentKeyMap = CurrentKeyMap->Next;
                continue;
            }
        }
        if(CurrentKeyMap->Keymap->KeyData.Key.UnicodeChar)
        {
            if(CurrentKeyMap->Keymap->KeyData.Key.UnicodeChar != KeyData->Key.UnicodeChar)
            {
                CurrentKeyMap = CurrentKeyMap->Next;
                continue;
            }
        }
        if(CurrentKeyMap->Keymap->KeyData.KeyState.KeyShiftState)
        {
            if(CurrentKeyMap->Keymap->KeyData.KeyState.KeyShiftState != KeyData->KeyState.KeyShiftState)
            {
                CurrentKeyMap = CurrentKeyMap->Next;
                continue;
            }
        }
        if(CurrentKeyMap->Keymap->KeyData.KeyState.KeyToggleState)
        {
            if(CurrentKeyMap->Keymap->KeyData.KeyState.KeyToggleState != KeyData->KeyState.KeyToggleState)
            {
                CurrentKeyMap = CurrentKeyMap->Next;
                continue;
            }
        }

        Status = CurrentKeyMap->Keymap->Callback(KeyData);
        if(EFI_ERROR(Status)){
            Print(L"CurrentKeyMap->Keymap->Callback : %r \r\n", Status);
            DEBUG ((DEBUG_ERROR, "%a %d CurrentKeyMap->Keymap->Callback : %r \n", __FUNCTION__, __LINE__, Status));
            return Status;
        }
        CurrentKeyMap = CurrentKeyMap->Next;
    }

    return Status;
}


EFI_STATUS
EFIAPI
KepMapInit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_KEY_DATA	KeyData = {{0,0},{0,0}};//Used to globalKeyBinding

    if(SimpleEx != NULL){
        Status = SimpleEx->RegisterKeyNotify (
            SimpleEx,
            &KeyData,
            KeyBindingHandler,
            &KeyMapHandle
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


    return Status;
}

EFI_STATUS
EFIAPI
KepMapEnd(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    if(SimpleEx != NULL){
        Status = SimpleEx->UnregisterKeyNotify(SimpleEx, KeyMapHandle);
        if(EFI_ERROR(Status)){
            DEBUG ((DEBUG_ERROR, "%a %d RegisterKeyNotify Status: %r \n", __FUNCTION__, __LINE__, Status));
            return Status;
        }
    }
    else
    {
        Print(L"SimpleEx == NULL\r\n");
    }

    return Status;
}