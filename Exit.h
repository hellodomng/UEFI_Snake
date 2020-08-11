#ifndef _EXIT_H_
#define _EXIT_H_

    #include "CommonInit.h"


extern EFI_STATUS
EFIAPI
KeyExitCallback(EFI_KEY_DATA *KeyData);

extern EFI_STATUS
EFIAPI
ExitInit(VOID);

extern EFI_STATUS
EFIAPI
ExitEnd(VOID);
#endif