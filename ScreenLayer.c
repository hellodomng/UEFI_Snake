#include "ScreenLayer.h"

OEM_SNAKE_LAYER                 *TopLayer       = NULL;
OEM_SNAKE_LAYER                 *BottomLayer    = NULL;
EFI_GRAPHICS_OUTPUT_BLT_PIXEL   *RenderBuffer = NULL;
EFI_EVENT                       LayerRenderEvent = NULL;

EFI_STATUS
EFIAPI
ScreenLayerCreateLayer(
    IN  UINT32          Width,
    IN  UINT32          Height,
    IN  UINT32          StartX,
    IN  UINT32          StartY,
    IN  UINT32           LayerNum,
    IN  CHAR16          *Intro,
    OUT OEM_SNAKE_BUFFER **Buffer
)
{
    EFI_STATUS      Status = EFI_SUCCESS;
    OEM_SNAKE_LAYER *NewLayer = NULL;
    OEM_SNAKE_LAYER *CurrentLayer = NULL;

    NewLayer= AllocateZeroPool (sizeof (OEM_SNAKE_LAYER));
    if(NewLayer == NULL){
        Print(L"AllocateZeroPool NewLayer out of memory \r\n");
        DEBUG ((DEBUG_ERROR, "%a %d AllocateZeroPool NewLayer out of memory \n", __FUNCTION__, __LINE__));
        Status = EFI_OUT_OF_RESOURCES;
        goto Error;
    }

    NewLayer->Buffer = AllocateZeroPool (Width * Height * sizeof (OEM_SNAKE_BUFFER));
    if(NewLayer == NULL){
        Print(L"AllocateZeroPool NewLayer->Buffer out of memory \r\n");
        DEBUG ((DEBUG_ERROR, "%a %d AllocateZeroPool NewLayer->Buffer out of memory \n", __FUNCTION__, __LINE__));
        Status = EFI_OUT_OF_RESOURCES;
        goto Error;
    }

    NewLayer->Width     = Width;
    NewLayer->Height    = Height;
    NewLayer->StartX    = StartX;
    NewLayer->StartY    = StartY;
    NewLayer->LayerNum  = LayerNum;
    NewLayer->Intro     = Intro;
    NewLayer->Next      = NULL;
    NewLayer->Prev      = NULL;

    *Buffer = NewLayer->Buffer;

    if(TopLayer == NULL && BottomLayer == NULL)
    {
        BottomLayer = TopLayer = NewLayer;
        return Status;
    }

    //Insert new to layer list
    CurrentLayer = TopLayer;
    while (CurrentLayer)
    {
        if(CurrentLayer->Next != NULL)
        {
            if(CurrentLayer->LayerNum == LayerNum)//Return if layer already exist
            {
                //return error
                Status = EFI_ALREADY_STARTED;
                goto Error;
            }
            else if(CurrentLayer->LayerNum < LayerNum && CurrentLayer->Next->LayerNum > LayerNum )
            {
                NewLayer->Next = CurrentLayer->Next;
                NewLayer->Prev = CurrentLayer;
                CurrentLayer->Next->Prev = NewLayer;
                CurrentLayer->Next = NewLayer;
                break;
            }
            CurrentLayer = CurrentLayer->Next;
        }else //End Node
        {
            CurrentLayer->Next = NewLayer;
            NewLayer->Prev = CurrentLayer;
            break;
        }
    }
    return Status;

Error:
    if(NewLayer)
    {
        if(NewLayer->Buffer)
        {
            FreePool(NewLayer->Buffer);
        }
        FreePool(NewLayer);
    }
    *Buffer = NULL;
    return Status;
}

EFI_STATUS
EFIAPI
ScreenLayerDestroyLayer(
    IN UINT32   LayerNum
)
{
    EFI_STATUS      Status          = EFI_SUCCESS;
    OEM_SNAKE_LAYER *CurrentLayer   = NULL;

    CurrentLayer = TopLayer;

    //more than one layer
    while(CurrentLayer)
    {
        if(CurrentLayer->LayerNum == LayerNum)
        {
            if(CurrentLayer == TopLayer)//top layer
            {
                TopLayer = CurrentLayer->Next;
                if(CurrentLayer == BottomLayer)
                {
                    BottomLayer = NULL;
                }
            }else if(CurrentLayer == BottomLayer)//bottom layer
            {
                BottomLayer = CurrentLayer->Prev;
                BottomLayer->Next = NULL;
            }else//middle layer
            {
                CurrentLayer->Prev->Next = CurrentLayer->Next;
                CurrentLayer->Next->Prev = CurrentLayer->Prev;
            }

            if(CurrentLayer->Buffer)
            {
                FreePool(CurrentLayer->Buffer);
            }
            FreePool(CurrentLayer);
            break;
        }
    }

    return Status;
}

EFI_STATUS
EFIAPI
ScreenLayerGetLayer(
    IN  UINT32           LayerNum,
    OUT OEM_SNAKE_LAYER **Layer
)
{
    EFI_STATUS      Status          = EFI_NOT_FOUND;
    OEM_SNAKE_LAYER *CurrentLayer   = TopLayer;

    *Layer = NULL;
    while (CurrentLayer)
    {
        if(CurrentLayer->LayerNum == LayerNum)
        {
            *Layer = CurrentLayer;
            Status = EFI_SUCCESS;
            break;
        }
        CurrentLayer = CurrentLayer->Next;
    }

    return Status;
}

