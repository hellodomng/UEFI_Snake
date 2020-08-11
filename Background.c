#include "Background.h"
#include "ScreenLayer.h"

EFI_EVENT                               TimerEvent = NULL;
OEM_SNAKE_LAYER                         *BackgroundLayer = NULL;
UINT32                                  BackgroundColor = 0x07D8BFD8;//Init for no random color env.

UINT8                                   ColorTop = 0xE0;
UINT8                                   ColorBtm = 0x20;
BOOLEAN                                 BackgroundFlags[3] = {TRUE, FALSE, FALSE};

VOID
EFIAPI
BreathBackground(
    IN EFI_EVENT    Event,
    IN VOID         *Context
)
{
    // EFI_STATUS Status = EFI_SUCCESS;
    UINT8      *BGColor = (UINT8 *)&BackgroundColor;

    for (UINT8 i = 0; i < 3; i++)
    {
        if (BGColor[i] < ColorBtm)
        {
            BackgroundFlags[i] = TRUE;
        }
        else if (BGColor[i] > ColorTop)
        {
            BackgroundFlags[i] = FALSE;
        }

        if(BackgroundFlags[i] == TRUE)
        {
            BGColor[i]++;
        }
        else
        {
            BGColor[i]--;
        }
    }

    //need optimized code
    for (UINT32 i = 0; i < BackgroundLayer->Height * BackgroundLayer->Width; i++)
    {
        *(UINT32 *)&BackgroundLayer->Buffer[i] = BackgroundColor;
    }
    return;
}

EFI_STATUS
EFIAPI
BackgroundInit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    //Generate random background color
    if(Rng != NULL){
        Status = Rng->GetRNG(Rng, NULL, 4, (UINT8 *)&BackgroundColor);
        if(EFI_ERROR(Status))
        {
            Print(L"GetRNG Status: %r \n", Status);
            DEBUG ((DEBUG_ERROR, "%a %d GetRNG Status: %r \n", __FUNCTION__, __LINE__, Status));
        }
    }else{
        Print(L"LocateProtocol gEfiRngProtocolGuid Status: %r \n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d LocateProtocol gEfiRngProtocolGuid Status: %r \n", __FUNCTION__, __LINE__, Status));
    }

    for (UINT8 i = 0; i < 3; i++)
    {
        if(BackgroundColor&(0x01 << i))
        {
            BackgroundFlags[i] = TRUE;
        }else{
            BackgroundFlags[i] = FALSE;
        }
    }
    BackgroundColor &= 0x00FFFFFF;

    //Get Video Layer
    Status = ScreenLayerGetLayer(0, &BackgroundLayer);
    if(EFI_ERROR(Status)){
        Print(L"ScreenLayerGetLayer(%d) Status: %r \r\n", 0, Status);
        DEBUG ((DEBUG_ERROR, "%a %d ScreenLayerGetLayer(%d) Status: %r \n", __FUNCTION__, __LINE__, 0, Status));
        return Status;
    }

    //Run Background
    Status = gBS->CreateEvent(
            EVT_NOTIFY_SIGNAL | EVT_TIMER,
            TPL_CALLBACK,
            BreathBackground,
            NULL,
            &TimerEvent);
    if(EFI_ERROR(Status)){
        Print(L"CreateEvent Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d CreateEvent Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = gBS->SetTimer(
        TimerEvent,
        TimerPeriodic,
        EFI_TIMER_PERIOD_MILLISECONDS(100)
    );
    if(EFI_ERROR(Status)){
        Print(L"SetTimer Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d SetTimer Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = gBS->SignalEvent (TimerEvent);
    if(EFI_ERROR(Status))
    {
        Print(L"SignalEvent Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d SignalEvent: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    return Status;
}

EFI_STATUS
EFIAPI
BackgroundEnd(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = gBS->CloseEvent(TimerEvent);
    if(EFI_ERROR(Status))
    {
        Print(L"CloseEvent Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d SignalEvent: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }
    return Status;
}