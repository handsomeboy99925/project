#include "stdlib.h"
#include "oled.h"
#include "oledfont.h"
#include "main.h"

/*
 * OLED���Դ�
 * ÿ���ֽڱ�ʾ8������, 128,��ʾ��128��, 8��ʾ��64��, ��λ��ʾ������.
 * ����:OLED_GRAM[0][0],�����˵�һ��,��1~8�е�����. OLED_GRAM[0][0].0,����ʾ����(0,0)
 * ���Ƶ�: OLED_GRAM[1][0].1,��ʾ����(1,1), OLED_GRAM[10][1].2,��ʾ����(10,10),
 *
 * ��Ÿ�ʽ����(��λ��ʾ������).
 * [0]0 1 2 3 ... 127
 * [1]0 1 2 3 ... 127
 * [2]0 1 2 3 ... 127
 * [3]0 1 2 3 ... 127
 * [4]0 1 2 3 ... 127
 * [5]0 1 2 3 ... 127
 * [6]0 1 2 3 ... 127
 * [7]0 1 2 3 ... 127
 */
static uint8_t OLED_GRAM[128][8];



//���Ժ���
void OLED_ColorTurn(uint8_t i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xA6,OLED_CMD);//������ʾ
        }
    if(i==1)
        {
            OLED_WR_Byte(0xA7,OLED_CMD);//��ɫ��ʾ
        }
    }

//��Ļ��ת180��
void OLED_DisplayTurn(uint8_t i)
{
    if(i==0)
        {
            OLED_WR_Byte(0xC8,OLED_CMD);//������ʾ
            OLED_WR_Byte(0xA1,OLED_CMD);
        }
    if(i==1)
        {
            OLED_WR_Byte(0xC0,OLED_CMD);//��ת��ʾ
            OLED_WR_Byte(0xA0,OLED_CMD);
        }
}



/**
 * @brief       �����Դ浽OLED
 * @param       ��
 * @retval      ��
 */
void OLED_Refresh(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)
    {
        OLED_WR_Byte (0xb0 + i, OLED_CMD); /* ����ҳ��ַ��0~7�� */
        OLED_WR_Byte (0x00, OLED_CMD);     /* ������ʾλ�á��е͵�ַ */
        OLED_WR_Byte (0x10, OLED_CMD);     /* ������ʾλ�á��иߵ�ַ */

        for (n = 0; n < 128; n++)
        {
            OLED_WR_Byte(OLED_GRAM[n][i], OLED_DATA);
        }
    }
}

#if OLED_MODE == 1    /* ʹ��8080��������OLED */
/**
 * @brief       ͨ��ƴ�յķ�����OLED���һ��8λ����
 * @param       data: Ҫ���������
 * @retval      ��
 */
static void oled_data_out(uint8_t data)
{
    GPIOC->ODR = (GPIOC->ODR & 0XFF00) | (data & 0X00FF);
}

/**
 * @brief       ��OLEDд��һ���ֽ�
 * @param       data: Ҫ���������
 * @param       cmd: ����/�����־ 0,��ʾ����;1,��ʾ����;
 * @retval      ��
 */
static void OLED_WR_Byte(uint8_t data, uint8_t cmd)
{
    oled_data_out(data);
    OLED_RS(cmd);
    OLED_CS(0);
    OLED_WR(0);
    OLED_WR(1);
    OLED_CS(1);
    OLED_RS(1);
}
#else   /* ʹ��SPI����OLED */

/**
 * @brief       ��OLEDд��һ���ֽ�
 * @param       data: Ҫ���������
 * @param       cmd: ����/�����־ 0,��ʾ����;1,��ʾ����;
 * @retval      ��
 */
static void OLED_WR_Byte(uint8_t data, uint8_t cmd)
{
    uint8_t i;
    OLED_RS(cmd);   /* д���� */
    OLED_CS(0);

    for (i = 0; i < 8; i++)
    {
        OLED_SCLK(0);

        if (data & 0x80)
        {
            OLED_SDIN(1);
        }
        else
        {
            OLED_SDIN(0);
        }

        OLED_SCLK(1);
        data <<= 1;
    }

    OLED_CS(1);
    OLED_RS(1);
}

#endif
/**
 * @brief       ����OLED��ʾ
 * @param       ��
 * @retval      ��
 */
