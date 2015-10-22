/*
 * Copyright (c) 2010 Nicolas George
 * Copyright (c) 2011 Stefano Sabatini
 * Copyright (c) 2015 Erkki Seppälä <erkki.seppala.ext@nokia.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
 * THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 * THE SOFTWARE.
 */

/**
 * @file
 * API example extracting timed metadata
 * @example extract_timed_metadata.c
 */

#define _XOPEN_SOURCE 600 /* for usleep */
#include <unistd.h>
#include <stdint.h>

#include <libavcodec/avcodec.h>
#include <libavformat/avformat.h>
#include <libavutil/opt.h>
#include <assert.h>
#include <ctype.h>

#define MAX_METADATA_STREAMS 10

static AVFormatContext *fmt_ctx;
static AVCodecContext *dec_ctx[MAX_METADATA_STREAMS];
static int metadata_stream_indices[MAX_METADATA_STREAMS + 1] = { -1 }; /* terminated with -1 */

static int open_input_file(const char *filename)
{
    int ret;
    AVCodec *dec = avcodec_find_decoder(AV_CODEC_ID_META);
    int i;
    int nb_metadata = 0;

    if ((ret = avformat_open_input(&fmt_ctx, filename, NULL, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot open input file\n");
        return ret;
    }

    if ((ret = avformat_find_stream_info(fmt_ctx, NULL)) < 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find stream information\n");
        return ret;
    }

    for (i = 0; i < fmt_ctx->nb_streams; i++) {
        if (fmt_ctx->streams[i]->codecpar->codec_type == AVMEDIA_TYPE_DATA) {
            assert(nb_metadata < MAX_METADATA_STREAMS);
            metadata_stream_indices[nb_metadata] = i;
            nb_metadata++;
        }
    }
    metadata_stream_indices[nb_metadata] = -1;

    if (nb_metadata == 0) {
        av_log(NULL, AV_LOG_ERROR, "Cannot find a video stream in the input file\n");
        return ret;
    }

    for (i = 0; i < nb_metadata; i++) {
        dec_ctx[i] = avcodec_alloc_context3(dec);
        if ((ret = avcodec_open2(dec_ctx[i], dec, NULL)) < 0) {
            av_log(NULL, AV_LOG_ERROR, "Cannot open metadata decoder for metadata stream on track %d\n", i);
            return ret;
        }
    }

    return 0;
}

static void* memndup(void* ptr, int n)
{
    void* buffer = malloc(n);
    memcpy(buffer, ptr, n);
    return buffer;
}

static char* local_strndup(char* str, int n)
{
    int len = 0;
    while (str[len] && len < n) {
        ++len;
    }
    char* buffer = malloc(n + 1);
    memcpy(buffer, str, n);
    buffer[len] = 0;
    return buffer;
}

int main(int argc, char **argv)
{
    int ret;
    AVPacket packet;
    AVFrame *metadata = av_frame_alloc();
    int i;

    if (!metadata) {
        perror("Could not allocate metadata");
        exit(1);
    }
    if (argc != 2) {
        fprintf(stderr, "Usage: %s file\n", argv[0]);
        exit(1);
    }

    av_register_all();

    if ((ret = open_input_file(argv[1])) < 0)
        goto end;

    for (i = 0; metadata_stream_indices[i] >= 0; i++) {
        char *uri = NULL;
        char *tref_tag = NULL;
        int size;
        int trefsSize;
        AVTimedMetadataInfo* metadata =
            (AVTimedMetadataInfo*) av_stream_get_side_data(fmt_ctx->streams[metadata_stream_indices[i]],
                                                           AV_PKT_DATA_TIMED_METADATA_INFO,
                                                           &size);
        AVTrackReferences* trefs =
            (AVTrackReferences*) av_stream_get_side_data(fmt_ctx->streams[metadata_stream_indices[i]],
                                                         AV_PKT_DATA_TRACK_REFERENCES,
                                                         &trefsSize);
        if (!trefs)
            printf("No track references!\n");
        else if (!metadata)
            printf("No metadata info!\n");
        else if (metadata) {
            tref_tag = local_strndup(metadata->meta_tag, 4);
            uri = local_strndup((char*) (metadata + 1), metadata->meta_length);
            int j;
            int conf_len = metadata->conf_length;
            void* conf = memndup((char*) (metadata + 1) + metadata->meta_length, metadata->conf_length);
            char* conf_tag = local_strndup(metadata->conf_tag, 4);
            printf("Track #%d describing track%s ",
                   metadata_stream_indices[i] + 1,
                   trefs->nb_tracks > 1 ? "s" : "");
            for (j = 0; j < trefs->nb_tracks; ++j)
                printf("#%d ", trefs->tracks[j]);
            printf("with tref %s",
                   tref_tag);
            printf(" url: %s \n", uri);
            printf(" configuration conf tag %s, %d bytes:", conf_tag, conf_len);
            for (j = 0; j < conf_len; ++j) {
                unsigned char ch = ((unsigned char*) conf)[j];
                if (isprint(ch)) {
                    printf(" %c", ch);
                } else {
                    printf(" %2x", ((unsigned char*) conf)[j]);
                }
            }
            printf("\n");
            free(conf);
            free(conf_tag);
            free(uri);
            free(tref_tag);
        } else {
            printf("No meta data info for track  %d?!\n", metadata_stream_indices[i]);
        }
    }

    /* read all packets */
    while (1) {
        if ((ret = av_read_frame(fmt_ctx, &packet)) < 0)
            break;

        for (i = 0; metadata_stream_indices[i] >= 0; i++) {
            if (packet.stream_index == metadata_stream_indices[i]) {
                ret = avcodec_send_packet(dec_ctx[i], &packet);

                // We always empty decoded frames so we don't handle AVERROR(EAGAIN) here
                if (ret < 0) {
                    av_log(NULL, AV_LOG_ERROR, "Error decoding meta data\n");
                    break;
                }

                while (avcodec_receive_frame(dec_ctx[i], metadata) == 0) {
                    int c;
                    printf("track #%d at %" PRId64 " (%d bytes), ",
                           packet.stream_index + 1,
                           metadata->pts,
                           metadata->buf[0]->size);
                    for (c = 0; c < metadata->buf[0]->size; ++c) {
                        char ch = ((char*) metadata->buf[0]->data)[c];
                        if (ch == '\\') {
                            printf("\\\\");
                        } else if (isprint(ch)) {
                            putchar(ch);
                        } else {
                            printf("\\0x%2x", (unsigned char) ch);
                        }
                    }
                    putchar('\n');
                }
            }
        }
        av_packet_unref(&packet);
    }
end:
    for (i = 0; metadata_stream_indices[i] >= 0; i++) {
        avcodec_close(dec_ctx[i]);
    }
    avformat_close_input(&fmt_ctx);
    av_frame_free(&metadata);

    if (ret < 0 && ret != AVERROR_EOF) {
        fprintf(stderr, "Error occurred: %s\n", av_err2str(ret));
        exit(1);
    }

    exit(0);
}
