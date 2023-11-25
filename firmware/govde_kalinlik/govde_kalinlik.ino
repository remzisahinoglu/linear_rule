/*
yazan: remzi şahinoğlu
tarih: 08.08.2016
board: arduino uno

bu program analog pine bağlı linear cetvelin değeri okuuyp serialdan yazar

kalibrasyon için ilk önce boşda iken 2.pine bağlı olan  butona(yukardaki) 
bir sn boyunca basılı tut ekranda 0 yazar, sonra linear cetveli 10mm ye 
mastar ile ayarlayıp 3.pine bağlı olan buyona(aşağıdaki) bir sn süre basılı
tutup ekranda 10000 değerini gördüğünde kalibre edilmiş demektir.

linear cetvel kablo bağlantısı

1       : kahve (GND)
2       : beyaz (A0)
3       : sarı (+5V)
toprak  : yeşil (GND)

*/
#include <EEPROM.h>
#include "LedControl.h"
LedControl lc = LedControl(10,12,11,1);                                   // Arduino Pins: (DIN:10, CLK:12, LOAD/CS:11, no.of devices is 1)

long deger = 0;
float deger_float = 0;

long max_coz = 541;                                                       // 2 byte olduğu için farklı bir komut kullanımlalı
long min_coz = 148;

long max_mm = 10000;
long min_mm = 0;

char ekran[8] = {};                                                       // max7219 modulunde 8 adet dipslay olduğu için 
unsigned int index = 0;                                                   // max7219u ayarlamak için kullanıldı

//--------------------------- EEPROM WRİTE -------------------------------//
void ee_write_long(unsigned int addr , signed long *longptr)
{
unsigned char edata;
unsigned char I;
    for(I=0;I<4;I++){
      edata = *((unsigned char *)longptr+I);
      EEPROM.write(addr+I,edata);
   }
}

//---------------------------- EEPROM READ -------------------------------//
void ee_read_long(unsigned int addr , signed long *longptr)
{
unsigned char edata;
unsigned char I;
   for(I=0;I<4;I++){
      edata=EEPROM.read(I+addr);   
        *((unsigned char *)longptr+I) = edata;
   }
}

