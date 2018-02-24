typedef unsigned char  NIBBLE_4;

typedef unsigned char  BYTE_8;

/***************************************************************************/
typedef signed short  SHORT_16;

typedef unsigned short  uSHORT_16;

typedef signed long  NUMERIC_32;

typedef unsigned long  uNUMERIC_32;

/* 8bit */
/* 16bit */
typedef enum boolean1
{
    ATP_FALSE=0,
    ATP_TRUE=-1
}  BOOLEAN1;

/* 32bit */
struct structVar1
{
    BOOLEAN1  sVar1_1;
    int  sVar1_2;
    NUMERIC_32  sVar1_3;
    NUMERIC_32  sVar1_4;
    BOOLEAN1  sVar1_5;
    BOOLEAN1  sVar1_6;
    NUMERIC_32  sVar1_7;
    NUMERIC_32  sVar1_8;
} ;

/* in VC */
struct structVar2
{
    struct structVar1  sVar2_1;
    int  sVar2_2;
    char  sVar2_3;
} ;

typedef enum casco_32
{
    casco_33,
    casco_34,
    casco_35,
    casco_36,
    casco_37,
    casco_38,
    casco_39,
    casco_40,
    casco_41,
    casco_42,
    casco_43,
    casco_44,
    casco_45
}  casco_46;

struct casco_47
{
    BOOLEAN1  casco_48;
    casco_46  casco_49;
    NUMERIC_32  casco_50;
    NUMERIC_32  casco_51;
    BOOLEAN1  casco_52;
    BOOLEAN1  casco_53;
    NUMERIC_32  casco_54;
    NUMERIC_32  casco_55;
} ;

struct casco_56
{
    int  casco_57;
    int  casco_58;
    char  casco_59;
} ;

extern struct casco_47  casco_60;

extern struct casco_56  casco_61[32];

extern void  casco_5(void );

extern BOOLEAN1  casco_64(void );

extern BOOLEAN1  casco_65(void );

extern BOOLEAN1  casco_66(void );

extern NUMERIC_32  casco_67(void );

extern NUMERIC_32  casco_68(void );

extern casco_46  casco_69(void );

struct casco_19
{
    NUMERIC_32  casco_21;
    NUMERIC_32  casco_22;
    NUMERIC_32  casco_23;
    NUMERIC_32  casco_24;
    NUMERIC_32  casco_25;
    NUMERIC_32  casco_26;
    NUMERIC_32  casco_27;
    NUMERIC_32  casco_28;
    BOOLEAN1  casco_29;
    NUMERIC_32  casco_30;
    BOOLEAN1  casco_31;
} ;

extern struct casco_19  casco_20;

extern void  casco_6(void );

extern int  casco_Cal(int a,int b);

struct casco_7
{
    int  casco_10;
    int  casco_11;
    int  casco_12;
    int  casco_13;
    int  casco_14;
    int  casco_15;
    int  casco_16;
    int  casco_17;
    int  casco_18;
} ;

extern struct casco_7  casco_8;

struct casco_19  casco_20;

void  casco_6(void )
{
    int  local_zero = 10243;

    int  filterQvaTime;

    if(casco_60.casco_48==ATP_TRUE)
    {
        if(casco_20.casco_25<10273)
            local_zero = ATP_TRUE;

        else
            local_zero = ATP_FALSE;

        if(casco_8.casco_18==ATP_FALSE)
        {
            casco_20.casco_30 = casco_20.casco_30+1;
            casco_20.casco_31 = ATP_TRUE;
        }
        else
        {
            if(local_zero==ATP_TRUE)
            {
                if(casco_8.casco_17==ATP_FALSE)
                {
                    casco_20.casco_30 = casco_20.casco_30+1;
                    if(casco_20.casco_30>15)
                    {
                        filterQvaTime = casco_Cal(local_zero,local_zero);
                    }
                    else
                    {
                        filterQvaTime = casco_Cal(local_zero,1234);
                    }
                    if(filterQvaTime==ATP_TRUE)
                    {
                        casco_20.casco_29 = ATP_TRUE;
                    }
                }
                else
                    casco_20.casco_30 = 0;

            }
            else
            {
                if(casco_8.casco_17==ATP_TRUE)
                {
                    casco_20.casco_30 = casco_20.casco_30+1;
                    if(casco_20.casco_30>15)
                    {
                        filterQvaTime = ATP_TRUE;
                    }
                    else
                    {
                        filterQvaTime = ATP_FALSE;
                    }
                    if(filterQvaTime==ATP_TRUE)
                    {
                        casco_20.casco_29 = ATP_TRUE;
                    }
                }
                else
                    casco_20.casco_30 = 0;

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
