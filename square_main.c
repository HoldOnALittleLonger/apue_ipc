#include<rpc/rpc.h>
#include<stdio.h>
#include<stdlib.h>
#include"square.h"

int main(int argc, char *argv[])
{
	CLIENT *cl;
	square_in in;
	square_out *outp;

	if (argc != 3) {
		fprintf(stderr, "usage: client <hostname> <integer-value>\n");
		return 1;
	}

	cl = clnt_create(argv[1], SQUARE_PROG, SQUARE_VERS, "tcp");

	in.arg1 = atol(argv[2]);
	
	if ((outp = squareproc_1(&in, cl)) == NULL) {
		fprintf(stderr, "rpc error\n");
		return 2;
	}

	printf("result: %ld\n", outp->res1);
	return 0;
}
