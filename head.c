#include "myhead.h"

// #define malloc(a) ({printf("%d malloc: ",__LINE__);void *p=malloc(a); printf("%p\n", p);p;})
// #define calloc(a, b) ({printf("%d calloc: ",__LINE__);void *p = calloc(a,b);printf("%p\n", p);p;})

//展示购物车红点
void showcartnum(node*head,char*maplcd,font*f,bitmap*screen)
{
    xy redcoor;
    redcoor.x = 50;
    redcoor.y = 418;
    char redbuf[2];
    bmpinfo *redbmp = getrgb("red.bmp");
    bmpinfo*noredbmp=getrgb("nored.bmp");
    if (head->data.countcart != 0)
    {
        dspybmp(redbmp, maplcd, &redcoor);
        sprintf(redbuf, "%d", head->data.countcart);
        fontSetSize(f, 20);
        fontPrint(f, screen, 55, 418, redbuf, color1, 800);
    }
    else
    {
        dspybmp(noredbmp, maplcd, &redcoor);
    }
}

//获取当前时间并转成字符串
char *getime()
{

    time_t timer;
    struct tm *now;
    static char nowtime[44];

    time(&timer);
    now = localtime(&timer);

    nowtime[0] = '2';
    nowtime[1] = '0';

    nowtime[2] = now->tm_year % 100 / 10 + '0';
    nowtime[3] = now->tm_year % 100 % 10 + '0';

    nowtime[4] = -27; //年
    nowtime[5] = -71;
    nowtime[6] = -76;

    nowtime[7] = ' ';

    nowtime[8] = (now->tm_mon + 1) / 10 + '0';
    nowtime[9] = (now->tm_mon + 1) % 10 + '0';

    nowtime[10] = -26; //月
    nowtime[11] = -100;
    nowtime[12] = -120;

    nowtime[13] = now->tm_mday / 10 + '0';
    nowtime[14] = now->tm_mday % 10 + '0';

    nowtime[15] = -26; //日
    nowtime[16] = -105;
    nowtime[17] = -91;

    nowtime[18] = ' ';

    nowtime[19] = -26; //星期
    nowtime[20] = -104;
    nowtime[21] = -97;
    nowtime[22] = -26;
    nowtime[23] = -100;
    nowtime[24] = -97;

    switch (now->tm_wday)
    {
    case 1:
        nowtime[25] = -28;
        nowtime[26] = -72;
        nowtime[27] = -128;
        break;

    case 2:
        nowtime[25] = -28;
        nowtime[26] = -70;
        nowtime[27] = -116;
        break;

    case 3:
        nowtime[25] = -28;
        nowtime[26] = -72;
        nowtime[27] = -119;
        break;

    case 4:
        nowtime[25] = -27;
        nowtime[26] = -101;
        nowtime[27] = -101;
        break;

    case 5:
        nowtime[25] = -28;
        nowtime[26] = -70;
        nowtime[27] = -108;
        break;

    case 6:
        nowtime[25] = -27;
        nowtime[26] = -123;
        nowtime[27] = -83;
        break;

    case 0:
        nowtime[25] = -26;
        nowtime[26] = -105;
        nowtime[27] = -91;
        break;

    default:
        break;
    }

    nowtime[28] = ' ';

    nowtime[29] = now->tm_hour / 10 + '0';
    nowtime[30] = now->tm_hour % 10 + '0';

    nowtime[31] = -26; //时
    nowtime[32] = -105;
    nowtime[33] = -74;

    nowtime[34] = now->tm_min / 10 + '0';
    nowtime[35] = now->tm_min % 10 + '0';

    nowtime[36] = -27; //分
    nowtime[37] = -120;
    nowtime[38] = -122;

    nowtime[39] = now->tm_sec / 10 + '0';
    nowtime[40] = now->tm_sec % 10 + '0';

    nowtime[41] = -25; //秒
    nowtime[42] = -89;
    nowtime[43] = -110;

    nowtime[44] = '\0';

    return nowtime;
}

void show(char *maplcd)
{
    xy coor;
    coor.x = 0;
    coor.y = 0;
    bmpinfo *blink = getrgb("hint.bmp");
    while (1)
    {

        dspybmp(blink, maplcd, &coor);
        for (int i = 0; i < 800 * 480; i++)
        {
            if (i % 2 == 0)
            {
                bzero(maplcd + i * 4, 4);
            }
        }
        sleep(1);
        for (int i = 0; i < 800 * 480; i++)
        {
            if (i / 800 % 2 == 0)
            {
                bzero(maplcd + i * 4, 4);
            }
        }
        sleep(1);
        for (int i = 0; i < 800 * 480; i++)
        {
            if (i % 2 == 1)
            {
                bzero(maplcd + i * 4, 4);
            }
        }
        sleep(1);
        for (int i = 0; i < 800 * 480; i++)
        {
            if (i / 800 % 2 == 1)
            {
                bzero(maplcd + i * 4, 4);
            }
        }
        sleep(1);

        // for(int i=0;i<800*480;i++)
        // {
        //     bzero(maplcd + i * 4, 4);
        // }
        // sleep(1);
    }
}
//初始化已注册的登录信息
loginfo *initloginfo(void)
{
    loginfo *acpw = calloc(1, sizeof(loginfo));
    acpw->account = calloc(14, sizeof(char));
    acpw->password = calloc(14, sizeof(char));
    sprintf(acpw->account, "");
    sprintf(acpw->password, "");
    acpw->flag1 = 0;
    acpw->flag2 = 0;
    return acpw;
}

//初始化用户输入的登录信息
loginfo *stdinloginfo(void)
{
    loginfo *stdinacpw = calloc(1, sizeof(loginfo));
    stdinacpw->account = calloc(14, sizeof(char));
    stdinacpw->password = calloc(14, sizeof(char));
    stdinacpw->fake = calloc(14, sizeof(char));
    stdinacpw->flag1 = 0;
    stdinacpw->flag2 = 0;
    return stdinacpw;
}

//登录程序:账号
bool stdin_account(char *maplcd, xy *otc, loginfo *stdinacpw, font *f, bitmap *screen)
{
    bmpinfo *account = getrgb("account.bmp");
    bmpinfo *xbmp = getrgb("x.bmp");
    xy coor;
    coor.x = 0;
    coor.y = 0;
    dspybmp(account, maplcd, &coor);

    coor.x = 618;
    coor.y = 158;

    char acitfbuf[800 * 480 * 4];
    memcpy(acitfbuf, maplcd, 800 * 480 * 4);

    char *hint = {"请输入账号"};
    fontSetSize(f, 25);
    fontPrint(f, screen, 462, 163, hint, gray, 800);

    char input[14];
    int icount = -1;
    bzero(input, 14);

    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));
        //判断是否发生触碰事件
        switch (buf.type)
        {
        case EV_ABS: //发生了坐标事件
            switch (buf.code)
            {
            case ABS_X: //触发了x坐标
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            case ABS_Y: //触发了y坐标
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }

                continue;

            default:
                break;
            }
            continue;

        case EV_KEY: //发生了触碰事件
            switch (buf.code)
            {
            case BTN_TOUCH:
                if (buf.value == 0) //手离开了触摸屏
                {
                    fflush(stdout);
                    //判断手是从哪个按钮离开的
                    if ((pay_button(otc) >= 0) && (pay_button(otc) <= 9))
                    {
                        if (icount < 12)
                        {

                            sprintf(&input[++icount], "%d", pay_button(otc));
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 163, input, black, 800);
                            if (icount >= 0)
                            {
                                dspybmp(xbmp, maplcd, &coor);
                            }
                        }
                    }

                    else if (pay_button(otc) == 11)
                    {
                        if (icount < 12)
                        {
                            input[++icount] = '*';
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 163, input, black, 800);
                            if (icount >= 0)
                            {
                                dspybmp(xbmp, maplcd, &coor);
                            }
                        }
                    }
                    else if (pay_button(otc) == 12)
                    {
                        //退格键
                        if (icount != -1)
                        {
                            input[icount] = '\0';
                            icount--;
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 163, input, black, 800);
                            if (icount >= 0)
                            {
                                dspybmp(xbmp, maplcd, &coor);
                            }
                        }
                        if (icount == -1)
                        {
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 163, hint, gray, 800);
                        }
                    }
                    else if (pay_button(otc) == 13)
                    {
                        //取消
                        bzero(input, 14);
                        icount = -1;
                        return false;
                    }
                    else if (pay_button(otc) == 15)
                    {
                        //x
                        if (icount != -1)
                        {
                            bzero(input, 14);
                            icount = -1;
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 163, hint, gray, 800);
                        }
                    }
                    else if (pay_button(otc) == 14)
                    {
                        //确认
                        if (icount == -1)
                        {
                            stdinacpw->flag1 = 0;
                        }
                        else
                        {
                            stdinacpw->flag1 = 1;
                            // printf("此时buf:%s",buf);
                            sprintf(stdinacpw->account, "%s", input);
                        }

                        return true;
                    }
                }
                break;

            default:
                break;
            }
            continue;
        }
    }
}

//登录程序：密码
bool stdin_password(char *maplcd, xy *otc, loginfo *stdinacpw, font *f, bitmap *screen)
{
    bmpinfo *account = getrgb("password.bmp");
    bmpinfo *xbmp = getrgb("x.bmp");
    xy coor;
    coor.x = 0;
    coor.y = 0;
    dspybmp(account, maplcd, &coor);

    coor.x = 618;
    coor.y = 158;

    char acitfbuf[800 * 480 * 4];
    memcpy(acitfbuf, maplcd, 800 * 480 * 4);

    char *hint = {"请输入密码"};
    fontSetSize(f, 25);
    fontPrint(f, screen, 462, 163, hint, gray, 800);

    char input[14];
    char inputfake[14];
    int icount = -1;
    int fakecount = -1;
    bzero(input, 14);
    bzero(inputfake, 14);

    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));
        //判断是否发生触碰事件
        switch (buf.type)
        {
        case EV_ABS: //发生了坐标事件
            switch (buf.code)
            {
            case ABS_X: //触发了x坐标
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            case ABS_Y: //触发了y坐标
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }

                continue;

            default:
                break;
            }
            continue;

        case EV_KEY: //发生了触碰事件
            switch (buf.code)
            {
            case BTN_TOUCH:
                if (buf.value == 0) //手离开了触摸屏
                {
                    fflush(stdout);
                    //判断手是从哪个按钮离开的
                    if ((pay_button(otc) >= 0) && (pay_button(otc) <= 9))
                    {
                        if (icount < 12)
                        {

                            sprintf(&input[++icount], "%d", pay_button(otc));
                            inputfake[++fakecount] = '*';
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 166, inputfake, black, 800);
                            if (icount >= 0)
                            {
                                dspybmp(xbmp, maplcd, &coor);
                            }
                        }
                    }

                    else if (pay_button(otc) == 11)
                    {
                        if (icount < 12)
                        {
                            input[++icount] = '*';
                            inputfake[++fakecount] = '*';
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 166, inputfake, black, 800);
                            if (icount >= 0)
                            {
                                dspybmp(xbmp, maplcd, &coor);
                            }
                        }
                    }
                    else if (pay_button(otc) == 12)
                    {
                        //退格键
                        if (icount != -1)
                        {
                            input[icount] = '\0';
                            inputfake[fakecount] = '\0';
                            icount--;
                            fakecount--;
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 166, inputfake, black, 800);
                            if (icount >= 0)
                            {
                                dspybmp(xbmp, maplcd, &coor);
                            }
                        }
                        if (icount == -1)
                        {
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 163, hint, gray, 800);
                        }
                    }
                    else if (pay_button(otc) == 13)
                    {
                        //取消
                        bzero(input, 14);
                        bzero(inputfake, 14);
                        icount = -1;
                        fakecount = -1;
                        return false;
                    }
                    else if (pay_button(otc) == 15)
                    {
                        //x
                        if (icount != -1)
                        {
                            bzero(input, 14);
                            bzero(inputfake, 14);
                            icount = -1;
                            fakecount = -1;
                            memcpy(maplcd, acitfbuf, 800 * 480 * 4);
                            fontPrint(f, screen, 462, 163, hint, gray, 800);
                        }
                    }
                    else if (pay_button(otc) == 14)
                    {
                        //确认
                        if (icount == -1)
                        {
                            stdinacpw->flag2 = 0;
                        }
                        else
                        {
                            stdinacpw->flag2 = 1;
                            // printf("此时buf:%s",buf);
                            sprintf(stdinacpw->password, "%s", input);
                            sprintf(stdinacpw->fake, "%s", inputfake);
                        }

                        return true;
                    }
                }
                break;

            default:
                break;
            }
            continue;
        }
    }
}

