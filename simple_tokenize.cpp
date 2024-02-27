#include <stdio.h>
#include <stdlib.h>

typedef struct {
    int vocab_size;
    int max_token_len;
    char **vocab;
} vocab_info;

void load_vocab(vocab_info *vinfo) {
    FILE *f = fopen("vocab.bin", "r");
    if (!f) {
        printf("vocab load error\n");
        return;
    }

    do {
        if (fread(&vinfo->vocab_size, sizeof(int), 1, f) != 1) {
            printf("vocab size error\n");
            break;
        }

        if (fread(&vinfo->max_token_len, sizeof(int), 1, f) != 1) {
            printf("vocab max token len error\n");
            break;
        }

        int len = 0;
        vinfo->vocab = (char **)malloc(vinfo->vocab_size * sizeof(char *));
        for (int i = 0; i < vinfo->vocab_size; i++) {
            if (fread(&len, sizeof(int), 1, f) != 1) {
                printf("vocab len read error\n");
                break;
            }

            vinfo->vocab[i] = (char *)malloc(len + 1);
            if (fread(vinfo->vocab[i], len, 1, f) != 1) {
                printf("vocab content read error\n");
                break;
            }
            vinfo->vocab[i][len] = '\0';
        }
    } while (0);
    
    fclose(f);
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage:\n\t%s sentence\n", argv[0]);
        return -1;
    }

    vocab_info vinfo;
    load_vocab(&vinfo);

    for (int i = 0; i < vinfo.vocab_size; i++) {
        printf("id %d, %s\n", i, vinfo.vocab[i]);
    }

    return 0;
}
