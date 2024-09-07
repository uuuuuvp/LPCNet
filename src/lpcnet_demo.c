#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

#include <math.h>
#include <stdio.h>
#include "arch.h"
#include "lpcnet.h"
#include "freq.h"

#define MODE_ENCODE 0
#define MODE_DECODE 1
#define MODE_FEATURES 2
#define MODE_SYNTHESIS 3

int main(int argc, char **argv) {
    int mode;
    FILE *fin, *fout;
    if (argc != 4)
    {
        fprintf(stderr, "usage: lpcnet_demo -encode <input.pcm> <compressed.lpcnet>\n");
        fprintf(stderr, "       lpcnet_demo -decode <compressed.lpcnet> <output.pcm>\n");
        fprintf(stderr, "       lpcnet_demo -features <input.pcm> <features.f32>\n");
        fprintf(stderr, "       lpcnet_demo -synthesis <features.f32> <output.pcm>\n");
        return 0;
    }
    if (strcmp(argv[1], "-encode") == 0) mode=MODE_ENCODE;
    else if (strcmp(argv[1], "-decode") == 0) mode=MODE_DECODE;
    else if (strcmp(argv[1], "-features") == 0) mode=MODE_FEATURES;
    else if (strcmp(argv[1], "-synthesis") == 0) mode=MODE_SYNTHESIS;
    else {
        exit(1);
    }
    fin = fopen(argv[2], "rb");
    if (fin == NULL) {
	fprintf(stderr, "Can't open %s\n", argv[2]);
	exit(1);
    }

    fout = fopen(argv[3], "wb");
    if (fout == NULL) {
	fprintf(stderr, "Can't open %s\n", argv[3]);
	exit(1);
    }

    if (mode == MODE_ENCODE) {
        LPCNetEncState *net;
        net = lpcnet_encoder_create();
        while (1) {
            unsigned char buf[LPCNET_COMPRESSED_SIZE];
            short pcm[LPCNET_PACKET_SAMPLES];
            size_t ret;
            ret = fread(pcm, sizeof(pcm[0]), LPCNET_PACKET_SAMPLES, fin);
            if (feof(fin) || ret != LPCNET_PACKET_SAMPLES) break;
            lpcnet_encode(net, pcm, buf);
            fwrite(buf, 1, LPCNET_COMPRESSED_SIZE, fout);
        }
        lpcnet_encoder_destroy(net);
    } else if (mode == MODE_DECODE) {
        LPCNetDecState *net;
        net = lpcnet_decoder_create();
        while (1) {
            unsigned char buf[LPCNET_COMPRESSED_SIZE];
            short pcm[LPCNET_PACKET_SAMPLES];
            size_t ret;
            ret = fread(buf, sizeof(buf[0]), LPCNET_COMPRESSED_SIZE, fin);
            if (feof(fin) || ret != LPCNET_COMPRESSED_SIZE) break;
            lpcnet_decode(net, buf, pcm);
            fwrite(pcm, sizeof(pcm[0]), LPCNET_PACKET_SAMPLES, fout);
        }
        lpcnet_decoder_destroy(net);
    } else if (mode == MODE_FEATURES) {
        LPCNetEncState *net;
        net = lpcnet_encoder_create();
        while (1) {
            float features[NB_TOTAL_FEATURES];
            short pcm[LPCNET_FRAME_SIZE];
            size_t ret;
            ret = fread(pcm, sizeof(pcm[0]), LPCNET_FRAME_SIZE, fin);
            if (feof(fin) || ret != LPCNET_FRAME_SIZE) break;
            lpcnet_compute_single_frame_features(net, pcm, features);
            fwrite(features, sizeof(float), NB_TOTAL_FEATURES, fout);
        }
        lpcnet_encoder_destroy(net);
    } else if (mode == MODE_SYNTHESIS) {
        LPCNetState *net;
        net = lpcnet_create();
        while (1) {
            float in_features[NB_TOTAL_FEATURES];
            float features[NB_FEATURES];
            short pcm[LPCNET_FRAME_SIZE];
            size_t ret;
            ret = fread(in_features, sizeof(features[0]), NB_TOTAL_FEATURES, fin);
            if (feof(fin) || ret != NB_TOTAL_FEATURES) break;
            RNN_COPY(features, in_features, NB_FEATURES);
            lpcnet_synthesize(net, features, pcm, LPCNET_FRAME_SIZE);
            fwrite(pcm, sizeof(pcm[0]), LPCNET_FRAME_SIZE, fout);
        }
        lpcnet_destroy(net);
    } else {
        fprintf(stderr, "unknown action\n");
    }
    fclose(fin);
    fclose(fout);
    return 0;
}
