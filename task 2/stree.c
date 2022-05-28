#define _XOPEN_SOURCE 600 /* Get nftw() */
#include <ftw.h>
#include <sys/types.h> 
#include <stdio.h>       
#include <stdlib.h>      
#include <unistd.h>     
#include <errno.h>     
#include <string.h>       
#include <pwd.h>
#include <grp.h>
#include <limits.h>
#include "stree.h"

/* Convert permissions to letters */
void convert_permissions(int per){

char permissions_line[] = "---------";
//owner permissions
if ( per & S_IRUSR ) permissions_line[0] = 'r';
if ( per & S_IWUSR ) permissions_line[1] = 'w'; 
if ( per & S_IXUSR ) permissions_line[2] = 'x';
//group permissions
if ( per & S_IRGRP ) permissions_line[3] = 'r';
if ( per & S_IWGRP ) permissions_line[4] = 'w';
if ( per & S_IXGRP ) permissions_line[5] = 'x';
//other users permissions
if (per & S_IROTH ) permissions_line[6] = 'r';
if (per & S_IWOTH ) permissions_line[7] = 'w';
if (per & S_IXOTH ) permissions_line[8] = 'x';


printf("%s" , permissions_line); //Print the built permessions line
}

/* Callback function called by ftw() */
static int dir_my_tree(const char *pathname, const struct stat *sbuf, int type, struct FTW *ftwb){

    if (type != FTW_NS){

    }
    else {printf("        ");}
	
    if (type == FTW_NS) {               
        printf("?");
    }

    else {
    	 printf(" %*s", 4 * ftwb->level, "|__ ");       
    	/* Print file type */
        switch (sbuf->st_mode & S_IFMT) {  
        case S_IFREG:  printf("[-"); break;
        case S_IFDIR:  printf("[d"); break;
        case S_IFCHR:  printf("[c"); break;
        case S_IFBLK:  printf("[b"); break;
        case S_IFLNK:  printf("[l"); break;
        case S_IFIFO:  printf("[p"); break;
        case S_IFSOCK: printf("[s"); break;
        default:       printf("[?"); break; //never happend (on Linux)
        }
        
    }
    
    convert_permissions(sbuf->st_mode);
	printf(" %s", getpwuid(sbuf->st_uid)->pw_name); 
	printf(" %s", getgrgid(sbuf->st_gid)->gr_name); 
	printf(" %ld", sbuf->st_size);
    printf("] ");
        printf("%s\n",  &pathname[ftwb->base]);    

    return 0;
}

int main(int argc, char *argv[]){

    if(argc > 2){
        printf("ERROR! too many agruments\n");
        exit(1);
    }

    /* no Arguments at all*/
    else if (argc == 1) {
        
        /* find current path */
        char cwd[PATH_MAX];
        if(getcwd(cwd, sizeof(cwd)) == NULL){
            printf("ERRO!, can't find our path");
            exit(1);
        }

        /* print tree */
        if (nftw(cwd, dir_my_tree, 10, 0) == -1) {
            perror("args not valid");
            exit(1);
        }
    }

    /* One Argument (need to be valid path) */
    else if(argc == 2){
        if (nftw(argv[1], dir_my_tree, 10, 0) == -1) {
            perror("args not valid");
            exit(1);
        }
    }

    return 0;
}

