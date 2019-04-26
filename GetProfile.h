#ifndef GETPROFILE_H_INCLUDED
#define GETPROFILE_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<memory.h>
#include<ctype.h>
#include"cJSON.h"
#define SIZE 64
#define BUFF_SIZE 256

typedef struct field_data{
    char* name;
    int addr;
    int num;
    float coeff;
}Field;

typedef struct profile_data{
    unsigned char Address;
    unsigned char Function;
    Field Data[BUFF_SIZE];
    int len;
}Pro_Data;


int str_to_int(char*);
cJSON* OpenProfile(char*);
Pro_Data GetProfileData(cJSON*);

#endif // GETPROFILE_H_INCLUDED
