#include "head4chat.h"
pthread_mutex_t clientlist_lock;
client *head;
client *init_list(void)
{
    client *head = malloc(sizeof(client));
    if (head != NULL)
    {
        INIT_LIST_HEAD(&head->list);
    }
    return head;
}

client *new_cli(int connfd)
{
    client *new = malloc(sizeof(client));
    if (new != NULL)
    {
        new->connfd = connfd;
        pthread_mutex_init(&new->connfd_lock, NULL);
        INIT_LIST_HEAD(&new->list);
    }
    return new;
}

void del_client(int quitor)
{
    struct list_head *pos, *n;
    list_for_each_safe(pos, n, &head->list)
    {
        client *tmp = list_entry(pos, client, list);
        if (tmp->connfd == quitor)
        {
            list_del(pos);
            free(tmp);
            break;
        }
    }
}

void *update_fdlist()
{

    client *pos;
    client *lpos;
    int count_connfd = 0;
    int linksignal = cnt_updatefd;

    pthread_mutex_lock(&clientlist_lock);
    fprintf(stderr, "已连接客户端：");
    list_for_each_entry(pos, &head->list, list)
    {
        fprintf(stderr, "%d ", pos->connfd);
        count_connfd++;
    }
    fprintf(stderr, "\n");
    list_for_each_entry(pos, &head->list, list)
    {

        pthread_mutex_lock(&pos->connfd_lock);
        write(pos->connfd, &linksignal, sizeof(int));

        write(pos->connfd, &count_connfd, sizeof(int)); // 3
        list_for_each_entry(lpos, &head->list, list)
        {
            write(pos->connfd, &lpos->connfd, sizeof(int));
        }
        pthread_mutex_unlock(&pos->connfd_lock);
    }
    pthread_mutex_unlock(&clientlist_lock);
}

//监测客户端的连接状态
void *detect(void *arg)
{
    int connfd = *(int *)arg;
    char readbuf[SIZE];
    bzero(readbuf, SIZE);
    if (read(connfd, readbuf, SIZE) == 0)
    {
        fprintf(stderr, "监测到客户端%d已断开\n", connfd);
        pthread_mutex_lock(&clientlist_lock);
        del_client(connfd);
        pthread_mutex_unlock(&clientlist_lock);
        update_fdlist();
    }
}

void *clientinfo_tsftext_station(int tmp_fd)
{

    int chfd;
    read(tmp_fd, &chfd, sizeof(int)); //目标ID
    int linksignal = cnt_recvtext;

    pthread_mutex_t *connfd_lock;
    client *pos;

    pthread_mutex_lock(&clientlist_lock);
    list_for_each_entry(pos, &head->list, list)
    {
        if (chfd == pos->connfd)
        {
            connfd_lock = &pos->connfd_lock;
            break;
        }
    }
    pthread_mutex_unlock(&clientlist_lock);

    pthread_mutex_lock(connfd_lock);
    write(chfd, &linksignal, sizeof(int)); //通知目标ID
    write(chfd, &tmp_fd, sizeof(int));     //通知目标ID临时来源fd
    pthread_mutex_unlock(connfd_lock);
}

void *clientinfo_tsffile_station(int tmp_fd)
{
    int chfd;
    read(tmp_fd, &chfd, sizeof(int)); //接收目标ID  //s2
    int linksignal = cnt_recvfile;

    pthread_mutex_t *connfd_lock;
    client *pos;

    //通知目标ID建立连接
    pthread_mutex_lock(&clientlist_lock);
    list_for_each_entry(pos, &head->list, list)
    {
        if (chfd == pos->connfd)
        {
            connfd_lock = &pos->connfd_lock;
            break;
        }
    }
    pthread_mutex_unlock(&clientlist_lock);

    pthread_mutex_lock(connfd_lock);
    write(chfd, &linksignal, sizeof(int)); //给目标ID发信号 //s2.1
    write(chfd, &tmp_fd, sizeof(int));     //通知目标ID临时来源fd //s2.2
    pthread_mutex_unlock(connfd_lock);
}

