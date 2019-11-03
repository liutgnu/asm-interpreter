#include<stdio.h>
#include<fcntl.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<sys/types.h>
#include<sys/stat.h>

typedef enum {false = 0, true} bool;
typedef struct label_to_line {
	int line;
	char *label;
} label_to_line;

int PC = 0;
bool FLAGS = false;

extern void do_execute(const char *);
extern void init_regs(void);
static int line_num = 0;
static char **program = NULL;

static int label_num = 0;
static label_to_line *labels = NULL;

int convert_label_to_line(const char *label)
{
	int i;
	if (!labels)
		goto out;

	for (i = 0; i < label_num; i++) {
		if (!strcmp(label, labels[i].label))
			return labels[i].line;
	}
out:
	return -1;
}

static int parse_file(const char *file_name)
{
	int fd = 0, index = 0, i = 0, j = 0;
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
		if (*file_scan_end == ':' && !in_quot) {
			label_num++;
		}
		if (*file_scan_end == '\n' && !in_quot) {
			line_num++;
		}
	}
	if(in_quot) {
		printf("file error, \" unpair\n");
		return -1;
	}

	/*store address of each line of instruction string in array*/
	/*also store label(if any) of each line in array struct*/
	program = malloc(sizeof(char *) * line_num);
	if (!program) {
		printf("malloc program error!\n");
		goto out;
	}
	memset(program, 0, sizeof(char *) * line_num);

	if (label_num) { //OK, we do have labels in the script
		labels = malloc(sizeof(label_to_line) * label_num);
		if (!labels) {
			printf("malloc labels error!\n");
			goto out;
		}
		memset(labels, 0, sizeof(label_to_line) * label_num);
	}

	in_quot = false;
	file_scan_start = file_scan_end = file_cache;
	while(file_scan_start < file_cache + file_stat.st_size) {
		/*we process on single line, aka instruction*/
		if (!in_quot && (*file_scan_end == '\n' || *file_scan_end == '\0')) {
			program[index++] = strndup(file_scan_start, file_scan_end - file_scan_start);
			file_scan_start = ++file_scan_end;
			continue;
		}
		/*now process label of this line, return error if label repets*/
		if (!in_quot && *file_scan_end == ':') {
			labels[i].label = strndup(file_scan_start, file_scan_end - file_scan_start);
			labels[i].line = index;
			for (j = 0; j < i; j++) {
				if (!strcmp(labels[j].label, labels[i].label)) {
					printf("error get repeted label: %s\n", labels[i].label);
					goto out;
				}
			}
			i++;
		}
		if (*file_scan_end == '"') {
			in_quot = !in_quot;
		}
		file_scan_end++;
	}
	free(file_cache);
	for (int j = 0; j<label_num;j++) {
		printf("label:\"%s\", %d\n", labels[j].label, labels[j].line);
	}

	return 0;

out:
	if (fd)
		close(fd);
	if (program) {
		for (i = 0; i < line_num; i++) {
			if (program[i])
				free(program[i]);
		}
		free(program);
		program = NULL;
	}
	if (labels) {
		for (i = 0; i < label_num; i++) {
			if (labels[i].label)
				free(labels[i].label);
		}
		free(labels);
		labels = NULL;
	}
	if (file_cache) {
		free(file_cache);
		file_cache = NULL;
	}
	return -1;
}

void execute(void)
{
	while(PC < line_num)
		do_execute(program[PC++]);	
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
	execute();
	return 0;
}
