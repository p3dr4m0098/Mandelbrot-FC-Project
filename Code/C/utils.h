#ifndef __UTILS_H__
#define __UTILS_H__

#include "defs.h"
#include "image_utils.h"
#include <stdlib.h>
#define _USE_MATH_DEFINES
#include <math.h>
#include <stdbool.h>


double get_double(const char *line, int *pindex);
int get_int(const char* line, int* pindex);
char *first_word(char *line);
bool samestring(char *string1, char *string2);
void ProcessArgs(int argc, char **argv, Config *outcfg, ImageState *state)
{
    FILE *file = fopen(argv[1], "r");
    FILE *file2 = fopen(argv[1], "r");
    char line1[128], line2[128], renewable_line[128];
    fgets(renewable_line, 128, file2);
    fgets(renewable_line, 128, file2);
    (outcfg->CmdCount) = 0;
    while (fgets(renewable_line, 128, file2))
    {
        outcfg->CmdCount++;
    }
    fclose(file2);

    Cmd *CommandList = (Cmd *)malloc(outcfg->CmdCount * sizeof(Cmd));

    fgets(line1, 128, file);
    fgets(line2, 128, file);
    int result_width = 0;
    int result_height = 0;
    int i = 0;
    for (; line1[i] != '*'; i++)
    {
        result_width *= 10;
        result_width += line1[i] - '0';
    }
    i++;
    for (; line1[i] && line1[i] != '\n'; i++)
    {
        result_height *= 10;
        result_height += line1[i] - '0';
    }

    (*outcfg).height = result_height;
    (*outcfg).width = result_width;
    state->height = result_height;
    state->width = result_width;

    int j = 0;
    state->minx = get_double(line2, &j);
    state->maxx = get_double(line2, &j);
    state->initial_length = state->maxx - state->minx;
    state->miny = get_double(line2, &j);
    state->maxy = get_double(line2, &j);
    state->cx = (state->minx + state->maxx) / 2;
    state->cy = (state->miny + state->maxy) / 2;

    int command_index = 0;
    while (fgets(renewable_line, 128, file))
    {
        char *fw = first_word(renewable_line);
        if (samestring(fw, "center"))
        {
            CommandList[command_index].cmd = 1;
            int index = 7;
            CommandList[command_index].cx = get_double(renewable_line, &index);
            CommandList[command_index].cy = get_double(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "zoom"))
        {
            CommandList[command_index].cmd = 2;
            int index = 5;
            CommandList[command_index].zoom = get_double(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "rotate"))
        {
            CommandList[command_index].cmd = 3;
            int index = 7;
            CommandList[command_index].angle = get_double(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "hold"))
        {
            CommandList[command_index].cmd = 4;
            int index = 5;
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "zoomandrotate"))
        {
            CommandList[command_index].cmd = 5;
            int index = 14;
            CommandList[command_index].zoom = get_double(renewable_line, &index);
            CommandList[command_index].angle = get_double(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "centerandzoom"))
        {
            CommandList[command_index].cmd = 6;
            int index = 14;
            CommandList[command_index].cx = get_double(renewable_line, &index);
            CommandList[command_index].cy = get_double(renewable_line, &index);
            CommandList[command_index].zoom = get_double(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "centerandrotate"))
        {
            CommandList[command_index].cmd = 7;
            int index = 16;
            CommandList[command_index].cx = get_double(renewable_line, &index);
            CommandList[command_index].cy = get_double(renewable_line, &index);
            CommandList[command_index].angle = get_double(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "all"))
        {
            CommandList[command_index].cmd = 8;
            int index = 4;
            CommandList[command_index].cx = get_double(renewable_line, &index);
            CommandList[command_index].cy = get_double(renewable_line, &index);
            CommandList[command_index].zoom = get_double(renewable_line, &index);
            CommandList[command_index].angle = get_double(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "imgnumber"))
        {
            CommandList[command_index].cmd = 9;
            int index = 10;
            CommandList[command_index].imgNumber = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "movebypixel"))
        {
            CommandList[command_index].cmd = 10;
            int index = 12;
            CommandList[command_index].deltapixelx = get_int(renewable_line, &index);
            CommandList[command_index].deltapixely = get_int(renewable_line, &index);
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
        if (samestring(fw, "derotate"))
        {
            CommandList[command_index].cmd = 11;
            int index = 9;
            CommandList[command_index].steps = get_int(renewable_line, &index);
            command_index++;
            free(fw);
            continue;
        }
    }
    outcfg->Commands = CommandList;
    state->angle = 0;
    state->image_count = 0;

    InitImage(&state->bmFileData, state->height, state->width);
}















bool samestring(char* string1, char* string2)
{
    for (int i=0; string1[i] || string2[i]; i++)
    {
        if (string1[i] != string2[i])
        {
            return false;
        }
    }
    return true;
}

char *first_word(char *line)
{
    int line_size = 1;
    for (int i = 0; line[i]; i++)
    {
        line_size++;
    }
    char *result = (char *)malloc(line_size);
    int j = 0;
    for (; line[j] && line[j] != ' '; j++)
    {
        result[j] = line[j];
    }
    result[j] = 0;
    return result;
}

double get_double(const char *line, int *pindex)
{
    double result = 0.0;
    int after_dot = 0;
    int start = *pindex;
    if (line[*pindex] == '-')
    {
        (*pindex)++;
    }
    for (; line[*pindex] != ',' && line[*pindex] != '\n' && line[*pindex] != ':' && line[*pindex]; (*pindex)++)
    {
        if (line[*pindex] == '.')
        {
            (*pindex)++;
            break;
        }
        result *= 10;
        result += line[*pindex] - '0';
    }
    for (; line[*pindex] != ',' && line[*pindex] != '\n' && line[*pindex] != ':' && line[*pindex]; (*pindex)++)
    {
        result *= 10;
        result += line[*pindex] - '0';
        after_dot++;
    }
    (*pindex)++;
    for (int j = 0; j < after_dot; j++)
    {
        result /= 10;
    }
    if (line[start] == '-')
    {
        result *= -1;
    }
    return result;
}

int get_int(const char* line, int* pindex)
{
    double result = 0.0;
    int start = *pindex;
    if (line[*pindex] == '-')
    {
        (*pindex)++;
    }
    for (; line[*pindex] != ',' && line[*pindex] != '\n' && line[*pindex] != ':' && line[*pindex]; (*pindex)++)
    {
        result *= 10;
        result += line[*pindex] - '0';
    }
    (*pindex)++;
    if (line[start] == '-')
    {
        result *= -1;
    }
    return result;
}

#endif