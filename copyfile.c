#include "head4chat.h"
int choosing;
struct tidarg
{
    pthread_t selftid;
    pthread_t tid;
};
void *terminate_send_file(void *arg)
{
    pthread_t selftid=*(pthread_t*)arg;
    pthread_cancel(selftid);
    add_botton(420, 53, 143, 418, NULL, client_send_text);
    add_botton(53, 53, 652, 418, NULL, emojilist);
    add_botton(53, 53, 578, 418, NULL, client_send_file);
    delete_botton(127,0);//删除自身按钮
    dspybmp(writesth, 241, 429);
}
void *send_single_file(void *arg)
{
}
void *send_file(void *arg)
{
    pthread_t *selftid=(pthread_t*)arg;
    add_botton(673, 408, 127, 0, (void *)selftid, terminate_send_file);
    delete_botton(143, 418); //删除client_send_text按钮
    delete_botton(652, 418); //删除emojilist按钮
    delete_botton(578, 418); //删除client_send_file按钮
    fprintf(stderr, "请输入一个路径\n");
    char curpath[PATHSIZE]; //当前工作路径
    char srcpath[PATHSIZE]; //源文件、目录路径
    bzero(curpath, PATHSIZE);

    getcwd(curpath, PATHSIZE);
    int srcfile;

    while (1)
    {
        bzero(srcpath, PATHSIZE);
        fgets(srcpath, PATHSIZE, stdin);
        if (srcfile = open(srcpath, O_RDONLY) >= 0)
        {

            break;
        }
        else if (chdir(srcpath) == 0)
        {
            break;
        }
        else
        {
            chdir(curpath);
            fprintf(stderr, "非法路径，请再次输入\n");
        }
        pthread_testcancel();
    }
}
void *client_send_file(void *arg)
{
    pthread_t selftid = pthread_self();
    bmpinfo *writesth = getrgb("writesth.bmp");
    bmpinfo *emwritesth = getrgb("emwritesth.bmp");

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
    if (flag != 1)
    {
        return NULL;
    }

    pthread_t tid;
    pthread_create(&tid, NULL, send_file,(void*)&selftid);

    

    int pmy_choosing = choosing;
    while (1)
    {
        if (choosing != pmy_choosing)
        {
            pthread_cancel(tid);
            displaying_chat_history(choosing, NOWPL);
            add_botton(420, 53, 143, 418, NULL, client_send_text);
            add_botton(53, 53, 652, 418, NULL, emojilist);
            add_botton(53, 53, 578, 418, NULL, client_send_file);
            dspybmp(writesth, 241, 429);
            break;
        }
        pthread_testcancel();
    }
    free(writesth);
}
int main(int argc, char const *argv[])
{
    /* code */
    return 0;
}