void *clientinfo_tsfdrct_station(int tmp_fd)
{
    int chfd;
    read(tmp_fd, &chfd, sizeof(int)); //接收目标ID  //s2
    int linksignal = cnt_recvdrct;

    pthread_mutex_t *connfd_lock;
    client *pos;

    //通知目标ID建立连接
    pthread_mutex_lock(&clientlist_lock);
    list_for_each_entry(pos, &head->list, list)
    {
        if (chfd == pos->connfd)
        {
            connfd_lock = &pos->connfd_lock;
            break;
        }
    }
    pthread_mutex_unlock(&clientlist_lock);

    pthread_mutex_lock(connfd_lock);
    write(chfd, &linksignal, sizeof(int)); //给目标ID发信号 //s2.1
    write(chfd, &tmp_fd, sizeof(int));     //通知目标ID临时来源fd //s2.2
    pthread_mutex_unlock(connfd_lock);
}

void *deliver_text(void *arg)
{
    pthread_detach(pthread_self());

    textcon textcon_arg = *(textcon *)arg;
    char textbuf[SIZE];

    while (1)
    {
        bzero(textbuf, SIZE);
        if (recv(textcon_arg.src_fd, textbuf, SIZE, 0) == 0)
        {
            fprintf(stderr, "‘发送’客户端已断开,退出中转站\n");
            break;
        }
        if (send(textcon_arg.dst_fd, textbuf, strlen(textbuf), MSG_NOSIGNAL) <= 0)
        {
            fprintf(stderr, "‘接收’客户端已断开,退出中转站\n");
            break;
        }
    }
    close(textcon_arg.dst_fd);
    close(textcon_arg.src_fd);
    pthread_exit(NULL);
}

void *deliver_file(void *arg)
{
    pthread_detach(pthread_self());
    textcon textcon_arg = *(textcon *)arg;

    char readbuf[READSIZE];
    char filename[PATHSIZE];
    bzero(filename, PATHSIZE);
    int src_id;
    off_t filesize;

    read(textcon_arg.src_fd, &src_id, sizeof(int)); //把来源ID发给目标
    write(textcon_arg.dst_fd, &src_id, sizeof(int));
    fprintf(stderr,"发送来源ID:%d\n",src_id);


    read(textcon_arg.src_fd, filename, PATHSIZE); //把文件名发给目标
    write(textcon_arg.dst_fd, filename,PATHSIZE);
    fprintf(stderr,"发送文件名:%s\n",filename);


    read(textcon_arg.src_fd, &filesize, sizeof(off_t)); //把文件大小发给目标
    write(textcon_arg.dst_fd, &filesize, sizeof(off_t));
    fprintf(stderr,"发送文件大小:%lu\n",filesize);

    off_t count1 = 0,count2=0, total = 0;

    while (1)
    {
        count1 = 0,count2=0;
        bzero(readbuf, READSIZE);
        if ((count1=recv(textcon_arg.src_fd, readbuf, READSIZE, 0)) <= 0)
        {
            fprintf(stderr, "‘发送’客户端已断开,退出中转站\n");
            break;
        }
        if ((count2 = send(textcon_arg.dst_fd, readbuf, count1, MSG_NOSIGNAL)) <= 0)
        {
            fprintf(stderr, "‘接收’客户端已断开,退出中转站\n");
            break;
        }
        total += count2;
    }
    if(total!=filesize)
    {
        fprintf(stderr,"传输文件%s失败\n",filename);
    }
    else
    {
        fprintf(stderr,"传输文件%s成功\n",filename);
    }
    close(textcon_arg.dst_fd);
    close(textcon_arg.src_fd);
    pthread_exit(NULL);
}

