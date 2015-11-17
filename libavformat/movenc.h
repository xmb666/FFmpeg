/*
 * MOV, 3GP, MP4 muxer
 * Copyright (c) 2003 Thomas Raivio
 * Copyright (c) 2004 Gildas Bazin <gbazin at videolan dot org>
 * Copyright (c) 2009 Baptiste Coudurier <baptiste dot coudurier at gmail dot com>
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

#ifndef AVFORMAT_MOVENC_H
#define AVFORMAT_MOVENC_H

#include "avformat.h"
#include "movenccenc.h"
#include "movmeta.h"

#define MOV_FRAG_INFO_ALLOC_INCREMENT 64
#define MOV_INDEX_CLUSTER_SIZE 1024
#define MOV_TIMESCALE 1000

#define RTP_MAX_PACKET_SIZE 1450

#define MODE_MP4  0x01
#define MODE_MOV  0x02
#define MODE_3GP  0x04
#define MODE_PSP  0x08 // example working PSP command line:
// ffmpeg -i testinput.avi  -f psp -r 14.985 -s 320x240 -b 768 -ar 24000 -ab 32 M4V00001.MP4
#define MODE_3G2  0x10
#define MODE_IPOD 0x20
#define MODE_ISM  0x40
#define MODE_F4V  0x80

typedef struct MOVIentry {
    uint64_t     pos;
    int64_t      dts;
    unsigned int size;
    unsigned int samples_in_chunk;
    unsigned int chunkNum;              ///< Chunk number if the current entry is a chunk start otherwise 0
    unsigned int entries;
    int          cts;
#define MOV_SYNC_SAMPLE         0x0001
#define MOV_PARTIAL_SYNC_SAMPLE 0x0002
    uint32_t     flags;
} MOVIentry;

typedef struct HintSample {
    uint8_t *data;
    int size;
    int sample_number;
    int offset;
    int own_data;
} HintSample;

typedef struct HintSampleQueue {
    int size;
    int len;
    HintSample *samples;
} HintSampleQueue;

typedef struct MOVFragmentInfo {
    int64_t offset;
    int64_t time;
    int64_t duration;
    int64_t tfrf_offset;
    int size;
} MOVFragmentInfo;

typedef struct MOVTRef {
    uint32_t tag;
    int      nb_ids;
    int*     ids;
} MOVTRef;

typedef struct MOVTrack {
    int         mode;
    int         entry;
    unsigned    timescale;
    uint64_t    time;
    int64_t     track_duration;
    int         last_sample_is_subtitle_end;
    long        sample_count;
    long        sample_size;
    long        chunkCount;
    int         has_keyframes;
#define MOV_TRACK_CTTS         0x0001
#define MOV_TRACK_STPS         0x0002
#define MOV_TRACK_ENABLED      0x0004
    uint32_t    flags;
#define MOV_TIMECODE_FLAG_DROPFRAME     0x0001
#define MOV_TIMECODE_FLAG_24HOURSMAX    0x0002
#define MOV_TIMECODE_FLAG_ALLOWNEGATIVE 0x0004
    uint32_t    timecode_flags;
    int         language;
    int         track_id;
    int         tag; ///< stsd fourcc
    AVStream        *st;
    AVCodecParameters *par;
    int multichannel_as_mono;

    int         vos_len;
    uint8_t     *vos_data;
    MOVIentry   *cluster;
    unsigned    cluster_capacity;
    int         audio_vbr;
    int         height; ///< active picture (w/o VBI) height for D-10/IMX
    MOVTRef*    trefs;
    int         nb_trefs;
    int64_t     start_dts;
    int64_t     start_cts;
    int64_t     end_pts;
    int         end_reliable;

    int         hint_track;   ///< the track that hints this track, -1 if no hint track is set
    int         nb_src_tracks;  ///< number of src tracks
    int         *src_tracks;    ///< the tracks that this hint (or tmcd or cdsc) track describes
    AVFormatContext *rtp_ctx; ///< the format context for the hinting rtp muxer
    uint32_t    prev_rtp_ts;
    int64_t     cur_rtp_ts_unwrapped;
    uint32_t    max_packet_size;

    int64_t     default_duration;
    uint32_t    default_sample_flags;
    uint32_t    default_size;

    HintSampleQueue sample_queue;

    AVIOContext *mdat_buf;
    int64_t     data_offset;
    int64_t     frag_start;
    int         frag_discont;
    int         entries_flushed;

    int         nb_frag_info;
    MOVFragmentInfo *frag_info;
    unsigned    frag_info_capacity;

    struct {
        int     first_packet_seq;
        int     first_packet_entry;
        int     first_packet_seen;
        int     first_frag_written;
        int     packet_seq;
        int     packet_entry;
        int     slices;
    } vc1_info;

    void       *eac3_priv;

    MOVMuxCencContext cenc;

    uint32_t palette[AVPALETTE_COUNT];
    int pal_done;

    int is_unaligned_qt_rgb;
} MOVTrack;

typedef enum {
    MOV_ENC_NONE = 0,
    MOV_ENC_CENC_AES_CTR,
} MOVEncryptionScheme;

typedef struct MOVMuxContext {
    const AVClass *av_class;
    int     mode;
    int64_t time;
    int     nb_streams;
    int     nb_meta_tmcd;  ///< number of new created tmcd track based on metadata (aka not data copy)
    int     chapter_track; ///< qt chapter track number
    int64_t mdat_pos;
    uint64_t mdat_size;
    MOVTrack *tracks;

    int flags;
    int rtp_flags;

    int iods_skip;
    int iods_video_profile;
    int iods_audio_profile;

    int moov_written;
    int fragments;
    int max_fragment_duration;
    int min_fragment_duration;
    int max_fragment_size;
    int ism_lookahead;
    AVIOContext *mdat_buf;
    int first_trun;

    int video_track_timescale;

    int reserved_moov_size; ///< 0 for disabled, -1 for automatic, size otherwise
    int64_t reserved_header_pos;

    char *major_brand;

    int per_stream_grouping;
    AVFormatContext *fc;

    int use_editlist;
    float gamma;

    int frag_interleave;
    int missing_duration_warned;

    char *encryption_scheme_str;
    MOVEncryptionScheme encryption_scheme;
    uint8_t *encryption_key;
    int encryption_key_len;
    uint8_t *encryption_kid;
    int encryption_kid_len;

    int need_rewrite_extradata;

    int use_stream_ids_as_track_ids;
    int track_ids_ok;
} MOVMuxContext;

#define FF_MOV_FLAG_RTP_HINT              (1 <<  0)
#define FF_MOV_FLAG_FRAGMENT              (1 <<  1)
#define FF_MOV_FLAG_EMPTY_MOOV            (1 <<  2)
#define FF_MOV_FLAG_FRAG_KEYFRAME         (1 <<  3)
#define FF_MOV_FLAG_SEPARATE_MOOF         (1 <<  4)
#define FF_MOV_FLAG_FRAG_CUSTOM           (1 <<  5)
#define FF_MOV_FLAG_ISML                  (1 <<  6)
#define FF_MOV_FLAG_FASTSTART             (1 <<  7)
#define FF_MOV_FLAG_OMIT_TFHD_OFFSET      (1 <<  8)
#define FF_MOV_FLAG_DISABLE_CHPL          (1 <<  9)
#define FF_MOV_FLAG_DEFAULT_BASE_MOOF     (1 << 10)
#define FF_MOV_FLAG_DASH                  (1 << 11)
#define FF_MOV_FLAG_FRAG_DISCONT          (1 << 12)
#define FF_MOV_FLAG_DELAY_MOOV            (1 << 13)
#define FF_MOV_FLAG_GLOBAL_SIDX           (1 << 14)
#define FF_MOV_FLAG_WRITE_COLR            (1 << 15)
#define FF_MOV_FLAG_WRITE_GAMA            (1 << 16)
#define FF_MOV_FLAG_USE_MDTA              (1 << 17)

int ff_mov_write_packet(AVFormatContext *s, AVPacket *pkt);

int ff_mov_init_hinting(AVFormatContext *s, int index, int src_index);
int ff_mov_add_hinted_packet(AVFormatContext *s, AVPacket *pkt,
                             int track_index, int sample,
                             uint8_t *sample_data, int sample_size);
void ff_mov_close_hinting(MOVTrack *track);

/**
 * @brief Finds a track reference of certain tag from a track; if not
 * found, return NULL
 *
 * @param track The track to search from
 * @param tag The tag (4cc) to search for
 * @return a MOVTRef describing the track reference or NULL if not found.
 */
