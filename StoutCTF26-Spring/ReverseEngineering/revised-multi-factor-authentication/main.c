#include <stdio.h>
#define MAGIC 1337

int check(int input);
void decrypt_flag();
int check0(int input); 
int check1(int input); 
int check2(int input); 
int check3(int input); 
int check4(int input); 
int check5(int input); 
int check6(int input); 
int check7(int input); 
int check8(int input); 
int check9(int input); 
int check10(int input);
int check11(int input);
int check12(int input);
int check13(int input);
int check14(int input);
int check15(int input);
int check16(int input);
int check17(int input);
int check18(int input);
int check20(int input);
int check21(int input);
int check22(int input);
int check23(int input);
int check24(int input);
int check25(int input);
int check26(int input);
int check27(int input);
int check28(int input);
int check29(int input);
int check30(int input);
void dispatcher(int input);

int main() {
    int input;
    printf("Enter value: ");
    scanf("%i", &input);
    dispatcher(input);
    return 0;
}

int check0(int input) { return input ^ 0x10; }
int check1(int input) { return input + 1; }
int check2(int input) { return input - 1; }
int check3(int input) { return input * 3; }
int check4(int input) { return input / 2; }
int check5(int input) { return input ^ 0x55; }
int check6(int input) { return input + 42; }
int check7(int input) { return input - 99; }
int check8(int input) { return input * input; }
int check9(int input) { return input ^ 0xDEAD; }
int check10(int input) { return input + 7; }
int check11(int input) { return input - 7; }
int check12(int input) { return input ^ 0xAA; }
int check13(int input) { return input + 13; }
int check14(int input) { return input - 13; }
int check15(int input) { return input * 2; }
int check16(int input) { return input ^ 0x1337; }
int check17(int input) { return input + 0x10; }
int check18(int input) { return input - 0x10; }

int check20(int input) { return input ^ 0x20; }
int check21(int input) { return input + 21; }
int check22(int input) { return input - 21; }
int check23(int input) { return input * 5; }
int check24(int input) { return input ^ 0xBEEF; }
int check25(int input) { return input + 25; }

int check27(int input) { return input - 27; }
int check28(int input) { return input ^ 0x28; }
int check29(int input) { return input + 29; }
int check30(int input) { return input - 30; }

int check26(int input) {
    decrypt_flag();
    return 1;
}

void dispatcher(int input) {

    if (input < 0) {
        input = input ^ 0xdeadbeef;
    } else if (input > 1000) {

        if (input % 7 == 0) {
            if (input % 191 == 0) {
                if (input == MAGIC) {
                    check26(input);
                }
            }
        }

        if (input % 5 == 0) {
            // fake path
            check12(input);
        }

        if (input % 7 == 0) {
            // fake path
            check18(input);
        } else if (input % 11 == 0) {
            check3(input);
        }

    } else if (input > 500) {

        while (input > 0) {
            if (input == 9999) {
                check7(input);
            }
            input--;
        }

    } else if (input > 200) {

        for (int i = 0; i < 10; i++) {
            for (int j = 0; j < 5; j++) {

                if (i == j) {
                    if (input == MAGIC) {
                        check26(input);
                    } else if (input % 2 == 0) {
                        check14(input);
                    }

                    for (int k = 0; k < 3; k++) {
                        for (int m = 0; m < 2; m++) {
                            if (k == m) {
                                check9(input);
                            }
                        }
                    }
                }
            }
        }
    }

    return;
}

void decrypt_flag() {
    unsigned char key1 = 0x9f;
    unsigned char key2 = 0x4a;
    unsigned int seed = 0x57266b08;
    int chunk_size = 4;
    int chunk_count = 11;
    int perm[] = { 0, 1, 8, 7, 5, 9, 6, 4, 10, 3, 2 };
    unsigned char data[] = {
        0x86, 0x9e, 0xa4, 0xdd, 0xfd, 0x0d, 0x3b, 0x4a, 0x01, 0x7a, 0x93, 0xa7, 0xf3, 0x0e, 0x25, 0x27, 0xef, 0x08, 0x1a, 0x6f, 0xed, 0xec, 0x00, 0x2e, 0x4f, 0x9e, 0xc0, 0xa8, 0x61, 0xeb, 0xcb, 0xae, 0x3d, 0x5f, 0x00, 0x00, 0xcc, 0x0c, 0x5f, 0x4d, 0x56, 0xa0, 0xa9, 0xe7
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
    for(int i = 0; i < len- 2; i++) {
        printf("%c", out[i]);
    }
    printf("\n");

}