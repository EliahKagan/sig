// Unix signals demonstration on a subprocess with its own subprocess.

#define _POSIX_C_SOURCE 1

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <sys/types.h>
#include <unistd.h>

enum interval {
    k_interval_status = 3,
    k_interval_before_signal = 10,
};

noreturn static int die(const char *prefix)
{
    perror(prefix);
    exit(EXIT_FAILURE);
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
        printf("%s process (PID %d) is running.\n", name, (int)pid);

        // A terminal would usually be line buffered, which is ideal because it
        // makes it likely a single write operation will be performed. Setting
        // line buffering with setvbuf may not always work, because it may only
        // be supported for terminals. So flush explicitly after each print.
        fflush(stdout);

        sleep(k_interval_status);
    }
}

int main(int argc, char **argv)
{
    int sig = parse_signal(argc, argv);
    printf("Testing with signal %d.\n", sig);
    fflush(stdout); // In case it's not a terminal. See status_loop.

    pid_t parent = getpid();
    pid_t child = fork();
    if (child < 0) die("Can't fork child");

    if (child > 0) {
        // This is the parent.
        printf("Parent (%d) sleeping %d s before sending signal %d"
               " to child (%d).\n",
               (int)parent, k_interval_before_signal, sig, (int)child);
        sleep(k_interval_before_signal);

        printf("Parent (%d) sending signal %d to child (%d).\n",
               (int)parent, sig, (int)child);
        if (kill(child, sig) != 0) die("Can't send signal to child");

        status_loop("Parent");
    } else {
        // This is the child.
        pid_t grandchild = fork();
        if (grandchild < 0) die("Can't fork grandchild");

        status_loop(grandchild > 0 ? "CHILD" : "Grandchild");
    }
}