MOVTRef *ff_mov_find_tref(MOVTrack *track, uint32_t tag);

/**
 * @brief Finds a track reference of certain tag from a track; if not
 * found, create an empty track reference object for the track put it
 * into the track.
 *
 * @param track The track to search from
 * @param tag The tag (4cc) to search for
 * @param tref_ret A pointer to the the found or created tref is
 *                 returned here. Must not be NULL.
 * @return Zero on success, an AVERROR error code on failure. On error
 *         no modifications have been performed.
 */
int ff_mov_find_or_add_tref(MOVTrack *track, uint32_t tag, MOVTRef **tref_ret);

/**
 * @brief Adds a a track to a track reference that may or may not be
 * put into a track
 *
 * @param tref The track reference to add the track for
 * @param tag The tag (4cc) of the track reference
 * @return Zero on success, an AVERROR error code on failure. On error
 *         no modifications have been performed.
 */
int ff_mov_add_tref_track(MOVTRef *tref, int id);

/**
 * @brief Copies track->src_tracks from a track to the give tref
 *
 * @param mov The MOV muxer context
 * @param track The track to take the src_tracks from
 * @param tref The tref to translate the src_tracks into
 * @return Zero on success, an AVERROR error code on failure. On error
 *         no modifications have been performed. However, probably
 *         prior to this call src_tracks has been manipulated and now
 *         it is out-of-sync, so this is pretty fatal.
 */
int ff_mov_map_trefs_from_src_tracks(MOVMuxContext *mov, MOVTrack *track, MOVTRef *tref);

#endif /* AVFORMAT_MOVENC_H */
