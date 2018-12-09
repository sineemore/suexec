#include <sys/types.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <pwd.h>
#include <assert.h>
#include <grp.h>

extern char **environ;

static void die(const char *msg) {
	fputs(msg, stderr);
	if (msg[0] && msg[strlen(msg)-1] == ':') {
		fputc(' ', stderr);
		perror(NULL);
	} else {
		fputc('\n', stderr);
	}
	exit(1);
}

int main(int argc, char *argv[]) {
	
	close(0);
	environ = NULL;

	if (argc < 3)
		die("usage: suexec <username> <command>");

	struct passwd *pw = getpwnam(argv[1]);
	if (pw == NULL)
		die(errno ? "getpwname:" : "specified user not found");
	
	if (0 != getuid())
		die("must be root");

	if (0 != initgroups(pw->pw_name, pw->pw_gid))
		die("initgroups:");
	
	if (0 != setgid(pw->pw_gid))
		die("setgid:");

	if (0 != setuid(pw->pw_uid))
		die("setuid:");
	
	if (-1 != setuid(0))
		die("managed to regain root privileges");
	errno = 0;
	
	setenv("HOME", pw->pw_dir, 1);
	setenv("USER", pw->pw_name, 1);
	setenv("LOGNAME", pw->pw_name, 1);
	setenv("SHELL", pw->pw_shell, 1);
	if (errno != 0)
		die("setenv:");
	
	// Skip argv0 and username
	argv += 2;
	
	execvp(argv[0], argv);
	die("execvp:");
}