//增删商品
void addordel(char *maplcd, xy *otc, node *head, font *f, bitmap *screen)
{

    bmpinfo *mainbmp = getrgb("main_interface.bmp");
    bmpinfo *addordel = getrgb("addordel.bmp");
    xy coor;
    coor.x = 0;
    coor.y = 0;
    dspybmp(addordel, maplcd, &coor);

    //对触摸屏进行判断
    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));
        //判断是否发生触碰事件
        switch (buf.type)
        {
        case EV_ABS: //发生了坐标事件
            switch (buf.code)
            {
            case ABS_X: //触发了x坐标
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            case ABS_Y: //触发了y坐标
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }

                continue;

            default:
                break;
            }
            continue;

        case EV_KEY: //发生了触碰事件
            switch (buf.code)
            {
            case BTN_TOUCH:
                if (buf.value == 0) //手离开了触摸屏
                {
                    fflush(stdout);
                    //判断手是从哪个按钮离开的
                    switch (main_button(otc))
                    {
                    case 1:
                        printf("添加商品\n");

                        addgoods(head, maplcd);
                        dspybmp(addordel, maplcd, &coor);

                        //调用购物界面

                        break;

                    case 2:
                        printf("删除商品\n");
                        
                        delgoods(head, maplcd);
                        
                        dspybmp(addordel, maplcd, &coor);
                        

                        //调用登录界面
                        break;

                    case 3:

                        // free(addordel->rgbmemory);
                        // free(addordel);
                        return;

                        break;
                    default:
                        break;
                    }
                }
                break;

            default:
                break;
            }
            continue;
        }
    }
}

//比较用户输入与注册信息是否相等
bool login(loginfo *acpw, loginfo *stdinacpw)
{
    if (strcmp(stdinacpw->account, acpw->account) == 0 && strcmp(stdinacpw->password, acpw->password) == 0)
    {
        return true;
    }
    return false;
}

//加入商品
bool addgoods(node *head, char *maplcd)
{
    // pthread_t tid1;
    // int ret1;
    // ret1 = pthread_create(&tid1, NULL, (void *)&show, maplcd);
    bmpinfo *blink = getrgb("hint.bmp");
    xy coor;
    coor.x = 0;
    coor.y = 0;
    dspybmp(blink, maplcd, &coor);

    cmdyinfo newgoods;
    newgoods.name = calloc(30, sizeof(char));
    newgoods.bmpname = calloc(30, sizeof(char));
    printf("请输入商品编号：\n");
    scanf("%d", &newgoods.id);
    printf("请输入商品名称：\n");
    scanf("%s", newgoods.name);
    printf("请输入商品单价：\n");
    scanf("%f", &newgoods.price);
    printf("请输入商品数量：\n");
    scanf("%d", &newgoods.amount);
    printf("请输入商品图片名：\n");
    scanf("%s", newgoods.bmpname);
    bmpinfo *newbmp = getrgb(newgoods.bmpname);
    newgoods.bmp = newbmp;
    newgoods.cart=0;

    node *newnode = creatnode(newgoods);
    if (insertnode(head, newnode) == true)
    {
        printf("新建商品成功!\n");
        // pthread_cancel(tid1);
        return true;
    }
    // pthread_cancel(tid1);
    return false;
}

//删除商品
bool delgoods(node *head, char *maplcd)
{
    // pthread_t tid1;
    // int ret1;
    // ret1 = pthread_create(&tid1, NULL, (void *)&show, maplcd);

    bmpinfo *blink = getrgb("hint.bmp");

    xy coor;
    coor.x = 0;
    coor.y = 0;

    dspybmp(blink, maplcd, &coor);

    int id;
    printf("请输入要删除的商品ID：\n");
    scanf("%d", &id);
    if (id == 123)
    {
        return false;
    }
    if (deletenode(head, id) == true)
    {
        printf("删除商品成功\n");
        printf("删除后的商品信息：\n");
        spreadout(head);
        return true;
    }
    else
    {
        //pthread_cancel(tid1);
        return false;
    }
}

//始化商品链表
node *initialize_linklist()
{
    node *head = inithead();
    cmdyinfo tmp[qog];
    for (int i = 0; i < qog; i++)
    {

        tmp[i].bmpname = calloc(30, sizeof(char));
        tmp[i].name = calloc(30, sizeof(char));
        tmp[i].bmp = malloc(sizeof(bmpinfo));
        tmp[i].bmp->rgbmemory = calloc(800 * 480 * 4, sizeof(char));
    }

    char *a[qog] = {"可乐", "柠檬茶", "可可脆", "好丽友",
                    "咖啡", "面包", "维他命", "巧克力", "薯片", "香肠",
                    "纸巾", "啤酒", "糖果", "口香糖"};

    for (int i = 0; i < qog; i++)
    {
        tmp[i].cart = 0;        //购物车里的数量
        tmp[i].id = i;          //编号
        tmp[i].amount = i + 10; //数量
        tmp[i].price = i + 0.5; //单价
        if (i < 10)
        {
            sprintf(tmp[i].bmpname, "%c.bmp", 48 + i);
        }
        else
        {
            sprintf(tmp[i].bmpname, "%c%c.bmp", 48 + i / 10, 48 + i % 10);
        }
        tmp[i].bmp = getrgb(tmp[i].bmpname); //rgb数据包
        strcpy(tmp[i].name, a[i]);
    }
    int tmpnon = non;
    if (non > qog)
    {
        tmpnon = qog;
    }

    for (int i = 0; i < tmpnon; i++)
    {
        node *node = creatnode(tmp[i]);
        insertnode(head, node);
    }

    return head;
}

//初始化头节点
node *inithead()
{
    node *head = malloc(sizeof(node));
    head->data.bmpname = calloc(30, sizeof(char));
    head->data.name = calloc(30, sizeof(char));
    head->data.bmp = malloc(sizeof(bmpinfo));
    head->data.bmp->rgbmemory = calloc(800 * 480 * 4, sizeof(char));
    head->data.countcart = 0;
    if (head != NULL)
    {
        head->prev = head;
        head->next = head;
        return head;
    }
    else
    {
        perror("初始化头节点失败");
        return NULL;
    }
}

//新建节点
node *creatnode(cmdyinfo data)
{
    node *node = malloc(sizeof(node));
    node->data.bmpname = calloc(30, sizeof(char));
    node->data.name = calloc(30, sizeof(char));
    node->data.bmp = malloc(sizeof(bmpinfo));
    node->data.bmp->rgbmemory = calloc(800 * 480 * 4, sizeof(char));
    node->data = data;
    
       
    node->next = node;
    node->prev = node;
    
    return node;
}

//尾插法插入节点
bool insertnode(node *head, node *node)
{
    if (head == NULL)
    {
        perror("该链表不存在：");
        return false;
    }
    else
    {

        node->prev = head->prev;
        //printf("%s\n",node->data->name);
        node->next = head;
        //printf("%s\n",node->data->name);

        head->prev->next = node;
        //printf("%s\n",node->data->name);
        head->prev = node;
        //printf("%s\n",node->data->name);


        return true;
    }
}

//删除数据
bool deletenode(node *head, int id)
{
    if (head == NULL)
    {
        printf("该链表不存在\n");
        return false;
    }
    else
    {
        int flag = 0;
        node *p = head->next;
        while (p != head)
        {
            if (p->data.id == id)
            {
                flag = 1;
                p->prev->next = p->next;
                p->next->prev = p->prev;
                p->prev = NULL;
                p->next = NULL;
                //free(p);
                break;
            }

            p = p->next;
        }

        if (flag == 1)
        {
            //printf("删除节点成功\n");

            return true;
        }
        else
        {
            printf("删除节点失败");
            return false;
        }
    }
}

//在终端罗列商品信息
void spreadout(node *head)
{
    if (head == NULL)
    {
        perror("该链表不存在\n");
        return;
    }
    else
    {
        node *p = head->next;
        while (p != head)
        {
            printf("编号：%d\n", p->data.id);
            printf("名称：%s\n", p->data.name);
            printf("单价：%.2f\n", p->data.price);
            printf("数量：%d\n", p->data.amount);
            printf("图片：%s\n", p->data.bmpname);
            printf("=================================\n");
            p = p->next;
        }
    }
}

//打开lcd屏并映射内存
char *mapsm(void)
{
    int lcd = open(LCD, O_RDWR);
    if (lcd == -1)
    {
        perror("打开lcd失败");
        exit(1);
    }
    char *p = mmap(NULL, 800 * 480 * 4, PROT_WRITE, MAP_SHARED, lcd, 0);
    bzero(p, 800 * 480 * 4);
    return p;
}

//将bmp的图片信息进一步打包成一个结构体
//传入图片的名称即可将其数据信息打包成一个结构体
bmpinfo *getrgb(char const *bmpname)
{
    bmpinfo *bmpmassage = malloc(sizeof(bmpinfo));
    FILE *fp = fopen(bmpname, "r");
    if (fp == NULL)
    {
        printf("errno\n");
    }
    struct bitmap_header head;
    struct bitmap_info info;

    bzero(&head, sizeof(head));
    bzero(&info, sizeof(info));

    fread(&head, sizeof(head), 1, fp);
    fread(&info, sizeof(info), 1, fp);
    // fscanf(fp, "");
    bmpmassage->width = info.width;
    bmpmassage->heigh = info.height;

    // 展示图片的信息
    // printf("图片文件大小:%d\n", head.size);
    // printf("图片尺寸: %d × %d\n", info.width, info.height);
    // printf("图片色深:%d\n", info.bit_count);

    // 计算每行末尾的无效字节数
    int n = (4 - ((info.width * (info.bit_count / 8)) % 4)) % 4;
    // printf("无效字节：%d\n", n);

    // 计算每行实际的大小（包括无效字节）
    bmpmassage->lineBytes = info.width * (info.bit_count / 8) + n;

    // 计算整个RGB数据（包括无效字节）的大小
    int rgbSize = bmpmassage->lineBytes * info.height;

    //printf("RGB数据（包括无效字节）的大小：%d\n", rgbSize);

    if (info.compression)
    {
        struct rgb_quad q;
        fread(&q, sizeof(q), 1, fp);
    }

    // 读取BMP图片中的RGB数据
    bmpmassage->rgbmemory = calloc(1, rgbSize);
    fread(bmpmassage->rgbmemory, rgbSize, 1, fp);
    fclose(fp);
    return bmpmassage;
}

//在指定位置显示bmp
//bmp:通过getrgb()打包好的图片数据包
//maplcd:lcd屏的内存映射
//otc:在哪个坐标显示图片(从图片的左上角开始)
void dspybmp(bmpinfo *bmp, char *maplcd, xy *otc)
{

    if (maplcd == NULL)
    {
        perror("lcd屏未能映射：");
        exit(0);
    }
    else
    {

        char *tmp = bmp->rgbmemory + bmp->lineBytes * (bmp->heigh - 1);
        int start = otc->y * 800 * 4 + otc->x * 4;
        maplcd += start;

        int tmpx, tmpy;
        int lcd_lines = 800 * 4;
        if (bmp->width + otc->x > 800)
        {
            tmpx = 800 - otc->x;
        }
        else
        {
            tmpx = bmp->width;
        }
        if (bmp->heigh + otc->y > 480)
        {
            tmpy = 480 - otc->y;
        }
        else
        {
            tmpy = bmp->heigh;
        }
        for (int j = 0; j < tmpy; j++)
        {
            for (int i = 0; i < tmpx; i++)
            {
                memcpy(maplcd + j * lcd_lines + i * 4, tmp - j * bmp->lineBytes + i * 4, 3);
            }
        }
    }
}

//判断坐标事件是否在某个按钮区域里
//只需传入按钮的长宽和它在屏幕的坐标即可判断
bool button_position(xy *otc, int w, int h, int x, int y)
{
    if ((otc->x > x) && (otc->x < (x + w)) && (otc->y > y) && (otc->y < (y + h)))
    {
        return true;
    }
    return false;
}

//登录界面各按钮对应的序号
int logon_button(xy *otc)
{
    if (button_position(otc, 285, 38, 260, 221))
    {
        //账号区域
        return 1;
    }
    else if (button_position(otc, 285, 38, 260, 288))
    {
        //密码区域
        return 2;
    }
    else if (button_position(otc, 144, 36, 232, 369))
    {
        //登录按钮
        return 3;
    }
    else if (button_position(otc, 144, 36, 432, 368))
    {
        //退出按钮
        return 4;
    }
    else
    {
        return 0;
    }
}

