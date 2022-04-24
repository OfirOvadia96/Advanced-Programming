
#define _XOPEN_SOURCE 600 /* Get nftw() */
#include <ftw.h>
#include <sys/types.h>    /* Type definitions used by many programs */
#include <stdio.h>        /* Standard I/O functions */
#include <stdlib.h>       /* Prototypes of commonly used library functions,
                             plus EXIT_SUCCESS and EXIT_FAILURE constants */
#include <unistd.h>       /* Prototypes for many system calls */
#include <errno.h>        /* Declares errno and defines error constants */
#include <string.h>       /* Commonly used string-handling functions */

#include <pwd.h>
#include <grp.h>

void mode_to_letters(int mode);
void show_stat_info(const struct stat *buf);

/* Callback function called by ftw() */
static int dirTree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb){

    if (type != FTW_NS){
        //printf("%7ld ", (long) sbuf->st_ino);
        }
    else {printf("        ");}
	
        if (type == FTW_NS) {                  /* Could not stat() file */
        printf("?");
        }
     else {
    	 printf(" %*s", 4 * ftwb->level, "|__ ");         /* Indent suitably */
    	    
        switch (sbuf->st_mode & S_IFMT) {  /* Print file type */
        case S_IFREG:  printf("[-"); break;
        case S_IFDIR:  printf("[d"); break;
        case S_IFCHR:  printf("[c"); break;
        case S_IFBLK:  printf("[b"); break;
        case S_IFLNK:  printf("[l"); break;
        case S_IFIFO:  printf("[p"); break;
        case S_IFSOCK: printf("[s"); break;
        default:       printf("[?"); break; /* Should never happen (on Linux) */
        }
        
    }
    show_stat_info(sbuf);
    printf("] ");
        printf("%s\n",  &pathname[ftwb->base]);     /* Print basename */

    return 0;                                   /* Tell nftw() to continue */
    

}

int main(int argc, char *argv[]){

    if (argc == 1) {
        char* str = argv[0];
        printf("ERROR! no input: %s need argument\n", &str[2]);
        exit(1);
    }

    if(argc > 2){
        printf("ERROR! too many agruments\n");
        exit(1);
    }

    if (nftw(argv[1], dirTree, 10, 0) == -1) {
        printf("args not valid: yes its -1 \n");
        perror("nftw");
        exit(1);
    }

    return 0;
}



void show_stat_info(const struct stat *buf){

	mode_to_letters(buf->st_mode);
	printf(" %s", getpwuid(buf->st_uid)->pw_name); 
	printf(" %s", getgrgid(buf->st_gid)->gr_name); 
	printf(" %ld", buf->st_size); 

}

void mode_to_letters(int mode){

char str[] = "---------";

if ( mode & S_IRUSR ) str[0] = 'r'; /* 3 bits for user */
if ( mode & S_IWUSR ) str[1] = 'w';
if ( mode & S_IXUSR ) str[2] = 'x';

if ( mode & S_IRGRP ) str[3] = 'r'; /* 3 bits for group */
if ( mode & S_IWGRP ) str[4] = 'w';
if ( mode & S_IXGRP ) str[5] = 'x';

if (mode & S_IROTH ) str[6] = 'r'; /* 3 bits for other */
if (mode & S_IWOTH ) str[7] = 'w';
if (mode & S_IXOTH ) str[8] = 'x';

printf("%s",str);
}


	