void OLED_DisPlay_On(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    OLED_WR_Byte(0X14, OLED_CMD);   /* DCDC ON */
    OLED_WR_Byte(0XAF, OLED_CMD);   /* DISPLAY ON */
}

/**
 * @brief       �ر�OLED��ʾ
 * @param       ��
 * @retval      ��
 */
void OLED_DisPlay_Off(void)
{
    OLED_WR_Byte(0X8D, OLED_CMD);   /* SET DCDC���� */
    OLED_WR_Byte(0X10, OLED_CMD);   /* DCDC OFF */
    OLED_WR_Byte(0XAE, OLED_CMD);   /* DISPLAY OFF */
}

/**
 * @brief       ��������,������,������Ļ�Ǻ�ɫ��!��û����һ��!!!
 * @param       ��
 * @retval      ��
 */
void OLED_Clear(void)
{
    uint8_t i, n;

    for (i = 0; i < 8; i++)for (n = 0; n < 128; n++)
        OLED_GRAM[n][i] = 0X00;

    OLED_Refresh();    /* ������ʾ */
}

//���� 
//x:0~127
//y:0~63
void OLED_DrawPoint(uint8_t x,uint8_t y)
{
    uint8_t i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]|=n;
}

//���һ����
//x:0~127
//y:0~63
void OLED_ClearPoint(uint8_t x,uint8_t y)
{
    uint8_t i,m,n;
    i=y/8;
    m=y%8;
    n=1<<m;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
    OLED_GRAM[x][i]|=n;
    OLED_GRAM[x][i]=~OLED_GRAM[x][i];
}

//����
//x:0~128
//y:0~64
void OLED_DrawLine(uint8_t x1,uint8_t y1,uint8_t x2,uint8_t y2)
{
    uint8_t i,k,k1,k2;
    if((x1<0)||(x2>128)||(y1<0)||(y2>64)||(x1>x2)||(y1>y2))
        return;
    if(x1==x2)    //������
    {
            for(i=0;i<(y2-y1);i++)
            {
                OLED_DrawPoint(x1,y1+i);
            }
    }
    else if(y1==y2)   //������
    {
            for(i=0;i<(x2-x1);i++)
            {
                OLED_DrawPoint(x1+i,y1);
            }
    }
    else      //��б��
    {
        k1=y2-y1;
        k2=x2-x1;
        k=k1*10/k2;
        for(i=0;i<(x2-x1);i++)
            {
              OLED_DrawPoint(x1+i,y1+i*k/10);
            }
    }
}

//x,y:Բ������
//r:Բ�İ뾶
void OLED_DrawCircle(uint8_t x,uint8_t y,uint8_t r)
{
	int a, b,num;
    a = 0;
    b = r;
    while(2 * b * b >= r * r)      
    {
        OLED_DrawPoint(x + a, y - b);
        OLED_DrawPoint(x - a, y - b);
        OLED_DrawPoint(x - a, y + b);
        OLED_DrawPoint(x + a, y + b);
 
        OLED_DrawPoint(x + b, y + a);
        OLED_DrawPoint(x + b, y - a);
        OLED_DrawPoint(x - b, y - a);
        OLED_DrawPoint(x - b, y + a);
        
        a++;
        num = (a * a + b * b) - r*r;//���㻭�ĵ���Բ�ĵľ���
        if(num > 0)
        {
            b--;
            a--;
        }
    }
}


//��ָ��λ����ʾһ���ַ�,���������ַ�
//x:0~127
//y:0~63
//size:ѡ������ 12/16/24
//ȡģ��ʽ ����ʽ
void OLED_ShowChar(uint8_t x,uint8_t y,uint8_t chr,uint8_t size1)
{
    uint8_t i,m,temp,size2,chr1;
    uint8_t y0=y;
    size2=(size1/8+((size1%8)?1:0))*(size1/2);  //�õ�����һ���ַ���Ӧ������ռ���ֽ���
    chr1=chr-' ';  //����ƫ�ƺ��ֵ
    for(i=0;i<size2;i++)
    {
        if(size1==12)
        {temp=asc2_1206[chr1][i];} //����1206����
        else if(size1==16)
        {temp=asc2_1608[chr1][i];} //����1608����
        else if(size1==24)
        {temp=asc2_2412[chr1][i];} //����2412����
        else return;
                for(m=0;m<8;m++)           //д������
                {
                    if(temp&0x80)OLED_DrawPoint(x,y);
                    else OLED_ClearPoint(x,y);
                    temp<<=1;
                    y++;
                    if((y-y0)==size1)
                    {
                        y=y0;
                        x++;
                        break;
                    }
                }
    }
}


