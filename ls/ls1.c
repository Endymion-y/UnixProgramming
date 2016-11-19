/* ls1.c
 * List contents of directory(ies)
 * If no args, use . else list files in args
 */
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>

void do_ls(char* dirname);

int main(int argc, char* argv[]){
	if (argc == 1){
		do_ls(".");
	}
	else {
		while (--argc){
			printf("%s:\n", *++argv);
			do_ls(*argv);
		}
	}
	return 0;
}

// List files in directory called dirname
void do_ls(char* dirname){
	DIR* dir_ptr;
	struct dirent* direntp;

	if ((dir_ptr = opendir(dirname)) == NULL)
		fprintf(stderr, "ls1: Cannot open %s\n", dirname);
	else {
		while ((direntp = readdir(dir_ptr)) != NULL)
			printf("%s\n", direntp->d_name);
		closedir(dir_ptr);
	}
}
