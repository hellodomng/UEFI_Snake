#ifndef _OEM_SNAKE_H_
#define _OEM_SNAKE_H_

    #include <Protocol\GraphicsOutput.h>
    #include <Protocol\SimpleTextInEx.h>
    #include <Library\BaseLib.h>
    #include <Library\CpuLib.h>
    
    typedef EFI_STATUS (EFIAPI *OEM_SNAKE_INIT_LIST) (VOID);

    typedef EFI_STATUS (EFIAPI *OEM_SNAKE_END_LIST) (VOID);
#endif