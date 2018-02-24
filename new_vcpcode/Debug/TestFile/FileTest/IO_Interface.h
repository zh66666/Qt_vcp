#ifndef IO_INTERFACE_H
#define IO_INTERFACE_H


extern long int g_USR_BufRecv1[256];
extern long int g_USR_BufRecv2[256];
extern long int g_USR_BufFetch1[256];
extern long int g_USR_BufFetch2[256];
extern long int g_USR_BufSend1[256];
extern long int g_USR_BufSend2[256];
extern long int g_USR_BufDeliver1[256];
extern long int g_USR_BufDeliver2[256];

extern void F_USR_DataRecv(long int inBufId);
extern void F_USR_DataFetch(long int inBufId);
extern void F_USR_DataSend(long int inBufId);
extern void F_USR_DataDeliver(long int inBufId);


#endif
