/* spwd.c: a simplified version of pwd
 * Starts in current directory and recursively
 * climbs up to root of filesystem, prints top part
 * then prints current part
 * Uses readdir() to get info about each thing
 * BUG: prints an empty string if run from "/"
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>

ino_t get_inode(char* filename);
void printpathto(ino_t ino);
void inum_to_name(ino_t ino, char* filename, int buflen);

int main(){
	printpathto(get_inode("."));
	putchar('\n');
	return 0;
}

// Prints path leading down to an object with this inode 
// Kind of recursive
void printpathto(ino_t ino){
	ino_t my_inode;
	char its_name[BUFSIZ];
	if (get_inode("..") != ino){
		chdir("..");
		inum_to_name(ino, its_name, BUFSIZ);
		my_inode = get_inode(".");
		printpathto(my_inode);
		printf("/%s", its_name);
	}
}

// Looks through current directory for a file with this inode
// number and copies its name into namebuf
void inum_to_name(ino_t ino, char* filename, int buflen){
	DIR* dir_ptr;
	struct dirent* direntp;
	dir_ptr = opendir(".");
	if (dir_ptr == NULL){
		perror(".");
		exit(1);
	}
	// Search directory for a file with specified inum
	while ((direntp = readdir(dir_ptr)) != NULL){
		if (direntp->d_ino == ino){
			strncpy(filename, direntp->d_name, buflen);
			filename[buflen - 1] = '\0';      // Just in case
			closedir(dir_ptr);
			return;
		}
	}
	fprintf(stderr, "Error looking for inum %llu\n", ino);
	exit(1);
}

// Returns the inode number of the file
ino_t get_inode(char* filename){
	struct stat info;
	if (stat(filename, &info) == -1){
		fprintf(stderr, "Cannot stat ");
		perror(filename);
		exit(1);
	}
	return info.st_ino;
}