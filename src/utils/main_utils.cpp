#include "main_utils.h"

#include <stdlib.h>

#include "lib/logger/logger.h"
#include "src/config.h"

void print_label() {
    puts(PROGRAM_NAME);
    printf("Build from\n%s %s\n", __DATE__, __TIME__);
    log_printf(ABSOLUTE_IMPORTANCE, "build info", "Build from %s %s.\n",
               __DATE__, __TIME__);
}
