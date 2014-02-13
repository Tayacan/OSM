#include "kernel/io.h"
#include "kernel/assert.h"
#include "drivers/gcd.h"
#include "drivers/device.h"
#include "proc/syscall.h"
#include <stddef.h>

gcd_t* get_gcd() {
    device_t *dev;
    gcd_t *gcd;

    dev = device_get(YAMS_TYPECODE_TTY, 0);
    KERNEL_ASSERT(dev != NULL);

    gcd = (gcd_t *)dev->generic_device;
    KERNEL_ASSERT(gcd != NULL);

    return gcd;
}

int read(int fhandle, void *buffer, int length) {
    int read_bytes;
    
    if(fhandle == FILEHANDLE_STDIN && length > 0) {
        gcd_t *gcd = get_gcd();
        read_bytes = gcd->read(gcd, buffer, length);
        return read_bytes;
    }

    // TODO - other filehandles
    return 0;
}

int write(int fhandle, const void *buffer, int length) {
    int written_bytes;

    // Write to stdout
    if(fhandle == FILEHANDLE_STDOUT && length > 0) {
        gcd_t *gcd = get_gcd();
        written_bytes = gcd->write(gcd, buffer, length);
        return written_bytes;
    }

    // TODO - other filehandles
    return 0;
}
