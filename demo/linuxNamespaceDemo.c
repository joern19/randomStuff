#include <stdio.h>
#include <unistd.h>
#define __USE_GNU
#include <sched.h>
#include <signal.h>

void printUser() {
    uid_t realUid = getuid();
    uid_t effectiveUid = geteuid();

    printf("User id (uid): '%d'\n", effectiveUid);
}

void nothing() {}

void waitForSIG(int sig) {
    signal(sig, nothing);

    sigset_t set;

    sigemptyset(&set);
    sigaddset(&set, sig);
    sigprocmask(SIG_BLOCK, &set, NULL);
    sigwait(&set, NULL);
}

void waitForSIGUSR1() {
    signal(SIGUSR1, nothing);

    sigset_t set;
    int sig;

    sigemptyset(&set);
    sigaddset(&set, SIGUSR1);
    sigprocmask(SIG_BLOCK, &set, NULL);
    sigwait(&set, &sig);
}

int main() {
    // We need the CAP_SYS_ADMIN capability. This process can just create and join a namespace, where this process has that capability.
    //printUser(); // prints: User id (uid): '1000'
    unshare(CLONE_NEWUSER);
    //printUser(); // prints: User id (uid): '65534'

    printf("The program's pid is '%d'\n", getpid());
    unshare(CLONE_NEWPID);
    printf("The pid after the program entered a new pid namespace: '%d'\n", getpid());

    printf("-- forking --\n");
    fork(); // note: do not use fork. Use clone instead. Only using fork because it is simpler.
    pid_t pidA = getpid();
    printf("[1] My Pid is: '%d'\n", pidA);

    if (pidA == 1) {
        printf("-- forking --\n");
        fork();
        printf("[2] My Pid is: '%d'\n", getpid());
    }

    //    2433481
    //     |     \
    //  2433481   1
    //            | \
    //            1  2

    return 0;
}

int main_pid() {
    unshare(CLONE_NEWUSER | CLONE_NEWPID);

    printf("The program's pid is '%d'\n", getpid());
    int f = fork(); // clones this process. Fork pid will be 0 for the new process and for the old process, the pid of the new process will be returned
    printf("[%d] Result of fork: '%d'\n", getpid(), f);

    if (f == 0) { // only the process inside the namespace
        int forkInsideNamespace = fork(); // now there are 2 process inside the namespace
        printf("[%d] Result of fork inside namespace: '%d'\n", getpid(), forkInsideNamespace);
        waitForSIGUSR1();
        printf("[%d] Process finished\n", getpid());
        if (forkInsideNamespace != 0) {
            printf("[%d] Killing the process inside the namespace from within the namespace: '%d'\n", getpid(), forkInsideNamespace);
            kill(forkInsideNamespace, SIGUSR1);
        }
        return 0;
    } else {
        sleep(1);
        printf("-- Both process inside the namespace are now waiting for SIGUSR1 --\n");
        printf("[%d] Sending SIGUSR1 to the process, which has pid 1 (guess) inside the namespace but actually has pid '%d'\n", getpid(), f);
        kill(f, SIGUSR1);
    }

    sleep(1);
    return 0;
}
