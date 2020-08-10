#ifndef _SCREEN_LAYER_H_
#define _SCREEN_LAYER_H_

    #include "CommonInit.h"

typedef struct _OEM_SNAKE_BUFFER OEM_SNAKE_BUFFER;

typedef struct _OEM_SNAKE_LAYER OEM_SNAKE_LAYER;

struct _OEM_SNAKE_BUFFER{
    UINT8           Blue;
    UINT8           Green;
    UINT8           Red;
    UINT8           Transparency;
};

struct _OEM_SNAKE_LAYER{
    UINT32          Width;
    UINT32          Height;
    UINT32          StartX;
    UINT32          StartY;
    UINT32          LayerNum;
    CHAR16          *Intro;
    OEM_SNAKE_BUFFER *Buffer;
    OEM_SNAKE_LAYER *Next;
    OEM_SNAKE_LAYER *Prev;
};





extern EFI_STATUS
EFIAPI
ScreenLayerCreateLayer(
    IN  UINT32          Width,
    IN  UINT32          Height,
    IN  UINT32          StartX,
    IN  UINT32          StartY,
    IN  UINT32          LayerNum,
    IN  CHAR16          *Intro,
    OUT OEM_SNAKE_BUFFER **Buffer
);

extern EFI_STATUS
EFIAPI
ScreenLayerDestroyLayer(
    OUT UINT32   LayerNum
);

extern EFI_STATUS
EFIAPI
ScreenLayerGetLayer(
    IN  UINT32          LayerNum,
    OUT OEM_SNAKE_LAYER **Layer
);

extern EFI_STATUS
EFIAPI
ScreenLayerRender();

extern EFI_STATUS
EFIAPI
ScreenLayerInit(VOID);

extern EFI_STATUS
EFIAPI
ScreenLayerExit(VOID);

#endif