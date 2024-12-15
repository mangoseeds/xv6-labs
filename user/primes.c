// concurrent prime sieve program

#include "kernel/types.h"
#include "user/user.h"

void primes(int read_fd) __attribute__((noreturn));

void primes(int read_fd) {
    int prime;

    // read first number from pipe
    if (read(read_fd, &prime, sizeof(prime)) == 0) {
        close(read_fd);
        exit(0);
    }

    printf("prime %d\n", prime);

    int p[2];
    pipe(p);

    if (fork() == 0) { // child process : filters the next prime

        close(p[1]); // close write end in child
        close(read_fd);
        primes(p[0]);
        exit(0);

    } else { // parent process : filters numbers
    
        close(p[0]); // close read end in parent

        int num;
        while (read(read_fd, &num, sizeof(num)) > 0) {
            if (num % prime != 0) {
                write(p[1], &num, sizeof(num));
            }
        }

        close(read_fd); // close input pipe
        close(p[1]);    // close output pipe

        wait(0); // wait for child process
        exit(0);

    }
}

int main() {
    int p[2];
    pipe(p);

    if (fork() == 0) { // child process : starts the prime pipeline
        
        close(p[1]); // close write end
        primes(p[0]);

    } else { // parent process : feeds numbers into the pipeline
        
        close(p[0]); // close read end in parent

        for (int i = 2; i <= 280; i++) {
            write(p[1], &i, sizeof(i));
        }

        close(p[1]); // close write end after writing all numbers
        wait(0);     // wait for all child process
        exit(0);
        
    }
}