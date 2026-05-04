#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>

void decrypt_flag();
void validate_section(char* token, int iterator);
void check_one(char* token);
void check_two(char* token);
void check_three(char* token);

int main() {
    char input[50];
    printf("Please enter your license number\n> ");
    fgets(input, sizeof(input), stdin);
    input[strcspn(input, "\n")] = 0;

    char* token = strtok(input, "-");
    int iterator = 0;
    
    while (token != NULL) {
        iterator++;
        validate_section(token, iterator);
        token = strtok(NULL, "-");
    }
    
    if (1338 == 1337) {
        printf("debug mode\n");
    }
}

void validate_section(char * token, int iterator) {
    if (iterator == 1) {
        check_one(token);
    } else if (iterator == 2) {
        check_two(token);
    } else if (iterator == 3) {
        check_three(token);
    } else {
        printf("An error has occurred");
    }
}

void check_one(char* token) {
    printf("Checking Part One...\n");
    // ^Z]G
    char target[4] = { 'M', 'I', 'N', 'T'};

    for (int i = 0; i < 4; i++) {
        token[i] = token[i] ^ 0x13;
    }


    if (strncmp(token, target, 4) == 0) {
        printf("Part One OK\n");
    } else {
        printf("Part One Failed\n");
        exit(1);
    }
}

void check_two(char* token) {
    printf("Checking Part Two...\n");
    // PPPP
    int sum = 0;

    for (int i = 0; i < 4; i++) {
        sum += (token[i] - 'A') * (i + 1);
    }

    if (sum == 150) {
        printf("Part Two OK\n");
    } else {
        printf("Part Two Failed\n");
        exit(1);
    }
}

void check_three(char* token) {
    printf("Checking Part Three...\n");
    printf("You thought you could just get it? Nope ;)");
    sleep(50000);
    printf("ACCESS GRANTED\n");
    decrypt_flag();
}

void decrypt_flag() {
    unsigned char key1 = 0xad;
    unsigned char key2 = 0x0a;
    unsigned int seed = 0xb901750f;
    int chunk_size = 4;
    int chunk_count = 11;
    int perm[] = { 2, 5, 6, 0, 9, 10, 1, 7, 8, 4, 3 };
    unsigned char data[] = {
        0x24, 0xc2, 0xde, 0xbb, 0xfb, 0x1d, 0x39, 0x26, 0x7f, 0xfa, 0xf6, 0x89, 0xf4, 0xec, 0xd6, 0xaf, 0xaa, 0xaf, 0x52, 0x59, 0x0e, 0x2d, 0x00, 0x00, 0x8f, 0x7f, 0x49, 0x38, 0x8a, 0x77, 0x57, 0x29, 0x75, 0x3a, 0xff, 0xdc, 0x02, 0xe2, 0xda, 0x93, 0xe3, 0x61, 0x54, 0x15
    };
    int len = 44;

    unsigned char chunks[256][4];
    unsigned char stage1[512];
    unsigned char stage2[512];
    unsigned char out[512];

    int idx = 0;

    // ----------------------------
    // STEP 1: rebuild original chunks using perm
    // ----------------------------
    for(int i = 0; i < chunk_count; i++) {
        for(int j = 0; j < chunk_size; j++) {
            chunks[perm[i]][j] = data[idx++];
        }
    }

    // ----------------------------
    // STEP 2: flatten in correct order
    // ----------------------------
    idx = 0;
    for(int i = 0; i < chunk_count; i++) {
        for(int j = 0; j < chunk_size; j++) {
            stage1[idx++] = chunks[i][j];
        }
    }

    // ----------------------------
    // STEP 3: undo positional XOR
    // ----------------------------
    for(int i = 0; i < len; i++) {
        stage2[i] = stage1[i] ^ (key2 ^ ((i * 31) & 0xff));
    }

    // ----------------------------
    // STEP 4: undo base XOR
    // ----------------------------
    for(int i = 0; i < len; i++) {
        out[i] = stage2[i] ^ key1;
    }

    // ----------------------------
    // SAFE OUTPUT (NO %s BUG)
    // ----------------------------
    for(int i = 0; i < len - 2; i++) {
        printf("%c", out[i]);
    }
    printf("\n");

}