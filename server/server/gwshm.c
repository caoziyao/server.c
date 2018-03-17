//
//  gwshm.c
//  server
//

#define TEXT_SZ 2048

#include "gwshm.h"
#define ShmSize 1000

int
GwShmInit(){
    
    int size = sizeof(GwShm);
    int flg = 0666 | IPC_CREAT;
    
    // IPC_PRIVATE
    int shmid = shmget(IPC_PRIVATE, size, flg);
    if (shmid < 0) {
        quit("shmget()");
    }
    
    // 清零
    void *shm = (void *)shmat(shmid, 0, 0);
    memset(shm, 0, sizeof(GwShm));
    
//    GwShm *shm = (GwShm *)shmat(shmid, 0, 0);
//    if (shm == (void *)-1) {
//        quit("shmat()");
//    }
//    shm->size = sizeof(GwShm);
//    shm->shmid = shmid;
    
    return shmid;
}


// 把共享内存区对象映射到调用进程的地址空间
GwShm *
GwShmat(int shmid) {
    GwShm *shm = (GwShm *)shmat(shmid, 0, 0);
    if (shm == (void *)-1) {
        quit("shmat()");
    }
    shm->size = sizeof(GwShm);
    shm->shmid = shmid;
    
    return shm;
}


// 断开共享内存连接
void
GwShmdt(GwShm *shm) {
    const void *shmaddr = (void *)shm;
    shmdt(shmaddr) ;
}


// 共享内存管理
void
GwShmctl(int shmid, int cmd, struct shmid_ds *buf){
    // bug 共享内存管理结构体。具体说明参见共享内存内核结构定义部分
    int err = shmctl(shmid, cmd, buf);
    if (err == -1) {
        printf("GwShmctl()\n");
    }
}


// 删除shm, 等价于
void
GwShmRemove(int shmid) {
    GwShmctl(shmid, IPC_RMID, NULL);
}


//// todo struct
//GwShmData *
//GwShmReadData(GwShm *shm) {
//    char *shmptr = shm->shmptr;
//    GwShmData *data = malloc(sizeof(GwShmData));
//    memcpy(data, shmptr, sizeof(GwShmData));
//
//    return data;
//}
//
//// todo struct
//void
//GwShmWriteData(GwShm *shm, GwShmData *data){
//    char *shmptr = shm->shmptr;
//    memcpy(shmptr, data, sizeof(GwShmData));
//}


int
GwShmTest(){
    int shmid ;
    char *shmaddr ;
    struct shmid_ds buf ;
    int flag = 0 ;
    int pid ;
    
    shmid = shmget(IPC_PRIVATE, 1000, IPC_CREAT|0600 ) ;
    
    if ( shmid < 0 ) {
        perror("get shm  ipc_id error") ;
        return -1 ;
    }
    
    pid = fork() ;
    if ( pid == 0 ) {
        
        shmaddr = (char *)shmat( shmid, NULL, 0 ) ;
        if ( (int)shmaddr == -1 ) {
            perror("shmat addr error") ;
            return -1 ;
        }
        strcpy( shmaddr, "Hi, I am child process!\n") ;
        shmdt( shmaddr ) ;
        return  0;
    } else if ( pid > 0) {
        sleep(3 ) ;
        flag = shmctl( shmid, IPC_STAT, &buf) ;
        if ( flag == -1 ) {
            perror("shmctl shm error") ;
            return -1 ;
        }
        printf("shm_segsz =%zu bytes\n", buf.shm_segsz ) ;
        printf("parent pid=%d, shm_cpid = %d \n", getpid(), buf.shm_cpid ) ;
        printf("chlid pid=%d, shm_lpid = %d \n",pid , buf.shm_lpid ) ;
        shmaddr = (char *) shmat(shmid, NULL, 0 ) ;
        if ( (int)shmaddr == -1 ){
            perror("shmat addr error") ;
            return -1 ;
        }
        printf("%s", shmaddr) ;
        shmdt( shmaddr ) ;
        shmctl(shmid, IPC_RMID, NULL) ;
    }else{
        perror("fork error") ;
        shmctl(shmid, IPC_RMID, NULL) ;
    }
    return 0 ;
}
