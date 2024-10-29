/******** Tr??ng Dai Hoc Van Hien ********/
/****** H? và tên: Trieu Quang Ninh ******/
/*********** MSSV: 221A020065 ***********/
#pragma config FOSC = HS      
#pragma config WDTE = OFF      
#pragma config PWRTE = ON       
#pragma config BOREN = ON      
#pragma config LVP = OFF        
#pragma config CPD = OFF        
#pragma config WRT = OFF       
#pragma config CP = OFF         

#include <xc.h>
#define _XTAL_FREQ 20000000

#define RS PORTCbits.RC5
#define EN PORTCbits.RC7
#define D4 PORTDbits.RD4
#define D5 PORTDbits.RD5
#define D6 PORTDbits.RD6
#define D7 PORTDbits.RD7

#define Weather  PORTCbits.RC1
#define DHT11_Data_Pin   PORTAbits.RA5
#define DHT11_Data_Pin_Direction  TRISAbits.TRISA5
unsigned char Check_bit,Temp_byte_1,Temp_byte_2,RH_byte_1,RH_byte_2;
unsigned char temper,RH,Sumation;

void rotate0(void){
    RC2 =1;
    RC3 =1;
    __delay_us(1500);
    RC2 =0;
    RC3 =0;
    __delay_us(18500);
}
void rotate90(void){
    RC2 =1;
    RC3 =1;
    __delay_us(2000);
    RC2 =0;
    RC3 =0;
    __delay_us(18000);
}

//LCD Functions Developed by Circuit Digest.
void Lcd_SetBit(char A) //Based on the Hex value Set the Bits of the Data Lines
{
    if(A& 1) 
        D4 = 1;
    else
        D4 = 0;
    if(A& 2)
        D5 = 1;
    else
        D5 = 0;
    if(A& 4)
        D6 = 1;
    else
        D6 = 0;
    if(A& 8) 
        D7 = 1;
    else
        D7 = 0;
}

void Lcd_Cmd(char a)
{
    RS = 0;           
    Lcd_SetBit(a); //Incoming Hex value
    EN  = 1;         
    __delay_ms(4);
    EN  = 0;         
}

Lcd_Clear()
{
    Lcd_Cmd(0); //Clear the LCD
    Lcd_Cmd(1); //Move the curser to first position
}

void Lcd_Set_Cursor(char row, char column) {
    const char rowOffsets[] = {0x80, 0xC0, 0x94, 0xD4}; // Base addresses for rows

    if (row >= 1 && row <= 4 && column >= 1) {
        char address = rowOffsets[row - 1] + column - 1;
        Lcd_Cmd(address >> 4);  // Send high nibble
        Lcd_Cmd(address & 0x0F); // Send low nibble
    } 
}

void Lcd_Start()
{
  Lcd_SetBit(0x00);
  for(int i=1065244; i<=0; i--)  NOP();  
  Lcd_Cmd(0x03);
  __delay_ms(5);
  Lcd_Cmd(0x03);
  __delay_ms(11);
  Lcd_Cmd(0x03); 
  Lcd_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
  Lcd_Cmd(0x02); //02H is used for Return home -> Clears the RAM and initializes the LCD
  Lcd_Cmd(0x08); //Select Row 1
  Lcd_Cmd(0x00); //Clear Row 1 Display
  Lcd_Cmd(0x0C); //Select Row 2
  Lcd_Cmd(0x00); //Clear Row 2 Display
  Lcd_Cmd(0x06);
}

void Lcd_Print_Char(char data)  //Send 8-bits through 4-bit mode
{
   char Lower_Nibble,Upper_Nibble;
   Lower_Nibble = data&0x0F;
   Upper_Nibble = data&0xF0;
   RS = 1;             // => RS = 1
   Lcd_SetBit(Upper_Nibble>>4);             //Send upper half by shifting by 4
   EN = 1;
   for(int i=2130483; i<=0; i--)  NOP(); 
   EN = 0;
   Lcd_SetBit(Lower_Nibble); //Send Lower half
   EN = 1;
   for(int i=2130483; i<=0; i--)  NOP();
   EN = 0;
}

