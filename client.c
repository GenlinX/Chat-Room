#include "head4chat.h"
struct sockaddr *srvaddr;
socklen_t len;
font *f;
bitmap *screen;
int fd;              //本客户端的套接字
int connfd = 999;    //本客户端的connfd
int choosing = 999;  //当前选择的好友
int historyline = 7; //当前的聊天记录在第几行 7<historyline<总行数
int isemoji = 0;     //表情列表按钮是否被按下

clientlist *head; //客户端中的客户端链表
btif *bhead;      //按钮链表头节点
pthread_mutex_t clientlist_lock;
char *maplcd;

void *frezplay(bitmap *bm, int x, int y)
{
    int lcdline = 800 * 4;
    int lcdlist = 480;
    int bmpline = bm->width * 4;
    int start = y * lcdline + x * 4;
    char *q = maplcd;
    q += start;

    for (int j = 0; j < lcdlist && j < bm->height; j++)
    {
        for (int i = 0; i < 800 && i < bm->width; i++)
        {
            memcpy(q + j * lcdline + i * 4, bm->map + j * bmpline + i * 4, 4);
        }
    }
}

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

    // printf("RGB数据（包括无效字节）的大小：%d\n", rgbSize);

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

void dspybmp(bmpinfo *bmp, int x, int y)
{

    if (maplcd == NULL)
    {
        perror("lcd屏未能映射：");
        exit(0);
    }
    else
    {
        char *tmpmaplcd = maplcd;
        char *tmp = bmp->rgbmemory + bmp->lineBytes * (bmp->heigh - 1);
        int start = y * 800 * 4 + x * 4;
        tmpmaplcd += start;

        int tmpx, tmpy;
        int lcd_lines = 800 * 4;
        if (bmp->width + x > 800)
        {
            tmpx = 800 - x;
        }
        else
        {
            tmpx = bmp->width;
        }
        if (bmp->heigh + y > 480)
        {
            tmpy = 480 - y;
        }
        else
        {
            tmpy = bmp->heigh;
        }
        for (int j = 0; j < tmpy; j++)
        {
            for (int i = 0; i < tmpx; i++)
            {
                memcpy(tmpmaplcd + j * lcd_lines + i * 4, tmp - j * bmp->lineBytes + i * 4, 3);
            }
        }
    }
}

btif *init_blist(void)
{
    bhead = malloc(sizeof(btif));
    if (bhead != NULL)
    {
        INIT_LIST_HEAD(&bhead->blist);
    }
    return bhead;
}

//尾插法插入按钮
void *add_botton(int w, int h, int x, int y, void *arg, void *(*func)(void *arg))
{
    btif *newbotton = malloc(sizeof(btif));
    if (newbotton != NULL)
    {
        newbotton->w = w;
        newbotton->h = h;
        newbotton->x = x;
        newbotton->y = y;
        newbotton->arg = arg;
        newbotton->func = func;
        INIT_LIST_HEAD(&newbotton->blist);
    }
    list_add_tail(&newbotton->blist, &bhead->blist);
}

//删除某个按钮
void *delete_botton(int x, int y)
{
    btif *pos, *n;
    list_for_each_entry_safe(pos, n, &bhead->blist, blist)
    {
        if ((pos->x == x) && (pos->y == y))
        {
            list_del(&pos->blist);
            free(pos);
            break;
        }
    }
}

//摧毁所有的按钮
void *destroy_allbotton()
{
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &bhead->blist)
    {
        btif *tmp = list_entry(pos, btif, blist);
        list_del(pos);
        free(tmp);
    }
    bhead = init_blist();
}

//摧毁所有的好友按钮
void *destroy_allfriendbotton()
{
    btif *pos, *n;
    list_for_each_entry_safe(pos, n, &bhead->blist, blist)
    {
        if ((pos->x == 30) && (pos->w == 65) && (pos->h == 65))
        {
            list_del(&pos->blist);
            free(pos);
        }
    }
}

//交互函数
void *interaction()
{

    btif *pos, *n;
    int tfd = open("/dev/input/event0", O_RDWR);
    struct input_event buf;
    int x, y;

    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(tfd, &buf, sizeof(buf));

        switch (buf.type)
        {
        case EV_ABS: //发生了坐标事件
            switch (buf.code)
            {
            case ABS_X: //触发了x坐标
                x = (float)buf.value / 1.28;
                break;

            case ABS_Y: //触发了y坐标
                y = (float)buf.value / 1.25;
                break;

            default:
                break;
            }
            break;
        case EV_KEY: //发生了触碰事件
            switch (buf.code)
            {
            case BTN_TOUCH:
                if (buf.value == 0)
                {
                    btif *pos, *n;
                    list_for_each_entry_safe(pos, n, &bhead->blist, blist)
                    {
                        if ((x > pos->x) && (x < (pos->x + pos->w)) && (y > pos->y) && (y < (pos->y + pos->h)))
                        {
                            pos->func(pos->arg);
                            break;
                        }
                    }
                }
                break;

            default:
                break;
            }
            break;

        default:
            break;
        }
    }
}

//初始化客户端中的好友链表
void *init_list(void)
{
    head = malloc(sizeof(clientlist));
    if (head != NULL)
    {
        INIT_LIST_HEAD(&head->clist);
    }
}

//在客户端中创建新的好友节点
clientlist *new_cli(int client_fd)
{
    clientlist *new = malloc(sizeof(clientlist));
    if (new != NULL)
    {
        new->client_fd = client_fd;
        INIT_LIST_HEAD(&new->clist);
    }
    return new;
}

