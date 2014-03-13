/*
 * OSM shell.
 */

#include "tests/lib.h"

#define BUFFER_SIZE 100
#define PATH_LENGTH 256

char DISK[PATH_LENGTH];

typedef int (*cmd_fun_t)(int, char**);

typedef struct {
  char* name;
  cmd_fun_t func;
  char *desc;
} cmd_t;

int cmd_echo(int, char**);
int cmd_show(int, char**);
int cmd_read(int, char**);
int cmd_rm(int, char**);
int cmd_cp(int, char**);
int cmd_cmp(int, char**);
int cmd_size(int, char**);
int cmd_ls(int, char**);
int cmd_fnum();
int cmd_help();
int cmd_exit();

cmd_t commands[] =
  { {"echo", cmd_echo, "print the arguments to the screen"},
    {"show", cmd_show, "print the contents of the given file to the screen"},
    {"read", cmd_read, "read a line from standard in and write it to a new file"},
    {"rm"  , cmd_rm  , "remove one or more files"},
    {"cp"  , cmd_cp  , "copy a file"},
    {"cmp" , cmd_cmp , "compare two files"},
    {"size", cmd_size, "the size of the given file in bytes"},
    {"fnum", cmd_fnum, "number of files on the  harddisk"},
    {"ls"  , cmd_ls  , "run ls -h for full description"},
    {"help", cmd_help, "show this help message"},
    {"exit", cmd_exit, "exit the shell"}
  };
#define N_COMMANDS sizeof(commands) / sizeof(cmd_t)

void fix_filename(char *dest, const char *filename)
{
    if(filename[0] == '[')
    {
        strcpy(dest, filename);
        return;
    }

    strcpy(dest, DISK);
    strcat(dest, filename);
}

void print_prompt(void) {
  printf("unicorn@%s $ ", DISK);
}

/* Note that tokenize(cmdline, argv) modifies cmdline by inserting NUL
   characters. */
int tokenize(char* cmdline, char** argv) {
  int argc = 0;
  int inword=0;
  char *s, *p;
  for (s = cmdline, p = cmdline; *s; s++) {
    if (*s == ' ' && inword) {
      inword=0;
      argv[argc++]=p;
      *s = '\0';
    } else if (*s != ' ' && !inword) {
      inword=1;
      p=s;
    }
  }
  if (inword) {
    argv[argc++]=p;
  }
  return argc;
}

int run_program(char* prog) {
  char prog_name[PATH_LENGTH];
  fix_filename(prog_name, prog);
  int fd = syscall_open(prog_name);
  if(fd < 0)
  {
    printf("Unknown command: %s\n", prog_name);
    return fd;
  }
  syscall_close(fd);
  return syscall_join(syscall_exec(prog_name));
}

int run_command(char* cmdline) {
  char* argv[BUFFER_SIZE];
  int argc = tokenize(cmdline, argv);
  unsigned int i;
  if (argc == 0) {
    return 0;
  }
  for (i = 0; i < N_COMMANDS; i++) {
    if (strcmp(argv[0], commands[i].name) == 0) {
      return commands[i].func(argc, argv);
    }
  }
  return run_program(cmdline);
}

void help() {
  printf("The following commands are available:\n");
  unsigned int i;
  for (i = 0; i < N_COMMANDS; i++) {
    printf("  %s: %s\n", commands[i].name, commands[i].desc);
  }
}

void welcome(void)
{
  char *default_colors = "\033[0m";
  char *unicorn = "\033[35m";
  printf(default_colors);
  printf("Welcome to the %sUnicorn%s Shell\n", unicorn, default_colors);
  printf(unicorn);
  run_command("show unicorn.txt");
  printf(default_colors);
  printf("Type 'help' for a list of commands\n");
}

void set_default_disk(void)
{
  char buf[PATH_LENGTH];
  syscall_file(NULL, 0, buf);
  snprintf(DISK, PATH_LENGTH, "[%s]", buf);
}

int main(void) {
  char cmdline[BUFFER_SIZE];
  set_default_disk();
  welcome();
  while (1) {
    print_prompt();
    readline(cmdline, BUFFER_SIZE);
    run_command(cmdline);
  }
  syscall_halt();
  return 0;
}