EFI_STATUS
EFIAPI
ScreenLayerRender(VOID)
{
    EFI_STATUS                      Status        = EFI_SUCCESS;
    OEM_SNAKE_LAYER                 *CurrentLayer   = NULL;

    CurrentLayer = TopLayer;

    while (CurrentLayer)
    {
        if(CurrentLayer->StartX + CurrentLayer->Width > GraphicsInfo->HorizontalResolution  || CurrentLayer->StartY + CurrentLayer->Height > GraphicsInfo->VerticalResolution)
        {
            Print(L"LayerNum:%d StartX+Width:%d StartY+Height%d \r\n", CurrentLayer->LayerNum, CurrentLayer->StartX + CurrentLayer->Width, CurrentLayer->StartY + CurrentLayer->Height);
            DEBUG ((DEBUG_ERROR, "%a %d LayerNum:%d StartX+Width:%d StartY+Height%d \n", __FUNCTION__, __LINE__, CurrentLayer->LayerNum, CurrentLayer->StartX + CurrentLayer->Width, CurrentLayer->StartY + CurrentLayer->Height));
        }
        for (UINT16 Y = 0; Y < CurrentLayer->Height; Y++)
        {
            for (UINT16 X = 0; X < CurrentLayer->Width; X++)
            {
                if(CurrentLayer->Buffer[CurrentLayer->Width * Y + X].Transparency == 0)
                {
                    *(UINT32 *)&RenderBuffer[((CurrentLayer->StartY + Y) * GraphicsInfo->HorizontalResolution) + (CurrentLayer->StartX + X)] = *(UINT32 *)&CurrentLayer->Buffer[CurrentLayer->Width * Y + X];
                    // gBS->CopyMem(&RenderBuffer[((CurrentLayer->StartY + Y) * GraphicsInfo->HorizontalResolution) + (CurrentLayer->StartX + X)], &CurrentLayer->Buffer[CurrentLayer->Width * Y + X], sizeof(EFI_GRAPHICS_OUTPUT_BLT_PIXEL));
                }
            }
        }
        CurrentLayer = CurrentLayer->Next;
    }
    Status = GraphicsOutput->Blt(GraphicsOutput, RenderBuffer ,EfiBltBufferToVideo, 0, 0, 0, 0, GraphicsInfo->HorizontalResolution, GraphicsInfo->VerticalResolution, 0);
    if(EFI_ERROR(Status)){
        Print(L"GraphicsOutput->Blt Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d GraphicsOutput->Blt Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }
    return Status;
}



VOID
EFIAPI
LayerRenderTimerFunction(
    IN EFI_EVENT    Event,
    IN VOID         *Context
)
{
    EFI_STATUS Status = EFI_SUCCESS;
    Status = ScreenLayerRender();
    if(EFI_ERROR(Status)){
        Print(L"ScreenLayerRender Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d ScreenLayerRender Status: %r \n", __FUNCTION__, __LINE__, Status));
        return;
    }
    return;
}

//Init function and Exit function
EFI_STATUS
EFIAPI
ScreenLayerInit(VOID)
{
    EFI_STATUS      Status          = EFI_SUCCESS;
    UINT32          ScreenWidth     = GraphicsInfo->HorizontalResolution; 
    UINT32          ScreenHeight    = GraphicsInfo->VerticalResolution;
    UINT8           InitLayerNumber = 0;
    OEM_SNAKE_BUFFER *Pixel          = NULL;

    RenderBuffer = AllocateZeroPool (GraphicsInfo->HorizontalResolution * GraphicsInfo->VerticalResolution * sizeof (EFI_GRAPHICS_OUTPUT_BLT_PIXEL));

    ScreenLayerCreateLayer(
        ScreenWidth,ScreenHeight,
        0,0,
        InitLayerNumber,
        NULL,
        &Pixel
    );
    Status = gBS->CreateEvent(
        EVT_NOTIFY_SIGNAL | EVT_TIMER,
        TPL_CALLBACK,
        LayerRenderTimerFunction,
        NULL,
        &LayerRenderEvent
    );
    if(EFI_ERROR(Status)){
        Print(L"CreateEvent Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d CreateEvent Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = gBS->SetTimer(
        LayerRenderEvent,
        TimerPeriodic,
        EFI_TIMER_PERIOD_MILLISECONDS(20)
    );
    if(EFI_ERROR(Status)){
        Print(L"SetTimer LayerRenderEvent Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d SetTimer Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = gBS->SignalEvent (LayerRenderEvent);
    if(EFI_ERROR(Status))
    {
        Print(L"SignalEvent LayerRenderEvent: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d SignalEvent LayerRenderEvent: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }
    return Status;
}

EFI_STATUS
EFIAPI
ScreenLayerExit(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;
    OEM_SNAKE_LAYER *CurrentLayer   = NULL;
    OEM_SNAKE_LAYER *NextLayer   = NULL;

    //Destroy all Layer
    CurrentLayer = TopLayer;
    while(CurrentLayer)
    {
        NextLayer = CurrentLayer->Next;
        // ScreenLayerDestroyLayer(CurrentLayer->LayerNum);
        if(CurrentLayer->Buffer)
        {
            FreePool(CurrentLayer->Buffer);
        }
        FreePool(CurrentLayer);
        CurrentLayer = NextLayer;
    }

    Status = gBS->CloseEvent(LayerRenderEvent);
    return Status;
}