#include"setport.h"
#include"GetProfile.h"
#include"modbus.h"

#include<unistd.h>
#include<sys/types.h>
#include<sys/stat.h>
#include<fcntl.h>

#define BUFF_SIZE 256

int main(int argc,char **argv)
{
    int prot_fd;
    int baud;

    //命令行参数判断
    for(int i=1;i<argc;i++){
        if (strpbrk(argv[i],"./") == NULL){
            break;
        }
        if (fopen(argv[i],"r")==NULL){
            printf("open file error[%s]:[%s] [portfile] [<Serial file>] [baud]\n",argv[i],argv[0]);
            return -1;
        }
    }
    //打开串口
    if ((prot_fd = open(argv[1],O_RDWR|O_NOCTTY|O_NDELAY))== -1)
    {
        printf("cannot port fail!\n");
        return -1;
    }

    //设置串口参数
    baud=(strpbrk(argv[argc-1],"./")==NULL) ? (atoi(argv[argc-1])):DEFAULT_BAUD;
    if (set_info(prot_fd,baud,DEFAULT_DATABITS,DEFAULT_STOPBITS,DEFAULT_PARITY) == -1)
    {
        printf("Set info Errorn!\n");
        return -1;
    }

    //打开配置文件，整理数据到结构体
    int pro_num = 0;
    cJSON *json;
    Pro_Data pro[BUFF_SIZE];
    memset(pro,0,BUFF_SIZE);
    while(1){
        for(int i = 2; i < argc; i++){
            char data[8];
            if( strpbrk(argv[i] , "./" ) == NULL ){
                break;
            }
            json = OpenProfile(argv[i]);
            pro[pro_num] = GetProfileData(json);
            get_Data(prot_fd,pro[pro_num]);
            pro_num = pro_num+1;
            cJSON_Delete(json);
        }
        sleep(1);
    }

    //关闭串口
    close(prot_fd);
    return 0;
}
