/* ls2.c
 * List contents of directory(ies)
 * If no args, use . else list files in args
 * Uses stat and pwd.h and grp.h
 * BUG: try ls2 /tmp
 */
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <dirent.h>
#include <sys/stat.h>
#include <time.h>
#include <string.h>
#include <pwd.h>
#include <grp.h>

void do_ls(char* dirname);
void do_stat(char* filename);
void show_file_info(char* filename, struct stat* statp);
void mode_to_letters(int mode, char* str);
char* uid_to_name(uid_t uid);
char* gid_to_name(gid_t gid);

int main(int argc, char* argv[]){
	if (argc == 1) do_ls(".");
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
		fprintf(stderr, "ls2: Cannot open %s\n", dirname);
	else {
		while ((direntp = readdir(dir_ptr)) != NULL)
			do_stat(direntp->d_name);
		closedir(dir_ptr);
	}
}

void do_stat(char* filename){
	struct stat info;
	if (stat(filename, &info) == -1)
		perror(filename);
	else 
		show_file_info(filename, &info);
}

// Display the info about filename. The info is stored in struct at * statp
void show_file_info(char* filename, struct stat* statp){
	char modestr[11];

	mode_to_letters(statp->st_mode, modestr);

	printf("%s ", modestr);
	printf("%4d ", (int)statp->st_nlink);
	printf("%-8s ", uid_to_name(statp->st_uid));
	printf("%-8s ", gid_to_name(statp->st_gid));
	printf("%8ld ", (long)statp->st_size);
	printf("%.12s ", 4 + ctime(&(statp->st_mtime)));
	printf("%s\n", filename);
}

// Utility functions

/* This function takes a mode value and a char array
 * and puts into the char array the file type and the 
 * nine letters that correspond to the bits in mode.
 */
void mode_to_letters(int mode, char* str){
	strcpy(str, "----------");

	if (S_ISDIR(mode)) str[0] = 'd';
	else if (S_ISCHR(mode)) str[0] = 'c';
	else if (S_ISBLK(mode)) str[0] = 'b';

	if (mode & S_IRUSR) str[1] = 'r';
	if (mode & S_IWUSR) str[2] = 'w';
	if (mode & S_IXUSR) str[3] = 'x';

	if (mode & S_IRGRP) str[4] = 'r';
	if (mode & S_IWGRP) str[5] = 'w';
	if (mode & S_IXGRP) str[6] = 'x';

	if (mode & S_IROTH) str[7] = 'r';
	if (mode & S_IWOTH) str[8] = 'w';
	if (mode & S_IXOTH) str[9] = 'x';
}

// Return pointer to username associated with uid, uses getpwuid(3)
char* uid_to_name(uid_t uid){
	struct passwd* pw_ptr;
	static char numstr[10];

	if ((pw_ptr = getpwuid(uid)) == NULL){
		sprintf(numstr, "%d", uid);
		return numstr;
	}
	else 
		return pw_ptr->pw_name;
}

// Return pointer to group number gid, uses getgrgid(3)
char* gid_to_name(gid_t gid){
	struct group* grp_ptr;
	static char numstr[10];

	if ((grp_ptr = getgrgid(gid)) == NULL){
		sprintf(numstr, "%d", gid);
		return numstr;
	}
	else 
		return grp_ptr->gr_name;
}