//摧毁好友链表
void *destroy_clientlist(clientlist *tmphead)
{
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &tmphead->clist)
    {
        clientlist *tmp = list_entry(pos, clientlist, clist);
        list_del(pos);
        free(tmp);
    }
}

//写入聊天记录,注意这里的“client_fd”是和“哪个好友”聊天的fd，属于好友
//如果flag=1，则写入“好友发过来”的文本；如果flag=0，则写入“自己发给好友”的文本
void *record_chat_history(char *textbuf, int client_fd, int flag)
{
    clientlist *pos;
    pthread_mutex_t *tmplock;
    list_for_each_entry(pos, &head->clist, clist)
    {
        if (pos->client_fd == client_fd)
        {
            tmplock = &pos->chat_history_lock;
            break;
        }
    }
    char chn[7];
    sprintf(chn, "%c.txt", client_fd + '@');

    int n = strlen(textbuf);
    char topline[8]; //每个聊天框的标志

    if (flag == 0)
    {
        sprintf(topline, "/m/%c", connfd + '@');
    }
    else if (flag == 1)
    {
        sprintf(topline, "/m/%c", client_fd + '@');
    }

    pthread_mutex_lock(tmplock);
    int chn_fd = open(chn, O_WRONLY | O_CREAT, 0644);
    lseek(chn_fd, 0, SEEK_END);
    write(chn_fd, topline, strlen(topline));

    char ct[2 * n];
    int j = 0, count = 0, countline = 1;
    for (int i = 0; i < n; i++)
    {
        if (count > NOWPL)
        {
            ct[j++] = '\n';
            ct[j++] = '/';
            if (flag == 0)
            {
                ct[j++] = 'r';
            }
            else if (flag == 1)
            {
                ct[j++] = 'l';
            }
            ct[j++] = '/';
            count = 0;
            countline++;
        }

        //遇到换行符/nxt/
        if (textbuf[i] == 47 && textbuf[i + 1] == 110 && textbuf[i + 2] == 120 && textbuf[i + 3] == 116 && textbuf[i + 4] == 47)
        {

            count = 0;
            ct[j++] = '\n';
            countline++;
            i += 4;
        }

        else if (textbuf[i] > 127 && textbuf[i + 1] > 127 && textbuf[i + 1] > 127)
        {
            ct[j++] = textbuf[i];
            ct[j++] = textbuf[i + 1];
            ct[j++] = textbuf[i + 2];
            i += 2;
            count += 2;
        }
        else if (textbuf[i] == 13 && textbuf[i + 1] == 10)
        {
            i++;
        }
        else
        {
            ct[j++] = textbuf[i];
            count++;
        }
    }
    ct[j] = '\0';
    write(chn_fd, ct, strlen(ct));
    close(chn_fd);
    pthread_mutex_unlock(tmplock);
}

