#include "CommonInit.h"

EFI_GRAPHICS_OUTPUT_PROTOCOL            *GraphicsOutput = NULL;
EFI_GRAPHICS_OUTPUT_MODE_INFORMATION    *GraphicsInfo = NULL;
EFI_SIMPLE_TEXT_INPUT_EX_PROTOCOL       *SimpleEx = NULL;
EFI_SIMPLE_TEXT_OUTPUT_PROTOCOL         *SimpleOut = NULL;
EFI_RNG_PROTOCOL                        *Rng = NULL;

EFI_STATUS
EFIAPI
CommonInit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = gBS->LocateProtocol(
                    &gEfiGraphicsOutputProtocolGuid,
                    NULL,
                    (VOID **)&GraphicsOutput);
    if(EFI_ERROR(Status))
    {
        Print(L"gEfiGraphicsOutputProtocolGuid Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d LocateProtocol gEfiGraphicsOutputProtocolGuid Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    GraphicsInfo = GraphicsOutput->Mode->Info;

    if(EFI_ERROR(Status))
    {
        Print(L"QueryMode Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d QueryMode Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = gBS->LocateProtocol(
                    &gEfiRngProtocolGuid,
                    NULL,
                    (VOID **)&Rng);
    if(EFI_ERROR(Status))
    {
        Print(L"LocateProtocol gEfiRngProtocolGuid Status: %r \n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d LocateProtocol gEfiRngProtocolGuid Status: %r \n", __FUNCTION__, __LINE__, Status));
    }

    Status = gBS->OpenProtocol(
                        gST->ConsoleInHandle,
                        &gEfiSimpleTextInputExProtocolGuid,
                        (VOID **)&SimpleEx,
                        gImageHandle,
                        NULL,
                        EFI_OPEN_PROTOCOL_GET_PROTOCOL);
    if(EFI_ERROR(Status))
    {
        Print(L"gEfiSimpleTextInputExProtocolGuid Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d OpenProtocol gEfiSimpleTextInputExProtocolGuid Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = gBS->LocateProtocol(
                    &gEfiSimpleTextOutProtocolGuid,
                    NULL,
                    (VOID **)&SimpleOut);
    if(EFI_ERROR(Status))
    {
        Print(L"gEfiSimpleTextOutProtocolGuid Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d LocateProtocol gEfiSimpleTextOutProtocolGuid Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    return Status;
}


EFI_STATUS
EFIAPI
CommonEnd(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    return Status;
}