//主界面各按钮对应序号
int main_button(xy *otc)
{
    if (button_position(otc, 144, 138, 217, 267))
    {
        //购物
        return 1;
    }
    else if (button_position(otc, 144, 138, 452, 267))
    {
        //设置
        return 2;
    }
    else if (button_position(otc, 50, 40, 730, 420))
    {
        return 3;
    }
    else
    {
        return 0;
    }
}

//货物界面各按钮对应的序号
int commodity_button(xy *otc)
{
    if (button_position(otc, 120, 150, 120, 135))
    {
        //第一个商品
        return 1;
    }
    else if (button_position(otc, 120, 150, 340, 135))
    {
        //第二个商品
        return 2;
    }
    else if (button_position(otc, 120, 150, 560, 135))
    {
        //第三个商品
        return 3;
    }
    else if (button_position(otc, 120, 150, 120, 300))
    {
        //第四个商品
        return 4;
    }
    else if (button_position(otc, 120, 150, 340, 300))
    {
        //第五个商品
        return 5;
    }
    else if (button_position(otc, 120, 150, 560, 300))
    {
        //第六个商品
        return 6;
    }
    else if (button_position(otc, 55, 55, 30, 268))
    {
        //向左
        return 7;
    }
    else if (button_position(otc, 55, 55, 715, 268))
    {
        //向右
        return 8;
    }
    else if (button_position(otc, 35, 35, 740, 420))
    {
        //退出
        return 9;
    }
    else if (button_position(otc, 35, 35, 25, 420))
    {
        //购物车
        return 10;
    }
    else
    {
        return 0;
    }
}

//选购界面各按钮对应序号
int buying_button(xy *otc)
{
    if (button_position(otc, 238, 304, 280, 100))
    {
        if (button_position(otc, 25, 25, 450, 270))
        {
            //增加
            return 1;
        }
        else if (button_position(otc, 25, 25, 386, 270))
        {
            //减少
            return 2;
        }
        else if (button_position(otc, 68, 28, 318, 350))
        {
            //加入
            return 3;
        }
        else if (button_position(otc, 68, 28, 418, 350))
        {
            //购买
            return 4;
        }
        else
        {
            //空白区域
            return 5;
        }
    }
    else
    {
        return 0;
    }
}

//支付界面各按钮对应序号
int pay_button(xy *otc)
{
    if (button_position(otc, 48, 49, 446, 207))
    {
        return 1;
    }
    else if (button_position(otc, 48, 49, 502, 207))
    {
        return 2;
    }
    else if (button_position(otc, 48, 49, 558, 207))
    {
        return 3;
    }
    else if (button_position(otc, 48, 49, 446, 263))
    {
        return 4;
    }
    else if (button_position(otc, 48, 49, 502, 263))
    {
        return 5;
    }
    else if (button_position(otc, 48, 49, 558, 263))
    {
        return 6;
    }
    else if (button_position(otc, 48, 49, 446, 319))
    {
        return 7;
    }
    else if (button_position(otc, 48, 49, 502, 319))
    {
        return 8;
    }
    else if (button_position(otc, 48, 49, 558, 319))
    {
        return 9;
    }
    else if (button_position(otc, 48, 49, 614, 319))
    {
        return 0;
    }
    else if (button_position(otc, 48, 49, 614, 263))
    {
        //'.'
        return 11;
    }
    else if (button_position(otc, 48, 49, 614, 207))
    {
        //'退格键'
        return 12;
    }
    else if (button_position(otc, 103, 43, 446, 388))
    {
        //取消
        return 13;
    }
    else if (button_position(otc, 104, 43, 558, 388))
    {
        //确认
        return 14;
    }
    else if (button_position(otc, 35, 35, 618, 158))
    {
        //x
        return 15;
    }
    else
    {
        return 100;
    }
}

//购物车界面各按钮对应序号
int cart_button(xy *otc)
{
    int x1 = 180, y1 = 40; //-
    int x2 = 244, y2 = 40; //+
    int x3 = 314, y3 = 4;  //x
    if (button_position(otc, 35, 35, 730, 420))
    {
        //退出
        return 0;
    }
    else if (button_position(otc, 50, 50, 39, 413))
    {
        //去结算
        return 1;
    }
    else if (button_position(otc, 50, 50, 125, 411))
    {
        //清空购物车
        return 2;
    }

    else if (button_position(otc, 30, 30, 40 + x1, 140 + y1))
    {
        return 11;
    }
    else if (button_position(otc, 30, 30, 40 + x2, 140 + y2))
    {
        return 12;
    }
    else if (button_position(otc, 30, 30, 40 + x3, 140 + y3))
    {
        return 13;
    }

    else if (button_position(otc, 30, 30, 410 + x1, 140 + y1))
    {
        return 21;
    }
    else if (button_position(otc, 30, 30, 410 + x2, 140 + y2))
    {
        return 22;
    }
    else if (button_position(otc, 30, 30, 410 + x3, 140 + y3))
    {
        return 23;
    }

    else if (button_position(otc, 30, 30, 40 + x1, 280 + y1))
    {
        return 31;
    }
    else if (button_position(otc, 30, 30, 40 + x2, 280 + y2))
    {
        return 32;
    }
    else if (button_position(otc, 30, 30, 40 + x3, 280 + y3))
    {
        return 33;
    }

    else if (button_position(otc, 30, 30, 410 + x1, 280 + y1))
    {
        return 41;
    }
    else if (button_position(otc, 30, 30, 410 + x2, 280 + y2))
    {
        return 42;
    }
    else if (button_position(otc, 30, 30, 410 + x3, 280 + y3))
    {
        return 43;
    }
    else
    {
        return 100;
    }
}

//登录程序
void logon_interface(char *maplcd, xy *otc, node *head, loginfo *acpw, loginfo *stdinacpw, font *f, bitmap *screen)
{

    //获取登录界面图片的信息
    bmpinfo *mainbmp = getrgb("main_interface.bmp");
    bmpinfo *logonbmp = getrgb("logon.bmp");
    bmpinfo *wrongbmp = getrgb("wrong.bmp");

    bmpinfo *password = getrgb("password.bmp");
    //显示登录界面
    xy *coordinates = malloc(sizeof(xy));
    coordinates->x = 0;
    coordinates->y = 0;
    xy *wrongcoor = malloc(sizeof(xy));
    wrongcoor->x = 300;
    wrongcoor->y = 180;
    dspybmp(logonbmp, maplcd, coordinates);

    char tmplcd[800 * 480 * 4];
    char *achint = {"账号"};
    char *pwhint = {"密码"};

    fontSetSize(f, 25);
    fontPrint(f, screen, 304, 228, achint, gray, 800);
    fontPrint(f, screen, 304, 295, pwhint, gray, 800);

    //读取触摸屏信息
    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));

        switch (buf.type)
        {
        //发生了坐标事件
        case EV_ABS:
            switch (buf.code)
            {
            //触发了x坐标
            case ABS_X:
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            //触发了y坐标
            case ABS_Y:
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }
                continue;

            default:
                break;
            }
            continue;

        //发生了触碰事件
        case EV_KEY:
            switch (buf.code)
            {
            case BTN_TOUCH:
                //手指从触摸屏上松开
                if (buf.value == 0)
                {
                    fflush(stdout);
                    switch (logon_button(otc))
                    {
                    case 1:
                        printf("触碰了账号区域\n");
                        stdin_account(maplcd, otc, stdinacpw, f, screen);
                        dspybmp(logonbmp, maplcd, coordinates);
                        if (stdinacpw->flag1 == 0)
                        {
                            fontSetSize(f, 25);
                            fontPrint(f, screen, 304, 228, achint, gray, 800);
                        }
                        else
                        {
                            fontSetSize(f, 30);
                            fontPrint(f, screen, 304, 226, stdinacpw->account, black, 800);
                        }
                        if (stdinacpw->flag2 == 0)
                        {
                            fontSetSize(f, 25);
                            fontPrint(f, screen, 304, 296, pwhint, gray, 800);
                        }
                        else
                        {
                            fontSetSize(f, 30);
                            fontPrint(f, screen, 304, 297, stdinacpw->fake, black, 800);
                        }

                        break;

                    case 2:

                        printf("触碰了密码区域\n");
                        stdin_password(maplcd, otc, stdinacpw, f, screen);
                        dspybmp(logonbmp, maplcd, coordinates);

                        if (stdinacpw->flag1 == 0)
                        {
                            fontSetSize(f, 25);
                            fontPrint(f, screen, 304, 228, achint, gray, 800);
                        }
                        else
                        {
                            fontSetSize(f, 30);
                            fontPrint(f, screen, 304, 226, stdinacpw->account, black, 800);
                        }
                        if (stdinacpw->flag2 == 0)
                        {
                            fontSetSize(f, 25);
                            fontPrint(f, screen, 304, 297, pwhint, gray, 800);
                        }
                        else
                        {
                            fontSetSize(f, 30);
                            fontPrint(f, screen, 304, 296, stdinacpw->fake, black, 800);
                        }
                        break;

                    case 3:
                        printf("触碰了登录按钮\n");
                        if (login(acpw, stdinacpw))
                        {
                            printf("进入增删商品界面\n");
                            bzero(stdinacpw->account, 14);
                            bzero(stdinacpw->password, 14);
                            bzero(stdinacpw->fake, 14);
                            stdinacpw->flag1 = 0;
                            stdinacpw->flag2 = 0;
                            addordel(maplcd, otc, head, f, screen);
                            dspybmp(mainbmp, maplcd, coordinates);
                            return;
                            
                            // fontSetSize(f, 25);
                            // fontPrint(f, screen, 304, 228, achint, gray, 800);
                            // fontPrint(f, screen, 304, 296, pwhint, gray, 800);
                        }
                        else if (login(acpw, stdinacpw) == false)
                        {
                            memcpy(tmplcd, maplcd, 800 * 480 * 4);
                            close(otc->td);
                            dspybmp(wrongbmp, maplcd, wrongcoor);

                            sleep(1);
                            memcpy(maplcd, tmplcd, 800 * 480 * 4);
                            otc->td = open("/dev/input/event0", O_RDWR);
                        }

                        break;
                    case 4:
                        printf("触碰了退出按钮\n");
                        bzero(stdinacpw->account, 14);
                        bzero(stdinacpw->password, 14);
                        bzero(stdinacpw->fake, 14);
                        stdinacpw->flag1 = 0;
                        stdinacpw->flag2 = 0;
                        main_interface(maplcd, otc, head, acpw, stdinacpw, f, screen);
                        break;

                    default:
                        break;
                    }
                }
                break;

            default:
                break;
            }
            continue;
        }
    }
}

//主界面
void main_interface(char *maplcd, xy *otc, node *head, loginfo *acpw, loginfo *stdinacpw, font *f, bitmap *screen)
{
    //显示主界面的图片
    bmpinfo *main_interface = getrgb("main_interface.bmp");
    xy *coordinates = malloc(sizeof(xy));
    coordinates->x = 0;
    coordinates->y = 0;
    dspybmp(main_interface, maplcd, coordinates);
    free(main_interface->rgbmemory);
    free(main_interface);
    free(coordinates);

    //对触摸屏进行判断
    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));
        //判断是否发生触碰事件
        switch (buf.type)
        {
        case EV_ABS: //发生了坐标事件
            switch (buf.code)
            {
            case ABS_X: //触发了x坐标
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            case ABS_Y: //触发了y坐标
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }

                continue;

            default:
                break;
            }
            continue;

        case EV_KEY: //发生了触碰事件
            switch (buf.code)
            {
            case BTN_TOUCH:
                if (buf.value == 0) //手离开了触摸屏
                {
                    fflush(stdout);
                    //判断手是从哪个按钮离开的
                    switch (main_button(otc))
                    {
                    case 1:
                        printf("触碰了购物按钮\n");
                        //调用购物界面
                        commodity_interface(maplcd, otc, head, acpw, stdinacpw, f, screen);
                        break;

                    case 2:
                        printf("触碰了设置按钮\n");
                        //调用登录界面
                        logon_interface(maplcd, otc, head, acpw, stdinacpw, f, screen);
                        break;
                    default:
                        break;
                    }
                }
                break;

            default:
                break;
            }
            continue;
        }
    }
}