//显示聊天记录，line是从末尾第几行开始显示
void *displaying_chat_history(int client_fd)
{
    bmpinfo *up = getrgb("up.bmp");
    bmpinfo *down = getrgb("down.bmp");
    //删除所有emoji
    bmpinfo *emojilist = getrgb("emojilist.bmp");

    if (isemoji == 1)
    {
        historyline -= 1;
    }

    if (choosing != client_fd)
    {
        if (isemoji == 1)
        {
            historyline += 1;
        }
        return NULL;
    }
    bmpinfo *chatbg = getrgb("chatbg.bmp");

    char chat_history_name[7];
    sprintf(chat_history_name, "%c.txt", client_fd + '@');
    int chn_fd = open(chat_history_name, O_RDONLY);

    struct stat info;
    stat(chat_history_name, &info);

    char buf[info.st_size];

    if (read(chn_fd, buf, info.st_size) != info.st_size)
    {
        if (isemoji == 1)
        {
            historyline += 1;
        }
        fprintf(stderr, "读取聊天记录缺失\n");
        return NULL;
    }

    int x, y = 20, i = 0, j = 0;
    int orientation; //文字的起始位置，1：左，0：右
    char mark[3];
    char outbuf[128];
    bzero(outbuf, 128);

    int totalline = 0; //文本总行数
    while (i <= (info.st_size - 1))
    {
        if (buf[i++] == '\n')
        {
            totalline++;
        }
    }
    if (historyline > totalline)
    {
        if (totalline >= 7)
        {
            historyline = totalline;
            if (isemoji == 1)
            {
                historyline += 1;
            }
            return NULL;
        }
        else
        {
            historyline = 7;
        }
    }

    i = 0;

    int linecount = 0;

    int targetline = totalline - historyline; //要去到第几行

    while ((i < (info.st_size - 1)) && (linecount < targetline))
    {
        if (buf[i++] == '\n')
        {
            linecount++;
        }
    }

    totalline = 0; //开始充当已显示行数
    linecount = 0; //开始充当每次对话的行数
    dspybmp(chatbg, 127, 0);
    if (isemoji == 0)
    {
        //如果表情包列表未被弹出，显示上下按钮
        dspybmp(up, 434, 0);
        dspybmp(down, 434, 385);
    }
    while (i <= (info.st_size - 1))
    {

        if ((buf[i] == '/') && ((buf[i + 1] == 'm') || (buf[i + 1] == 'r') || (buf[i + 1] == 'l')||buf[i+1]=='e') && (buf[i + 2] == '/'))
        {

            if (buf[i + 1] == 'm')
            {
                linecount = 0;
                fontSetSize(f, 60);
                bitmap *sign;
                if (buf[i + 3] == (connfd + '@'))
                {
                    orientation = 0; //记录文字应在位置
                    x = 740;         //头像所在位置
                    sprintf(mark, "%c", connfd + '@');

                    sign = createBitmapWithInit(50, 50, 4, DeepSkyBlue); //指定头像颜色
                }
                else
                {
                    orientation = 1; //记录文字应在位置
                    x = 150;         //头像所在位置
                    sprintf(mark, "%c", choosing + '@');
                    sign = createBitmapWithInit(50, 50, 4, lightgreen);
                }

                fontPrint(f, sign, 6, -2, mark, white, 65); //打印出头像
                frezplay(sign, x, y);
                if (x == 740)
                {
                    //如果头像在右边，则接下来的文本也在右边
                    x = 500;
                }
                else
                {
                    //如果头像在左边，则文本也在左边
                    x = 210;
                }
                i += 4;
            }
            else if (buf[i + 1] == 'r')
            {
                x = 500;
                i += 3;
            }
            else if(buf[i+1]=='l')
            {
                x = 200;
                i += 3;
            }
            else if(buf[i+1]=='e')
            {
                char emojiname[10];
                bzero(emojiname,10);
                sprintf(emojiname,"em%c.bmp",buf[i+3]);
                bmpinfo*emoji=getrgb(emojiname);
                if(x==500)
                {
                    dspybmp(emoji,x+180,y);
                }
                else
                {
                    dspybmp(emoji,x,y);
                }
                i+=4;

            }
        }
        else if (buf[i] == '\n')
        {
            linecount++;
            fontSetSize(f, 35);
            outbuf[j] = '\0';
            if ((linecount == 1) && (x == 500))
            {
                //如果是右边第一行，那么无论什么情况下都要缩进
                int i = 0;
                while (i <= j)
                {

                    if (outbuf[i] > 127 && outbuf[i + 1] > 127 && outbuf[i + 2] > 127)
                    {
                        x += 20;
                        i += 3;
                    }
                    if (outbuf[i] == 'i' || outbuf[i] == 'j' || outbuf[i] == 'I' || outbuf[i] == 'f' || outbuf[i] == 'l' || outbuf[i] == 't')
                    {
                        x += 6;
                        i++;
                    }
                    else
                    {
                        i++;
                    }
                }
                x += (NOWPL + 1 - j) * 13;
            }
            // int k = 0;
            // if ((outbuf[0] == '/') && (outbuf[1] == 'e') && (outbuf[2] == '/'))
            // {
            //     char emojiname[10];
            //     bzero(emojiname,10);
            //     sprintf(emojiname,"em%d.bmp",outbuf[4]);
            //     dspybmp(emojiname,x,y);
            // }
            // else
            // {
                fontPrint(f, screen, x, y + 10, outbuf, black, 800);
                totalline++;
                if (totalline >= NORTD)
                {
                    break;
                }
                bzero(outbuf, 128);
                j = 0;
                i += 1;
                y += 55;
            // }
        }
        else
        {
            outbuf[j] = buf[i];
            j += 1;
            i += 1;
        }
    }
    // fprintf(stderr,"显示了末尾%d行\n",historyline);
    if (isemoji == 1)
    {
        historyline += 1;
        //如果emoji按钮被按下，则把列表重新显示出来
        dspybmp(emojilist, 127, 352);
    }
}
void *up(void *arg)
{
    // historyline自加
    //检测好友是否存在
    clientlist *pos;
    int flag = 0;
    list_for_each_entry(pos, &head->clist, clist)
    {
        if (choosing == pos->client_fd)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("请选择好友\n");
        return NULL;
    }

    historyline += 1;
    displaying_chat_history(choosing);
}

