#include "guid.h"

#include <stdio.h>

#include "generation/noise.h"

GUID GUID::gen() {
    return (GUID){
        rand_uint64(),
        rand_uint64(),
    };
}