//��ʾ�ַ���
//x,y:�������  
//size1:�����С 
//*chr:�ַ�����ʼ��ַ 
void OLED_ShowString(uint8_t x,uint8_t y,char *chr,uint8_t size1)
{
    while((*chr>=' ')&&(*chr<='~'))//�ж��ǲ��ǷǷ��ַ�!
    {
        OLED_ShowChar(x,y,*chr,size1);
        x+=size1/2;
        if(x>128-size1)  //����
        {
            x=0;
            y+=2;
         }
        chr++;
    }
}

//m^n
uint32_t OLED_Pow(uint8_t m,uint8_t n)
{
    uint32_t result=1;
    while(n--)
    {
      result*=m;
    }
    return result;
}

////��ʾ2������
////x,y :�������	 
////len :���ֵ�λ��
////size:�����С
void OLED_ShowNum(uint8_t x,uint8_t y,uint32_t num,uint8_t len,uint8_t size1)
{
    uint8_t t,temp;
    for(t=0;t<len;t++)
    {
        temp=(num/OLED_Pow(10,len-t-1))%10;
            if(temp==0)
            {
                OLED_ShowChar(x+(size1/2)*t,y,'0',size1);
      }
            else 
            {
              OLED_ShowChar(x+(size1/2)*t,y,temp+'0',size1);
            }
    }
}

//��ʾ����
//x,y:�������
//num:���ֶ�Ӧ�����
//ȡģ��ʽ ����ʽ
void OLED_ShowChinese(uint8_t x,uint8_t y,uint8_t num,uint8_t size1)
{
    uint8_t i,m,n=0,temp,chr1;
    uint8_t x0=x,y0=y;
    uint8_t size3=size1/8;
    while(size3--)
    {
        chr1=num*size1/8+n;
        n++;
            for(i=0;i<size1;i++)
            {
                if(size1==16)
                        {temp=Hzk1[chr1][i];}//����16*16����
                else if(size1==24)
                        {temp=Hzk2[chr1][i];}//����24*24����
                else if(size1==32)       
                        {temp=Hzk3[chr1][i];}//����32*32����
                else if(size1==64)
                        {temp=Hzk4[chr1][i];}//����64*64����
                else return;
                            
                        for(m=0;m<8;m++)
                            {
                                if(temp&0x01)OLED_DrawPoint(x,y);
                                else OLED_ClearPoint(x,y);
                                temp>>=1;
                                y++;
                            }
                            x++;
                            if((x-x0)==size1)
                            {x=x0;y0=y0+8;}
                            y=y0;
             }
    }
}

//num ��ʾ���ֵĸ���
//space ÿһ����ʾ�ļ��
void OLED_ScrollDisplay(uint8_t num,uint8_t space)
{
    uint8_t i,n,t=0,m=0,r;
    while(1)
    {
        if(m==0)
        {
        OLED_ShowChinese(128,24,t,16); //д��һ�����ֱ�����OLED_GRAM[][]������
            t++;
        }
        if(t==num)
            {
                for(r=0;r<16*space;r++)      //��ʾ���
                 {
                    for(i=0;i<144;i++)
                        {
                            for(n=0;n<8;n++)
                            {
                                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
                            }
                        }
           OLED_Refresh();
                 }
        t=0;
      }
        m++;
        if(m==16){m=0;}
        for(i=0;i<144;i++)   //ʵ������
        {
            for(n=0;n<8;n++)
            {
                OLED_GRAM[i-1][n]=OLED_GRAM[i][n];
            }
        }
        OLED_Refresh();
    }
}

//����д�����ݵ���ʼλ��
void OLED_WR_BP(uint8_t x,uint8_t y)
{
    OLED_WR_Byte(0xb0+y,OLED_CMD);//��������ʼ��ַ
    OLED_WR_Byte(((x&0xf0)>>4)|0x10,OLED_CMD);
    OLED_WR_Byte((x&0x0f),OLED_CMD);
}