void ls(char *vname)
{
    int i, j, ret;
    int n = syscall_filecount(vname);
    char buffer[PATH_LENGTH];
    for(i = 0, j = 0; i < n;)
    {
        ret = syscall_file(vname, i+j, buffer);
        if(ret < 0)
        {
            j++;
        }
        else
        {
            i++;
            printf("%s\n", buffer);
        }
    }
}

int cmd_ls(int argc, char** argv)
{
    int i;
    for(i = 1; i < argc; i++)
    {
        if(strcmp(argv[i], "-h") == 0)
        {
            printf("Usage: ls                         - output the names of all files on current file system (%s)\n", DISK);
            printf("       ls <volume1> <volume2> ... - output the names of all files each <volumen>\n");
            printf("       ls -all                    - output the names of all files on all mounted volumes\n");
            printf("       ls -fs                     - output the names of all mounted volumes\n");
            printf("       ls -h                      - output this help message\n");
        } else if(strcmp(argv[i], "-fs") == 0)
        {
            ls(NULL);
            return 0;
        } else if(strcmp(argv[i], "-all") == 0)
        {
            int j, k, ret;
            char buffer[PATH_LENGTH];
            int n = syscall_filecount(NULL);
            for(j = 0, k = 0; j < n;)
            {
                ret = syscall_file(NULL, j+k, buffer);
                if(ret < 0)
                {
                    k++;
                }
                else
                {
                    printf("[%s]:\n", buffer);
                    ls(buffer);
                    j++;
                }
            }
            return 0;
        } else
        {
            printf("%s:\n", argv[i]);
            ls(argv[i]);
        }
    }

    if(argc == 1) {
        ls(DISK);
    }
    return 0;
}

int cmd_fnum()
{
    printf("%d\n", syscall_filecount(DISK));
    return 0;
}

int cmd_size(int argc, char** argv) {
    char filename[PATH_LENGTH];

    if(argc != 2) {
        printf("Usage: size <file>\n");
        return 1;
    }

    fix_filename(filename, argv[1]);
    int ret = syscall_filesize(filename);
    if(ret < 0) {
        printf("Failed with reason %d\n", ret);
        return 1;
    }

    printf("%d\n", ret);

    return 0;
}

int cmd_echo(int argc, char** argv) {
  int i;
  for (i = 1; i < argc; i++) {
    printf("%s ", argv[i]);
  }
  puts("\n");
  return 0;
}

int cmd_show(int argc, char** argv) {
  if (argc != 2) {
    printf("Usage: show <file>\n");
    return 1;
  }

  char filename[PATH_LENGTH];
  fix_filename(filename, argv[1]);

  int fd;
  if ((fd=syscall_open(filename)) < 0) {
    printf("Could not open %s.  Reason: %d\n", filename, fd);
    return 1;
  }

  int rd;
  char buffer[BUFFER_SIZE];
  while ((rd = syscall_read(fd, buffer, BUFFER_SIZE))) {
    int wr=0, thiswr;
    while (wr < rd) {
      if ((thiswr = syscall_write(1, buffer+wr, rd-wr)) <= 0) {
        printf("\nCall to syscall_write() failed.  Reason: %d.\n", wr);
        syscall_close(fd);
        return 1;
      }
      wr += thiswr;
    }
  }
  if (rd < 0) {
    printf("\nCall to syscall_read() failed.  Reason: %d.\n", rd);
    syscall_close(fd);
    return 1;
  } else {
    syscall_close(fd);
    return 0;
  }
}

int cmd_read(int argc, char** argv) {
  if (argc < 2) {
    printf("Usage: read <file>\n");
    return 1;
  }

  char filename[PATH_LENGTH];
  char text[BUFFER_SIZE];
  int count, ret, fd, wr;
  fix_filename(filename, argv[1]);
  count = readline(text, BUFFER_SIZE - 1) + 1;
  text[count - 1] = '\n';
  text[count] = '\0';
  if ((ret=syscall_create(filename, count)) < 0) {
    printf("Could not create %s with initial size %d.  Reason: %d\n", filename, count, ret);
    return 1;
  }
  if ((fd=syscall_open(filename)) < 0) {
    printf("Could not open %s.  Reason: %d\n", filename, fd);
    return 1;
  }
  if ((wr=syscall_write(fd, text, count)) <= 0) {
    printf("\nCall to syscall_write() failed.  Reason: %d.\n", wr);
    syscall_close(fd);
    return 1;
  }
  syscall_close(fd);
  return 0;
}

