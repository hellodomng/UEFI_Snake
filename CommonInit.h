#ifndef _COMMON_INIT_H_
#define _COMMON_INIT_H_

    #include <Uefi.h>
    #include <Library/UefiLib.h>
    #include <Library/UefiApplicationEntryPoint.h>
    #include <Library\UefiBootServicesTableLib.h>
    #include <Library\UefiRuntimeServicesTableLib.h>
    #include <Library/MemoryAllocationLib.h>
    #include <Library/IoLib.h>
    #include <Library/DebugLib.h>
    #include <Protocol\Rng.h>
    #include <Protocol\HiiFont.h>

    extern EFI_GRAPHICS_OUTPUT_PROTOCOL            *GraphicsOutput;
    extern EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    *GraphicsInfo;
    extern EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL       *SimpleEx;
    extern EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL         *SimpleOut;
    extern EFI_RNG_PROTOCOL                        *Rng;

extern EFI_STATUS \
EFIAPI \
CommonInit(VOID);


extern EFI_STATUS
EFIAPI
CommonEnd(VOID);

#endif