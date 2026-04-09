#include <stdio.h>

void decrypt_flag();

int main() {
    printf("Please enter the password\n> ");
    int input;
    scanf("%i", &input);
    if (input > 100) {
        if(input < 200) {
            if (input % 2 == 0) {
                decrypt_flag();
            } else {
                printf("Incorrect!");
            }
        } else {
            printf("Incorrect!");
        }
    } else {
        printf("Incorrect!");
    }
    return 0;
}

void decrypt_flag() {
    unsigned char key = 0xe5;
    unsigned char data[] = {
        0xb6, 0xb1, 0xaa, 0xb0, 0xb1, 0xa6, 0xb1, 0xa3, 0x9e, 0xa1, 0xac, 0xd5, 0xaf, 0xd3, 0x8b, 0xa6, 0xab, 0xac, 0xb5, 0xa9, 0xbd, 0x8a, 0xd2, 0x93, 0xb7, 0x88, 0xab, 0xb6, 0x94, 0xd6, 0xb0, 0xa9, 0xaf, 0xd6, 0xa1, 0xb4, 0xd2, 0x87, 0x84, 0xd5, 0xa1, 0x98
    };
    int len = 42;

    for(int i = 0; i < len; i++) {
        data[i] ^= key;
    }

    printf("%s\n", data);
}