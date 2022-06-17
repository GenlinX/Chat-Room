#include "head4chat.h"
struct sockaddr *srvaddr;
socklen_t len;
int fd;
int connfd;
int choosing;
clientlist *head;
pthread_mutex_t clientlist_lock;

clientlist *init_list(void)
{
    head = malloc(sizeof(client));
    if (head != NULL)
    {
        INIT_LIST_HEAD(&head->clist);
    }
    return head;
}

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

void *destroy_clientlist()
{
    struct list_head *pos, *n;
    pthread_mutex_lock(&clientlist_lock);

    list_for_each_safe(pos, n, &head->clist)
    {
        clientlist *tmp = list_entry(pos, clientlist, clist);
        list_del(pos);
        free(tmp);
    }

    head = init_list();
    pthread_mutex_unlock(&clientlist_lock);
}

void *update_clientlist()
{
    destroy_clientlist();

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
    list_for_each_entry(pos, &head->clist, clist)
    {
        printf("%d ", pos->client_fd);
    }
    pthread_mutex_unlock(&clientlist_lock);
    printf("\n");
}

void *send_text(void *arg)
{
    int chfd = *(int *)arg; //选择的好友
    int linksignal = cnt_sendtext;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int));
    write(newfd, &chfd, sizeof(int)); //目标的ID

    char writebuf[SIZE];
    fprintf(stderr, "请开始输入文本\n");
    write(newfd,&connfd,sizeof(int));

    while (1)
    {
        bzero(writebuf, SIZE);
        fgets(writebuf, SIZE, stdin);
        if(strcmp(writebuf,"quit\n")==0)
        {
            fprintf(stderr,"退出发送文本\n");
            return NULL;
        }
        if (send(newfd, writebuf, strlen(writebuf), MSG_NOSIGNAL) <= 0)
        {
            fprintf(stderr, "中转站已退出，退出'发送文本'\n");
            break;
        }
    }
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
    while (1)
    {
        bzero(textbuf, SIZE);
        if (recv(newfd, textbuf, SIZE, 0) == 0)
        {
            fprintf(stderr, "中转站已断开,退出'接收信息'\n");
            close(newfd);
            break;
        }
        fprintf(stderr, "%s", textbuf);
    }
    pthread_exit(NULL);
}
void*copy_single_file(void*arg)
{
    pthread_detach(pthread_self());
    int linksignal = cnt_sendfile;
    int newfd = socket(AF_INET, SOCK_STREAM, 0);
    connect(newfd, srvaddr, len);

    write(newfd, &linksignal, sizeof(int)); //发送信号给服务器  //c1
    write(newfd, &choosing, sizeof(int)); //目标ID发给服务器   //c2
    write(newfd, &connfd, sizeof(int));   //把自己ID发给对方    //c3

    int singlefile = open((char *)arg, O_RDONLY);
    struct stat info;       //获取文件信息
    stat((char*)arg,&info);
    char filename[PATHSIZE];

    char *p = strtok((char *)arg, "/");
    while (p)
    {
        bzero(filename, PATHSIZE);
        strcpy(filename, p);
        p = strtok(NULL, "/");
    }

    free(arg);
    
    write(newfd, filename, PATHSIZE);   //上传文件名给服务器
    write(newfd, &info.st_size, sizeof(off_t)); //上传文件大小给服务器
    printf("发送文件名：%s 发送文件大小：%lu\n", filename, info.st_size);
    char readbuf[READSIZE];
    size_t count1=0,count2=0,total=0;
    while(1)
    {   
        count1=0;
        count2=0;
        bzero(readbuf,READSIZE);
        if((count1=read(singlefile,readbuf,READSIZE))<=0)
        {
            fprintf(stderr,"文件读取已终止\n");
            break;
        }
        if((count2=send(newfd,readbuf,count1,MSG_NOSIGNAL))<=0)
        {
            fprintf(stderr,"中转站已退出，终止发送文件\n");
            break;
        }
        total+=count2;
    }

    if(total!=info.st_size)
    {
        fprintf(stderr,"发文件失败\n");
    }
    else 
    {
        fprintf(stderr,"发送文件成功\n");
    }
    pthread_exit(NULL);
}