void *down(void *arg)
{

    clientlist *pos;
    int flag = 0;
    list_for_each_entry(pos, &head->clist, clist)
    {
        if (choosing == pos->client_fd)
        {
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        printf("请选择好友\n");
        return NULL;
    }

    if (historyline > 7)
    {
        historyline -= 1;
        displaying_chat_history(choosing);
    }
}

//单个表情包按钮
void *emoji(void *arg)
{
    char *emojibuf = calloc(6, sizeof(char));
    sprintf(emojibuf, "/e/%c\n", *(int *)arg);
    fprintf(stderr, emojibuf);
    record_chat_history(emojibuf, choosing, 0);
    displaying_chat_history(choosing);
}

//删除表情列表，并显示聊天记录
void *destroy_allemoji(void *arg)
{
    isemoji = 0;
    btif *pos, *n;
    list_for_each_entry_safe(pos, n, &bhead->blist, blist)
    {
        if (pos->x > 127 && pos->x < 800 && pos->y > 352 && pos->y < 408)
        {
            list_del(&pos->blist);
            free(pos);
        }
    }
    displaying_chat_history(choosing);
}

//右侧表情包列表按钮
void *emojilist(void *arg)
{
    if (isemoji == 1)
    {
        //如果表情列表已经被按下
        isemoji = 0;
        destroy_allemoji(NULL);
        add_botton(60, 23, 434, 385, NULL, down);
        add_botton(60, 23, 434, 0, NULL, up);
        return NULL;
    }

    isemoji = 1; //列表按钮被按下
    delete_botton(434, 0);
    delete_botton(434, 385);
    int flag = 0;
    clientlist *pos;
    list_for_each_entry(pos, &head->clist, clist)
    {
        if (pos->client_fd == choosing)
        {
            //选择的好友存在
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        fprintf(stderr, "请选择好友\n");
        return NULL;
    }
    displaying_chat_history(choosing);
    bmpinfo *emojilist = getrgb("emojilist.bmp");
    dspybmp(emojilist, 127, 352);

    int x = 139;
    for (int i = 0; i < 10; i++)
    {
        char *p = malloc(sizeof(int));
        *p = i+'0';
        add_botton(40, 44, x + i * 61, 358, p, emoji);
    }
    char *p=malloc(sizeof(int));
    *p='x';
    add_botton(40,44,x+10*61,358,p,emoji);

}

//更新“当前选择好友”
void *update_choosing(void *arg)
{
    fontSetSize(f, 60);
    bmpinfo *cfriend = getrgb("cfriend.bmp");
    bmpinfo *friend = getrgb("friend.bmp");
    clientlist *pos;
    int x = 30, y = 30;
    int pmy_choosing = choosing;
    choosing = *(int *)arg;

    list_for_each_entry(pos, &head->clist, clist)
    {
        if (pos->client_fd == connfd)
        {
            continue;
        }
        if (pos->client_fd == pmy_choosing)
        { //把原来的选择置黑
            dspybmp(friend, x, y);
        }
        if (pos->client_fd == choosing)
        { //把最新的选择置绿
            dspybmp(cfriend, x, y);
        }
        char mark[3];
        bzero(mark, 3);
        sprintf(mark, "%c", pos->client_fd + '@');
        fontPrint(f, screen, x + 20, y + 3, mark, white, 800);
        y += 95;
    }

    if (choosing != pmy_choosing)
    {

        //显示新的聊天记录
        // dspybmp(chatbg, 127, 0);
        if (isemoji == 1)
        {
            add_botton(60, 23, 434, 0, NULL, up);
        }
        isemoji = 0;
        historyline = 7;
        displaying_chat_history(choosing);
        destroy_allemoji(NULL);
        add_botton(60, 23, 434, 385, NULL, down);
    }
}

//更新好友列表，顺便更新按钮
void *update_clientlist()
{

    bmpinfo *vertical = getrgb("vertical.bmp");
    dspybmp(vertical, 30, 0);

    clientlist *tmphead = malloc(sizeof(clientlist));
    if (tmphead != NULL)
    {

        tmphead->clist.next = head->clist.next;
        tmphead->clist.prev = head->clist.prev;
        head->clist.next->prev = &tmphead->clist;
        head->clist.prev->next = &tmphead->clist;
        INIT_LIST_HEAD(&head->clist);
    }
    else
    {
        fprintf(stderr, "创建临时列表失败\n");
        return NULL;
    }

    bmpinfo *chatbg = getrgb("chatbg.bmp");
    bmpinfo *friend = getrgb("friend.bmp");
    bmpinfo *cfriend = getrgb("cfriend.bmp");
    int count_connfd;
    bzero(&count_connfd, sizeof(int));
    read(fd, &count_connfd, sizeof(int));
    pthread_mutex_lock(&clientlist_lock);
    for (int i = 0; i < count_connfd; i++)
    {
        int client_fd;
        bzero(&client_fd, sizeof(int));
        read(fd, &client_fd, sizeof(int));

        clientlist *new = new_cli(client_fd);
        list_add_tail(&new->clist, &head->clist);
    }
    pthread_mutex_unlock(&clientlist_lock);

    printf("更新了客户端列表：");
    clientlist *pos;

    pthread_mutex_lock(&clientlist_lock);
    int x = 30, y = 30;
    fontSetSize(f, 60);

    //更新按钮位置
    destroy_allfriendbotton();
    // bhead = init_blist();
    int flag = 0; //之前选的好友是否存在
    list_for_each_entry(pos, &head->clist, clist)
    {
        if (pos->client_fd == choosing)
        {
            flag = 1;
        }
        if (pos->client_fd == connfd)
        {
            //如果是本人
            continue;
        }
        if (pos->client_fd == choosing)
        {
            dspybmp(cfriend, x, y);
        }
        else
        {
            dspybmp(friend, x, y);
        }

        fprintf(stderr, "%d ", pos->client_fd);

        char mark[3];
        bzero(mark, 3);
        sprintf(mark, "%c", pos->client_fd + '@');
        fontPrint(f, screen, x + 20, y + 3, mark, white, 800);

        add_botton(65, 65, x, y, (void *)&pos->client_fd, update_choosing);
        y += 95;

        //创建聊天记录,如果客户端不存在，则创建并置空聊天记录
        int existflag = 0;
        clientlist *tmppos;
        list_for_each_entry(tmppos, &tmphead->clist, clist)
        {
            if (pos->client_fd == tmppos->client_fd)
            {
                existflag = 1;
                break;
            }
        }
        if (existflag == 0)
        {
            char chat_history_name[7];
            sprintf(chat_history_name, "%c.txt", pos->client_fd + '@');
            int chn_fd = open(chat_history_name, O_RDWR | O_CREAT | O_TRUNC, 0644);
            pthread_mutex_init(&pos->chat_history_lock, NULL);
        }
    }
    pthread_mutex_unlock(&clientlist_lock);
    if (flag == 0)
    {
        dspybmp(chatbg, 127, 0);
    }
    // destroy_clientlist(tmphead);
    clientlist *tmppos, *n;
    list_for_each_entry_safe(tmppos, n, &tmphead->clist, clist)
    {
        list_del(&tmppos->clist);
    }
    printf("\n");
}

void *send_directory_recursively(int newfd, char *src_path, char *dst_path)
{
    DIR *dp = opendir(src_path);
    struct dirent *ep;
    struct stat info;

    while (1)
    {
        ep = readdir(dp);
        if (ep == NULL)
        {
            break;
        }
        if (strcmp(ep->d_name, ".") == 0 || strcmp(ep->d_name, "..") == 0)
        {
            continue;
        }
        bzero(&info, sizeof(struct stat));
        stat(ep->d_name, &info);

        if (S_ISREG(info.st_mode))
        {
            //如果是普通文件
            continue;
        }
        else if (S_ISDIR(info.st_mode))
        {
            //如果是目录
            strcat(src_path, "/");
            strcat(src_path, ep->d_name);
            strcat(dst_path, "/");
            strcat(dst_path, ep->d_name);
            send(newfd, dst_path, PATHSIZE, MSG_NOSIGNAL);
            send_directory_recursively(newfd, src_path, dst_path);
        }
    }
}
//发送目录
void *send_directory(void *arg)
{
    pthread_detach(pthread_self());
    char src_path[PATHSIZE]; //用于存储原始路径
    bzero(src_path, PATHSIZE);
    strcpy(src_path, (char *)arg);

    char dst_path[PATHSIZE];
    char *p = strtok((char *)arg, "/");
    while (p)
    {
        bzero(dst_path, PATHSIZE);
        strcpy(dst_path, p);
        p = strtok(NULL, "/");
    }
    free(arg);

    int linksignal = cnt_senddrct;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int)); //发送信号给服务器  //c1
    write(newfd, &choosing, sizeof(int));   //目标ID发给服务器   //c2

    write(newfd, &connfd, sizeof(int)); //把自己ID发给对方    //c3
    write(newfd, dst_path, PATHSIZE);   //先把最外层目录发给目标

    //开始递归地发送目录
    send_directory_recursively(newfd, src_path, dst_path);
}