//x0,y0���������
//x1,y1���յ�����
//BMP[]��Ҫд���ͼƬ����
void OLED_ShowPicture(uint8_t x0,uint8_t y0,uint8_t x1,uint8_t y1,uint8_t BMP[])
{
    uint32_t j=0;
    uint8_t x=0,y=0;
    if(y%8==0)y=0;
    else y+=1;
    for(y=y0;y<y1;y++)
     {
         OLED_WR_BP(x0,y);
         for(x=x0;x<x1;x++)
         {
             OLED_WR_Byte(BMP[j],OLED_DATA);
             j++;
          }
     }
}


/**
 * @brief       ��ʼ��OLED(SSD1306)
 * @param       ��
 * @retval      ��
 */
void OLED_Init(void)
{

    OLED_SDIN(1);
    OLED_SCLK(1);


    OLED_CS(1);
    OLED_RS(1);

    OLED_RST(0);
    HAL_Delay(100);
    OLED_RST(1);

    OLED_WR_Byte(0xAE, OLED_CMD);   /* �ر���ʾ */
    OLED_WR_Byte(0xD5, OLED_CMD);   /* ����ʱ�ӷ�Ƶ����,��Ƶ�� */
    OLED_WR_Byte(80, OLED_CMD);     /* [3:0],��Ƶ����;[7:4],��Ƶ�� */
    OLED_WR_Byte(0xA8, OLED_CMD);   /* ��������·�� */
    OLED_WR_Byte(0X3F, OLED_CMD);   /* Ĭ��0X3F(1/64) */
    OLED_WR_Byte(0xD3, OLED_CMD);   /* ������ʾƫ�� */
    OLED_WR_Byte(0X00, OLED_CMD);   /* Ĭ��Ϊ0 */

    OLED_WR_Byte(0x40, OLED_CMD);   /* ������ʾ��ʼ�� [5:0],����. */

    OLED_WR_Byte(0x8D, OLED_CMD);   /* ��ɱ����� */
    OLED_WR_Byte(0x14, OLED_CMD);   /* bit2������/�ر� */
    OLED_WR_Byte(0x20, OLED_CMD);   /* �����ڴ��ַģʽ */
    OLED_WR_Byte(0x02, OLED_CMD);   /* [1:0],00���е�ַģʽ;01���е�ַģʽ;10,ҳ��ַģʽ;Ĭ��10; */
    OLED_WR_Byte(0xA1, OLED_CMD);   /* ���ض�������,bit0:0,0->0;1,0->127; */
    OLED_WR_Byte(0xC8, OLED_CMD);   /* ����COMɨ�跽��;bit3:0,��ͨģʽ;1,�ض���ģʽ COM[N-1]->COM0;N:����·�� */
    OLED_WR_Byte(0xDA, OLED_CMD);   /* ����COMӲ���������� */
    OLED_WR_Byte(0x12, OLED_CMD);   /* [5:4]���� */

    OLED_WR_Byte(0x81, OLED_CMD);   /* �Աȶ����� */
    OLED_WR_Byte(0xEF, OLED_CMD);   /* 1~255;Ĭ��0X7F (��������,Խ��Խ��) */
    OLED_WR_Byte(0xD9, OLED_CMD);   /* ����Ԥ������� */
    OLED_WR_Byte(0xf1, OLED_CMD);   /* [3:0],PHASE 1;[7:4],PHASE 2; */
    OLED_WR_Byte(0xDB, OLED_CMD);   /* ����VCOMH ��ѹ���� */
    OLED_WR_Byte(0x30, OLED_CMD);   /* [6:4] 000,0.65*vcc;001,0.77*vcc;011,0.83*vcc; */

    OLED_WR_Byte(0xA4, OLED_CMD);   /* ȫ����ʾ����;bit0:1,����;0,�ر�;(����/����) */
    OLED_WR_Byte(0xA6, OLED_CMD);   /* ������ʾ��ʽ;bit0:1,������ʾ;0,������ʾ */
    OLED_WR_Byte(0xAF, OLED_CMD);   /* ������ʾ */
    OLED_Clear();
}

