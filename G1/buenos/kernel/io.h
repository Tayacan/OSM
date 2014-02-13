#ifndef IO
#define IO

/* Read at most length bytes from fhandle (currently only 0),
 * and store them in buffer. */
int read(int fhandle, void *buffer, int length);

/* Write at most length bytes from buffer to fhandle (currently
 * only 0). */
int write(int fhandle, const void *buffer, int length);

#endif
