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

static av_cold int meta_init(AVCodecContext *avctx)
{
    return 0;
}

static av_cold int meta_encode(AVCodecContext *avctx, AVPacket *avpkt, const AVFrame *frame,
                               int *got_packet_ptr)
{
    int ret;

    *got_packet_ptr = 0;

    if ((ret = ff_alloc_packet2(avctx, avpkt, frame->nb_samples, 0)) < 0)
        return ret;
    memcpy(avpkt->data, frame->data[0], frame->nb_samples);
    avpkt->size = frame->nb_samples;
    avpkt->pts = frame->pts;
    avpkt->dts = frame->pts;
    *got_packet_ptr = 1;

    return 0;
}

static int meta_decode(AVCodecContext *avctx, void *data, int *got_packet_ptr, AVPacket *avpkt)
{
    AVData *metadata = data;

    *got_packet_ptr = 0;

    av_buffer_unref(&metadata->data);
    metadata->data = av_buffer_alloc(avpkt->size);
    if (!metadata->data)
        return AVERROR(ENOMEM);

    metadata->dts = avpkt->dts;
    metadata->pts = avpkt->pts;
    memcpy(((char*) metadata->data->data), avpkt->data, avpkt->size);
    *got_packet_ptr = 1;

    return 0;
}

static const AVClass metacodec_class = {
    .class_name = "Meta data codec",
    .item_name  = av_default_item_name,
    .version    = LIBAVUTIL_VERSION_INT,
};

AVCodec ff_meta_encoder = {
    .name           = "meta",
    .long_name      = NULL_IF_CONFIG_SMALL("Meta Data Encoder"),
    .type           = AVMEDIA_TYPE_DATA,
    .id             = AV_CODEC_ID_META,
    .init           = meta_init,
    .encode2        = meta_encode,
    .priv_class     = &metacodec_class,
    .priv_data_size = 0,
};

AVCodec ff_meta_decoder = {
    .name           = "meta",
    .long_name      = NULL_IF_CONFIG_SMALL("Meta Data Decoder"),
    .type           = AVMEDIA_TYPE_DATA,
    .id             = AV_CODEC_ID_META,
    .init           = meta_init,
    .decode         = meta_decode,
    .priv_class     = &metacodec_class,
    .priv_data_size = 0,
};
