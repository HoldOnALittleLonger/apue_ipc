#include<sys/types.h>
#include<sys/ipc.h>
#include<sys/msg.h>
#include<fcntl.h>
#include<unistd.h>
#include<stdio.h>


static inline key_t make_key(void)
{
	return ftok("/home/sagiri/nvidia-env", 32); 
}

int main(void)
{
	int msgidentifier = 0;
	struct msqid_ds msqidds;
	msgidentifier = msgget(make_key(),
			       IPC_CREAT | S_IRUSR | S_IWUSR | S_IRGRP);
	if (msgidentifier < 0)
		fprintf(stderr, "failed to create SystemV IPC msg.\n");
	else {
		msgctl(msgidentifier, IPC_STAT, &msqidds);
		printf("key = %d, "
		       "identifier = %d\n",
		       make_key(), msgidentifier);
		printf("ipc_perm.key = %d\n"
		       "ipc_perm.uid = %u\n"
		       "ipc_perm.gid = %u\n"
		       "ipc_perm.cuid = %u\n"
		       "ipc_perm.cgid = %u\n"
		       "ipc_perm.mode = %o\n"
		       "ipc_perm.seq = %u\n",
		       msqidds.msg_perm.__key,
		       msqidds.msg_perm.uid,
		       msqidds.msg_perm.gid,
		       msqidds.msg_perm.cuid,
		       msqidds.msg_perm.cgid,
		       msqidds.msg_perm.mode,
		       msqidds.msg_perm.__seq);
	}

	pause();
	return 0;
}
