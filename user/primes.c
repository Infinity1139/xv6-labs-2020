#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

void sieve(int p_left[2]){
    int p=-1;
    read(p_left[0],&p,sizeof(p));
    if(p==-1){
        exit(0);
    }
    printf("prime %d\n",p);
    int p_right[2];
    pipe(p_right);
    if(fork()!=0){
        close(p_right[0]);
        int i=-1;
        while(read(p_left[0],&i,sizeof(i))!=0 && i!=-1){
            if(i%p!=0)
                write(p_right[1],&i,sizeof(i));
        }
        i=-1;
        write(p_right[1],&i,sizeof(int));
        close(p_right[1]);
        close(p_left[0]);
        wait(0);
        exit(0);
    }
    else{
        close(p_right[1]);
        close(p_left[0]);
        sieve(p_right);
        exit(0);
    }
}


int main(int argc,char* argv[]){
    int p_left[2];
    pipe(p_left);
    if(fork()!=0){
        for(int i=2;i<=35;i++){
            write(p_left[1],&i,sizeof(i));
        }
        int i=-1;
        write(p_left[1],&i,sizeof(i));
        close(p_left[1]);
        wait((int*)0);
        exit(0);
    }
    else{
        close(p_left[1]);
        sieve(p_left);
        exit(0);
    }
}