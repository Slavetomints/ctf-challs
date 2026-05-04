#include <stdio.h>
#include <string.h>

void xor_str(char* str);

int main() {
    // char flag[] = "STOUTCTF{yd6A0XJphfKjPA1FlgFSauE9AO53pc8I}";
    char encrypted_flag[] = "LKPJK\\KYdf{)^/GUowyTuO^.YsxYL~jZ&^P*,o|'Vb";
    printf("We have the flag, but its encrypted: %s\n\n", encrypted_flag);
    printf("You can enter in anything to have the same encryption applied to it\n> ");
    char input[42];
    scanf("%s", &input);
    if (strcmp(input, encrypted_flag) == 0) {
        printf("\nYou cant enter that silly. That'd be too easy.");
        return 0;
    }
    xor_str(input);
    printf("%s", input);
    return 0;
}

void xor_str(char* str) {
    while (*str) {
        *str ^= 31;
        str++;
    }
}