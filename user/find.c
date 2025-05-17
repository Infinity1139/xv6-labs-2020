#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fs.h"

void find(char* path,char* target){
    int fd;
    char buf[512];
    struct stat st;
    struct dirent dr;
    fd = open(path,0);// only read
    if(fd<0){
        fprintf(2,"find:cannot open %s\n",path);
        return;
    }
    if(fstat(fd,&st)<0){
        fprintf(2,"find:cannot stat %s\n",path);
        return;
    }

    switch(st.type){
        case T_FILE:
            if(strcmp(path+strlen(path)-strlen(target),target)==0){
                printf("%s\n",path);
            }
            break;
        case T_DIR:
            if(strlen(path)+1+DIRSIZ+1>512){
                printf("find:path too long\n");
                break;
            }
            strcpy(buf,path);
            char* p = buf+strlen(path);
            *p++ = '/';
            while(read(fd,&dr,sizeof(dr))==sizeof(dr)){
                if(dr.inum==0)
                    continue;
                memmove(p,dr.name,DIRSIZ);
                p[DIRSIZ]='\0';
                if(stat(buf,&st)<0){
                    fprintf(2,"find:cannot stat %s",buf);
                    continue;
                }
                if(strcmp(buf+strlen(buf)-2,"/.")!=0 && strcmp(buf+strlen(buf)-3,"/..")!=0){
                    find(buf,target);
                }
            }
            break;
    }
    close(fd);
}

int main(int argc,char* argv[]){
    if(argc<3){
        exit(1);
    }
    char target[512];
    target[0] = '/';
    strcpy(target+1,argv[2]);
    find(argv[1],target);
    exit(0);
}