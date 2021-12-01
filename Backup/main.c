#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>
#include <string.h>

int buf_i;

char* Concatinate(char *part1, char *part2);

int GetFileSize(int fd);

int CopyFile(char *path_out, char *path_to);

int CopyDir(char *path_out, char *path_to){
	printf("CopyDir, path_out = %s\n", path_out);
	mkdir(path_to, 0777);
	DIR* pdir = opendir(path_out);
	printf("pdir = %p\n", pdir);
	struct dirent* dt;
	while ((dt = readdir(pdir)) != NULL){
		char *new_path_to = Concatinate(path_to, dt->d_name);
		if(new_path_to == NULL){
			return -1;
		}
		if(dt->d_type == 4) { // dt директория
			if(dt->d_name[0] != '.'){
				char *new_path_from = Concatinate(path_out, dt->d_name);
				CopyDir(new_path_from, new_path_to);
				free(new_path_from);
			}
		} else{ // беспонтовый файл
			char *new_path_from = Concatinate(path_out, dt->d_name);
			CopyFile(new_path_from, new_path_to);
			free(new_path_from);
		}

		free(new_path_to);
	}
	return 0;
}

int main(int argc, char ** argv) {
	if (argc != 3){
		printf("Wrong number of argument.\n");
		exit(EXIT_FAILURE);
	}
	char s1[10] = "Hello";
	char s2[10] = "World";

	char * summ = Concatinate(s1, s2);
	printf("summ = %s\n", summ);
	free(summ);

	CopyDir(argv[1], argv[2]);

	return 0;
}

char* Concatinate(char *part1, char *part2){
	char* result = (char *) calloc(256, sizeof (char));
	buf_c = part1[0];
	buf_i = 0;
	while (part1[buf_i] != 0){
		result[buf_i] = part1[buf_i];
		buf_i++;
		if(buf_i == 256){
			free(result);
			return NULL;
		}
	}
	result[buf_i] = '/';
	buf_i++;
	if(buf_i == 256){
		free(result);
		return NULL;
	}
	int len_part_1 = buf_i;
	while (part2[buf_i - len_part_1] != 0){
		result[buf_i] = part2[buf_i - len_part_1];
		buf_i++;
		if(buf_i == 256){
			free(result);
			return NULL;
		}
	}
	//result[buf_i] = '/';
	return result;
}

int GetFileSize(int fd){
	if(fd == -1)
		return -1;

	struct stat buf;
	fstat(fd, &buf);
	off_t file_size = buf.st_size;
	return file_size;
}

int CopyFile(char *path_out, char *path_to){
	printf("CopyFile\n");
	int fd_in = open(path_out, O_RDONLY);
	int fd_out = open(path_to, O_RDWR | O_CREAT | O_TRUNC, 0666);
	if(fd_out == -1){
		return -1;
	}
	int size = GetFileSize(fd_in);
	char* src = mmap(0, size, PROT_READ, MAP_SHARED, fd_in, 0);
	if(src == NULL){
		return -1;
	}
	lseek(fd_out, size - 1, SEEK_SET);
	write(fd_out, "", 1);
	char* dst = mmap(0, size, PROT_WRITE | PROT_READ, MAP_SHARED, fd_out, 0);
	if(dst == NULL){
		return -1;
	}
	memcpy(dst, src, size);
	return 0;
}
