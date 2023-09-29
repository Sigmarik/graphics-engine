#include "common_utils.h"

#include <fcntl.h>
#include <stdio.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>

#include "logger/logger.h"

MmapResult map_file(const char* name, int oflag, int prot, int privacy) {
    int fd = open(name, oflag);
    log_printf(STATUS_REPORTS, "status", "Mapping file %s with descriptor %d\n",
               name, fd);
    if (fd < 0) {
        log_dup(ERROR_REPORTS, "error", "Failed to open file: %s\n", name);
        close(fd);
        return {};
    }
    struct stat st = {};
    if (fstat(fd, &st) < 0) {
        log_dup(ERROR_REPORTS, "error", "Failed to stat file: %s\n", name);
        close(fd);
        return {};
    }
    void* map = mmap(NULL, (size_t)st.st_size, prot, privacy, fd, 0);
    if (map == MAP_FAILED) {
        log_dup(ERROR_REPORTS, "error", "Failed to mmap file: %s\n", name);
        close(fd);
        return {};
    }
    return (MmapResult){.ptr = map, .fd = fd, .size = (size_t)st.st_size};
}
