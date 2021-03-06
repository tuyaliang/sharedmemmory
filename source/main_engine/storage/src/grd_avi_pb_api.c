#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <pthread.h>
#include <dirent.h>

#include "avi_common.h"
#include "log.h"

#define _MSG_DEBUG_WR 0

///#define PLAYBACK_DEBUG
#ifdef PLAYBACK_DEBUG
#define PLAY_DPRINT(fmt, args...) printf(fmt, ## args)
#else
#define PLAY_DPRINT(a...)
#endif

//#define SEEK_DEBUG
#ifdef SEEK_DEBUG
#define SEEK_DPRINT(fmt, args...) printf(fmt, ## args)
#else
#define SEEK_DPRINT(a...)
#endif

static int avi_get_32(void *pp)
{
    unsigned char *p = pp;
    int tmp32 = (int)((p[0])|(p[1])<<8|(p[2])<<16|(p[3])<<24);
    return tmp32;
}

static uint32_t avi_get_u32(void *pp)
{
    unsigned char *p = pp;
    uint32_t tmp32 = (uint32_t)((p[0])|(p[1])<<8|(p[2])<<16|(p[3])<<24);
    return tmp32;
}

static int check_avi_header_fcc(AviHeader *avi_header)
{
   if(avi_header->fcc_riff != MAKE_FOURCC('R','I','F','F'))
        return -1;

    if(avi_header->fcc_avi != MAKE_FOURCC('A','V','I',' '))
        return -1;

    if(avi_header->fcc_list_hdrl != MAKE_FOURCC('L','I','S','T'))
        return -1;

    if(avi_header->fcc_hdrl != MAKE_FOURCC('h','d','r','l'))
        return -1;

    if(avi_header->fcc_list_strl_video != MAKE_FOURCC('L','I','S','T'))
        return -1;
    if(avi_header->fcc_strl_video != MAKE_FOURCC('s','t','r','l'))
        return -1;
    if(avi_header->fcc_strf_video != MAKE_FOURCC('s','t','r','f'))
        return -1;

    if(avi_header->fcc_list_strl_audio != MAKE_FOURCC('L','I','S','T'))
        return -1;
    if(avi_header->fcc_strl_audio != MAKE_FOURCC('s','t','r','l'))
        return -1;
    if(avi_header->fcc_strf_audio != MAKE_FOURCC('s','t','r','f'))
        return -1;

    if(avi_header->fcc_list_movi != MAKE_FOURCC('L','I','S','T'))
        return -1;
    if(avi_header->fcc_movi != MAKE_FOURCC('m','o','v','i'))
        return -1;

    return 0;
}