//接收单个文件
void*recv_single_file(void*arg)
{
    pthread_detach(pthread_self());
    int tmp_fd=*(int*)arg;
    int linksignal;
    linksignal = cnt_recvfile;
    int newfd=socket(AF_INET,SOCK_STREAM,0);
    connect(newfd,srvaddr,len);

    write(newfd,&linksignal,sizeof(int));   //cc1
    write(newfd, &tmp_fd, sizeof(int));     //cc2

    int src_id;                             //接收对方ID
    read(newfd,&src_id,sizeof(int));
    fprintf(stderr,"已收到来源ID:%d\n",src_id);

    char filename[PATHSIZE];
    off_t filesize;
    bzero(filename,PATHSIZE);
    read(newfd,filename,PATHSIZE);
    read(newfd,&filesize,sizeof(off_t));

    fprintf(stderr,"收到文件名：%s 文件大小：%lu\n",filename,filesize);
    int dstfile;
    if ((dstfile = open(filename, O_RDWR | O_TRUNC | O_CREAT, 0644)) == -1)
    {
        perror("创建文件失败");
        return NULL;
    }
    char readbuf[READSIZE];
    off_t count1=0,count2=0,total=0;

    while (1)
    {
        count1 = 0,count2=0;
        bzero(readbuf, READSIZE);
        if ((count1=recv(newfd, readbuf, READSIZE, 0)) <= 0)
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
    if (total == filesize)
    {
        fprintf(stderr,"接收文件%s成功\n", filename);
    }
    else
    {
        fprintf(stderr, "接收文件%s失败，总大小：%lu,拷贝总量：%lu", filename,filesize, total);
    }
}


//发送文件按钮
void*send_file(void*arg)
{
    fprintf(stderr, "请输入一个路径\n");
    char curpath[PATHSIZE]; //当前工作路径
    char *srcpath=calloc(PATHSIZE,sizeof(char)); //源文件、目录路径
    bzero(curpath, PATHSIZE);
    getcwd(curpath, PATHSIZE);
    int srcfile;

    while (1)
    {
        bzero(srcpath, PATHSIZE);
        fgets(srcpath, PATHSIZE, stdin);
        strtok(srcpath,"\n");
        if (chdir(srcpath) == 0)
        {
            chdir(curpath);
            fprintf(stderr, "目录\n");
            send_directory((void*)srcpath);
            break;
        }
        else if ((srcfile = open(srcpath, O_RDONLY))>= 0)
        {
            fprintf(stderr, "单个文件\n");
            copy_single_file((void*)srcpath);
            break;
        }
        else
        {
            chdir(curpath);
            fprintf(stderr, "非法路径，请再次输入\n");
        }
    }
}

void *send_directory_recursively(int newfd, char *src_path, char *dst_path)
{
    DIR *dp = opendir(src_path);
    printf("获得的源路径:%s\n",src_path);
    struct dirent *ep;
    struct stat info;

    while(1)
    {
        ep=readdir(dp);
        if(ep==NULL)
        {
            break;
        }
        if(strcmp(ep->d_name,".")==0||strcmp(ep->d_name,"..")==0)
        {
            continue;
        }
        bzero(&info,sizeof(struct stat));
        stat(ep->d_name,&info);

        if(S_ISREG(info.st_mode))
        {   
            //如果是普通文件
            continue;
        }
        else if(S_ISDIR(info.st_mode))
        {
            //如果是目录
            strcat(src_path,"/");
            strcat(src_path,ep->d_name);
            printf("整合后的源路径%s\n",src_path);
            strcat(dst_path,"/");
            strcat(dst_path,ep->d_name);
            printf("整合后的目标路径%s\n",src_path);
            send(newfd,dst_path,PATHSIZE,MSG_NOSIGNAL);
            printf("发送%s\n",dst_path);
            send_directory_recursively(newfd,src_path,dst_path);
        }
    
    }
}

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

    write(newfd, &linksignal, sizeof(int)); //发送信号给服务器  //c1    v1
    write(newfd, &choosing, sizeof(int));   //目标ID发给服务器   //c2   v2
    
    printf("发送自己ID%d\n",connfd);
    write(newfd, &connfd, sizeof(int));     //把自己ID发给对方    //c3
    
    write(newfd, dst_path, PATHSIZE); //先把最外层目录发给目标

    //开始递归地发送目录
    send_directory_recursively(newfd, src_path, dst_path);
}

