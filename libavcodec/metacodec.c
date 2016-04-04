/*
 * Copyright (c) 2015 Erkki Seppälä <erkki.seppala.ext@nokia.com>
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

#include "libavutil/intreadwrite.h"
#include "internal.h"
#include "libavutil/opt.h"
#include "libavformat/avio.h"

static int meta_encode(AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame,
                       int *got_packet_ptr)
{
    *got_packet_ptr = 0;

    av_packet_unref(avpkt);
    if (frame->buf) {
        avpkt->buf = av_buffer_ref(frame->buf[0]);
        if (!avpkt->buf)
          return AVERROR(ENOMEM);
        avpkt->data = frame->data[0];
    } else {
        avpkt->buf = av_buffer_alloc(frame->nb_samples);
        if (!avpkt->buf)
          return AVERROR(ENOMEM);
        avpkt->data = avpkt->buf;
        memcpy(avpkt->data, frame->data[0], frame->nb_samples);
    }
    avpkt->size = frame->nb_samples;
    avpkt->pts = frame->pts;
    avpkt->dts = frame->pts;
    *got_packet_ptr = 1;

    return 0;
}

static int meta_decode(AVCodecContext *avctx, void *data, int *got_packet_ptr, AVPacket *avpkt)
{
    AVFrame *metadata = data;

    *got_packet_ptr = 0;

    av_frame_unref(metadata);
    if (avpkt->buf) {
        metadata->buf[0] = av_buffer_ref(avpkt->buf);
        if (!metadata->buf[0])
            return AVERROR(ENOMEM);
        metadata->buf[0]->size = avpkt->size;
    } else {
        metadata->buf[0] = av_buffer_alloc(avpkt->size);
        if (!metadata->buf[0])
            return AVERROR(ENOMEM);
        metadata->data[0] = metadata->buf[0]->data;
        memcpy(metadata->data[0], avpkt->data, avpkt->size);
    }

    metadata->nb_samples = avpkt->size;
    metadata->pts = avpkt->pts;
    metadata->pkt_pts = avpkt->pts;
    metadata->pkt_dts = avpkt->dts;
    *got_packet_ptr = 1;

    return 0;
}

AVCodec ff_meta_encoder = {
    .name           = "meta",
    .long_name      = NULL_IF_CONFIG_SMALL("Metadata Encoder"),
    .type           = AVMEDIA_TYPE_DATA,
    .id             = AV_CODEC_ID_META,
    .encode2        = meta_encode,
};

AVCodec ff_meta_decoder = {
    .name           = "meta",
    .long_name      = NULL_IF_CONFIG_SMALL("Metadata Decoder"),
    .type           = AVMEDIA_TYPE_DATA,
    .id             = AV_CODEC_ID_META,
    .decode         = meta_decode,
};
