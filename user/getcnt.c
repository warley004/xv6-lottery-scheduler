#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
    if (argc < 2) {
        fprintf(2, "Usage: getcnt <syscall_num>\n");
        exit(1);
    }

    int syscall_num = atoi(argv[1]);
    int count = getcnt(syscall_num);

    if (count < 0) {
        fprintf(2, "Error: Invalid syscall number\n");
        exit(1);
    }

    printf("syscall %d has been called %d times\n", syscall_num, count);
    exit(0);
}