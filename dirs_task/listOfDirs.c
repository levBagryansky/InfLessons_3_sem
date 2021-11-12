#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>

int main(int argc, char ** argv){
    if(argc != 2)
    {
        printf("Wrong format\n");
        exit(1);
    }

    DIR* pdir = opendir(argv[1]);
    if(pdir == 0){
        printf("Can't open directory");
        exit(1);
    }

    struct dirent* dt;
    int i = 0;
    while ((dt = readdir(pdir)) != NULL){
        if(dt->d_type == 4) {
            printf("%i) %s\n", i, dt->d_name);
            i++;
        }
    }
}
