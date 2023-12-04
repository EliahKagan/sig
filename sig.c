#define _POSIX_C_SOURCE 1

#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdnoreturn.h>
#include <unistd.h>
#include <sys/types.h>

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
        sleep(k_interval_status);
    }
}

int main(int argc, char **argv)
{
    // setbuf(stdout, NULL);

    int sig = parse_signal(argc, argv);
    printf("Testing with signal %d.\n", sig);

    pid_t child = fork();
    if (child < 0) die("can't fork child");

    if (child > 0) {
        // This is the parent.
        sleep(k_interval_before_signal);
        kill(child, sig);
        status_loop("Parent");
    } else {
        // This is the child.
        pid_t grandchild = fork();
        if (grandchild < 0) die("can't fork grandchild");

        status_loop(grandchild > 0 ? "Child" : "Grandchild");
    }
}