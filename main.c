#include "myhead.h"

int main(int argc, char const *argv[])
{
    //映射lcd
    char *maplcd = mapsm();

    //打开触碰屏
    xy *otc = (xy *)malloc(sizeof(xy));
    otc->td = open("/dev/input/event0", O_RDWR);
    if (otc->td == -1)
    {
        perror("触摸屏未打开");
        exit(0);
    }

    //加载链表
    node *head = initialize_linklist();
    spreadout(head);

    //初始化登录信息
    loginfo *acpw = initloginfo();
    loginfo *stdinacpw = stdinloginfo();

    font *f = fontLoad("myh.ttf"); //打开字库
    bitmap *screen = createBitmap2(800, 480, 4, maplcd);

    //进入主界面
    main_interface(maplcd, otc, head, acpw, stdinacpw,f,screen);
}
