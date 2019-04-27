#include <stdio.h>
#include <stdlib.h>

int main(int argc, char* argv[]) {
    FILE *fp = fopen("task_list.txt", "w");

    if (fp == NULL) {
        printf("Error: Failed to open file.");
        return 1;
    }

    for (int i = 1, n; i <= 20; i++)
        fprintf(fp, "%d\n", rand() % 5 + 1);

    fclose(fp);

    return 0;
}