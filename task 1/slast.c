/*who1.c - a first version of the who command
 *         open, read UPMP file ,and show results.
 *
 */
#include "slast.h"

#define SHOWHOST    /*include remote machine on output*/


int main(int argc, char * argv[]){
    if(argc < 2){
        printf("no argument\n");
        exit(EXIT_FAILURE);
    }
    else if(argc > 2){
        printf("too many args\n");
        exit(EXIT_FAILURE);
    }

    int i;
    long val;
    char *next;

    // Process each argument given.

        // Get value with failure detection.
        val = strtol (argv[1], &next, 10);

        // Check for empty string and characters left after conversion.
        if ((next == argv[1]) || (*next != '\0')) {
            printf(" %s not valid\n", argv[1]);
            exit(EXIT_FAILURE);
        }

    if(val == 0){
        return 0;
    }
    else if(val < 0){
        printf("negative number");
        exit(1);
    }
    
    struct utmp current_record;     /*read info into here*/
    int utmpfd;     /*file descriptor*/
    int reclen = sizeof(current_record);

    if ((utmpfd = open(WTMP_FILE, O_RDONLY)) == -1){
        perror( WTMP_FILE );    /*WTMP_FILE is in utmp.h*/
        exit(1);
    }
    bool flag = false;
    int j = 0;
    int offset = 0;
    if(offset = lseek(utmpfd,-reclen,SEEK_END) == -1){ //start from the end
        printf("lseek , start from end\n");
        exit(1);
    }

    while(read(utmpfd, &current_record, reclen) == reclen && j < val){
        flag = show_info(&current_record);
        
        if(flag){
            j++;
        }

        if(lseek(utmpfd,-(reclen*2),SEEK_CUR) == -1){
            break;
        }
    }
    close(utmpfd);
    return 0;   /*went ok*/
}

void show_time(long time_val){
    char *cp = ctime(&time_val);
    printf("%16.16s" , cp);
}

bool show_info(struct utmp *utbufp){
    bool flag = false;
    if (utbufp->ut_type == USER_PROCESS || utbufp->ut_type == BOOT_TIME){
        printf("%-8.8s", utbufp->ut_name);     /*the logname */
        printf(" ");

        if(utbufp->ut_type == BOOT_TIME){
            printf("%-12.12s","system boot");
        }
        else{
            printf("%-12.12s",utbufp->ut_line);     /* the tty*/
        }
        printf(" ");

        #ifdef SHOWHOST
        printf("%-16.16s", utbufp->ut_host);
        #endif
        printf(" ");

        show_time(utbufp->ut_time);        /* login time */
        printf(" ");

        printf("\n");
        flag = true;
    }
    return flag;
}