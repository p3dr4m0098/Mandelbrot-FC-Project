#ifndef __MANDELBROTSET_H__
#define __MANDELBROTSET_H__

#include "defs.h"
#include <math.h>
#define PI 3.14159265358979323846

int max_i;

int get_mbs_iter(double x, double y)
{
    int i = 0;
    double zx = 0.0;
    double zy = 0.0;
    while ((sqrt(zx * zx + zy * zy) < 2) && i < max_i)
    {
        double tempx = zx;
        zx = zx * zx - zy * zy + x;
        zy = 2 * tempx * zy + y;
        i++;
    }
    return i;
}

void hsv_to_rgb(int hue, int min, int max, COLORINDEX *p)
{
    // TODO
}

double to_radian(double angle);
double my_arc(double x, double y, double r);
void UpdateImageData(ImageState *state)
{
    max_i = 500 + (int)(((-log10((state->maxx - state->minx)/state->initial_length))/12)*10000);
    double xunit = (state->maxx - state->minx) / state->width;
    double yunit = (state->maxy - state->miny) / state->height;

    for (int i = 0; i < state->width; i++)
    {
        double converted_x = state->minx + xunit * i;
        for (int j = 0; j < state->height; j++)
        {
            double converted_y = state->miny + j * yunit;
            double r = sqrt((converted_x - state->cx) * (converted_x - state->cx) +
                            (converted_y - state->cy) * (converted_y - state->cy));
            double theta = my_arc(converted_x - state->cx, converted_y - state->cy, r);
            double rotated_x = state->cx + r * cos(theta - to_radian(state->angle));
            double rotated_y = state->cy + r * sin(theta - to_radian(state->angle));
            int iter = get_mbs_iter(rotated_x, rotated_y);
            if (iter < max_i)
            {
                state->bmFileData.bmData[j * state->width + i] = 1 + (int)(180 * iter / max_i);
            }
            else
            {
                state->bmFileData.bmData[j * state->width + i] = 0;
            }
        }
    }
}

void ChangeCenter(ImageState *state, double newcx, double newcy, int steps)
{
    double step_x = (newcx - state->cx) / steps;
    double step_y = (newcy - state->cy) / steps;

    for (int i = 0; i < steps; i++)
    {
        state->minx += step_x;
        state->maxx += step_x;
        state->cx += step_x;
        state->miny += step_y;
        state->maxy += step_y;
        state->cy += step_y;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void ChangeZoom(ImageState *state, double zoom, int steps)
{
    double step_zoom = pow(zoom, 1.0 / steps);

    for (int i = 0; i < steps; i++)
    {
        state->minx = state->cx - (state->cx - state->minx) / step_zoom;
        state->miny = state->cy - (state->cy - state->miny) / step_zoom;
        state->maxx = state->cx + (state->maxx - state->cx) / step_zoom;
        state->maxy = state->cy + (state->maxy - state->cy) / step_zoom;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void ChangeRotation(ImageState *state, double angle, int steps)
{
    double step_angle = angle / steps;

    for (int i = 0; i < steps; i++)
    {
        state->angle = fmod(state->angle + step_angle, 360);
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void Hold(ImageState *state, int steps)
{

    for (int i = 0; i < steps; i++)
    {
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void ChangeZoomandRotation(ImageState *state, double zoom, double angle, int steps)
{
    double step_zoom = pow(zoom, 1.0 / steps);
    double step_angle = angle / steps;

    for (int i = 0; i < steps; i++)
    {
        state->minx = state->cx - (state->cx - state->minx) / step_zoom;
        state->miny = state->cy - (state->cy - state->miny) / step_zoom;
        state->maxx = state->cx + (state->maxx - state->cx) / step_zoom;
        state->maxy = state->cy + (state->maxy - state->cy) / step_zoom;
        state->angle = fmod(state->angle + step_angle, 360);
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void ChangeCenterandZoom(ImageState *state, double newcx, double newcy, double zoom, int steps)
{
    double step_x = (newcx - state->cx) / steps;
    double step_y = (newcy - state->cy) / steps;
    double step_zoom = pow(zoom, 1.0 / steps);

    for (int i = 0; i < steps; i++)
    {
        state->minx += step_x;
        state->maxx += step_x;
        state->cx += step_x;
        state->miny += step_y;
        state->maxy += step_y;
        state->cy += step_y;
        state->minx = state->cx - (state->cx - state->minx) / step_zoom;
        state->miny = state->cy - (state->cy - state->miny) / step_zoom;
        state->maxx = state->cx + (state->maxx - state->cx) / step_zoom;
        state->maxy = state->cy + (state->maxy - state->cy) / step_zoom;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void ChangeCenterandRotation(ImageState *state, double newcx, double newcy, double angle, int steps)
{
    double step_x = (newcx - state->cx) / steps;
    double step_y = (newcy - state->cy) / steps;
    double step_angle = angle / steps;

    for (int i = 0; i < steps; i++)
    {
        state->minx += step_x;
        state->maxx += step_x;
        state->cx += step_x;
        state->miny += step_y;
        state->maxy += step_y;
        state->cy += step_y;
        state->angle = fmod(state->angle + step_angle, 360);
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void ChangeAll(ImageState *state, double newcx, double newcy, double zoom, double angle, int steps)
{
    double step_x = (newcx - state->cx) / steps;
    double step_y = (newcy - state->cy) / steps;
    double step_zoom = pow(zoom, 1.0/steps);
    double step_angle = angle / steps;

    for (int i = 0; i < steps; i++)
    {
        state->minx += step_x;
        state->maxx += step_x;
        state->cx += step_x;
        state->miny += step_y;
        state->maxy += step_y;
        state->cy += step_y;
        state->minx = state->cx - (state->cx - state->minx) / step_zoom;
        state->miny = state->cy - (state->cy - state->miny) / step_zoom;
        state->maxx = state->cx + (state->maxx - state->cx) / step_zoom;
        state->maxy = state->cy + (state->maxy - state->cy) / step_zoom;
        state->angle = fmod(state->angle + step_angle, 360);
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void MoveByPixel(ImageState *state, int dpixelx, int dpixely, int steps)
{
    double step_x = (state->maxx - state->minx) * (dpixelx+0.0) / (state->width * steps);
    double step_y = (state->maxy - state->miny) * (dpixely+0.0) / (state->height * steps);

    for (int i = 0; i < steps; i++)
    {
        state->minx += step_x;
        state->maxx += step_x;
        state->cx += step_x;
        state->miny += step_y;
        state->maxy += step_y;
        state->cy += step_y;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

void Derotate(ImageState *state, int steps)
{
    double step_angle = (-(state->angle)) / steps;

    for (int i = 0; i < steps; i++)
    {
        state->angle += step_angle;
        UpdateImageData(state);
        WriteBitmapFile(state->image_count++, &state->bmFileData);
    }
}

double to_radian(double angle)
{
    return angle * PI / 180;
}

double my_arc(double x, double y, double r)
{
    if (x < 0)
    {
        return PI - asin(y / r);
    }
    return asin(y / r);
}

#endif