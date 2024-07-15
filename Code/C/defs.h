#ifndef __DEFS_H__
#define __DEFS_H__
#include"image_utils.h"

typedef enum 
{
    ct_Center = 1,
    ct_Zoom = 2,
    ct_Rotate = 3,
    ct_Hold = 4,
    ct_ZoomandRotate = 5,
    ct_CenterandZoom = 6,
    ct_CenterandRoate = 7,
    ct_All = 8,
    ct_imgNumber = 9,
    ct_movebypixel = 10,
    ct_Derotate = 11
} cmd_type;


typedef struct _cmd
{
    cmd_type cmd;
    int steps;
    double cx,cy;
    double zoom;
    double angle;
    int imgNumber;
    int deltapixelx;
    int deltapixely;
} Cmd;


typedef struct _config
{
    int height;
    int width;
    Cmd* Commands;
    int CmdCount;

} Config;

typedef struct _image_state
{
    double cx, cy;
    double minx, maxx, miny, maxy;
    double initial_length;
    double angle;
    int height, width;
    int image_count;
    BitMapFile bmFileData;
} ImageState;
#endif