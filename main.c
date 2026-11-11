#include <stdio.h>
#include <string.h>

int main(void)
{
    char text[32] = "hallo wereld";

    printf("Strlen: %zu\n", strlen(text) + 1);

    memmove(text + 6, text + 5, strlen(text + 5) + 1);
    text[5] = ',';

    printf("%p\n", text);
    printf("%p\n", text + 6);

    printf("%s\n", text);
    return 0;
}