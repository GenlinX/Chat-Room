#ifndef _HEAD4CHAT_H_
#define _HEAD4CHAT_H_

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdint.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <poll.h>
#include <net/ethernet.h>
#include <strings.h>
#include <string.h>
#include <pthread.h>
#include <linux/fb.h>    // LCD
#include <linux/input.h> // 触摸屏
#include <fcntl.h>
#include<signal.h>
#include<sys/signal.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include<sys/sysmacros.h>
#include<pwd.h>
#include<time.h>
#include<grp.h>
#include<dirent.h>
#include "kernel_list.h"
#include"thread_pool.h"
#include "bmp.h"
#include "font.h"
#define SCREENSIZE 800*480*4
#define SIZE 1024
#define PORT_ROSE 65512
#define ADDR "192.168.26.16"
#define PATHSIZE 255
#define READSIZE 65535
#define NOWPL 14 //Number of words per line
#define NORTD 7  //Number of rows to display

#define lightgreen 0x00da7000
#define white 0xffffff00
#define red 0xff000000
#define green 0x00ff0000
#define blue 0x0000ff00
#define black 0x00000000
#define DeepSkyBlue 0x00BFFF00
#define cyan 0x00ffff00
#define pink 0xff00ff00
#define yellow 0xffff0000
#define gray 0x80808000
#define purple 0x786fe200
#define color1 white
#define color2 purple

typedef struct client   //服务器中，客户端列表
{
    int connfd;
    pthread_mutex_t connfd_lock;
    struct list_head list;
} client;

typedef struct clientlist   //客户端中，客户端列表
{
    int client_fd;
    pthread_mutex_t chat_history_lock;
    struct list_head clist;
} clientlist;

typedef struct textconnect
{
    int src_fd;
    int dst_fd;
} textcon;

typedef struct thread_signal
{
    pthread_t selftid;
    int newfd;
}threadsig;

enum linksignal
{
    cnt_clicnsvr = 1,
    cnt_updatefd,
    cnt_sendtext,
    cnt_recvtext,
    cnt_sendfile,
    cnt_recvfile,
    cnt_senddrct,
    cnt_recvdrct
};

typedef struct botton_info
{
    int w;
    int h;
    int x;
    int y;
    void*arg;
    void* (*func)(void*arg);
    struct list_head blist;
}btif;

typedef struct bmpinfo
{
    char *rgbmemory;
    int lineBytes;
    int width;
    int heigh;
} bmpinfo;

typedef struct tidarg
{
    pthread_t parent_tid;
    pthread_t child_tid;
}tidarg;

typedef struct drctarg
{
    int connfd;
    char drctpath[PATHSIZE];
}drctarg;

void*emojilist(void*arg);
void *delete_botton(int x, int y);
void *update_clientlist();
void *client_send_text(void *arg);
void *client_send_file(void *arg);
void *terminate(void *arg);
void dspybmp(bmpinfo *bmp, int x, int y);
void *displaying_chat_history(int client_fd);
void *send_directory(void *arg);
void *send_directory_recursively(int newfd, char *src_path, char *dst_path);
void *add_botton(int w, int h, int x, int y, void *arg, void *(*func)(void *arg));
bmpinfo *getrgb(char const *bmpname);

#endif