void*recv_directory(void*arg)
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
    while(1)
    {
        bzero(dst_path,PATHSIZE);
        if(recv(newfd,dst_path,PATHSIZE,0)<=0)
        {
            fprintf(stderr,"中转站已退出，退出接收目录\n");
            break;
        }
        fprintf(stderr,"接收到目录%s\n",dst_path);
        mkdir(dst_path,0644);
    }
    close(newfd);
    pthread_exit(NULL);
}

void *client_choice(void *arg)
{
    int chfd;

    while (1)
    {
        fprintf(stderr, "请选择好友\n");

        scanf("%d", &chfd);

        if (chfd == connfd)
        {
            fprintf(stderr, "不能选择自己\n");
            continue;
        }

        choosing=chfd;
        clientlist *pos;
        int flag = 0;

        pthread_mutex_lock(&clientlist_lock);
        list_for_each_entry(pos, &head->clist, clist)
        {
            if (pos->client_fd == chfd)
            {
                flag = 1;
                break;
            }
        }
        pthread_mutex_unlock(&clientlist_lock);
        if (flag == 1)
        {
            fprintf(stderr, "选择好友%d成功\n", chfd);
        }
        else
        {
            fprintf(stderr, "找不到好友%d\n", chfd);
            continue;
        }

        int chfc;
        pthread_t tid;
        while (1)
        {
            fprintf(stderr, "请输入功能:3发送文本，5发送文件\n");
            scanf("%d", &chfc);
            getchar();
            if (chfc == 3)
            {
                send_text((void *)&chfd);
                // pthread_create(&tid,NULL,send_text,(void*)&chfd);
                // pthread_join(tid,NULL);
                break;
            }
            else if (chfc == 5)
            {

                send_file((void*)&chfd);
                break;
            }
        }
    }
}

int main(int argc, char const *argv[])
{
    printf("%ld\n",sizeof(off_t));
    struct sockaddr_in src_srvaddr;
    len = sizeof(src_srvaddr);
    bzero(&src_srvaddr, len);
    src_srvaddr.sin_family = AF_INET;
    src_srvaddr.sin_port = htons(PORT_ROSE);
    src_srvaddr.sin_addr.s_addr = inet_addr(ADDR);
    // src_srvaddr.sin_addr.s_addr = htonl(INADDR_ANY);
    srvaddr = (struct sockaddr *)&src_srvaddr;
    head = init_list();

    fd = socket(AF_INET, SOCK_STREAM, 0);
    int flag = 0;
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

        int linksignal = cnt_clicnsvr;
        write(fd, &linksignal, sizeof(int)); // 1
        fprintf(stderr, "连接服务器成功!\n");

        read(fd, &connfd, sizeof(int)); // 2
        fprintf(stderr, "本客户端fd:%d\n", connfd);

        pthread_t tid;
        pthread_create(&tid, NULL, client_choice, NULL);

        int tmp_fd;
        while (1)
        {
            bzero(&linksignal, sizeof(int));

            if (read(fd, &linksignal, sizeof(int)) == 0)
            {

                fprintf(stderr, "服务器已断开\n");
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
            case cnt_recvfile:
                read(fd, &tmp_fd, sizeof(int));
                pthread_create(&tid, NULL, recv_single_file, (void *)&tmp_fd);
                break;

            default:
                break;
            }
        }
    }
    return 0;
}