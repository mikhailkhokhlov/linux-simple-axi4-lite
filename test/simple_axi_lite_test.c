#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

#define CLED_PROC "/proc/simple_axi_lite"

int main(int argc, char* argv[]) {
    int ret = 0;
    const char value[] = "0x0F\n";

    int fd = open(CLED_PROC, O_RDWR);

    if (fd == -1) {
        perror("Error open");
        goto err;
    }

    ret = write(fd, value, strlen(value));
    if (ret == -1) {
       perror("Error write to file");
       goto err_close;
    }

err_close:
    close(fd);

err:
    return 0;
}
