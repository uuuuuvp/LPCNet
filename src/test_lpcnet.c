#include <math.h>
#include <stdio.h>
#include "arch.h"
#include "lpcnet.h"
#include "freq.h"


int main(int argc, char **argv) {
    FILE *fin, *fout;
    LPCNetState *net;
    net = lpcnet_create();
    if (argc != 3)
    {
        fprintf(stderr, "usage: test_lpcnet <features.f32> <output.pcm>\n");
        return 0;
    }
    fin = fopen(argv[1], "rb");
    if (fin == NULL) {
	fprintf(stderr, "Can't open %s\n", argv[1]);
	exit(1);
    }

    fout = fopen(argv[2], "wb");
    if (fout == NULL) {
	fprintf(stderr, "Can't open %s\n", argv[2]);
	exit(1);
    }

    while (1) {
        float in_features[NB_TOTAL_FEATURES];
        float features[NB_FEATURES];
        short pcm[FRAME_SIZE];
        fread(in_features, sizeof(features[0]), NB_TOTAL_FEATURES, fin);
        if (feof(fin)) break;
        RNN_COPY(features, in_features, NB_FEATURES);
        lpcnet_synthesize(net, features, pcm, FRAME_SIZE);
        fwrite(pcm, sizeof(pcm[0]), FRAME_SIZE, fout);
    }
    fclose(fin);
    fclose(fout);
    lpcnet_destroy(net);
    return 0;
}
