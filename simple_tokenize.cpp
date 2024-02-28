#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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

void free_vocab(vocab_info *vinfo) {
    for (int i = 0; i < vinfo->vocab_size; i++) {
        free(vinfo->vocab[i]);
    }

    free(vinfo->vocab);
}

int str2token(char *str, vocab_info *vinfo) {
    for (int i = 0; i < vinfo->vocab_size; i++) {
        if (strcmp(str, vinfo->vocab[i]) == 0) {
            return i;
        }
    }

    return -1;
}

void token_encode(vocab_info *vinfo, const char *str, int *tokens, int *tokens_num) {
    char buf[5];
    int len = 0;
    *tokens_num = 0;

    for (int i = 0; str[i] != '\0'; i++) {
        if ((str[i] & 0xC0) != 0x80) {
            len = 0;
        }

        buf[len++] = str[i];
        buf[len] = '\0';

        if ((str[i + 1] & 0xC0) == 0x80 && len < 4) {
            continue;
        }

        int id = str2token(buf, vinfo);
        if (id < 0) {
            printf("content not found %s\n", buf);
            return;
        }

        tokens[(*tokens_num)++] = id;
        len = 0;
    }
}

int main(int argc, char *argv[]) {
    if (argc != 2) {
        printf("Usage:\n\t%s sentence\n", argv[0]);
        return -1;
    }

    const char *str = argv[1];
    int tokens_num = 0;
    int *tokens = (int *)malloc((strlen(str) + 1) * sizeof(int));
    printf("[1] original sentence is: [%s]\n", str);

    vocab_info vinfo;
    load_vocab(&vinfo);
    token_encode(&vinfo, str, tokens, &tokens_num);

    printf("[2] tokenize result is:\n");
    for (int i = 0; i < tokens_num; i++) {
        printf("\tid %d, %d\n", i, tokens[i]);
    }

    free_vocab(&vinfo);

    return 0;
}
