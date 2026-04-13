#include<stdio.h>
#include<stdlib.h>
#include<string.h>

int main(int argc, char *argv[]) {
    char src[100], cmd[205] = "cat ";
    printf("Please enter name file: ");
    fgets(src, 100, stdin);
    src[strcspn(src, "\n")] = 0; 
    strcat(cmd, src);
    system(cmd);
}