#ifndef _KEY_MAP_H_
#define _KEY_MAP_H_

    #include "CommonInit.h"


    typedef EFI_STATUS (EFIAPI *OEM_SNAKE_KEYMAP_Callback) (EFI_KEY_DATA *KeyData);

    typedef struct {
        EFI_KEY_DATA KeyData;
        OEM_SNAKE_KEYMAP_Callback Callback;
    }OEM_SNAKE_KEYMAP;


#endif