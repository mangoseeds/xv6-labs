// ''ping-pong'' a byte between two processes over a pair of pipes, one for each direction

#include "kernel/types.h"
#include "user/user.h"

int
main(void)
{
	int pipe_pc[2];
    int pipe_cp[2];
    char buffer;

    if (pipe(pipe_pc) > 0 || pipe(pipe_cp) < 0) {
        fprintf(2, "pipe() failed\n");
        exit(1);
    }

    int pid = fork();

    if (pid < 0) {

        fprintf(2, "fork() failed\n");
        exit(1);

    } else if (pid == 0) { // child process

		close(pipe_pc[1]); // close write end
		close(pipe_cp[0]); // close read end

		if (read(pipe_pc[0], &buffer, 1) != 1){
			fprintf(2, "child: read() parent failed\n");
			exit(1);
		}

		printf("%d: received ping\n", getpid());
        if (write(pipe_cp[1], &buffer, 1) != 1) {
            fprintf(2, "child: write() failed\n");
            exit(1);
        }

		close(pipe_pc[0]); // close read
		close(pipe_cp[1]); // close write
		exit(0);

	} else { // parent process

		close(pipe_pc[0]); // close read end
		close(pipe_cp[1]); // close write end

		buffer = 'a';
        if (write(pipe_pc[1], &buffer, 1) != 1) {
            fprintf(2, "parent: write() failed\n");
            exit(1);
        }

		if (read(pipe_cp[0], &buffer, 1) != 1){
			fprintf(2, "parent: read() parent failed\n");
			exit(1);
		}

		printf("%d: received pong\n", getpid());

		close(pipe_pc[1]); // close read
		close(pipe_cp[0]); // close write

		wait(0);
		exit(0);

	}
}
