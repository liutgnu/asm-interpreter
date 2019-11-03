#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

typedef enum {false = 0, true} bool;

extern double calc_num(const char *);
extern void init_regs(void);
int lines = 0;
char **program;

int parse_file(const char *file_name)
{
	int fd = 0, index = 0;
	struct stat file_stat;
	char *file_cache = NULL;
	bool in_quot = false;
	char *file_scan_start, *file_scan_end;

	/*read file in cache*/
	fd = open(file_name, O_RDONLY);
	if (fd < 0) {
		printf("open file error!\n");
		goto out;
	}
	fstat(fd, &file_stat);
	file_cache = malloc(file_stat.st_size + 1);
	if (!file_cache) {
		printf("malloc cache error!\n");
		goto out;
	}
	memset(file_cache, 0, file_stat.st_size + 1);
	read(fd, file_cache, file_stat.st_size);
	
	/*prepare to parse file, first we get the number of lines*/
	for (file_scan_end = file_cache; file_scan_end <= file_cache + 
		file_stat.st_size; file_scan_end++) {
		if (*file_scan_end == '"') {
			in_quot = !in_quot;
		}
		if (*file_scan_end == '\n' && !in_quot) {
			lines++;
		}
	}
	if(in_quot) {
		printf("file error, \" unpair\n");
		return -1;
	}

	/*store address of each line of instruction string in array*/
	program = malloc(sizeof(char *) * lines);
	if (!program) {
		printf("malloc program error!\n");
		goto out;
	}
	memset(program, 0, sizeof(char *) * lines);

	in_quot = false;
	file_scan_start = file_scan_end = file_cache;
	while(file_scan_start < file_cache + file_stat.st_size) {
		if (!in_quot && (*file_scan_end == '\n' || *file_scan_end == '\0')) {
			program[index++] = strndup(file_scan_start, file_scan_end - file_scan_start);
			file_scan_start = ++file_scan_end;
			continue;
		}
		if (*file_scan_end == '"') {
			in_quot = !in_quot;
		}
		file_scan_end++;
	}
	free(file_cache);
	return 0;

out:
	if (fd)
		close(fd);
	if (program)
		free(program);
	if (file_cache)
		free(file_cache);
	return -1;
}

int main(int argc, char **argv)
{
/*
	init_regs();
	const char *exp[] = {"mov x,117.123","label:mov y,3","loop:","print \"hello world\"", "print x + y"};
	int i;
	for (i = 0; i < 5; i++) {
		calc_num(exp[i]);
	}
	return 0;
*/
	if (argc != 2) {
		printf("Useage: ./a.out <script_file>\n");
		return -1;
	}
	if (parse_file(argv[1])) {
		return -1;
	}
	init_regs();
	printf("lines: %d\n",lines);
	for (int i = 0; i < lines; i++) {
		calc_num(program[i]);
	}
	return 0;
}
