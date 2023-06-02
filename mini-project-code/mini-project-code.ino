#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x27, 16, 2); 
#include <SoftwareSerial.h>
#include <String.h>

int led=13;

int temp=0,i=0,x=0,k=0;
char str[100],msg[32];
SoftwareSerial SIM800(2 , 3);
void setup() 
{
  lcd.begin();
  lcd.backlight();
  SIM800.begin(9600);
  pinMode(led, OUTPUT);
  digitalWrite(led, HIGH);
  lcd.print("GSM Initilizing...");
  gsm_init();
  lcd.setCursor(0,0);
  lcd.print("Wireless Notice");
  lcd.setCursor(0,1);
  lcd.print("    Board      ");
  delay(2000);
  lcd.clear();
  lcd.print("communication mini project");
  delay(1000);
  lcd.setCursor(0,1);
  lcd.print("System Ready");
  SIM800.println("AT+CNMI=2,2,0,0,0");
  delay(500);
  SIM800.println("AT+CMGF=1");
  delay(500);
  digitalWrite(led, LOW);
}

void loop()    
{
  for(unsigned int t=0;t<30000;t++)
  {
    SIM800Event();
  if(temp==1)
  {
    x=0,k=0,temp=0;
    while(x<i)
    {
      while(str[x]=='#')
      {
        x++;
        while(str[x]!='*')
        {
          msg[k++]=str[x++];
        }
      }
      x++;
    }
    msg[k]='\0';
    lcd.clear();
    lcd.print(msg);
    delay(300);
    temp=0;
    i=0;
    x=0;
    k=0;
  }
  }
  lcd.scrollDisplayLeft();
 
}
void SIM800Event()
{
  while(SIM800.available())
  {
    char ch=(char)SIM800.read();
    str[i++]=ch;
    if(ch == '*')
    {
      temp=1;
      lcd.clear();
      lcd.print("Message Received");
      delay(1000);
    }
  }
}

void gsm_init()
{
  lcd.clear();
  lcd.print("Finding Module..");
  boolean at_flag=1;
  while(at_flag)
  {
    SIM800.println("AT");
    while(SIM800.available()>0)
    {
      if(SIM800.find("OK"))
      at_flag=0;
    }
    delay(1000);
  }

  lcd.clear();
  lcd.print("Module Connected..");
  delay(1000);
  lcd.clear();
  lcd.print("Disabling ECHO");
  boolean echo_flag=1;
  while(echo_flag)
  {
    SIM800.println("ATE0");
    while(SIM800.available()>0)
    {
      if(SIM800.find("OK"))
      echo_flag=0;
    }
    delay(1000);
  }

  lcd.clear();
  lcd.print("Echo OFF");
  delay(1000);
  lcd.clear();
  lcd.print("Finding Network..");
  boolean net_flag=1;
  while(net_flag)
  {
    SIM800.println("AT+CPIN?");
    while(SIM800.available()>0)
    {
      if(SIM800.find("+CPIN: READY"))
      net_flag=0;
    }
    delay(1000);
  }
  lcd.clear();
  lcd.print("Network Found..");
  delay(1000);
  lcd.clear();
}