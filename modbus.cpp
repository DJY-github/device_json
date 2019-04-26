#include"modbus.h"

/*CRC16*/
short CRC16_Modbus (char *pdata, int len)
{
    unsigned short crc=0xFFFF;
    int i, j;
    for ( j=0; j<len;j++)
    {
        crc=crc^pdata[j];
        for ( i=0; i<8; i++)
        {
            if( ( crc&0x0001) >0)
            {
                crc=crc>>1;
                crc=crc^ 0xa001;
            }
            else
                crc=crc>>1;
        }
    }
    return htons(crc);
}
//write
int write_data(int fd,char* buff,size_t bytes)
{
    int len=0;
    int res=0;
    while(1)
    {
        res = write(fd, buff, bytes-len);
        len = len + res;
        if (res<0)
        {
            printf("write error");
            return -1;
        }

        if (len >= bytes)
        {
            break;
        }
    }

    if(len<0)
    {
        perror("write fail!");
        return -1;
    }
    return len;
}
//read
int read_data(int fd, char *buff, size_t bytes, unsigned int timout)
{
    int nfds;

    fd_set  readfds;
    struct timeval tv;

    tv.tv_sec = timout;
    tv.tv_usec = 0;

    FD_ZERO(&readfds);
    FD_SET(fd, &readfds);
    nfds = select(fd+1, &readfds, NULL, NULL, &tv);
    int len=0;
    int res=0;
    memset(buff,0,SIZE_BUF);
    while(1)
    {
        if(nfds > 0 && FD_ISSET(fd,&readfds))
        {
            res=read(fd,buff+len,bytes-len);

            len=len+res;
            if (res<0)
            {
                printf("read error");
                return -1;
            }

            if (len>=bytes)
            {
                break;
            }
        }else if (nfds==0){
            printf("Waiting for timeout!");
            return -1;
        }else if(nfds==-1){
            printf("select error!");
            return -1;
        }
    }
    return len;
}

float analysis(char high,char low)
{
    float data=(high<<8)+(low&0xff);
    return data;
}

void modbus_data(Pro_Data pro,int filed,char* data)
{
    data[0]=pro.Address;
    data[1]=pro.Function;
    if (pro.len<=0)
    {
        printf("No ID or N parameters are set\n");
        return;
    }

    data[2]=pro.Data[filed].addr>>8;
    data[3]=pro.Data[filed].addr&0xff;

    data[4]=pro.Data[filed].num>>8;
    data[5]=pro.Data[filed].num&0xff;

    short crc1=CRC16_Modbus(data,6);
    data[6]=(crc1&0xff00)>>8;
    data[7]=crc1&0xff;
}

int get_Data(int fd,Pro_Data pro){
    if (pro.len<=0)
    {
        printf("No ID or N parameters are set\n");
        return -1;
    }

    for(int i=0;i<pro.len;i++)
    {
        char data[8];
        memset(data,0,8);
        modbus_data(pro,i,data);

        //发送
        write_data(fd,data,8);

        //接收
        int rlen=0;
        char buff[SIZE_BUF]={0};
        rlen=read_data(fd,buff,3,3);
        rlen=read_data(fd,buff,buff[rlen-1]+2,3);

        //打印
        float coeff = pro.Data[i].coeff;
        if ( coeff <= 0.0)
        {
           coeff = 1.0;
        }

         float w=(float)(analysis(buff[0],buff[1])*coeff);
         printf("%s:%0.2f\n",pro.Data[i].name,w);
    }

    return 0;
}
















