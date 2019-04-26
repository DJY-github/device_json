#include"setport.h"
/*setting Speed*/
TerMios SetSpeed(int fd,TerMios Opt,int speed)
{
    int speed_arr[] = { B38400, B19200, B9600, B4800, B2400, B1200, B300};
    int name_arr[] = {38400, 19200, 9600, 4800, 2400, 1200, 300};

    if(tcgetattr(fd, &Opt) != 0)  //obtain terminal parameters
    {
        perror("tcgetattr fd");
    }

    for(int i = 0; i < sizeof(speed_arr)/sizeof(int); i++)
    {
        if(speed == name_arr[i])
        {
            tcflush(fd, TCIOFLUSH);
            cfsetispeed(&Opt, speed_arr[i]);

            cfsetospeed(&Opt, speed_arr[i]);

            if(tcsetattr(fd, TCSANOW, &Opt) != 0)
            {
                perror("tcsetattr fd");
            }
        }
        tcflush(fd, TCIOFLUSH);
    }
    return Opt;
}

/*set databits,stopbits,parity*/
int set_info(int fd,int speed,int databits,int stopbits,int parity)
{
    TerMios Opt;
    bzero(&Opt,sizeof(Opt));

    Opt=SetSpeed(fd,Opt,speed);
    Opt.c_cflag |= CLOCAL|CREAD;
    Opt.c_cflag &= ~CSIZE;

    /*databits:7,8*/
    switch (databits)
    {
        case 7:
            Opt.c_cflag |= CS7;
            break;
        case 8:
            Opt.c_cflag |= CS8;
            break;
        default:
            perror("unsupported data sizen!");
        return -1;
    }

    /*parity:N,E,O,S*/
    switch(parity)
    {
        case 'n':
        case 'N':
            Opt.c_cflag &= ~PARENB; /* Clear parity enable */
            Opt.c_iflag &= ~INPCK; /* Enable parity checking */
            break;
        case 'o':
        case 'O':
            Opt.c_cflag |= (PARODD | PARENB); /* 设置为奇效验*/
            Opt.c_iflag |= (INPCK | ISTRIP); /* Disnable parity checking */
            break;
        case 'e':
        case 'E':
            Opt.c_cflag |= PARENB; /* Enable parity */
            Opt.c_cflag &= ~PARODD; /* Conversion to Duality Test*/
            Opt.c_iflag |= (INPCK | ISTRIP); /* Disnable parity checking */
            break;
        case 's':
        case 'S': /*as no parity*/
            Opt.c_cflag &= ~PARENB;
            Opt.c_cflag &= ~CSTOPB;
            break;
        default:
            perror("Unsupported parityn");
            return -1;
    }

    /*stopbits:1,2*/
    switch (stopbits)
    {
        case 1:
            Opt.c_cflag &= ~CSTOPB;
            break;
        case 2:
            Opt.c_cflag |= CSTOPB;
            break;
        default:
            perror("Unsupported stop bitsn");
            return -1;
    }
    /* Set input parity option */
    if (parity != 'n')
    {
        Opt.c_iflag |= INPCK;
        Opt.c_cc[VTIME] = 0; /* setsockopt 15 seconds*/
        Opt.c_cc[VMIN] = 0; /* Update the options and do it NOW */
        tcflush(fd,TCIFLUSH);
    }

    Opt.c_cc[VTIME] = 1; //
    Opt.c_cc[VMIN] = 0;

    if (tcsetattr(fd,TCSANOW,&Opt) != 0)
    {
        perror("SetupSerial 2");
        return -1;
    }
    return 0;
}