//购物界面
void commodity_interface(char *maplcd, xy *otc, node *head, loginfo *acpw, loginfo *stdinacpw, font *f, bitmap *screen)
{
    

    xy a;
    a.x = 0;
    a.y = 0;
    bmpinfo *main_interface = getrgb("main_interface.bmp");
    node *p = malloc(sizeof(node));
    p->data.bmpname = calloc(30, sizeof(char));
    p->data.name = calloc(30, sizeof(char));
    p->data.bmp = malloc(sizeof(bmpinfo));
    p->data.bmp->rgbmemory = calloc(800 * 480 * 4, sizeof(char));

    p = head;
    if (p->next == head)
    {
        printf("暂无商品\n");
        return;
    }

    p = loadthegood_right(head, p, maplcd, f, screen);
    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;

    node *tmpnode = malloc(sizeof(node));
    tmpnode->data.bmpname = calloc(30, sizeof(char));
    tmpnode->data.name = calloc(30, sizeof(char));
    tmpnode->data.bmp = malloc(sizeof(bmpinfo));
    tmpnode->data.bmp->rgbmemory = calloc(800 * 480 * 4, sizeof(char));

    tmpnode = p;

    for (int i = 0; i < (p->count - 1); i++)
    {
        tmpnode = tmpnode->prev;
    }

    node *tmppp = malloc(sizeof(node));
    tmppp->data.bmpname = calloc(30, sizeof(char));
    tmppp->data.name = calloc(30, sizeof(char));
    tmppp->data.bmp = malloc(sizeof(bmpinfo));
    tmppp->data.bmp->rgbmemory = calloc(800 * 480 * 4, sizeof(char));

    char *copymaplcd = calloc(800 * 480 * 4, sizeof(char));
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));
        //判断是否发生触碰事件
        switch (buf.type)
        {
        case EV_ABS:
            switch (buf.code)
            {
            case ABS_X:
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            case ABS_Y:
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }

                continue;

            default:
                break;
            }
            continue;

        case EV_KEY:
            switch (buf.code)
            {
            case BTN_TOUCH:
                if (buf.value == 0)
                {
                    fflush(stdout);
                    switch (commodity_button(otc))
                    {
                    case 1:
                        if (1 > p->count)
                        {
                            printf("此处无商品\n");
                        }
                        else
                        {
                            tmppp = tmpnode;
                            printf("%s\n", tmppp->data.name);
                            memcpy(copymaplcd, maplcd, 800 * 480 * 4);
                            for (int i = 0; i < 800 * 480; i++)
                            {
                                if (i % 2 == 0 || i / 800 % 2 == 0)
                                {
                                    bzero(maplcd + i * 4, 4);
                                }
                            }
                            buying_interface(maplcd, otc, head, tmppp, copymaplcd, f, screen);
                        }
                        break;

                    case 2:
                        if (2 > p->count)
                        {
                            printf("此处无商品\n");
                        }
                        else
                        {
                            tmppp = tmpnode->next;
                            printf("%s\n", tmppp->data.name);
                            memcpy(copymaplcd, maplcd, 800 * 480 * 4);
                            for (int i = 0; i < 800 * 480; i++)
                            {
                                if (i % 2 == 0 || i / 800 % 2 == 0)
                                {
                                    bzero(maplcd + i * 4, 4);
                                }
                            }
                            buying_interface(maplcd, otc, head, tmppp, copymaplcd, f, screen);
                        }
                        break;

                    case 3:
                        if (3 > p->count)
                        {
                            printf("此处无商品\n");
                        }
                        else
                        {
                            tmppp = tmpnode;
                            for (int i = 0; i < 2; i++)
                            {
                                tmppp = tmppp->next;
                            }
                            printf("%s\n", tmppp->data.name);
                            memcpy(copymaplcd, maplcd, 800 * 480 * 4);
                            for (int i = 0; i < 800 * 480; i++)
                            {
                                if (i % 2 == 0 || i / 800 % 2 == 0)
                                {
                                    bzero(maplcd + i * 4, 4);
                                }
                            }
                            buying_interface(maplcd, otc, head, tmppp, copymaplcd, f, screen);
                        }
                        break;
                    case 4:
                        if (4 > p->count)
                        {
                            printf("此处无商品\n");
                        }
                        else
                        {
                            tmppp = tmpnode;
                            for (int i = 0; i < 3; i++)
                            {
                                tmppp = tmppp->next;
                            }
                            printf("%s\n", tmppp->data.name);
                            memcpy(copymaplcd, maplcd, 800 * 480 * 4);
                            for (int i = 0; i < 800 * 480; i++)
                            {
                                if (i % 2 == 0 || i / 800 % 2 == 0)
                                {
                                    bzero(maplcd + i * 4, 4);
                                }
                            }
                            buying_interface(maplcd, otc, head, tmppp, copymaplcd, f, screen);
                        }
                        break;
                    case 5:
                        if (5 > p->count)
                        {
                            printf("此处无商品\n");
                        }
                        else
                        {
                            tmppp = tmpnode;
                            for (int i = 0; i < 4; i++)
                            {
                                tmppp = tmppp->next;
                            }
                            printf("%s\n", tmppp->data.name);
                            memcpy(copymaplcd, maplcd, 800 * 480 * 4);
                            for (int i = 0; i < 800 * 480; i++)
                            {
                                if (i % 2 == 0 || i / 800 % 2 == 0)
                                {
                                    bzero(maplcd + i * 4, 4);
                                }
                            }
                            buying_interface(maplcd, otc, head, tmppp, copymaplcd, f, screen);
                        }
                        break;
                    case 6:
                        if (6 > p->count)
                        {
                            printf("此处无商品\n");
                        }
                        else
                        {
                            tmppp = tmpnode;
                            for (int i = 0; i < 5; i++)
                            {
                                tmppp = tmppp->next;
                            }
                            printf("%s\n", tmppp->data.name);
                            memcpy(copymaplcd, maplcd, 800 * 480 * 4);
                            for (int i = 0; i < 800 * 480; i++)
                            {
                                if (i % 2 == 0 || i / 800 % 2 == 0)
                                {
                                    bzero(maplcd + i * 4, 4);
                                }
                            }
                            buying_interface(maplcd, otc, head, tmppp, copymaplcd, f, screen);
                        }
                        break;
                    case 7:
                        p = loadthegood_left(head, p, maplcd, f, screen);
                        tmpnode = p;
                        for (int i = 0; i < (p->count - 1); i++)
                        {
                            tmpnode = tmpnode->prev;
                        }
                        break;
                    case 8:
                        p = loadthegood_right(head, p, maplcd, f, screen);
                        tmpnode = p;
                        for (int i = 0; i < (p->count - 1); i++)
                        {
                            tmpnode = tmpnode->prev;
                        }
                        break;
                    case 9:

                        dspybmp(main_interface, maplcd, &a);
                        free(main_interface->rgbmemory);
                        free(main_interface);
                        return;
                        break;
                    case 10:
                        memcpy(copymaplcd, maplcd, 800 * 480 * 4);
                        cart_interface(maplcd, otc, head, f, screen);
                        memcpy(maplcd, copymaplcd, 800 * 480 * 4);
                        showcartnum(head, maplcd, f, screen);
                        break;
                    default:
                        break;
                    }
                }
                break;

            default:
                break;
            }
            continue;
        }
    }
}

//选购界面
void buying_interface(char *maplcd, xy *otc, node *head, node *tmppp, char *copymaplcd, font *f, bitmap *screen)
{
    //获取选购界面图片信息
    bmpinfo *buybmp = getrgb("buy2.bmp");
    bmpinfo *cannotuse = getrgb("cannotuse.bmp");
    bmpinfo *join = getrgb("join.bmp");
    xy *buycoor = malloc(sizeof(xy));
    buycoor->x = 280;
    buycoor->y = 100;
    xy *buygoodscoor = malloc(sizeof(xy));
    buygoodscoor->x = 370;
    buygoodscoor->y = 126;
    dspybmp(buybmp, maplcd, buycoor);
    dspybmp(tmppp->data.bmp, maplcd, buygoodscoor);
    fontSetSize(f, 30);
    fontPrint(f, screen, 380, 234, tmppp->data.name, color2, 800);

    char *buyinglcd = calloc(800 * 480 * 4, sizeof(char));
    memcpy(buyinglcd, maplcd, 800 * 480 * 4);

    xy cnucoor;
    cnucoor.x = buycoor->x + 76;
    cnucoor.y = buycoor->y + 116;

    xy joincoor;
    joincoor.x = cnucoor.x + 62;
    joincoor.y = cnucoor.y + 16;

    //初始化选购商品的数量
    int numofcmdy = 0;

    if ((tmppp->data.amount - tmppp->data.cart) >= 1)
    {
        numofcmdy = 1;
    }
    char numbuf[10];
    char pricebuf[10];

    sprintf(numbuf, "%d/%d", numofcmdy, tmppp->data.amount - tmppp->data.cart);
    fontSetSize(f, 20);
    fontPrint(f, screen, 411, 270, numbuf, color2, 800);
    sprintf(pricebuf, "￥%.2f", tmppp->data.price * (float)numofcmdy);
    fontSetSize(f, 31);
    fontPrint(f, screen, 380, 295, pricebuf, color2, 800);

    node *p = head->next;
    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));

        switch (buf.type)
        {
        //发生了坐标事件
        case EV_ABS:
            switch (buf.code)
            {
            //触发了x坐标
            case ABS_X:
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            //触发了y坐标
            case ABS_Y:
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }
                continue;

            default:
                break;
            }
            continue;

        //发生了触碰事件
        case EV_KEY:
            switch (buf.code)
            {
            case BTN_TOUCH:
                //手指从触摸屏上松开
                if (buf.value == 0)
                {
                    fflush(stdout);
                    switch (buying_button(otc))
                    {
                    case 1:
                        printf("增加\n");
                        if (numofcmdy != (tmppp->data.amount - tmppp->data.cart))
                        {
                            numofcmdy++;
                        }
                        sprintf(numbuf, "%d/%d", numofcmdy, tmppp->data.amount - tmppp->data.cart);
                        sprintf(pricebuf, "￥%.2f", tmppp->data.price * (float)numofcmdy);
                        memcpy(maplcd, buyinglcd, 800 * 480 * 4);
                        fontSetSize(f, 20);
                        //让？/？居中
                        if (numofcmdy < 10)
                        {
                            fontPrint(f, screen, 412, 270, numbuf, color2, 800);
                        }
                        else
                        {
                            fontPrint(f, screen, 408, 270, numbuf, color2, 800);
                        }

                        fontSetSize(f, 31);
                        fontPrint(f, screen, 380, 295, pricebuf, color2, 800);
                        
                        break;
                    case 2:
                        printf("减少\n");
                        if (numofcmdy != 0)
                        {
                            numofcmdy--;
                        }
                        sprintf(numbuf, "%d/%d", numofcmdy, tmppp->data.amount - tmppp->data.cart);
                        sprintf(pricebuf, "￥%.2f", tmppp->data.price * (float)numofcmdy);
                        memcpy(maplcd, buyinglcd, 800 * 480 * 4);
                        fontSetSize(f, 20);
                        if (numofcmdy < 10)
                        {
                            fontPrint(f, screen, 412, 270, numbuf, color2, 800);
                        }
                        else
                        {
                            fontPrint(f, screen, 408, 270, numbuf, color2, 800);
                        }
                        fontSetSize(f, 31);
                        fontPrint(f, screen, 380, 295, pricebuf, color2, 800);
                        break;
                    case 3:

                        if (numofcmdy != 0)
                        {

                            printf("加入\n");

                            tmppp->data.cart += numofcmdy;

                            p = head->next;
                            head->data.countcart = 0;
                            while (p != head)
                            {
                                if (p->data.cart != 0)
                                {
                                    head->data.countcart++;
                                }
                                p = p->next;
                            }

                            if (head->data.countcart > 4)
                            {
                                tmppp->data.cart = 0;
                                printf("已经达到上限\n");
                                close(otc->td);
                                char a[800 * 480 * 4];
                                memcpy(a, maplcd, 800 * 480 * 4);
                                dspybmp(cannotuse, maplcd, &cnucoor);
                                usleep(500000);
                                memcpy(maplcd, a, 800 * 480 * 4);
                                otc->td = open("/dev/input/event0", O_RDWR);

                                
                                p = head->next;
                                head->data.countcart = 0;
                                while (p != head)
                                {
                                    if (p->data.cart != 0)
                                    {
                                        head->data.countcart++;
                                    }
                                    p = p->next;
                                }
                                printf("购物车现存数量：%d\n",head->data.countcart);
                            }
                            else
                            {
                                printf("加入成功\n");
                                close(otc->td);
                                char joinbuf[10];
                                bzero(joinbuf, 10);
                                sprintf(joinbuf, "%d", numofcmdy);
                                fontSetSize(f, 20);

                                dspybmp(join, maplcd, &cnucoor);
                                fontPrint(f, screen, joincoor.x, joincoor.y, joinbuf, color1, 800);
                                usleep(100000);

                                otc->td = open("/dev/input/event0", O_RDWR);
                            }

                            if ((tmppp->data.amount - tmppp->data.cart) >= 1)
                            {
                                numofcmdy = 1;
                            }
                            else
                            {
                                numofcmdy = 0;
                            }
                            sprintf(numbuf, "%d/%d", numofcmdy, tmppp->data.amount - tmppp->data.cart);
                            sprintf(pricebuf, "￥%.2f", tmppp->data.price * (float)numofcmdy);

                            memcpy(maplcd, buyinglcd, 800 * 480 * 4);

                            fontSetSize(f, 20);
                            fontPrint(f, screen, 411, 270, numbuf, color2, 800);

                            fontSetSize(f, 31);
                            fontPrint(f, screen, 380, 295, pricebuf, color2, 800);
                        }

                        break;
                    case 4:
                        printf("购买\n");
                        if (numofcmdy != 0)
                        {
                            char *ordertime = getime();
                            pay_interface(maplcd, otc, tmppp, copymaplcd, f, screen, numofcmdy, ordertime);
                            memcpy(maplcd, copymaplcd, 800 * 480 * 4);
                            showcartnum(head, maplcd, f, screen);
                            return;
                        }

                        break;
                    case 0:
                        memcpy(maplcd, copymaplcd, 800 * 480 * 4);
                        showcartnum(head, maplcd, f, screen);
                        return;

                    default:
                        break;
                    }
                }
                break;

            default:
                break;
            }
            continue;
        }
    }
}

