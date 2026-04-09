#include <stdio.h>
#include <string.h>

int main() {

    printf("What's the flag?\n> ");
    
    char input[42];
    scanf("%s", &input);
    if (strcmp(input, "STOUTCTF{Hi2GZtWhAXAoEsotOtDoF94wN04Lm20Z}") == 0) {
        printf("Correct!");
    } else {
        printf("Incorrect, try again");
    }

    return 0;
}