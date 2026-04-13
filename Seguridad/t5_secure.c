#include <stdio.h>
#include <unistd.h>

int main() {
    char src[100], dst[100];

    printf("Source file: ");
    scanf("%s", src);

    printf("Destination file: ");
    scanf("%s", dst);

    char *args[] = {"cp", src, dst, NULL};

    execvp("cp", args); // Seguro

    return 0;

}