//--------------------------- EKRANDA GÖSTER -----------------------------//
void display_set()
{ 
  index = 0;
  for (int i = 0; i < String(deger).length(); i++)
  {
    char inChar = (char)String(deger)[i];                                 // tek bir byte oku
    switch(index)
    {
      case 0:                 // 1.karakter okunduğunda
        ekran[7] = ' ';       // değer yoksa display boş gözüksün
        ekran[6] = ' ';       // değer yoksa display boş gözüksün
        ekran[5] = ' ';       // değer yoksa display boş gözüksün
        ekran[4] = ' ';       // değer yoksa display boş gözüksün
        ekran[3] = ' ';       // değer yoksa display boş gözüksün
        ekran[2] = ' ';       // değer yoksa display boş gözüksün
        ekran[1] = ' ';       // değer yoksa display boş gözüksün
        ekran[0] = inChar;
        break;
        
      case 1:                 // 2.karakter okunduğunda
        ekran[7] = ' ';       // değer yoksa display boş gözüksün
        ekran[6] = ' ';       // değer yoksa display boş gözüksün
        ekran[5] = ' ';       // değer yoksa display boş gözüksün
        ekran[4] = ' ';       // değer yoksa display boş gözüksün
        ekran[3] = ' ';       // değer yoksa display boş gözüksün
        ekran[2] = ' ';       // değer yoksa display boş gözüksün
        ekran[1] = ekran[0];  // 1.okunan katakteri 2.displaye kaydır
        ekran[0] = inChar;
        break;
        
      case 2:                 // 3.karakter okunduğunda
        ekran[7] = ' ';       // değer yoksa display boş gözüksün
        ekran[6] = ' ';       // değer yoksa display boş gözüksün
        ekran[5] = ' ';       // değer yoksa display boş gözüksün
        ekran[4] = ' ';       // değer yoksa display boş gözüksün
        ekran[3] = ' ';       // değer yoksa display boş gözüksün
        ekran[2] = ekran[1];  // 1.okunan katakteri 3.displaye kaydır
        ekran[1] = ekran[0];  // 2.okunan katakteri 2.displaye kaydır
        ekran[0] = inChar;
        break;
        
      case 3:                 // 4.karakter okunduğunda
        ekran[7] = ' ';       // değer yoksa display boş gözüksün
        ekran[6] = ' ';       // değer yoksa display boş gözüksün
        ekran[5] = ' ';       // değer yoksa display boş gözüksün
        ekran[4] = ' ';       // değer yoksa display boş gözüksün
        ekran[3] = ekran[2];  // 1.okunan katakteri 4.displaye kaydır
        ekran[2] = ekran[1];  // 2.okunan katakteri 3.displaye kaydır
        ekran[1] = ekran[0];  // 3.okunan katakteri 2.displaye kaydır
        ekran[0] = inChar;
        break;
        
      case 4:                 // 5.karakter okunduğunda
        ekran[7] = ' ';       // değer yoksa display boş gözüksün
        ekran[6] = ' ';       // değer yoksa display boş gözüksün
        ekran[5] = ' ';       // değer yoksa display boş gözüksün
        ekran[4] = ekran[3];  // 1.okunan katakteri 4.displaye kaydır
        ekran[3] = ekran[2];  // 2.okunan katakteri 3.displaye kaydır
        ekran[2] = ekran[1];  // 3.okunan katakteri 2.displaye kaydır
        ekran[1] = ekran[0];  // 4.okunan katakteri 1.displaye kaydır
        ekran[0] = inChar;
        break;
        
      case 5:                 // 6.karakter okunduğunda
        ekran[7] = ' ';       // değer yoksa display boş gözüksün
        ekran[6] = ' ';       // değer yoksa display boş gözüksün
        ekran[5] = ekran[4];  // 1.okunan katakteri 5.displaye kaydır
        ekran[4] = ekran[3];  // 2.okunan katakteri 4.displaye kaydır
        ekran[3] = ekran[2];  // 3.okunan katakteri 3.displaye kaydır
        ekran[2] = ekran[1];  // 4.okunan katakteri 2.displaye kaydır
        ekran[1] = ekran[0];  // 5.okunan katakteri 1.displaye kaydır
        ekran[0] = inChar;
        break;
        
      case 6:                 // 7.karakter okunduğunda
        ekran[7] = ' ';       // değer yoksa display boş gözüksün
        ekran[6] = ekran[5];  // 1.okunan katakteri 6.displaye kaydır
        ekran[5] = ekran[4];  // 2.okunan katakteri 5.displaye kaydır
        ekran[4] = ekran[3];  // 3.okunan katakteri 4.displaye kaydır
        ekran[3] = ekran[2];  // 4.okunan katakteri 3.displaye kaydır
        ekran[2] = ekran[1];  // 5.okunan katakteri 2.displaye kaydır
        ekran[1] = ekran[0];  // 6.okunan katakteri 1.displaye kaydır
        ekran[0] = inChar;
        break;
        
      case 7:                 // 8.karakter okunduğunda
        ekran[7] = ekran[6];  // 1.okunan katakteri 7.displaye kaydır
        ekran[6] = ekran[5];  // 2.okunan katakteri 6.displaye kaydır
        ekran[5] = ekran[4];  // 3.okunan katakteri 5.displaye kaydır
        ekran[4] = ekran[3];  // 4.okunan katakteri 4.displaye kaydır
        ekran[3] = ekran[2];  // 5.okunan katakteri 3.displaye kaydır
        ekran[2] = ekran[1];  // 6.okunan katakteri 2.displaye kaydır
        ekran[1] = ekran[0];  // 7.okunan katakteri 1.displaye kaydır
        ekran[0] = inChar;
        break;
    }
    index++;
  }

  for (int i = 0; i < String(deger).length(); i++)                        // çevrim uzunluğu kadar döngü yap, çünkü o kadar display yansın
  {
    lc.setDigit(0,i,(String(ekran[i])).toInt(),false);                    // (grup no, digit no, data, point) eğer true ise point yanar
  }
}

//********************** BAŞLANGIÇ AYARLARI *****************************//
void setup()
{
  Serial.begin(9600);
  delay(1000);
  
  lc.shutdown(0,false);                                                  // Enable display
  lc.setIntensity(0,3);                                                  // Set brightness level (0 is min, 15 is max)
  lc.clearDisplay(0);                                                    // Clear display register
  
  pinMode(2, INPUT_PULLUP);                                              // min_coz değerini ayarla, yukardaki buton
  pinMode(3, INPUT_PULLUP);                                              // max_coz değerini ayarla, aşağıdaki buton

  ee_read_long(0x00, &min_coz);                                          // epromdaki cycle değerini oku
  ee_read_long(0x04, &max_coz);                                          // epromdaki cycle değerini oku
}

//************************** ANA PROGRAM ********************************//
void loop()
{

  delay(1000);
  lc.clearDisplay(0);
  deger = analogRead(2);        // analog 2.pin
  deger_float = deger;

  deger = max_mm - (((max_coz - deger_float) * (max_mm - min_mm))/(max_coz - min_coz));

  display_set();

  if(digitalRead(2) == 0)                                                // min değer kalibrasyon butonu
  {
    delay(500);
    min_coz = analogRead(2);
    ee_write_long(0x00, &min_coz);                                       // değeri eproma yaz
  }

  if(digitalRead(3) == 0)                                                // max değer kalibrasyon butonu
  {
    delay(500);
    max_coz = analogRead(2);
    ee_write_long(0x04, &max_coz);                                       // değeri eproma yaz
  }

}