//购物车界面
void cart_interface(char *maplcd, xy *otc, node *head, font *f, bitmap *screen)
{
    xy coor;
    coor.x = 0;
    coor.y = 0;
    node *p = head->next;
    bmpinfo *cart = getrgb("cart.bmp");
    xy bmpcoor;
    char much[10];

    bmpinfo *singlecart = getrgb("singlecart.bmp");

    bmpinfo *cart1 = getrgb("cart1.bmp");
    if (head->data.countcart == 0)
    {

        dspybmp(cart, maplcd, &coor);
        printf("购物车为空\n");
    }
    else
    {

        dspybmp(cart1, maplcd, &coor);
        for (int i = 0; i < head->data.countcart; i++)
        {
            if (i % 2 == 0)
            {
                coor.x = 40;
            }
            else
            {
                coor.x = 410;
            }

            if (i / 2 == 0)
            {
                coor.y = 140;
            }
            else
            {
                coor.y = 280;
            }
            dspybmp(singlecart, maplcd, &coor);
            while (p != head)
            {
                if (p->data.cart != 0)
                {
                    bmpcoor.x = 22 + coor.x;
                    bmpcoor.y = 3 + coor.y;
                    dspybmp(p->data.bmp, maplcd, &bmpcoor);
                    fontSetSize(f, 30);
                    fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                    sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                    fontSetSize(f, 18);
                    fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                    break;
                }
                p = p->next;
            }
            p = p->next;
        }
    }

    int count = 0;
    struct input_event buf;
    int tmp1 = 0, tmp2 = 1;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(otc->td, &buf, sizeof(buf));
        //判断是否发生触碰事件
        switch (buf.type)
        {
        case EV_ABS: //发生了坐标事件
            switch (buf.code)
            {
            case ABS_X: //触发了x坐标
                if (tmp2 == 1)
                {
                    otc->x = (float)buf.value / 1.28;
                    //printf("(%d,", otc->x);
                    tmp1 = 1;
                    tmp2 = 0;
                }
                continue;
            case ABS_Y: //触发了y坐标
                if (tmp1 == 1)
                {
                    otc->y = (float)buf.value / 1.25;
                    //printf("%d)\n", otc->y);
                    tmp1 = 0;
                    tmp2 = 1;
                }

                continue;

            default:
                break;
            }
            continue;

        case EV_KEY: //发生了触碰事件
            switch (buf.code)
            {
            case BTN_TOUCH:
                if (buf.value == 0) //手离开了触摸屏
                {
                    fflush(stdout);
                    //判断手是从哪个按钮离开的
                    switch (cart_button(otc))
                    {
                    case 1: //去结算
                        if (head->data.countcart == 0)
                        {
                            printf("暂无商品\n");
                        }
                        else
                        {
                            char buf[800 * 480 * 4];
                            memcpy(buf, maplcd, 800 * 480 * 4);
                            char *ordertime = getime();

                            if (cartpay_interface(maplcd, otc, head, f, screen, ordertime) == false)
                            {
                                memcpy(maplcd, buf, 800 * 480 * 4);
                            }
                            else
                                return;
                        }
                        break;
                    case 2:
                        //清空购物车
                        p = head->next;
                        while (p != head)
                        {
                            p->data.cart = 0;
                            p = p->next;
                        }
                        head->data.countcart = 0;
                        xy cartcoor;
                        cartcoor.x = 0;
                        cartcoor.y = 0;
                        dspybmp(cart, maplcd, &cartcoor);

                        break;
                    case 11:

                        //购物车里的第一个商品至少存在
                        if (head->data.countcart >= 1)
                        {

                            p = head->next;
                            //遍历寻找第一个商品
                            while (p != head)
                            {
                                if (p->data.cart > 0)
                                {

                                    p->data.cart--;

                                    //如果它的数量被减到0
                                    if (p->data.cart == 0)
                                    {
                                        //此时购物车商品类数-1
                                        head->data.countcart--;
                                        //如果购物车里还有别的商品
                                        if (head->data.countcart != 0)
                                        {
                                            //覆盖原来所有信息
                                            coor.x = 0;
                                            coor.y = 0;
                                            dspybmp(cart1, maplcd, &coor);
                                            for (int i = 0; i < head->data.countcart; i++)
                                            {
                                                if (i % 2 == 0)
                                                {
                                                    coor.x = 40;
                                                }
                                                else
                                                {
                                                    coor.x = 410;
                                                }

                                                if (i / 2 == 0)
                                                {
                                                    coor.y = 140;
                                                }
                                                else
                                                {
                                                    coor.y = 280;
                                                }
                                                dspybmp(singlecart, maplcd, &coor);
                                                while (p != head)
                                                {
                                                    if (p->data.cart != 0)
                                                    {
                                                        bmpcoor.x = 22 + coor.x;
                                                        bmpcoor.y = 3 + coor.y;
                                                        dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                                        fontSetSize(f, 30);
                                                        fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                                        sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                                        fontSetSize(f, 18);
                                                        fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                                        break;
                                                    }
                                                    p = p->next;
                                                }
                                                p = p->next;
                                            }
                                        }
                                        else
                                        {
                                            coor.x = 0;
                                            coor.y = 0;
                                            dspybmp(cart, maplcd, &coor);
                                        }
                                    }
                                    //如果它的数量没有被减到0
                                    //把该商品所在框刷新一遍
                                    else
                                    {
                                        coor.x = 40;
                                        coor.y = 140;

                                        dspybmp(singlecart, maplcd, &coor);
                                        bmpcoor.x = 22 + coor.x;
                                        bmpcoor.y = 3 + coor.y;
                                        dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                        fontSetSize(f, 30);
                                        fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                        sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                        fontSetSize(f, 18);
                                        fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                    }
                                    break;
                                }
                                p = p->next;
                            }
                        }
                        break;
                    case 12:
                        if (head->data.countcart >= 1)
                        {
                            p = head->next;
                            while (p != head)
                            {
                                if (p->data.cart > 0 && p->data.cart < p->data.amount)
                                {
                                    p->data.cart++;
                                    coor.x = 40;
                                    coor.y = 140;

                                    dspybmp(singlecart, maplcd, &coor);
                                    bmpcoor.x = 22 + coor.x;
                                    bmpcoor.y = 3 + coor.y;
                                    dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                    fontSetSize(f, 30);
                                    fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                    sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                    fontSetSize(f, 18);
                                    fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);

                                    break;
                                }
                                p = p->next;
                            }
                        }
                        break;
                    case 13:

                        if (head->data.countcart >= 1)
                        {
                            p = head->next;
                            while (p != head)
                            {
                                if (p->data.cart > 0)
                                {
                                    p->data.cart = 0;
                                    head->data.countcart--;
                                    //如果购物车里还有别的商品
                                    if (head->data.countcart != 0)
                                    {
                                        //覆盖原来所有信息
                                        coor.x = 0;
                                        coor.y = 0;
                                        dspybmp(cart1, maplcd, &coor);
                                        for (int i = 0; i < head->data.countcart; i++)
                                        {
                                            if (i % 2 == 0)
                                            {
                                                coor.x = 40;
                                            }
                                            else
                                            {
                                                coor.x = 410;
                                            }

                                            if (i / 2 == 0)
                                            {
                                                coor.y = 140;
                                            }
                                            else
                                            {
                                                coor.y = 280;
                                            }
                                            dspybmp(singlecart, maplcd, &coor);
                                            while (p != head)
                                            {
                                                if (p->data.cart != 0)
                                                {
                                                    bmpcoor.x = 22 + coor.x;
                                                    bmpcoor.y = 3 + coor.y;
                                                    dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                                    fontSetSize(f, 30);
                                                    fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                                    sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                                    fontSetSize(f, 18);
                                                    fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                                    break;
                                                }
                                                p = p->next;
                                            }
                                            p = p->next;
                                        }
                                    }
                                    else
                                    {
                                        coor.x = 0;
                                        coor.y = 0;
                                        dspybmp(cart, maplcd, &coor);
                                    }

                                    break;
                                }
                                p = p->next;
                            }
                        }
                        break;
                    case 21:
                        //这个商品至少是存在的
                        if (head->data.countcart >= 2)
                        {
                            int count = 0;
                            p = head->next;
                            //遍历寻找第二个商品
                            while (p != head && count != 2)
                            {
                                if (p->data.cart > 0)
                                {
                                    count++;
                                }
                                p = p->next;
                            }
                            p = p->prev;

                            p->data.cart--;

                            //如果它的数量被减到0
                            if (p->data.cart == 0)
                            {
                                p = head->next;
                                //此时购物车商品类数-1
                                head->data.countcart--;
                                //此时购物车里肯定还有别的商品

                                //覆盖原来所有信息
                                coor.x = 0;
                                coor.y = 0;
                                dspybmp(cart1, maplcd, &coor);
                                for (int i = 0; i < head->data.countcart; i++)
                                {
                                    if (i % 2 == 0)
                                    {
                                        coor.x = 40;
                                    }
                                    else
                                    {
                                        coor.x = 410;
                                    }

                                    if (i / 2 == 0)
                                    {
                                        coor.y = 140;
                                    }
                                    else
                                    {
                                        coor.y = 280;
                                    }
                                    dspybmp(singlecart, maplcd, &coor);

                                    while (p != head)
                                    {
                                        if (p->data.cart != 0)
                                        {
                                            bmpcoor.x = 22 + coor.x;
                                            bmpcoor.y = 3 + coor.y;
                                            dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                            fontSetSize(f, 30);
                                            fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                            sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                            fontSetSize(f, 18);
                                            fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                            break;
                                        }
                                        p = p->next;
                                    }
                                    p = p->next;
                                }
                            }
                            //如果它的数量没有被减到0
                            //把该商品所在框刷新一遍
                            else
                            {
                                coor.x = 410;
                                coor.y = 140;

                                dspybmp(singlecart, maplcd, &coor);
                                bmpcoor.x = 22 + coor.x;
                                bmpcoor.y = 3 + coor.y;
                                dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                fontSetSize(f, 30);
                                fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                fontSetSize(f, 18);
                                fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                            }
                            break;
                        }
                        break;
                    case 22:
                        if (head->data.countcart >= 2)
                        {
                            int count = 0;
                            p = head->next;
                            while (p != head && count != 2)
                            {

                                if (p->data.cart > 0)
                                {

                                    count++;
                                }
                                p = p->next;
                            }
                            p = p->prev;
                            p->data.cart++;
                            coor.x = 410;
                            coor.y = 140;

                            dspybmp(singlecart, maplcd, &coor);
                            bmpcoor.x = 22 + coor.x;
                            bmpcoor.y = 3 + coor.y;
                            dspybmp(p->data.bmp, maplcd, &bmpcoor);
                            fontSetSize(f, 30);
                            fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                            sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                            fontSetSize(f, 18);
                            fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                        }
                        break;
                    case 23:
                        if (head->data.countcart >= 2)
                        {
                            int count = 0;
                            p = head->next;
                            while (p != head && count != 2)
                            {
                                if (p->data.cart > 0)
                                {
                                    count++;
                                }
                                p = p->next;
                            }
                            p = p->prev;

                            p->data.cart = 0;
                            head->data.countcart--;
                            //此时购物车肯定还有别的商品

                            
                            //覆盖原来所有信息
                            coor.x = 0;
                            coor.y = 0;
                            dspybmp(cart1, maplcd, &coor);
                            p = head->next;
                            for (int i = 0; i < head->data.countcart; i++)
                            {
                                if (i % 2 == 0)
                                {
                                    coor.x = 40;
                                }
                                else
                                {
                                    coor.x = 410;
                                }

                                if (i / 2 == 0)
                                {
                                    coor.y = 140;
                                }
                                else
                                {
                                    coor.y = 280;
                                }
                                dspybmp(singlecart, maplcd, &coor);
                                
                                while (p != head)
                                {
                                    if (p->data.cart != 0)
                                    {
                                        bmpcoor.x = 22 + coor.x;
                                        bmpcoor.y = 3 + coor.y;
                                        dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                        fontSetSize(f, 30);
                                        fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                        sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                        fontSetSize(f, 18);
                                        fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                        break;
                                    }
                                    p = p->next;
                                }
                                p = p->next;
                            }
                        }
                        break;

                        case 31:
                            if (head->data.countcart >= 3)
                            {
                                int count = 0;
                                p = head->next;
                                //遍历寻找第二个商品
                                while (p != head && count != 3)
                                {
                                    if (p->data.cart > 0)
                                    {
                                        count++;
                                    }
                                    p = p->next;
                                }
                                p = p->prev;

                                p->data.cart--;

                                //如果它的数量被减到0
                                if (p->data.cart == 0)
                                {
                                    p = head->next;
                                    //此时购物车商品类数-1
                                    head->data.countcart--;
                                    //此时购物车里肯定还有别的商品

                                    //覆盖原来所有信息
                                    coor.x = 0;
                                    coor.y = 0;
                                    dspybmp(cart1, maplcd, &coor);
                                    for (int i = 0; i < head->data.countcart; i++)
                                    {
                                        if (i % 2 == 0)
                                        {
                                            coor.x = 40;
                                        }
                                        else
                                        {
                                            coor.x = 410;
                                        }

                                        if (i / 2 == 0)
                                        {
                                            coor.y = 140;
                                        }
                                        else
                                        {
                                            coor.y = 280;
                                        }
                                        dspybmp(singlecart, maplcd, &coor);

                                        while (p != head)
                                        {
                                            if (p->data.cart != 0)
                                            {
                                                bmpcoor.x = 22 + coor.x;
                                                bmpcoor.y = 3 + coor.y;
                                                dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                                fontSetSize(f, 30);
                                                fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                                sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                                fontSetSize(f, 18);
                                                fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                                break;
                                            }
                                            p = p->next;
                                        }
                                        p = p->next;
                                    }
                                }
                                //如果它的数量没有被减到0
                                //把该商品所在框刷新一遍
                                else
                                {
                                    coor.x = 40;
                                    coor.y = 280;

                                    dspybmp(singlecart, maplcd, &coor);
                                    bmpcoor.x = 22 + coor.x;
                                    bmpcoor.y = 3 + coor.y;
                                    dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                    fontSetSize(f, 30);
                                    fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                    sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                    fontSetSize(f, 18);
                                    fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                }
                                break;
                            }
                            break;
                            
                        case 32:
                            if (head->data.countcart >= 3)
                            {
                                int count = 0;
                                p = head->next;
                                while (p != head && count != 3)
                                {

                                    if (p->data.cart > 0)
                                    {

                                        count++;
                                    }
                                    p = p->next;
                                }
                                p = p->prev;
                                p->data.cart++;
                                coor.x = 40;
                                coor.y = 280;

                                dspybmp(singlecart, maplcd, &coor);
                                bmpcoor.x = 22 + coor.x;
                                bmpcoor.y = 3 + coor.y;
                                dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                fontSetSize(f, 30);
                                fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                fontSetSize(f, 18);
                                fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                            }
                            break;
                        case 33:
                            if (head->data.countcart >= 3)
                            {
                                int count = 0;
                                p = head->next;
                                while (p != head && count != 3)
                                {
                                    if (p->data.cart > 0)
                                    {
                                        count++;
                                    }
                                    p = p->next;
                                }
                                p = p->prev;

                                p->data.cart = 0;
                                head->data.countcart--;
                                //此时购物车肯定还有别的商品

                                //覆盖原来所有信息
                                coor.x = 0;
                                coor.y = 0;
                                dspybmp(cart1, maplcd, &coor);
                                p = head->next;
                                for (int i = 0; i < head->data.countcart; i++)
                                {
                                    if (i % 2 == 0)
                                    {
                                        coor.x = 40;
                                    }
                                    else
                                    {
                                        coor.x = 410;
                                    }

                                    if (i / 2 == 0)
                                    {
                                        coor.y = 140;
                                    }
                                    else
                                    {
                                        coor.y = 280;
                                    }
                                    dspybmp(singlecart, maplcd, &coor);

                                    while (p != head)
                                    {
                                        if (p->data.cart != 0)
                                        {
                                            bmpcoor.x = 22 + coor.x;
                                            bmpcoor.y = 3 + coor.y;
                                            dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                            fontSetSize(f, 30);
                                            fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                            sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                            fontSetSize(f, 18);
                                            fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                            break;
                                        }
                                        p = p->next;
                                    }
                                    p = p->next;
                                }
                            }
                            break;
                        case 41:
                            if (head->data.countcart >= 4)
                            {
                                int count = 0;
                                p = head->next;
                                //遍历寻找第二个商品
                                while (p != head && count != 4)
                                {
                                    if (p->data.cart > 0)
                                    {
                                        count++;
                                    }
                                    p = p->next;
                                }
                                p = p->prev;

                                p->data.cart--;

                                //如果它的数量被减到0
                                if (p->data.cart == 0)
                                {
                                    p = head->next;
                                    //此时购物车商品类数-1
                                    head->data.countcart--;
                                    //此时购物车里肯定还有别的商品

                                    //覆盖原来所有信息
                                    coor.x = 0;
                                    coor.y = 0;
                                    dspybmp(cart1, maplcd, &coor);
                                    for (int i = 0; i < head->data.countcart; i++)
                                    {
                                        if (i % 2 == 0)
                                        {
                                            coor.x = 40;
                                        }
                                        else
                                        {
                                            coor.x = 410;
                                        }

                                        if (i / 2 == 0)
                                        {
                                            coor.y = 140;
                                        }
                                        else
                                        {
                                            coor.y = 280;
                                        }
                                        dspybmp(singlecart, maplcd, &coor);

                                        while (p != head)
                                        {
                                            if (p->data.cart != 0)
                                            {
                                                bmpcoor.x = 22 + coor.x;
                                                bmpcoor.y = 3 + coor.y;
                                                dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                                fontSetSize(f, 30);
                                                fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                                sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                                fontSetSize(f, 18);
                                                fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                                break;
                                            }
                                            p = p->next;
                                        }
                                        p = p->next;
                                    }
                                }
                                //如果它的数量没有被减到0
                                //把该商品所在框刷新一遍
                                else
                                {
                                    coor.x = 410;
                                    coor.y = 280;

                                    dspybmp(singlecart, maplcd, &coor);
                                    bmpcoor.x = 22 + coor.x;
                                    bmpcoor.y = 3 + coor.y;
                                    dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                    fontSetSize(f, 30);
                                    fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                    sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                    fontSetSize(f, 18);
                                    fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                }
                                
                            }
                            break;
                        case 42:
                            if (head->data.countcart >= 4)
                            {
                                int count = 0;
                                p = head->next;
                                while (p != head && count != 4)
                                {

                                    if (p->data.cart > 0)
                                    {

                                        count++;
                                    }
                                    p = p->next;
                                }
                                p = p->prev;
                                p->data.cart++;
                                coor.x = 40;
                                coor.y = 280;

                                dspybmp(singlecart, maplcd, &coor);
                                bmpcoor.x = 22 + coor.x;
                                bmpcoor.y = 3 + coor.y;
                                dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                fontSetSize(f, 30);
                                fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                fontSetSize(f, 18);
                                fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                            }
                            break;
                        case 43:
                            if (head->data.countcart >= 4)
                            {
                                int count = 0;
                                p = head->next;
                                while (p != head && count != 4)
                                {
                                    if (p->data.cart > 0)
                                    {
                                        count++;
                                    }
                                    p = p->next;
                                }
                                p = p->prev;

                                p->data.cart = 0;
                                head->data.countcart--;
                                //此时购物车肯定还有别的商品

                                //覆盖原来所有信息
                                coor.x = 0;
                                coor.y = 0;
                                dspybmp(cart1, maplcd, &coor);
                                p = head->next;
                                for (int i = 0; i < head->data.countcart; i++)
                                {
                                    if (i % 2 == 0)
                                    {
                                        coor.x = 40;
                                    }
                                    else
                                    {
                                        coor.x = 410;
                                    }

                                    if (i / 2 == 0)
                                    {
                                        coor.y = 140;
                                    }
                                    else
                                    {
                                        coor.y = 280;
                                    }
                                    dspybmp(singlecart, maplcd, &coor);

                                    while (p != head)
                                    {
                                        if (p->data.cart != 0)
                                        {
                                            bmpcoor.x = 22 + coor.x;
                                            bmpcoor.y = 3 + coor.y;
                                            dspybmp(p->data.bmp, maplcd, &bmpcoor);
                                            fontSetSize(f, 30);
                                            fontPrint(f, screen, coor.x + 92, coor.y + 41, p->data.name, purple, 800);
                                            sprintf(much, "%d/%d", p->data.cart, p->data.amount);
                                            fontSetSize(f, 18);
                                            fontPrint(f, screen, coor.x + 210, coor.y + 47, much, purple, 800);
                                            break;
                                        }
                                        p = p->next;
                                    }
                                    p = p->next;
                                }
                            }
                            break;

                        case 0:
                            return;

                            break;
                        default:
                            break;
                        }
                    }
                    break;

                default:
                    break;
                }
                continue;
            }
        }
    }

    //支付界面
