#include <iostream>
#include <string>

void decrypt_flag();

int main() {
    std::string input = "";
    int result;
    std::cout << "What's 9 + 10?" << std::endl;
    std::cin >> input;
    result = stoi(input, 0, 16);
    if ( (result - 200 ) == 21 ) {
        decrypt_flag();
    }
}

void decrypt_flag() {
    unsigned char key1 = 0xaa;
    unsigned char key2 = 0xa9;
    unsigned int seed = 0xd66201;
    int chunk_size = 4;
    int chunk_count = 11;
    int perm[] = { 5, 2, 10, 9, 7, 0, 1, 6, 4, 8, 3 };
    unsigned char data[] = {
        0x18, 0xde, 0xfb, 0xf2, 0x80, 0x57, 0x6d, 0x15, 0x95, 0x89, 0x00, 0x00, 0x1a, 0x49, 0xe1, 0xf3, 0x2f, 0xc9, 0xc5, 0xfb, 0x50, 0x48, 0x72, 0x0b, 0x2b, 0xdb, 0xed, 0x9c, 0x8c, 0x73, 0x67, 0x00, 0x91, 0x7c, 0x5e, 0x0c, 0xab, 0x9f, 0x67, 0x5c, 0x19, 0xf8, 0x82, 0xbe
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