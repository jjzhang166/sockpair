#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>          
#include <sys/socket.h>

int main()
{
	int sv[2]={0,0};
	int sock=socketpair(AF_LOCAL,SOCK_STREAM,0,sv);

	if(sock<0)
	{
		perror("socketpair");
		exit(0);
	}

	pid_t id=fork();
	char buf[1024];
	if(id<0)
	{
		perror("fork");
		exit(0);
	}
	else if(id==0)
	{
		close(sv[0]);  //子进程关闭读端
		while(1)
		{
			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"i am your child");
			write(sv[1],buf,strlen(buf));//子进程写入
			memset(buf,'\0',sizeof(buf));
			ssize_t _s=read(sv[1],buf,sizeof(buf)-1);//子进程读取父进程的内容
			buf[_s]='\0';
			printf("father-->child:%s\n",buf);
			sleep(1);
		}
		close(sv[1]);//子进程关闭读端
	}
	else
	{//父进程
		close(sv[1]);
		while(1)
		{
			memset(buf,'\0',sizeof(buf));
			ssize_t _s=read(sv[0],buf,sizeof(buf)-1);
			buf[_s]='\0';
			printf("child-->father:%s\n",buf);
			memset(buf,'\0',sizeof(buf));
			strcpy(buf,"i am your father");
			write(sv[0],buf,strlen(buf));
			sleep(1);
		}
		close(sv[0]);
	}
	return 0;
}
