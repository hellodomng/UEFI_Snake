#ifndef _SNAKE_H_
#define _SNAKE_H_

    #include "CommonInit.h"

extern EFI_STATUS
EFIAPI
SnakeInit(VOID);

extern EFI_STATUS
EFIAPI
SnakeStart(EFI_KEY_DATA *KeyData);

extern EFI_STATUS
EFIAPI
SnakeEnd(VOID);

#endif