// SPDX-License-Identifier: 0BSD
// Unix signals demonstration on a subprocess with its own subprocess.

#define _POSIX_C_SOURCE 1

#include <signal.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <sys/types.h>
#include <unistd.h>

enum interval {
    // Offset to improve readability and make ordering more stable.
    k_interval_offset = 1,

    // How often processes with nothing else planned print their status.
    k_interval_status = 3,

    // How long the parent waits before sending a signal to the child.
    k_interval_before_signal = 10,
};

noreturn static int die(const char *prefix)
{
    perror(prefix);
    exit(EXIT_FAILURE);
}

static void lprintf(const char *restrict format, ...)
{
    va_list args;
    va_start(args, format);
    vprintf(format, args);
    va_end(args);

    // A terminal would usually be line buffered, which is ideal because, for
    // short messages, a single write operation is usually performed. This
    // makes it less likely for each individual message to appear whole, rather
    // than having output of another process mixed in. When stdout isn't a
    // terminal, it is usually fully buffered. Setting line buffering with
    // setvbuf may not work, as non-terminals need not support it. So flush
    // explicitly after each print. (Actual synchronization would complicate
    // and obscure the demonstration.)
    fflush(stdout);
}

static int parse_signal(int argc, char **argv)
{
    if (argc > 2) abort();

    if (argc < 2) return SIGTERM; // 15

    int sig = 0;
    if (sscanf(argv[1], "%d", &sig) != 1)
        abort();

    return sig;
}

noreturn static void status_loop(const char *name)
{
    pid_t pid = getpid();

    for (; ; ) {
        lprintf("%s process (PID %d) is running.\n", name, (int)pid);
        sleep(k_interval_status);
    }
}

int main(int argc, char **argv)
{
    int sig = parse_signal(argc, argv);
    lprintf("Testing with signal %d.\n", sig);

    pid_t parent = getpid();
    pid_t child = fork();
    if (child < 0) die("Can't fork child");

    if (child > 0) {
        // This is the parent.
        lprintf("Parent (%d) sleeping %d s before sending signal %d"
                " to child (%d).\n",
                (int)parent, k_interval_before_signal, sig, (int)child);
        sleep(k_interval_before_signal);

        lprintf("Parent (%d) sending signal %d to child (%d).\n",
                (int)parent, sig, (int)child);
        if (kill(child, sig) != 0) die("Can't send signal to child");

        status_loop("Parent");
    } else {
        // This is the child.
        pid_t grandchild = fork();
        if (grandchild < 0) die("Can't fork grandchild");

        if (grandchild > 0) {
            // This is (still) the child.
            sleep(k_interval_offset);
            status_loop("CHILD");
        } else {
            // This is the grandchild.
            sleep(k_interval_offset * 2);
            status_loop("Grandchild");
        }
    }
}
