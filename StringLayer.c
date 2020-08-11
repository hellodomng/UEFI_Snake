#include "StringLayer.h"
#include "ScreenLayer.h"

EFI_HII_FONT_PROTOCOL *HiiFontProtocol = NULL;
OEM_SNAKE_BUFFER      *Pixel = NULL;

UINT32 StringLayerNumber = 0xff;
UINT32 StringLayerX = 0;
UINT32 StringLayerY = 0;

EFI_STATUS
EFIAPI
StringLayerInit(VOID)
{
    EFI_STATUS          Status = EFI_SUCCESS;
    // OEM_SNAKE_BUFFER    *Pixel = NULL;
    UINT32              i = 0;


    //Init String Layer
    ScreenLayerCreateLayer(
        StringLayerX, StringLayerY,
        GraphicsInfo->HorizontalResolution, GraphicsInfo->VerticalResolution,
        StringLayerNumber,
        NULL,
        (OEM_SNAKE_BUFFER *)&Pixel
    );

    //Init to transparent layer
    for (UINT32 i = 0; i < GraphicsInfo->HorizontalResolution * GraphicsInfo->VerticalResolution; i++)
    {
        *(UINT32 *)&Pixel[i] = 0xff000000;
    }

    gBS->LocateProtocol(
        &gEfiHiiFontProtocolGuid,
        NULL,
        (VOID **)&HiiFontProtocol
    );
    if(EFI_ERROR(Status))
    {
        Print(L"LocateProtocol gEfiRngProtocolGuid Status: %r \n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d LocateProtocol gEfiRngProtocolGuid Status: %r \n", __FUNCTION__, __LINE__, Status));
    }

    
    return Status;
}


EFI_STATUS
EFIAPI
StringLayerDrawString(
    UINT32  x,
    UINT32  y,
    EFI_STRING String,
    EFI_FONT_DISPLAY_INFO *StringInfo,

)
{
    EFI_STATUS Status = EFI_SUCCESS;
    EFI_HII_OUT_FLAGS Flags = EFI_HII_IGNORE_IF_NO_GLYPH | EFI_HII_OUT_FLAG_TRANSPARENT | ;
    HiiFontProtocol->StringToImage(
        HiiFontProtocol,
        Flags,
        String,
        StringInfo,
        Pixel,
        x,
        y,
        NULL,
        NULL,
        NULL
    );


    return Status;
}





EFI_STATUS
EFIAPI
StringLayerEnd(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;



    return Status;
}