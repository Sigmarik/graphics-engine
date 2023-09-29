#include "debug.h"

#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

void log_end_program() {
    log_printf(TERMINATE_REPORTS, "exit", "Program closed with errno = %d.\n",
               errno);
    log_close();
    if (errno == 0) return;
    printf("Error code: %d\n", errno);
    perror("Error");
}

#ifdef _DEBUG
bool check_ptr(const void* ptr) {
    if (ptr == NULL) return false;
    int mem_errno = errno;
    int fd = open("/dev/null", O_WRONLY);
    ssize_t result = write(fd, ptr, 1);
    close(fd);
    errno = mem_errno;
    return result != -1;
}
#else
bool check_ptr(const void* ptr) {
    SILENCE_UNUSED(ptr);
    return 1;
}
#endif

unsigned long long get_simple_hash(const void* start, const void* end) {
    unsigned long long hash = 0xDEADBABEDEAD;
    for (const char* ptr = (const char*)start; ptr < (const char*)end; ++ptr) {
        hash *= 0xC0FEBABEDEAD;
        hash += (unsigned char)*ptr;
    }
    return hash;
}