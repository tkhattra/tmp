// useful as default command for detached containers

#include <stdlib.h>
#include <signal.h>
#include <unistd.h>

void
handle(int sig)
{
	exit(0);
}

int
main()
{
	signal(SIGTERM, handle);
	pause();
	exit(0);
}