void *recv_directory(void *arg)
{
    pthread_detach(pthread_self());
    int tmp_fd = *(int *)arg;
    int linksignal;
    linksignal = cnt_recvdrct;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int)); // cc1
    write(newfd, &tmp_fd, sizeof(int));     // cc2

    int src_id; //接收对方ID
    read(newfd, &src_id, sizeof(int));
    fprintf(stderr, "已收到来源ID:%d\n", src_id);

    char dst_path[PATHSIZE];
    while (1)
    {
        bzero(dst_path, PATHSIZE);
        if (recv(newfd, dst_path, PATHSIZE, 0) <= 0)
        {
            fprintf(stderr, "中转站已退出，退出接收目录\n");
            break;
        }
        fprintf(stderr, "接收到目录%s\n", dst_path);
        mkdir(dst_path, 0644);
    }
    close(newfd);
    pthread_exit(NULL);
}

//上传单个文件
void *copy_single_file(void *arg)
{
    pthread_detach(pthread_self());
    int linksignal = cnt_sendfile;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int)); //发送信号给服务器  //c1
    write(newfd, &choosing, sizeof(int));   //目标ID发给服务器   //c2
    write(newfd, &connfd, sizeof(int));     //把自己ID发给对方    //c3

    int singlefile = open((char *)arg, O_RDONLY);
    struct stat info; //获取文件信息
    stat((char *)arg, &info);
    char filename[PATHSIZE];

    char *p = strtok((char *)arg, "/");
    while (p)
    {
        bzero(filename, PATHSIZE);
        strcpy(filename, p);
        p = strtok(NULL, "/");
    }

    free(arg);

    uint64_t filesize = info.st_size;
    write(newfd, filename, PATHSIZE); //上传文件名给服务器
    write(newfd, &filesize, 8UL);     //上传文件大小给服务器
    printf("发送文件名：%s 发送文件大小：%lu\n", filename, info.st_size);
    char readbuf[READSIZE];
    off_t count1 = 0, count2 = 0, total = 0;
    while (1)
    {
        count1 = 0;
        count2 = 0;
        bzero(readbuf, READSIZE);
        if ((count1 = read(singlefile, readbuf, READSIZE)) <= 0)
        {
            fprintf(stderr, "文件读取已终止\n");
            break;
        }
        if ((count2 = send(newfd, readbuf, count1, MSG_NOSIGNAL)) <= 0)
        {
            fprintf(stderr, "中转站已退出，终止发送文件\n");
            break;
        }
        total += count2;
    }

    if (total != info.st_size)
    {
        fprintf(stderr, "发文件失败\n");
    }
    else
    {
        fprintf(stderr, "发送文件成功\n");
    }
    pthread_exit(NULL);
}

