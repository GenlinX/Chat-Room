#include "bmp.h"
#include "font.h"
#include <pthread.h>
#include <fcntl.h>
#include <stdio.h>
#include <errno.h>
#include <stdint.h>
#include <stdlib.h>
#include <unistd.h>
#include <strings.h>
#include <string.h>
#include <stdbool.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <linux/fb.h>    // LCD
#include <linux/input.h> // 触摸屏
#define qog 14
#define non 14

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
//密码
typedef struct loginfo
{
    char *account;
    char *password;
    char *fake;
    int flag1;
    int flag2;
} loginfo;

typedef struct bmpinfo
{
    char *rgbmemory;
    int lineBytes;
    int width;
    int heigh;
} bmpinfo;

typedef struct xy
{
    int x;
    int y;
    int td;

} xy;

//商品信息
typedef struct cmdyinfo
{
    bmpinfo *bmp;  //图片rgb数据包
    int id;        //商品编号
    char *bmpname; //图片名
    int amount;    //数量
    float price;   //单价
    char *name;    //名称
    int cart;      //该商品在购物车里的数量
    int countcart; //购物车里的商品种数

} cmdyinfo;

typedef struct node // 大结构体
{
    // 用户数据
    cmdyinfo data;
    int count;
    //指针域
    struct node *prev;
    struct node *next;
} node;

void showcartnum(node *head, char *maplcd, font *f, bitmap *screen);

void show(char *maplcd);
bool addgoods(node *head, char *maplcd);
bool delgoods(node *head, char *maplcd);
bool deletenode(node *head, int id);
bool insertnode(node *head, node *node);

loginfo *initloginfo(void);
loginfo *stdinloginfo(void);

bool stdin_account(char *maplcd, xy *otc, loginfo *stdinacpw, font *f, bitmap *screen);
bool stdin_password(char *maplcd, xy *otc, loginfo *stdinacpw, font *f, bitmap *screen);

void addordel(char *maplcd, xy *otc, node *head, font *f, bitmap *screen);

node *inithead();
node *initialize_linklist();
node *creatnode(cmdyinfo data);
bool insertnode(node *head, node *node);
void spreadout(node *head);

char *mapsm(void);
bmpinfo *getrgb(char const *bmpname);
void getouchxy(xy *otc);
void dspybmp(bmpinfo *bmp, char *maplcd, xy *otc);

int logon_button(xy *otc);
void logon_interface(char *maplcd, xy *otc, node *head, loginfo *acpw, loginfo *stdinacpw, font *f, bitmap *screen);

void main_interface(char *maplcd, xy *otc, node *head, loginfo *acpw, loginfo *stdinacpw, font *f, bitmap *screen);
int main_button(xy *otc);

void commodity_interface(char *maplcd, xy *otc, node *head, loginfo *acpw, loginfo *stdinacpw, font *f, bitmap *screen);
int commodity_button(xy *otc);

int buying_button(xy *otc);
void buying_interface(char *maplcd, xy *otc, node *head, node *tmppp, char *copymaplcd, font *f, bitmap *screen);

int cart_button(xy *otc);
void cart_interface(char *maplcd, xy *otc, node *head, font *f, bitmap *screen);

bool cartpay_interface(char *maplcd, xy *otc, node *head, font *f, bitmap *screen, char *ordertime);

int pay_button(xy *otc);
bool pay_interface(char *maplcd, xy *otc, node *tmppp, char *copymaplcd, font *f, bitmap *screen, int numofcmdy, char *ordertime);

node *loadthegood_right(node *head, node *p, char *maplcd, font *f, bitmap *screen);
node *loadthegood_left(node *head, node *p, char *maplcd, font *f, bitmap *screen);

bool button_position(xy *otc, int w, int h, int x, int y);