void Lcd_Print_String(char *a)
{
    int i;
    for(i=0;a[i]!='\0';i++)
    Lcd_Print_Char(a[i]);  //Split the string using pointers and call the Char function 
}

void dht11_init(){
 DHT11_Data_Pin_Direction= 0; 
 DHT11_Data_Pin = 0; 
 __delay_ms(18);
 DHT11_Data_Pin = 1; 
 __delay_us(30);
 DHT11_Data_Pin_Direction = 1;
 }

void find_response(){
 Check_bit = 0;
 __delay_us(40);
 if (DHT11_Data_Pin == 0){
 __delay_us(80);
 if (DHT11_Data_Pin == 1){
    Check_bit = 1;
 }     
 __delay_us(50);}
 }

 char read_dht11(){
 char data, for_count;
 for(for_count = 0; for_count < 8; for_count++){
     while(!DHT11_Data_Pin); 
    __delay_us(30);
    if(DHT11_Data_Pin == 0){
        data&= ~(1<<(7 - for_count)); 
    }
    else{
        data|= (1 << (7 - for_count)); 
        while(DHT11_Data_Pin);
    } 
    }
 return data;
 }
 
char Temp[] = "00.0  C";
char Hum[] = "00.0  %";
int a, b,c, d, e;

int main(){
    ANSEL = ANSELH = 0x00;
    PORTCbits.RC2 = 0;
    PORTCbits.RC3 = 0;
    TRISC = 0x01;
    TRISD = 0x00;
    unsigned int a;
    Lcd_Start();
    Lcd_Clear();
    while(1)
    { 
        dht11_init();
        find_response();
        if(Check_bit == 1){
        RH_byte_1 = read_dht11();
        RH_byte_2 = read_dht11();
        Temp_byte_1 = read_dht11();
        Temp_byte_2 = read_dht11();
        Sumation = read_dht11();
        if(Sumation == ((RH_byte_1+RH_byte_2+Temp_byte_1+Temp_byte_2) & 0XFF)){
            temper = Temp_byte_1;
            RH = RH_byte_1;                        
            Lcd_Set_Cursor(3,1);                  
            Lcd_Print_String("NHIET DO : ");
            Lcd_Set_Cursor(4,1);            
            Lcd_Print_String("DO AM    : ");
            
            e= temper * 100; //2523
            a = (e/1000) % 10; //2
            b = (e/100) %10;  //5
            c = (e/10) %10;  //2
            d = (e/1) %10; //3
        
            Temp[0] = a + '0';
            Temp[1] = b + '0';
            Temp[3] = c + '0';
            Temp[4] = d + '0';
            Lcd_Set_Cursor(3,11);
            Lcd_Print_String(Temp);
            
            e= RH * 100; //2523
            a = (e/1000) % 10; //2
            b = (e/100) %10;  //5
            c = (e/10) %10;  //2
            d = (e/1) %10; //3
        
            Hum[0] = a + '0';
            Hum[1] = b + '0';
            Hum[3] = c + '0';
            Hum[4] = d + '0';
            Lcd_Set_Cursor(4,11);
            Lcd_Print_String(Hum);
            
            Lcd_Set_Cursor(1, 3);
            Lcd_Print_String("AUTOMATIC GARDEN");

            Lcd_Set_Cursor(2, 1);
            Lcd_Print_String("THOI TIET:");
            
            Lcd_Set_Cursor(2, 11);
            Lcd_Print_String("    "); 
            Lcd_Set_Cursor(2, 11);
           
            if(Weather==1){
                __delay_ms(100);
                Lcd_Print_String("NANG");
                rotate0();
            }
            else{
                __delay_ms(100);
                Lcd_Print_String("MUA");
                rotate90();
        }
       } 
        else {
            Lcd_Set_Cursor(1,1);
            Lcd_Print_String("Check sum error");
      }
    }
    }
    return 0;
}