//接收单个文件
void *recv_single_file(void *arg)
{
    pthread_detach(pthread_self());
    int tmp_fd = *(int *)arg;
    int linksignal;
    linksignal = cnt_recvfile;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int)); // cc1
    write(newfd, &tmp_fd, sizeof(int));     // cc2

    int src_id; //接收对方ID
    read(newfd, &src_id, sizeof(int));
    fprintf(stderr, "已收到来源ID:%d\n", src_id);

    char filename[PATHSIZE];
    uint64_t filesize;
    bzero(filename, PATHSIZE);
    read(newfd, filename, PATHSIZE);
    read(newfd, &filesize, 8UL);

    fprintf(stderr, "收到文件名：%s 文件大小：%lu\n", filename, filesize);

    int dstfile;
    if ((dstfile = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0644)) == -1)
    {
        perror("创建文件失败");
        return NULL;
    }

    char readbuf[READSIZE];
    off_t count1 = 0, count2 = 0, total = 0;

    while (1)
    {
        count1 = 0, count2 = 0;
        bzero(readbuf, READSIZE);
        if ((count1 = recv(newfd, readbuf, READSIZE, 0)) <= 0)
        {
            fprintf(stderr, "中转站已退出，退出接收文件\n");
            close(newfd);
            close(dstfile);
            break;
        }
        if ((count2 = write(dstfile, readbuf, count1)) != count1)
        {
            fprintf(stderr, "写入数据失败\n");
            close(newfd);
            close(dstfile);
            break;
        }
        total += count2;
    }
     char record[50];
    bzero(record,50);
    sprintf(record,"已收到文件:%s\n",filename);
    record_chat_history(record,src_id,1);
    displaying_chat_history(choosing);
    // if (total ==(off_t) filesize)
    // {
    //     fprintf(stderr, "接收文件%s成功\n", filename);
    //    
        
    // }
    // else
    // {
    //     fprintf(stderr, "接收文件%s失败，总大小：%lu,拷贝总量：%lu", filename, filesize, total);
    // }
}

void *send_file2(void *arg)
{
    bmpinfo *writesth = getrgb("writesth.bmp");
    fprintf(stderr, "请输入一个路径\n");
    char curpath[PATHSIZE];                         //当前工作路径
    char *srcpath = calloc(PATHSIZE, sizeof(char)); //源文件、目录路径
    bzero(curpath, PATHSIZE);
    getcwd(curpath, PATHSIZE);
    int srcfile;

    while (1)
    {
        bzero(srcpath, PATHSIZE);
        fgets(srcpath, PATHSIZE, stdin);
        strtok(srcpath, "\n");
        printf("整理后的路径：%s", srcpath);
        if ((srcfile = open(srcpath, O_RDONLY)) >= 0)
        {
            fprintf(stderr, "单个文件\n");
            copy_single_file((void *)srcpath);
            break;
        }
        else if (chdir(srcpath) == 0)
        {
            chdir(curpath);
            fprintf(stderr, "目录\n");
            send_directory((void *)srcpath);
            break;
        }
        else
        {
            chdir(curpath);
            fprintf(stderr, "非法路径，请再次输入\n");
        }
        pthread_testcancel();
    }
    add_botton(420, 53, 143, 418, NULL, client_send_text);
    add_botton(53, 53, 578, 418, NULL, client_send_file);
    delete_botton(127, 50);
    dspybmp(writesth, 241, 429);
}

void *send_file1(void *arg)
{
    bmpinfo *writesth = getrgb("writesth.bmp");
    bmpinfo *emwritesth = getrgb("emwritesth.bmp");
    int pmy_choosing = choosing;
    int flag = 0;
    clientlist *pos;
    list_for_each_entry(pos, &head->clist, clist)
    {
        //如果选择的好友存在
        if (choosing == pos->client_fd)
        {
            dspybmp(emwritesth, 241, 429);
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        add_botton(420, 53, 143, 418, NULL, client_send_text);
        add_botton(53, 53, 578, 418, NULL, client_send_file);
        fprintf(stderr, "请选择好友\n");
        return NULL;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, send_file2, NULL);
    tidarg sometid;
    sometid.child_tid = tid;
    sometid.parent_tid = pthread_self();

    //添加取消按钮
    add_botton(673, 304, 127, 50, (void *)&sometid, (void *)terminate);

    //检测好友状态是否发生改变
    while (1)
    {
        if (choosing != pmy_choosing)
        {
            pthread_cancel(tid);
            add_botton(420, 53, 143, 418, NULL, client_send_text);
            add_botton(53, 53, 578, 418, NULL, client_send_file);
            delete_botton(127, 50);
            dspybmp(writesth, 241, 429);
            break;
        }
    }
}

//发送文件按钮
void *client_send_file(void *arg)
{
    delete_botton(143, 418); //删除发送文本
    delete_botton(578, 418); //删除自身
    pthread_t tid;
    pthread_create(&tid, NULL, send_file1, NULL);
}

void *send_text2(void *arg)
{
    fprintf(stderr, "请在终端输入文字\n");
    int linksignal = cnt_sendtext;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int));
    write(newfd, &choosing, sizeof(int)); //目标的ID

    char writebuf[SIZE];

    write(newfd, &connfd, sizeof(int)); //先把自己的ID发给对方
    while (1)
    {
        bzero(writebuf, SIZE);
        fgets(writebuf, SIZE, stdin);

        if (send(newfd, writebuf, strlen(writebuf), MSG_NOSIGNAL) <= 0)
        {
            fprintf(stderr, "中转站已退出，退出'发送文本'\n");
            break;
        }
        record_chat_history(writebuf, choosing, 0);
        historyline = 7;
        displaying_chat_history(choosing);
        pthread_testcancel();
    }
}

