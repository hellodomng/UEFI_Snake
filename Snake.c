#include "Snake.h"
#include "ScreenLayer.h"

typedef struct _OEM_SNAKE_POSITION OEM_SNAKE_POSITION;

typedef struct _OEM_SNAKE_BODY OEM_SNAKE_BODY;

struct _OEM_SNAKE_POSITION{
    UINT32              relativeX;
    UINT32              relativeY;
    UINT32              NextNumber;
    UINT32              PrevNumber;
};

struct _OEM_SNAKE_BODY{
    UINT32              Width;
    UINT32              Height;
    UINT32              Color;
    UINT32              Length;
    UINT32              Step;
    OEM_SNAKE_POSITION *Position;
};

BOOLEAN                     SnakeFlag        = TRUE;
EFI_EVENT                   SnakeMoveEvent   = NULL;
UINT32                      SnakeSectionWidth       = 20;
UINT32                      SnakeSectionHeight      = 20;

enum SNAKE_DIRECTION{
    SnakeNorth,
    SnakeSouth,
    SnakeEast,
    SnakeWest
};

VOID
EFIAPI
SnakeMoveCallback(
    IN EFI_EVENT    Event,
    IN VOID         *Context
)
{
    // EFI_STATUS      Status = EFI_SUCCESS

    return;
}


EFI_STATUS
EFIAPI
SnakeInit(VOID)
{
    EFI_STATUS       Status = EFI_SUCCESS;
    OEM_SNAKE_BUFFER *Pixel = NULL;

    //init position
    //init direction 0 = up 1 = down 2 = left 3 = right
    //init draw snake body


    //draw init snake
    ScreenLayerCreateLayer(
        200,200,
        SnakeSectionWidth,SnakeSectionHeight,
        1,
        NULL,
        &Pixel
    );
    // ScreenLayerGetLayer(0, &SnakeLayer);
    // Pixel = SnakeLayer->Buffer;

    return Status;
}





EFI_STATUS
EFIAPI
SnakeStart(EFI_KEY_DATA *KeyData)
{
    EFI_STATUS Status = EFI_SUCCESS;
    
    // UINT8 Color[4] = 0;
    // UINT32 Direction = 0;

    // if(Rng)
    // {
    //     Rng->GetRNG(Rng, NULL, 4, Color);
    //     Direction = Color[3]&0x03;
    //     Color[3] = 0;
    // }

    // OEM_SNAKE_BODY SnakeBaby = {
    //     20,20,              //width height
    //     *(UINT32 *)Color,   //Snake Body color
    //     1,                  //Length
    //     20,                 //Step
    //     NULL
    // };


    Status = gBS->CreateEvent(
        EVT_NOTIFY_SIGNAL | EVT_TIMER,
        TPL_CALLBACK,
        SnakeMoveCallback,
        NULL,
        &SnakeMoveEvent
    );
    if(EFI_ERROR(Status)){
        Print(L"CreateEvent Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d CreateEvent Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    Status = gBS->SetTimer(
        SnakeMoveEvent,
        TimerPeriodic,
        EFI_TIMER_PERIOD_MILLISECONDS(50)
    );
    if(EFI_ERROR(Status)){
        Print(L"SetTimer Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d SetTimer Status: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }

    return Status;
}

EFI_STATUS
EFIAPI
SnakeEnd(VOID)
{
    EFI_STATUS Status = EFI_SUCCESS;

    Status = gBS->CloseEvent(SnakeMoveEvent);
    if(EFI_ERROR(Status))
    {
        Print(L"CloseEvent Status: %r \r\n", Status);
        DEBUG ((DEBUG_ERROR, "%a %d CloseEvent: %r \n", __FUNCTION__, __LINE__, Status));
        return Status;
    }
    return Status;
}