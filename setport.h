#ifndef SETPORT_H_INCLUDED
#define SETPORT_H_INCLUDED

#include<stdio.h>
#include<errno.h>
#include<termios.h>
#include<string.h>
#include<termios.h>

#define DEFAULT_BAUD 9600
#define DEFAULT_DATABITS 8
#define DEFAULT_STOPBITS 1
#define DEFAULT_PARITY 'N'

typedef struct termios TerMios;

/*setting Speed*/
TerMios SetSpeed(int ,TerMios ,int );

/*set databits,stopbits,parity*/
int set_info(int ,int ,int ,int ,int );

#endif // SETPORT_H_INCLUDED
