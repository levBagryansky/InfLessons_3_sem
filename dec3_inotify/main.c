#include <stdio.h>
#include <stdlib.h>
#include <sys/inotify.h>
#include <unistd.h>
#include <linux/limits.h>
#include <sys/poll.h>

#define MIN_BUF_LEN (sizeof(struct inotify_event) + 3)

void PrintEvent(struct inotify_event *event){
	if (event->mask & IN_CREATE){
		printf("%s in create\n", event->name);
	}
	if (event->mask & IN_DELETE){
		printf("%s in delete\n", event->name);
	}

}

int main(int argc, char** argv) {
	if(argc != 2){
		printf("Wrong numbers of arguments\n");
		exit(1);
	}
	//struct inotify_event *event;
	char buf[sizeof(struct inotify_event) + PATH_MAX];
	int inotify_fd = inotify_init();
	inotify_add_watch(inotify_fd, argv[1], IN_CREATE | IN_DELETE);


	while (1) {
		int i = 1;
		while ((read(inotify_fd, (void *) buf, i * MIN_BUF_LEN)) <= 0) {
			i++;
		}
		PrintEvent((struct inotify_event*) buf);
	}
	return 0;
}
