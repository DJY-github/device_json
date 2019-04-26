#ifndef MODBUS_H_INCLUDED
#define MODBUS_H_INCLUDED

#include<stdio.h>
#include"cJSON.h"
#include<stdlib.h>
#include<memory.h>
#include <unistd.h>
#include <arpa/inet.h>
#include<ctype.h>

#include"GetProfile.h"

#define SIZE_BUF 256

short CRC16_Modbus (char, int);

int write_data(int ,char*,size_t );

int read_data(int , char *, size_t , unsigned int );

float analysis(char ,char );

void modbus_data(Pro_Data,int,char *);

int get_Data(int ,Pro_Data);
#endif // MODBUS_H_INCLUDED