void *send_text1(void *arg)
{
    bmpinfo *writesth = getrgb("writesth.bmp");
    bmpinfo *emwritesth = getrgb("emwritesth.bmp");
    int pmy_choosing = choosing;
    int flag = 0;
    clientlist *pos;
    list_for_each_entry(pos, &head->clist, clist)
    {
        //如果选择的好友存在
        if (choosing == pos->client_fd)
        {
            dspybmp(emwritesth, 241, 429); //把输入栏置空
            flag = 1;
            break;
        }
    }
    if (flag == 0)
    {
        add_botton(420, 53, 143, 418, NULL, client_send_text);
        add_botton(53, 53, 578, 418, NULL, client_send_file);
        fprintf(stderr, "请选择好友\n");
        return NULL;
    }
    else
    {
        pthread_t tid;
        pthread_create(&tid, NULL, send_text2, NULL);

        tidarg sometid;
        sometid.child_tid = tid;
        sometid.parent_tid = pthread_self();

        add_botton(673, 304, 127, 50, (void *)&sometid, (void *)terminate); //添加取消按钮

        //检测好友状态是否发生改变
        while (1)
        {
            if (choosing != pmy_choosing)
            {
                pthread_cancel(tid);
                add_botton(420, 53, 143, 418, NULL, client_send_text);
                add_botton(53, 53, 578, 418, NULL, client_send_file);
                delete_botton(127, 50); //删除取消按钮
                dspybmp(writesth, 241, 429);
                break;
            }
        }
    }
}

void *client_send_text(void *arg)
{
    delete_botton(143, 418); //删除自身
    delete_botton(578, 418); //删除发送文件
    pthread_t tid;
    pthread_create(&tid, NULL, send_text1, NULL);
}

void *terminate(void *arg)
{
    bmpinfo *writesth = getrgb("writesth.bmp");
    tidarg *sometid = (tidarg *)arg;
    pthread_cancel(sometid->parent_tid);
    pthread_cancel(sometid->child_tid);
    add_botton(420, 53, 143, 418, NULL, client_send_text);
    add_botton(53, 53, 578, 418, NULL, client_send_file);
    delete_botton(127, 50); //删除自身按钮
    dspybmp(writesth, 241, 429);
}

void *recv_text(void *arg)
{
    pthread_detach(pthread_self());
    int tmp_fd = *(int *)arg;
    int linksignal;
    linksignal = cnt_recvtext;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int));
    write(newfd, &tmp_fd, sizeof(int)); //把临时来源ID返回给服务器

    // int src_connfd;
    // read(fd,&src_connfd,sizeof(int));

    char textbuf[SIZE];

    int src_id; //读取客户端（原始id）
    read(newfd, &src_id, sizeof(int));

    while (1)
    {
        bzero(textbuf, SIZE);
        if (recv(newfd, textbuf, SIZE, 0) == 0)
        {
            fprintf(stderr, "中转站已断开,退出'接收信息'\n");
            close(newfd);
            break;
        }
        record_chat_history(textbuf, src_id, 1);
        displaying_chat_history(src_id);
        fprintf(stderr, "%s", textbuf);
    }
    pthread_exit(NULL);
}

// void *copy_file(char srcpath_buf[], int newfd)
// {

//     char filename[SIZE];
//     bzero(filename, SIZE);

//     char *p = strtok(srcpath_buf, "/");
//     while (p)
//     {
//         bzero(filename, SIZE);
//         strcpy(filename, p);
//         p = strtok(NULL, "/");
//     }
//     fprintf(stderr, "处理完毕后的文件名：%s\n", filename);

//     write(newfd, filename, strlen(filename));
// }
// void *send_file(int chfd)
// {
//     int source_fd;
//     char srcpath_buf[PATHSIZE];
//     char current_path[PATHSIZE];
//     bzero(current_path, PATHSIZE);
//     getcwd(current_path, PATHSIZE);

//     while (1)
//     {
//         bzero(srcpath_buf, PATHSIZE);
//         fprintf(stderr, "请从键盘输入路径\n");
//         fgets(srcpath_buf, PATHSIZE, stdin);
//         strtok(srcpath_buf, "\n");
//         fprintf(stderr, "你输入的路径是:%s\n", srcpath_buf);

//         //如果是普通文件
//         if ((source_fd = open(srcpath_buf, O_RDONLY)) >= 0)
//         {
//             int linksignal = cnt_sendfile;
//             int newfd = socket(AF_INET, SOCK_STREAM, 0);
//             connect(newfd, srvaddr, len);
//             write(newfd, &linksignal, sizeof(int));
//             write(newfd, &chfd, sizeof(int)); //目标ID
//             copy_file(srcpath_buf, newfd);
//         }
//         else
//         {
//             fprintf(stderr, "文件无法打开\n");
//         }
//     }
// }
// void *recv_file(void *arg)
// {
//     pthread_detach(pthread_self());
//     int tmp_fd = *(int *)arg;
//     int linksignal;
//     linksignal = cnt_recvfile;
//     int newfd = socket(AF_INET, SOCK_STREAM, 0);
//     connect(newfd, srvaddr, len);

//     write(newfd, &linksignal, sizeof(int));
//     write(newfd, &tmp_fd, sizeof(int)); //把临时来源ID返回给服务器

//     char filename[SIZE];
//     bzero(filename, SIZE);
//     read(newfd, filename, SIZE);

//     fprintf(stderr, "已从服务器收到a：%s\n", filename);
// }

// void *client_choice(void *arg)
// {
//     int chfd;

//     while (1)
//     {
//         fprintf(stderr, "请选择好友\n");

