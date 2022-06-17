#include"head4chat.h"
btif*bthead;

int main(int argc, char const *argv[])
{
    int tfd=open("/dev/input/event0", O_RDWR);
    struct input_event buf;
    //int tmp1 = 0, tmp2 = 1;
    int x,y;
    while (1)
    {
        bzero(&buf, sizeof(buf));
        read(tfd, &buf, sizeof(buf));

        switch (buf.type)
        {
        case EV_ABS:    //发生了坐标事件
            switch (buf.code)
            {
            case ABS_X: //触发了x坐标    
                x=(float)buf.value / 1.28;
                break;
                
            case ABS_Y: //触发了y坐标
                y=(float)buf.value / 1.25;
                break;
            
            default:
                break;
            }
            break;
        case EV_KEY:    //发生了触碰事件
            switch (buf.code)
            {
            case BTN_TOUCH:
                if(buf.value==0)
                {
                    fprintf(stderr,"从（%d,%d)处离开\n",x,y);
                    
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
