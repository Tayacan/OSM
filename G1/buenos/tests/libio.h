#ifndef LIB_IO
#define LIB_IO

#define true 1
#define false 0

typedef int bool;

/* Read one line from stdin (at most length bytes). If length bytes
 * are typed, stop reading.
 *
 * Return the number of bytes read.
 * */
int readline(char *buffer, int length);

/* Like readline, but writes prompt to stdout first. */
int prompt(char *prompt, char *buffer, int length);

/* Write a string to stdout. It is the caller's resposibility
 * to ensure that the string ends in a 0-byte. */
int write(char *buffer);

int strlength(char *buffer);
bool strcmp(char *b1, char *b2);

#endif