void*deliver_drct(void*arg)
{
    pthread_detach(pthread_self());
    textcon textcon_arg = *(textcon *)arg;

    int src_id;
    read(textcon_arg.src_fd, &src_id, sizeof(int)); //把来源ID发给目标
    write(textcon_arg.dst_fd, &src_id, sizeof(int));
    fprintf(stderr,"发送来源ID:%d\n",src_id);

    char buf[50];
    bzero(buf,50);
    read(textcon_arg.src_fd,buf,50);
    //read(textcon_arg.src_fd,buf,50);
    fprintf(stderr,"%s\n",buf);

    // char dst_path[PATHSIZE];
    // while(1)
    // {
    //     bzero(dst_path,PATHSIZE);
    //     if(recv(textcon_arg.src_fd,dst_path,PATHSIZE,0)<=0)
    //     {
    //         fprintf(stderr,"发送目录端已退出，退出中转站\n");
    //         break;
    //     }
    //     if(send(textcon_arg.dst_fd,dst_path,PATHSIZE,MSG_NOSIGNAL)<=0)
    //     {
    //         fprintf(stderr,"接收目录端已退出，退出中转站\n");
    //         break;
    //     }
    // }
    close(textcon_arg.dst_fd);
    close(textcon_arg.src_fd);
    pthread_exit(NULL);


}

int main(int argc, char const *argv[])
{
    int fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in srvaddr, cliaddr;
    socklen_t len = sizeof(srvaddr);
    bzero(&srvaddr, len);
    srvaddr.sin_family = AF_INET;
    srvaddr.sin_port = htons(PORT_ROSE);
    srvaddr.sin_addr.s_addr = inet_addr("0.0.0.0");
    // srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    int optval = 1;
    setsockopt(fd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(int));
    bind(fd, (struct sockaddr *)&srvaddr, len);

    pthread_mutex_init(&clientlist_lock, NULL);

    listen(fd, SIZE);

    head = init_list();
    client *new, *pos;
    pthread_t tid;
    textcon textcon_arg;

    while (1)
    {

        int connfd = accept(fd, (struct sockaddr *)&cliaddr, &len);

        int linksignal;
        int tmp_fd;
        read(connfd, &linksignal, sizeof(int)); // 1

        switch (linksignal)
        {
        case cnt_clicnsvr:
            new = new_cli(connfd);
            pthread_mutex_lock(&clientlist_lock);
            list_add_tail(&new->list, &head->list);
            pthread_mutex_unlock(&clientlist_lock);

            write(connfd, &connfd, sizeof(int));
            update_fdlist();
            pthread_create(&tid, NULL, detect, (void *)&connfd);
            // pthread_create(&tid,NULL,update_fdlist,(void*)&connfd);
            break;
        case cnt_sendtext:
            clientinfo_tsftext_station(connfd);
            break;
        case cnt_recvtext:
            bzero(&textcon_arg, sizeof(textcon));
            read(connfd, &textcon_arg.src_fd, sizeof(int));
            textcon_arg.dst_fd = connfd;
            pthread_create(&tid, NULL, deliver_text, (void *)&textcon_arg);
            break;
        case cnt_sendfile: // s1
            clientinfo_tsffile_station(connfd);
            break;
        case cnt_recvfile: // ss1
            bzero(&textcon_arg, sizeof(textcon));
            read(connfd, &textcon_arg.src_fd, sizeof(int)); // ss2
            textcon_arg.dst_fd = connfd;
            pthread_create(&tid, NULL, deliver_file, (void *)&textcon_arg);
            break;
        case cnt_senddrct://v1
            clientinfo_tsfdrct_station(connfd);
        case cnt_recvdrct:
            bzero(&textcon_arg,sizeof(textcon));
            read(connfd,&textcon_arg.src_fd,sizeof(int));
            textcon_arg.dst_fd=connfd;
            pthread_create(&tid,NULL,deliver_drct,(void*)&textcon_arg);
            break;

        default:
            break;
        }
    }
    return 0;
}
