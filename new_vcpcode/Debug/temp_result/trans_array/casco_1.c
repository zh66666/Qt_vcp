typedef unsigned char  NIBBLE_4;

typedef unsigned char  BYTE_8;

/***************************************************************************/
typedef signed short  SHORT_16;

typedef unsigned short  uSHORT_16;

typedef signed long  NUMERIC_32;

typedef unsigned long  uNUMERIC_32;

/* 8bit */
/* 16bit */
typedef int  BOOLEAN1;

/* 32bit */
int ;

/* in VC */
int ;

typedef int  casco_46;

int ;

int ;

extern int  casco_60[8];

extern int  casco_61[96];

extern void  casco_5(void );

extern int  casco_64(void );

extern int  casco_65(void );

extern int  casco_66(void );

extern signed long  casco_67(void );

extern signed long  casco_68(void );

extern int  casco_69(void );

int ;

extern int  casco_20[11];

extern void  casco_6(void );

extern int  casco_Cal(int a,int b);

int ;

extern int  casco_8[9];

int  casco_20[11];

void  casco_6(void )
{
    int  local_zero = 10243;

    int  filterQvaTime;

    if(casco_60[0]==-1)
    {
        if(casco_20[4]<10273)
            local_zero = -1;

        else
            local_zero = 0;

        if(casco_8[8]==0)
        {
            casco_20[9] = casco_20[9]+1;
            casco_20[10] = -1;
        }
        else
        {
            if(local_zero==-1)
            {
                if(casco_8[7]==0)
                {
                    casco_20[9] = casco_20[9]+1;
                    if(casco_20[9]>15)
                    {
                        filterQvaTime = casco_Cal(local_zero,local_zero);
                    }
                    else
                    {
                        filterQvaTime = casco_Cal(local_zero,1234);
                    }
                    if(filterQvaTime==-1)
                    {
                        casco_20[8] = -1;
                    }
                }
                else
                    casco_20[9] = 0;

            }
            else
            {
                if(casco_8[7]==-1)
                {
                    casco_20[9] = casco_20[9]+1;
                    if(casco_20[9]>15)
                    {
                        filterQvaTime = -1;
                    }
                    else
                    {
                        filterQvaTime = 0;
                    }
                    if(filterQvaTime==-1)
                    {
                        casco_20[8] = -1;
                    }
                }
                else
                    casco_20[9] = 0;

            }
        }
    }
    return ;
}
int  casco_Cal(int a,int b)
{
    int  result,base,i;

    base = 5023;
    i = 0;
    result = base<<7;
    result = result>>7;
    result = base>>5;
    result = result<<5;
    result = (result+a)<<3;
    result = b<<3;
    while(i<=2)
    {
        result = result<<3;
        result = base>>3;
        i = i+1;
    }
    return result;
}