bool pay_interface(char *maplcd, xy *otc, node *tmppp, char *copymaplcd, font *f, bitmap *screen, int numofcmdy, char *ordertime)
    {
        bmpinfo *paybmp = getrgb("pay.bmp");
        bmpinfo *xbmp = getrgb("x.bmp");
        bmpinfo *febmp = getrgb("formalerror.bmp");
        bmpinfo *payfail = getrgb("payfail.bmp");
        bmpinfo *paysuccess = getrgb("paysuccess.bmp");
        xy failcoor;
        failcoor.x = 300;
        failcoor.y = 240;

        xy paycoor;
        paycoor.x = 0;
        paycoor.y = 0;
        dspybmp(paybmp, maplcd, &paycoor);

        paycoor.x = 156;
        paycoor.y = 190;
        dspybmp(tmppp->data.bmp, maplcd, &paycoor);

        paycoor.x = 618;
        paycoor.y = 158;

        xy pscoor;
        pscoor.x = 0;
        pscoor.y = 0;

        char namebuf[20];
        char numbuf[10];
        char pricebuf[15];
        char tpricebuf[15];
        char *hint = {"请输入消费金额"};

        sprintf(namebuf, tmppp->data.name);
        sprintf(numbuf, "%d", numofcmdy);
        sprintf(pricebuf, "￥%.2f", tmppp->data.price);
        sprintf(tpricebuf, "￥%.2f", tmppp->data.price * numofcmdy);

        fontSetSize(f, 25);
        fontPrint(f, screen, 160, 302, namebuf, black, 800);
        fontPrint(f, screen, 160, 351, numbuf, black, 800);
        fontPrint(f, screen, 158, 327, pricebuf, black, 800);
        fontPrint(f, screen, 190, 399, tpricebuf, black, 800);
        char paybuf[800 * 480 * 4];
        memcpy(paybuf, maplcd, 800 * 480 * 4);

        fontSetSize(f, 25);
        fontPrint(f, screen, 462, 163, hint, gray, 800);

        char input[10];
        int icount = -1;
        bzero(input, 10);

        struct input_event buf;
        int tmp1 = 0, tmp2 = 1;
        while (1)
        {
            bzero(&buf, sizeof(buf));
            read(otc->td, &buf, sizeof(buf));
            //判断是否发生触碰事件
            switch (buf.type)
            {
            case EV_ABS: //发生了坐标事件
                switch (buf.code)
                {
                case ABS_X: //触发了x坐标
                    if (tmp2 == 1)
                    {
                        otc->x = (float)buf.value / 1.28;
                        //printf("(%d,", otc->x);
                        tmp1 = 1;
                        tmp2 = 0;
                    }
                    continue;
                case ABS_Y: //触发了y坐标
                    if (tmp1 == 1)
                    {
                        otc->y = (float)buf.value / 1.25;
                        //printf("%d)\n", otc->y);
                        tmp1 = 0;
                        tmp2 = 1;
                    }

                    continue;

                default:
                    break;
                }
                continue;

            case EV_KEY: //发生了触碰事件
                switch (buf.code)
                {
                case BTN_TOUCH:
                    if (buf.value == 0) //手离开了触摸屏
                    {
                        fflush(stdout);
                        //判断手是从哪个按钮离开的
                        if ((pay_button(otc) >= 0) && (pay_button(otc) <= 9))
                        {
                            if (icount < 8)
                            {
                                if (input[0] != '0' || (input[0] == '0' && input[1] == '.'))
                                {
                                    sprintf(&input[++icount], "%d", pay_button(otc));
                                    memcpy(maplcd, paybuf, 800 * 480 * 4);
                                    fontPrint(f, screen, 462, 163, input, black, 800);
                                    if (icount >= 0)
                                    {
                                        dspybmp(xbmp, maplcd, &paycoor);
                                    }
                                }
                            }
                        }
                        else if (pay_button(otc) == 11)
                        {
                            int flag = 0;
                            if (icount < 7 && icount > -1)
                            {
                                for (int i = 0; i <= icount; i++)
                                {
                                    if (input[i] == '.')
                                    {
                                        flag = 1;
                                        break;
                                    }
                                }
                                if (flag != 1)
                                {
                                    flag = 0;
                                    input[++icount] = '.';
                                    memcpy(maplcd, paybuf, 800 * 480 * 4);
                                    fontPrint(f, screen, 462, 163, input, black, 800);
                                    if (icount >= 0)
                                    {
                                        dspybmp(xbmp, maplcd, &paycoor);
                                    }
                                }
                            }
                        }
                        else if (pay_button(otc) == 12)
                        {
                            if (icount != -1)
                            {
                                input[icount] = '\0';
                                icount--;
                                memcpy(maplcd, paybuf, 800 * 480 * 4);
                                fontPrint(f, screen, 462, 163, input, black, 800);
                                if (icount >= 0)
                                {
                                    dspybmp(xbmp, maplcd, &paycoor);
                                }
                            }
                            if (icount == -1)
                            {
                                memcpy(maplcd, paybuf, 800 * 480 * 4);
                                fontPrint(f, screen, 462, 163, hint, gray, 800);
                            }
                        }

                        else if (pay_button(otc) == 13)
                        {
                            return false;
                        }
                        else if (pay_button(otc) == 15)
                        {
                            if (icount != -1)
                            {
                                bzero(input, 10);
                                icount = -1;
                                memcpy(maplcd, paybuf, 800 * 480 * 4);
                                fontPrint(f, screen, 462, 163, hint, gray, 800);
                            }
                        }
                        else if (pay_button(otc) == 14)
                        {
                            char inputingbuf[800 * 480 * 4];
                            memcpy(inputingbuf, maplcd, 800 * 480 * 4);
                            if ((input[0] == '0' && input[1] != '.') || (input[icount] == '.'))
                            {
                                printf("格式错误\n");
                                dspybmp(febmp, maplcd, &failcoor);
                                close(otc->td);
                                sleep(1);
                                otc->td = open("/dev/input/event0", O_RDWR);
                                memcpy(maplcd, inputingbuf, 800 * 480 * 4);
                            }
                            else
                            {
                                float pay = 0;
                                float totalprice = tmppp->data.price * numofcmdy;
                                input[5]='\0';
                                sscanf(input, "%f", &pay);
                                float changemoney = pay - totalprice;

                                if (pay < numofcmdy * tmppp->data.price)
                                {
                                    printf("支付失败\n");
                                    dspybmp(payfail, maplcd, &failcoor);
                                    close(otc->td);
                                    sleep(1);
                                    otc->td = open("/dev/input/event0", O_RDWR);
                                    memcpy(maplcd, inputingbuf, 800 * 480 * 4);
                                }
                                else if (pay >= numofcmdy * tmppp->data.price)
                                {
                                    printf("支付成功");
                                    dspybmp(paysuccess, maplcd, &pscoor);
                                    fontSetSize(f, 18);
                                    srand((unsigned)time(NULL));
                                    int n = rand() % 1000000000;
                                    char random[10];
                                    char change[10];
                                    sprintf(change, "￥%.2f", changemoney);
                                    sprintf(random, "%d", n);

                                    fontPrint(f, screen, 317, 230, random, black, 800);
                                    fontPrint(f, screen, 317, 263, ordertime, black, 800);
                                    fontPrint(f, screen, 317, 363, numbuf, black, 800);
                                    fontPrint(f, screen, 317, 329, change, black, 800);
                                    fontPrint(f, screen, 317, 396, getime(), black, 800);
                                    fontSetSize(f, 60);
                                    sprintf(tpricebuf, "%.2f", tmppp->data.price * numofcmdy);
                                    fontPrint(f, screen, 375, 128, tpricebuf, black, 800);
                                    tmppp->data.amount -= numofcmdy;

                                    struct input_event buf;
                                    int tmp1 = 0, tmp2 = 1;
                                    while (1)
                                    {
                                        bzero(&buf, sizeof(buf));
                                        read(otc->td, &buf, sizeof(buf));
                                        //判断是否发生触碰事件
                                        switch (buf.type)
                                        {
                                        case EV_ABS: //发生了坐标事件
                                            switch (buf.code)
                                            {
                                            case ABS_X: //触发了x坐标
                                                if (tmp2 == 1)
                                                {
                                                    otc->x = (float)buf.value / 1.28;
                                                    //printf("(%d,", otc->x);
                                                    tmp1 = 1;
                                                    tmp2 = 0;
                                                }
                                                continue;
                                            case ABS_Y: //触发了y坐标
                                                if (tmp1 == 1)
                                                {
                                                    otc->y = (float)buf.value / 1.25;
                                                    //printf("%d)\n", otc->y);
                                                    tmp1 = 0;
                                                    tmp2 = 1;
                                                }

                                                continue;

                                            default:
                                                break;
                                            }
                                            continue;

                                        case EV_KEY: //发生了触碰事件
                                            switch (buf.code)
                                            {
                                            case BTN_TOUCH:
                                                if (buf.value == 0) //手离开了触摸屏
                                                {
                                                    fflush(stdout);
                                                    //判断手是从哪个按钮离开的
                                                    if (button_position(otc, 124, 41, 356, 427))
                                                    {
                                                        return true;
                                                    }
                                                }
                                                break;

                                            default:
                                                break;
                                            }
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
                }
                continue;
            }
        }
    }

    //购物车结算界面
bool cartpay_interface(char *maplcd, xy *otc, node *head, font *f, bitmap *screen, char *ordertime)
    {

        bmpinfo *cartpay = getrgb("cartpay.bmp");
        xy coor;
        coor.x = 0;
        coor.y = 0;
        dspybmp(cartpay, maplcd, &coor);

        bmpinfo *cart = getrgb("cart.bmp");
        bmpinfo *xbmp = getrgb("x.bmp");
        bmpinfo *febmp = getrgb("formalerror.bmp");
        bmpinfo *payfail = getrgb("payfail.bmp");
        bmpinfo *paysuccess = getrgb("paysuccess.bmp");

        xy failcoor;
        failcoor.x = 300;
        failcoor.y = 240;

        xy xcoor;
        xcoor.x = 618;
        xcoor.y = 158;

        char infobuf[30];
        char tpricebuf[10];

        char *hint = {"请输入消费金额"};

        node *p = head->next;
        xy infocoor;
        infocoor.x = 120;
        infocoor.y = 222;
        int count = 0;
        int totalnum = 0;
        float totalprice = 0;

        fontSetSize(f, 20);
        while ((p != head) && (count != 4))
        {

            if (p->data.cart != 0)
            {
                sprintf(infobuf, "%s  %.2fx%d  %.2f", p->data.name, p->data.price, p->data.cart, p->data.cart * p->data.price);
                fontPrint(f, screen, infocoor.x, infocoor.y + count * 30, infobuf, black, 800);
                totalnum += p->data.cart;
                count++;
                totalprice += p->data.cart * p->data.price;
            }
            p = p->next;
        }

        sprintf(tpricebuf, "￥%.2f", totalprice);
        fontSetSize(f, 30);
        fontPrint(f, screen, 198, 375, tpricebuf, black, 800);

        char cartpaybuf[800 * 480 * 4];
        memcpy(cartpaybuf, maplcd, 800 * 480 * 4);

        fontSetSize(f, 25);
        fontPrint(f, screen, 462, 163, hint, gray, 800);

        char input[10];
        int icount = -1;
        bzero(input, 10);

        struct input_event buf;
        int tmp1 = 0, tmp2 = 1;
        while (1)
        {
            bzero(&buf, sizeof(buf));
            read(otc->td, &buf, sizeof(buf));
            //判断是否发生触碰事件
            switch (buf.type)
            {
            case EV_ABS: //发生了坐标事件
                switch (buf.code)
                {
                case ABS_X: //触发了x坐标
                    if (tmp2 == 1)
                    {
                        otc->x = (float)buf.value / 1.28;
                        //printf("(%d,", otc->x);
                        tmp1 = 1;
                        tmp2 = 0;
                    }
                    continue;
                case ABS_Y: //触发了y坐标
                    if (tmp1 == 1)
                    {
                        otc->y = (float)buf.value / 1.25;
                        //printf("%d)\n", otc->y);
                        tmp1 = 0;
                        tmp2 = 1;
                    }

                    continue;

                default:
                    break;
                }
                continue;

            case EV_KEY: //发生了触碰事件
                switch (buf.code)
                {
                case BTN_TOUCH:
                    if (buf.value == 0) //手离开了触摸屏
                    {
                        fflush(stdout);
                        //判断手是从哪个按钮离开的
                        if ((pay_button(otc) >= 0) && (pay_button(otc) <= 9))
                        {
                            if (icount < 8)
                            {
                                if (input[0] != '0' || (input[0] == '0' && input[1] == '.'))
                                {
                                    sprintf(&input[++icount], "%d", pay_button(otc));
                                    memcpy(maplcd, cartpaybuf, 800 * 480 * 4);
                                    fontPrint(f, screen, 462, 163, input, black, 800);
                                    if (icount >= 0)
                                    {
                                        dspybmp(xbmp, maplcd, &xcoor);
                                    }
                                }
                            }
                        }
                        else if (pay_button(otc) == 11)
                        {
                            int flag = 0;
                            if (icount < 7 && icount > -1)
                            {
                                for (int i = 0; i <= icount; i++)
                                {
                                    if (input[i] == '.')
                                    {
                                        flag = 1;
                                        break;
                                    }
                                }
                                if (flag != 1)
                                {
                                    flag = 0;
                                    input[++icount] = '.';
                                    memcpy(maplcd, cartpaybuf, 800 * 480 * 4);
                                    fontPrint(f, screen, 462, 163, input, black, 800);
                                    if (icount >= 0)
                                    {
                                        dspybmp(xbmp, maplcd, &xcoor);
                                    }
                                }
                            }
                        }
                        else if (pay_button(otc) == 12)
                        {
                            if (icount != -1)
                            {
                                input[icount] = '\0';
                                icount--;
                                memcpy(maplcd, cartpaybuf, 800 * 480 * 4);
                                fontPrint(f, screen, 462, 163, input, black, 800);
                                if (icount >= 0)
                                {
                                    dspybmp(xbmp, maplcd, &xcoor);
                                }
                            }
                            if (icount == -1)
                            {
                                memcpy(maplcd, cartpaybuf, 800 * 480 * 4);
                                fontPrint(f, screen, 462, 163, hint, gray, 800);
                            }
                        }

                        else if (pay_button(otc) == 13)
                        {
                            return false;
                        }
                        else if (pay_button(otc) == 15)
                        {
                            if (icount != -1)
                            {
                                bzero(input, 10);
                                icount = -1;
                                memcpy(maplcd, cartpaybuf, 800 * 480 * 4);
                                fontPrint(f, screen, 462, 163, hint, gray, 800);
                            }
                        }
                        else if (pay_button(otc) == 14)
                        {
                            char inputingbuf[800 * 480 * 4];
                            memcpy(inputingbuf, maplcd, 800 * 480 * 4);
                            if ((input[0] == '0' && input[1] != '.') || (input[icount] == '.'))
                            {
                                printf("格式错误\n");
                                dspybmp(febmp, maplcd, &failcoor);
                                close(otc->td);
                                sleep(1);
                                otc->td = open("/dev/input/event0", O_RDWR);
                                memcpy(maplcd, inputingbuf, 800 * 480 * 4);
                            }
                            else
                            {
                                float pay = 0;
                                input[5]='\0';
                                sscanf(input, "%f", &pay);
                                float changemoney = pay - totalprice;

                                if (pay < totalprice)
                                {
                                    printf("支付失败\n");
                                    dspybmp(payfail, maplcd, &failcoor);
                                    close(otc->td);
                                    sleep(1);
                                    otc->td = open("/dev/input/event0", O_RDWR);
                                    memcpy(maplcd, inputingbuf, 800 * 480 * 4);
                                }
                                else if (pay >= totalprice)
                                {
                                    printf("支付成功");
                                    dspybmp(paysuccess, maplcd, &coor);
                                    fontSetSize(f, 18);
                                    srand((unsigned)time(NULL));
                                    int n = rand() % 1000000000;
                                    char random[10];
                                    char change[10];
                                    char totalnumbuf[10];
                                    sprintf(change, "￥%.2f", changemoney);
                                    sprintf(random, "%d", n);
                                    sprintf(totalnumbuf, "%d", totalnum);

                                    fontPrint(f, screen, 317, 230, random, black, 800);
                                    fontPrint(f, screen, 317, 263, ordertime, black, 800);
                                    fontPrint(f, screen, 317, 363, totalnumbuf, black, 800);
                                    fontPrint(f, screen, 317, 329, change, black, 800);
                                    fontPrint(f, screen, 317, 396, getime(), black, 800);
                                    bzero(tpricebuf, 10);
                                    sprintf(tpricebuf, "%.2f", totalprice);
                                    fontSetSize(f, 60);
                                    fontPrint(f, screen, 375, 128, tpricebuf, black, 800);
                                    //数量调整
                                    p = head->next;
                                    while (p != head)
                                    {
                                        p->data.amount-=p->data.cart;
                                        p->data.cart = 0;
                                        p = p->next;
                                    }
                                    head->data.countcart = 0;

                                    

                                    struct input_event buf;
                                    int tmp1 = 0, tmp2 = 1;
                                    while (1)
                                    {
                                        bzero(&buf, sizeof(buf));
                                        read(otc->td, &buf, sizeof(buf));
                                        //判断是否发生触碰事件
                                        switch (buf.type)
                                        {
                                        case EV_ABS: //发生了坐标事件
                                            switch (buf.code)
                                            {
                                            case ABS_X: //触发了x坐标
                                                if (tmp2 == 1)
                                                {
                                                    otc->x = (float)buf.value / 1.28;
                                                    //printf("(%d,", otc->x);
                                                    tmp1 = 1;
                                                    tmp2 = 0;
                                                }
                                                continue;
                                            case ABS_Y: //触发了y坐标
                                                if (tmp1 == 1)
                                                {
                                                    otc->y = (float)buf.value / 1.25;
                                                    //printf("%d)\n", otc->y);
                                                    tmp1 = 0;
                                                    tmp2 = 1;
                                                }

                                                continue;

                                            default:
                                                break;
                                            }
                                            continue;

                                        case EV_KEY: //发生了触碰事件
                                            switch (buf.code)
                                            {
                                            case BTN_TOUCH:
                                                if (buf.value == 0) //手离开了触摸屏
                                                {
                                                    fflush(stdout);
                                                    //判断手是从哪个按钮离开的
                                                    if (button_position(otc, 124, 41, 356, 427))
                                                    {
                                                        dspybmp(cart, maplcd, &coor);
                                                        return true;
                                                    }
                                                }
                                                break;

                                            default:
                                                break;
                                            }
                                            continue;
                                        }
                                    }
                                }
                            }
                        }
                    }
                    break;

                default:
                    break;
                }
                continue;
            }
        }
    }

    //向右加载商品
node *loadthegood_right(node * head, node * p, char *maplcd, font *f, bitmap *screen)
{
        bmpinfo *page1 = getrgb("page1.bmp");
        bmpinfo *page0 = getrgb("page0.bmp");
        p = p->next;
        if (p == head)
        {
            p = head->next;
        }
        //计算接下来有多少个目标要显示
        int count = 0;
        node *tempp = p;
        while (p != head)
        {
            if (count == 6)
            {
                break;
            }
            count++;
            p = p->next;
        }
        p = tempp;

        char *backbmpname = calloc(30, sizeof(char));
        sprintf(backbmpname, "commodity%c.bmp", count + 48);
        bmpinfo *commoditybmp = getrgb(backbmpname);
        xy *coordinates = malloc(sizeof(xy));
        coordinates->x = 0;
        coordinates->y = 0;
        dspybmp(commoditybmp, maplcd, coordinates);
        free(backbmpname);
        free(commoditybmp->rgbmemory);
        free(commoditybmp);
        free(coordinates);

        showcartnum(head, maplcd, f, screen);

        //计算当前所在页数
        int countnode = 0;
        int page = 0;
        int devide = 0;
        int remain = 0;
        node *pagetmp = p;
        while (pagetmp != head)
        {
            pagetmp = pagetmp->prev;
            countnode++;
        }
        devide = countnode / 6;
        remain = countnode % 6;
        if (remain > 0)
        {
            page = devide + 1;
        }
        else
        {
            page = devide;
        }

        //计算总页数
        int totalnode = 0;
        node*xxx=head->next;
        while(xxx!=head)
        {
            totalnode++;
            xxx=xxx->next;
        }
        
        int totalpage = 0;
        devide = totalnode / 6;
        remain = totalnode % 6;
        if (remain > 0)
        {
            totalpage = devide + 1;
        }
        else
        {
            totalpage = devide;
        }

        xy pagecoor;
        pagecoor.x = (800 - totalpage * 30) / 2;
        pagecoor.y = 107;

        for (int i = 1; i <= totalpage; i++)
        {
            if (page == i)
            {
                dspybmp(page1, maplcd, &pagecoor);
            }
            else
            {
                dspybmp(page0, maplcd, &pagecoor);
            }
            pagecoor.x += 30;
        }
        free(page0->rgbmemory);
        free(page1->rgbmemory);
        free(page0);
        free(page1);

        xy *cmdycoor = malloc(sizeof(xy));
        int tmp = 0;

        fontSetSize(f, 20); //设置字体大小
        for (int i = 0; i < count; i++)
        {
            if (i % 3 == 0)
            {
                tmp = 0;
            }
            if (i < 3)
            {
                cmdycoor->y = 136;
            }
            else
            {
                cmdycoor->y = 301;
            }
            cmdycoor->x = 148 + tmp;
            tmp += 220;

            dspybmp(p->data.bmp, maplcd, cmdycoor);
            cmdycoor->x -= 28;

            //让字体居中
            int namecount = 0;
            for (int i = 0; i < 30; i++)
            {
                if (p->data.name[i] != '\0')
                {
                    namecount++;
                }
                else
                {
                    break;
                }
            }
            int x = cmdycoor->x + (120 - namecount / 3 * 15) / 2;
            fontPrint(f, screen, x, cmdycoor->y + 107, p->data.name, color1, 800);

            char buf[10];
            sprintf(buf, "￥%.2f", p->data.price);
            fontPrint(f, screen, cmdycoor->x + 30, cmdycoor->y + 130, buf, color1, 800);

            p = p->next;
        }
        free(cmdycoor);

        //函数结束时，p都会指向本页最后一个节点
        p = p->prev;
        p->count = count;

        return p;
}

    //向左加载商品
node *loadthegood_left(node * head, node * p, char *maplcd, font *f, bitmap *screen)
{
        bmpinfo *page1 = getrgb("page1.bmp");
        bmpinfo *page0 = getrgb("page0.bmp");
        //计算链表里面有多少个节点
        int listcount = 0;
        node *q = head->next;
        while (q != head)
        {
            listcount++;
            q = q->next;
        }

        //计算最后一页会有多少个节点
        int remainder = listcount % 6;
        if(remainder==0)
        {
            //无需排除完全没有商品的情况
            //因为commodity_interface()已经帮忙排除
            remainder=6;
        }

        //让p指向上一页的最后一个节点
        //如果本页是最后最后一页
        if (p->next == head)
        {
            for (int i = 0; i < remainder; i++)
            {
                p = p->prev;
            }
            //且仅有一页，则跳到本页的最后一个节点
            if (p == head)
            {
                p = head->prev;
            }
        }
        //本页不是最后一页
        else
        {
            //让指针p指向上一页的最后一个节点
            for (int i = 0; i < 6; i++)
            {
                p = p->prev;
            }
            //如果此时是第一页，则跳到最后一页
            if (p == head)
            {
                p = head->prev;
            }
        }

        //让p指向本页的第一个节点
        //如果此时是最后一页
        if (p->next == head)
        {
            for (int i = 0; i < (remainder - 1); i++)
            {
                p = p->prev;
            }
        }
        //如果此时不是最后一页
        else
        {
            //意味着本页肯定有6个节点
            for (int i = 0; i < (6 - 1); i++)
            {
                p = p->prev;
            }
        }

        int count = 0;
        node *tempp = p;
        while (p != head)
        {
            if (count == 6)
            {
                break;
            }
            count++;
            p = p->next;
        }
        p = tempp;

        char *backbmpname = calloc(30, sizeof(char));
        sprintf(backbmpname, "commodity%c.bmp", count + 48);
        bmpinfo *commoditybmp = getrgb(backbmpname);
        xy *coordinates = malloc(sizeof(xy));
        coordinates->x = 0;
        coordinates->y = 0;
        dspybmp(commoditybmp, maplcd, coordinates);
        free(commoditybmp->rgbmemory);
        free(commoditybmp);

        free(backbmpname);
        free(coordinates);
        showcartnum(head, maplcd, f, screen);

        //计算当前所在页数
        int countnode = 0;
        int page = 0;
        int devide = 0;
        int remain = 0;
        node *pagetmp = p;
        while (pagetmp != head)
        {
            pagetmp = pagetmp->prev;
            countnode++;
        }
        devide = countnode / 6;
        remain = countnode % 6;
        if (remain > 0)
        {
            page = devide + 1;
        }
        else
        {
            page = devide;
        }

        //计算总页数
        
        int totalnode = listcount;
        int totalpage = 0;
        devide = totalnode / 6;
        remain = totalnode % 6;
        if (remain > 0)
        {
            totalpage = devide + 1;
        }
        else
        {
            totalpage = devide;
        }

        xy pagecoor;
        pagecoor.x = (800 - totalpage * 30) / 2;
        pagecoor.y = 107;

        for (int i = 1; i <= totalpage; i++)
        {
            if (page == i)
            {
                dspybmp(page1, maplcd, &pagecoor);
            }
            else
            {
                dspybmp(page0, maplcd, &pagecoor);
            }
            pagecoor.x += 30;
        }
        free(page0->rgbmemory);
        free(page1->rgbmemory);
        free(page0);
        free(page1);

        fontSetSize(f, 20); //设置字体大小

        xy *cmdycoor = malloc(sizeof(xy));
        int tmp = 0;
        for (int i = 0; i < count; i++)
        {
            if (i % 3 == 0)
            {
                tmp = 0;
            }
            if (i < 3)
            {
                cmdycoor->y = 136;
            }
            else
            {
                cmdycoor->y = 301;
            }
            cmdycoor->x = 148 + tmp;
            tmp += 220;
            dspybmp(p->data.bmp, maplcd, cmdycoor);

            cmdycoor->x -= 28;
            int namecount = 0;
            for (int i = 0; i < 30; i++)
            {
                if (p->data.name[i] != '\0')
                {
                    namecount++;
                }
                else
                {
                    break;
                }
            }
            int x = cmdycoor->x + (120 - namecount / 3 * 15) / 2;
            fontPrint(f, screen, x, cmdycoor->y + 107, p->data.name, color1, 800);

            char buf[10];
            sprintf(buf, "￥%.2f", p->data.price);
            fontPrint(f, screen, cmdycoor->x + 30, cmdycoor->y + 130, buf, color1, 800);

            p = p->next;
        }
        free(cmdycoor);
        p = p->prev;
        p->count = count;

        return p;
}