//         scanf("%d", &chfd);

//         if (chfd == connfd)
//         {
//             fprintf(stderr, "不能选择自己\n");
//             continue;
//         }

//         clientlist *pos;
//         int flag = 0;

//         pthread_mutex_lock(&clientlist_lock);
//         list_for_each_entry(pos, &head->clist, clist)
//         {
//             if (pos->client_fd == chfd)
//             {
//                 flag = 1;
//                 break;
//             }
//         }
//         pthread_mutex_unlock(&clientlist_lock);
//         if (flag == 1)
//         {
//             fprintf(stderr, "选择好友%d成功\n", chfd);
//         }
//         else
//         {
//             fprintf(stderr, "找不到好友%d\n", chfd);
//             continue;
//         }

//         int chfc;
//         pthread_t tid;
//         while (1)
//         {
//             fprintf(stderr, "请输入功能:3发送文本，5发送文件\n");
//             scanf("%d", &chfc);
//             getchar();
//             if (chfc == 3)
//             {
//                 send_text((void *)&chfd);
//                 // pthread_create(&tid,NULL,send_text,(void*)&chfd);
//                 // pthread_join(tid,NULL);
//                 break;
//             }
//             else if (chfc == 5)
//             {

//                 send_file(chfd);
//                 break;
//             }
//         }
//     }
// }

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

void *connect_cartoon(void *arg)
{
    bmpinfo *bmp1 = getrgb("1.bmp");
    bmpinfo *bmp2 = getrgb("2.bmp");
    bmpinfo *bmp3 = getrgb("3.bmp");
    bmpinfo *connecting = getrgb("connecting.bmp");

    dspybmp(connecting, 0, 0);
    while (1)
    {
        time_t second = time(NULL);
        if (second % 3 == 0)
        {
            dspybmp(bmp1, 525, 250);
        }
        else if (second % 3 == 1)
        {

            dspybmp(bmp2, 525, 250);
        }
        else
        {
            dspybmp(bmp3, 525, 250);
        }
        pthread_testcancel();
    }
}

int main(int argc, char const *argv[])
{
    printf("%ld\n", sizeof(off_t));
    maplcd = mapsm();

    struct sockaddr_in src_srvaddr;
    len = sizeof(src_srvaddr);
    bzero(&src_srvaddr, len);
    src_srvaddr.sin_family = AF_INET;
    src_srvaddr.sin_port = htons(PORT_ROSE);
    src_srvaddr.sin_addr.s_addr = inet_addr(ADDR);
    // src_srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr = (struct sockaddr *)&src_srvaddr;
    init_list();

    f = fontLoad("myh.ttf"); //打开字库
    screen = createBitmap2(800, 480, 4, maplcd);

    fd = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 0;
    pthread_t ctid;
    pthread_create(&ctid, NULL, connect_cartoon, NULL);
    while (1)
    {
        if (flag != 1)
        {
            fprintf(stderr, "正在连接服务器...\n");
            flag = 1;
        }

        if (connect(fd, srvaddr, len) == -1)
        {
            continue;
        }
        //先加入基本按钮

        int linksignal = cnt_clicnsvr;
        write(fd, &linksignal, sizeof(int)); // 1
        fprintf(stderr, "连接服务器成功!\n");
        pthread_cancel(ctid);
        pthread_join(ctid, NULL);

        read(fd, &connfd, sizeof(int)); // 2
        fprintf(stderr, "本客户端fd:%d\n", connfd);

        bmpinfo *chat = getrgb("chat.bmp");
        dspybmp(chat, 0, 0);
        fontSetSize(f, 50);
        char mark[5];
        bzero(mark, 5);
        sprintf(mark, "%c", connfd + '@');
        fontPrint(f, screen, 48, 408, mark, white, 800);
        bmpinfo *writesth = getrgb("writesth.bmp");
        dspybmp(writesth, 241, 429);

        pthread_t tid;
        pthread_create(&tid, NULL, interaction, NULL);

        init_blist();
        add_botton(420, 53, 143, 418, NULL, client_send_text);
        add_botton(53, 53, 652, 418, NULL, emojilist);
        add_botton(53, 53, 578, 418, NULL, client_send_file);
        add_botton(60, 23, 434, 0, NULL, up);
        add_botton(60, 23, 434, 385, NULL, down);
        int tmp_fd;
        while (1)
        {
            bzero(&linksignal, sizeof(int));

            if (read(fd, &linksignal, sizeof(int)) == 0)
            {

                fprintf(stderr, "服务器已断开\n");
                pthread_create(&ctid, NULL, connect_cartoon, NULL);
                fd = socket(AF_INET, SOCK_STREAM, 0);
                flag = 0;

                break;
            }
            switch (linksignal)
            {
            case cnt_updatefd:
                update_clientlist();

                break;
            case cnt_recvtext:
                read(fd, &tmp_fd, sizeof(int));
                pthread_create(&tid, NULL, recv_text, (void *)&tmp_fd);
                break;
            case cnt_recvfile:                  // c2.1
                read(fd, &tmp_fd, sizeof(int)); // c2.2
                pthread_create(&tid, NULL, recv_single_file, (void *)&tmp_fd);
                break;
            case cnt_recvdrct:
                read(fd, &tmp_fd, sizeof(int)); //接收来源fd c21
                pthread_create(&tid, NULL, recv_directory, (void *)&tmp_fd);
                break;

            default:
                break;
            }
        }
    }
    return 0;
}