int rm(char *path)
{
    int ret = syscall_delete(path);
    if(ret < 0)
    {
        printf("cannot remove file '%s': ", path);
        switch(ret)
        {
        case -5:
            printf("file not found.\n");
            break;
        default:
            printf("unknown error: %d.\n", ret);
        }
    }

    return ret;
}

int cmd_rm(int argc, char** argv)
{
    int i;
    char filename[PATH_LENGTH];

    if(argc < 2)
    {
        printf("Usage: rm <file1> <file2> ... <filen>\n");
    }

    for(i = 1; i < argc; i++)
    {
        fix_filename(filename, argv[i]);
        rm(filename);
    }
    return 0;
}

int cmd_exit()
{
    syscall_exit(0);
    return 0;
}

int cmd_cp(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: cp <source> <dest>\n");
    return 1;
  }
  int fd_src, fd_dest;
  char filename0[PATH_LENGTH];
  char filename1[PATH_LENGTH];
  fix_filename(filename0, argv[1]);
  fix_filename(filename1, argv[2]);
  if ((fd_src=syscall_open(filename0)) < 0) {
    printf("Could not open %s.  Reason: %d\n", filename0, fd_src);
    return 1;
  }
  if ((fd_dest=syscall_open(filename1)) < 0) {
    if(syscall_create(filename1, BUFFER_SIZE) < 0) {
      printf("Could not create %s.\n", filename1);
      syscall_close(fd_src);
      return 1;
    };

    if ((fd_dest=syscall_open(filename1)) < 0) {
      printf("I give up now!\n");
      syscall_close(fd_src);
      return 1;
    }
  }

  int rd;
  char buffer[BUFFER_SIZE];
  while ((rd = syscall_read(fd_src, buffer, BUFFER_SIZE))) {
    int wr=0, thiswr;
    while (wr < rd) {
      if ((thiswr = syscall_write(fd_dest, buffer+wr, rd-wr)) <= 0) {
        printf("\nCall to syscall_write() failed.  Reason: %d.\n", wr);
        syscall_close(fd_src);
        syscall_close(fd_dest);
        return 1;
      }
      wr += thiswr;
    }
  }
  if (rd < 0) {
    printf("\nCall to syscall_read() failed.  Reason: %d.\n", rd);
    syscall_close(fd_src);
    syscall_close(fd_dest);
    return 1;
  } else {
    syscall_close(fd_src);
    syscall_close(fd_dest);
    return 0;
  }
}

int cmd_cmp(int argc, char** argv) {
  if (argc != 3) {
    printf("Usage: cmp <source> <dest>\n");
    return 1;
  }
  int fd_0, fd_1;
  char filename0[PATH_LENGTH];
  char filename1[PATH_LENGTH];
  fix_filename(filename0, argv[1]);
  fix_filename(filename1, argv[2]);
  if ((fd_0=syscall_open(filename0)) < 0) {
    printf("Could not open %s.  Reason: %d\n", filename0, fd_0);
    return 1;
  }
  if ((fd_1=syscall_open(filename1)) < 0) {
    printf("Could not open %s.  Reason: %d\n", filename1, fd_1);
    syscall_close(fd_0);
    return 1;
  }

  int rd0, rd1;
  char buffer_0[BUFFER_SIZE], buffer_1[BUFFER_SIZE];
  while (1) {
    rd0 = syscall_read(fd_0, buffer_0, BUFFER_SIZE);
    rd1 = syscall_read(fd_1, buffer_1, BUFFER_SIZE);

    if(!(rd0 || rd1)) break;

    if(rd0 < 0 || rd1 < 0)
    {
        printf("Failed with reasons: %s: %d, %s: %d\n", filename0, rd0, filename1, rd1);
        syscall_close(fd_0);
        syscall_close(fd_1);
        return 1;
    }
    if(rd0 != rd1 || strncmp(buffer_0, buffer_1, BUFFER_SIZE) != 0)
    {
        printf("Files are not identical\n");
        syscall_close(fd_0);
        syscall_close(fd_1);
        return 0;
    }
  }
  printf("Files are identical\n");
  syscall_close(fd_0);
  syscall_close(fd_1);
  return 0;
}

int cmd_help() {
  help();
  return 0;
}
