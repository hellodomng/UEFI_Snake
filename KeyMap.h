#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

    #include "CommonInit.h"

typedef EFI_STATUS (EFIAPI *OEM_SNAKE_KEYMAP_Callback) (EFI_KEY_DATA *KeyData);

typedef struct _OEM_SNAKE_KEYMAP OEM_SNAKE_KEYMAP;

typedef struct _OEM_SNAKE_KEYMAP_LIST OEM_SNAKE_KEYMAP_LIST;

struct _OEM_SNAKE_KEYMAP{
    EFI_KEY_DATA                KeyData;
    OEM_SNAKE_KEYMAP_Callback   Callback;
};

struct _OEM_SNAKE_KEYMAP_LIST{
    UINT32                      Id;
    OEM_SNAKE_KEYMAP            *Keymap;
    OEM_SNAKE_KEYMAP_LIST       *Next;
};

extern EFI_STATUS
EFIAPI
KeyMapAddKeyMap(OEM_SNAKE_KEYMAP *Newkeymap,UINT32 KeymapId);

extern EFI_STATUS
EFIAPI
KeyMapDeleteKeyMap(UINT32 KeymapId);

extern EFI_STATUS
EFIAPI
KepMapInit(VOID);

extern EFI_STATUS
EFIAPI
KepMapEnd(VOID);
#endif