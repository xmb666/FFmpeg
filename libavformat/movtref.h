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
#ifndef AVFORMAT_MOVTREF_H
#define AVFORMAT_MOVTREF_H

typedef struct MOVTRef {
    uint32_t tag;
    int      nb_track_ids;
    int*     track_ids;
} MOVTRef;

typedef struct MOVTRefs {
    int      nb_trefs;
    MOVTRef  *trefs;
} MOVTRefs;

/**
 * @brief Finds a track reference of certain tag from a track; if not
 * found, return NULL
 *
 * @param track The track to search from
 * @param tag The tag (4cc) to search for
 * @return a MOVTRef describing the track reference or NULL if not found.
 */
MOVTRef *ff_mov_tref_find(MOVTRefs *trefs, uint32_t tag);

/**
 * @brief Allocate space for n more tracks (and increase nb_ids accordingly) returning
 * the pointer in *track_ids
 *
 * @param tref The track reference to allocate tracks to
 * @param n Number of tracks to add
 * @param track_ids_ret A non-null pointer where the pointer to the first free track
 *        id is returned to
 * @return Returns a value <0 on error, otherwise 0.
 */
int ff_mov_tref_alloc(MOVTRef *tref, int n, int **track_ids_ret);

/**
 * @brief Finds an existing MOVTRef for a track for the given tag, or
 * creates a new one if it is missing. Returns the tref in tref_ret.
 */

int ff_mov_tref_find_or_add(MOVTRefs *trefs, uint32_t tag, MOVTRef **tref_ret);

/**
 * Adds one tref track reference with given track id if it doesn't
 * already exist
 */
int ff_mov_tref_add_one_track(MOVTRefs *trefs, unsigned tag, int track_id);

/**
 * Release MOVTRefs
 */
void ff_mov_tref_free(MOVTRefs *trefs);

#endif /* AVFORMAT_MOVTREF_H */
