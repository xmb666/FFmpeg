/*
 * ISO base media file format track references
 * Copyright (c) 2016 Erkki Seppälä <erkki.seppala.ext@nokia.com>
 *
 * This file is part of FFmpeg.
 *
 * FFmpeg is free software; you can redistribute it and/or
 * modify it under the terms of the GNU Lesser General Public
 * License as published by the Free Software Foundation; either
 * version 2.1 of the License, or (at your option) any later version.
 *
 * FFmpeg is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * Lesser General Public License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public
 * License along with FFmpeg; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301 USA
 */

#include <stdint.h>
#include <inttypes.h>
#include <stdlib.h>

#include "movtref.h"
#include "libavutil/mem.h"

MOVTRef *ff_mov_tref_find(MOVTRefs *trefs, uint32_t tag)
{
    int i;
    MOVTRef *tref = NULL;

    for (i = 0; i < trefs->nb_trefs && !tref; ++i) {
        if (trefs->trefs[i].tag == tag) {
            tref = trefs->trefs + i;
        }
    }

    return tref;
}

int ff_mov_tref_alloc(MOVTRef *tref, int n, int **track_ids_ret)
{
    int ret = av_reallocp_array(&tref->track_ids, tref->nb_track_ids + n, sizeof(tref->track_ids));
    *track_ids_ret = NULL;
    if (ret >= 0) {
        *track_ids_ret = tref->track_ids + tref->nb_track_ids;
        tref->nb_track_ids += n;
    }
    return ret;
}

int ff_mov_tref_find_or_add(MOVTRefs *trefs, uint32_t tag, MOVTRef **tref_ret)
{
    int ret;
    int i;
    MOVTRef *tref = ff_mov_tref_find(trefs, tag);
    *tref_ret = NULL;

    for (i = 0; i < trefs->nb_trefs && !tref; ++i) {
        if (trefs->trefs[i].tag == tag) {
            tref = trefs->trefs + i;
        }
    }

    if (!tref) {
        ret = av_reallocp_array(&trefs->trefs, trefs->nb_trefs + 1, sizeof(*trefs->trefs));
        if (ret < 0)
            return ret;
        tref = trefs->trefs + trefs->nb_trefs;
        trefs->nb_trefs++;
        tref->tag = tag;
        tref->track_ids = NULL;
        tref->nb_track_ids = 0;
    }

    *tref_ret = tref;
    return 0;
}

int ff_mov_tref_add_one_track(MOVTRefs *trefs, unsigned tag, int track_id)
{
    int ret;
    MOVTRef *tref;
    int *ids;
    int track_id_exists = 0;
    int i;

    ret = ff_mov_tref_find_or_add(trefs, tag, &tref);
    if (ret < 0)
        return ret;

    for (i = 0; i < tref->nb_track_ids && !track_id_exists; i++) {
        if (tref->track_ids[i] == track_id) {
            track_id_exists = 1;
        }
    }

    if (!track_id_exists) {
        ret = ff_mov_tref_alloc(tref, 1, &ids);
        if (ret >= 0) {
            *ids = track_id;
        }
    }
    return ret;
}

void ff_mov_tref_free(MOVTRefs *trefs)
{
    int i;
    for (i = 0; i < trefs->nb_trefs; ++i)
        av_freep(&trefs->trefs[i].track_ids);
    av_freep(&trefs->trefs);
}
