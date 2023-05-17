/*  test write data to fifo has more than PIPE_BUF bytes  */

#include<unistd.h>
#include<fcntl.h>
#include<sys/stat.h>
#include<sys/types.h>
#include<sys/wait.h>
#include<signal.h>
#include<stdio.h>
#include<stdlib.h>
#include<limits.h>

char buff[PIPE_BUF + 4];

const char * const fifoname = "/tmp/test_fifo.fifo128";

int main(void)
{
	int parentfd = -1;
	int childfd = -1;
	int datafromfd = -1;
	int invokeret = -1;
	pid_t childpid = 0;

	if (mkfifo(fifoname, S_IRUSR | S_IWUSR) < 0) {
		fprintf(stderr, "failed to mkfifo,try to delete it first\n");
		exit(-1);
	}

	if ((childpid = fork()) == 0) {
		childfd = open(fifoname, O_RDONLY);
		if (childfd < 0) {
			fprintf(stderr, "child : failed to open fifo\n");
			exit(-13);
		}
		else
			pause();

	} else if (childpid < 0) {
		fprintf(stderr, "main : failed to fork a child process\n");
		exit(-2);
	} else {
		parentfd = open(fifoname, O_WRONLY);
		datafromfd = open("/dev/zero", O_RDONLY);
		read(datafromfd, buff, PIPE_BUF + 4);
		invokeret = write(parentfd, buff, PIPE_BUF + 4 );
	invokeret_checking:
		if (invokeret < 0) {
			fprintf(stderr, "parent : write fifo failed\n");
			goto main_exit;
		}
		fprintf(stderr, "parent : written %d \n", invokeret);
		
		invokeret = write(parentfd, buff, 256);
		if (invokeret < 0)
			goto invokeret_checking;
		fprintf(stderr, "parent : written %d \n", invokeret);
	}

main_exit:
	kill(childpid, SIGTERM);
	wait(NULL);
	close(datafromfd);
	close(parentfd);

	unlink(fifoname);

	return 0;
} 
