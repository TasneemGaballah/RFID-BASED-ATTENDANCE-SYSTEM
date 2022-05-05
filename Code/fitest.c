#include<reg51.h>
#include<string.h>
#include<stdio.h>
#define lcdport P2

sbit en=P1^0;
sbit rs=P1^1;
sbit led=P1^2;
sbit m1=P3^2;
sbit m2=P3^3;

char i;
char rfid[8],ch=0;
int count1, count2, count3;
unsigned char result[1];

 void delay(int itime)
{
    int i,j;
    for(i=0;i<itime;i++)
    for(j=0;j<1275;j++);
}

void daten()
{
    rs=1;
    
    en=1;
    delay(5);
    en=0;
}

void lcddata(unsigned char ch)
{
    lcdport=ch & 0xf0;
    daten();
    lcdport=(ch<<4) & 0xf0;
    daten();
}

void cmden(void)
{
    rs=0;
    en=1;
    delay(5);
    en=0;
}

void lcdcmd(unsigned char ch)
{
    lcdport=ch & 0xf0;
    cmden();
    lcdport=(ch<<4) & 0xf0;
    cmden();
}

void lcdstring(char *str)
{
    while(*str)
    {
        lcddata(*str);
        str++;
    }
}

void lcd_init(void)
{
    lcdcmd(0x02);
    lcdcmd(0x28);
    lcdcmd(0x0e);
    lcdcmd(0x01);
}

void uart_init()
{
 TMOD=0x20;
 SCON=0x50;
 TH1=0xfd;
 TR1=1;
}
char rxdata()
{
  while(!RI);
    ch=SBUF;    
    RI=0;
    return ch;
}
void int_valied (){
            m1=1;
            m2=0;
            delay(300);
            m1=0;
            m2=1;
            delay(300);
            m1=0;
            m2=0;
	          led=0;
}
void int_invalied (){
            m1=0;
            m2=0;
	          led=1;
	          delay(300);
	          led=0;
	           
	        
}
void main()
{
	char c;
    led=0;
    m1=0;
    m2=0;
    uart_init();
    lcd_init();
    lcdstring("  RFID Based    ");
    lcdcmd(0xc0);
    lcdstring("Attendance Systm"); 
    delay(400);
    while(1)
    {
        lcdcmd(1);
        lcdstring("Place Your Card:");
        lcdcmd(0xc0);
        i=0;
			while( c=rxdata()){
        rfid[i++]=c;
			}
        lcdcmd(1);
        lcdstring("Your ID No. is:");
        lcdcmd(0xc0);
			  i=0;
				while( rfid[i])
					lcddata(rfid[i++]);
        delay(100);
        if(strcmp(rfid,"69A5F5A2")==0)
        {
            count1++;
            lcdcmd(1);
            lcdstring(" Attendance ");
            lcdcmd(0xc0);
            lcdstring(" Registered");
            delay(200);
            lcdcmd(1);
            lcdstring(" Student1 ");
            lcdcmd(0xc0);
            lcdstring("Attnd. No.: ");
            sprintf(result, "%d", count1);
            lcdstring(result);
            
            int_valied ();
        }
        
        else if(strcmp(rfid,"ADDC5732")==0)
            {
            count2++;
            lcdcmd(1);
            lcdstring(" Attendance ");
            lcdcmd(0xc0);
            lcdstring(" Registered");
            delay(200);
            lcdcmd(1);
            lcdstring(" Student2 ");
            lcdcmd(0xc0);
            lcdstring("Attnd. No.: ");
            sprintf(result, "%d", count2);
            lcdstring(result);
            int_valied ();
      }
            

        else 
        {
           lcdcmd(1);
           lcdstring("Invalid Card");
           int_invalied ();
           delay(300);
          
        }
  }
}