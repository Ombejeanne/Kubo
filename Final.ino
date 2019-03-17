//module blutooth
#include<SoftwareSerial.h>
#define RX 10
#define TX 11
SoftwareSerial BlueT(RX,TX);
char Data;

//Leds
#include <Adafruit_NeoPixel.h>
#define intensite 150
#define PIN 6
#define NUMPIXELS 64
Adafruit_NeoPixel pixels0 = Adafruit_NeoPixel(NUMPIXELS, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels4 = Adafruit_NeoPixel(NUMPIXELS, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels[6]={pixels0, pixels1 ,pixels2 ,pixels3 ,pixels4};

int cardef[10][6][4]
{
  { // 0
    {0,1,1,1},
    {0,1,0,1},
    {0,1,0,1},
    {0,1,0,1},
    {0,1,1,1}
  },
  { // 1
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
  },
  { // 2
    {0,1,1,1},
    {0,0,0,1},
    {0,1,1,1},
    {0,1,0,0},
    {0,1,1,1}
  },
  { // 3
    {0,1,1,1},
    {0,0,0,1},
    {0,0,1,1},
    {0,0,0,1},
    {0,1,1,1}
  },
  { // 4
    {0,1,0,1},
    {0,1,0,1},
    {0,1,1,1},
    {0,0,0,1},
    {0,0,0,1}
  },
  { // 5
    {0,1,1,1},
    {0,1,0,0},
    {0,1,1,1},
    {0,0,0,1},
    {0,1,1,1}
  },
  { // 6
    {0,1,1,1},
    {0,1,0,0},
    {0,1,1,1},
    {0,1,0,1},
    {0,1,1,1}
  },
  { // 7
    {0,1,1,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1},
    {0,0,0,1}
  },
  { // 8
    {0,1,1,1},
    {0,1,0,1},
    {0,1,1,1},
    {0,1,0,1},
    {0,1,1,1}
  },
  { // 9
    {0,1,1,1},
    {0,1,0,1},
    {0,1,1,1},
    {0,0,0,1},
    {0,0,0,1}
  }
};

bool a = true;

void setup() {
//module blutooth
  Serial.begin(9600);
  delay(500);
  Serial.println("Bonjour -Pret pour les commandes AT");
  BlueT.begin(9600);
  delay(500);
  for(int i=0;i<8;i++){
    pixels[i].begin();
    pixels[i].show();
  }
}

void loop() {
//module blutooth
  while (BlueT.available()) {
  Serial.print(char(BlueT.read())); }
  while (Serial.available()) {
  BlueT.write(char(Serial.read())); }

//animation allumage
  if (a){
  colorWipe(20,0,0,intensite);}
  a = false;
  
  if (BlueT.available()){ 
    Serial.print("ok");
  Data=BlueT.read();      
    Serial.print(Data);
//mode veille    
    if (Data=='Y') {
      rainbow(20);
      rainbowCycle(20);
    }
//mode fête
    if (Data=='B') {
      theaterChase(20,127,127,127);
      theaterChaseRainbow(20);
    }
    if (Data=='G') {
     nombre(12,intensite,0,intensite);
     nombre(54,0,intensite,intensite);
    }
  }
}

void colorWipe(int wait,int r, int g, int b) {
  for (int i=0; i<NUMPIXELS; i++) {
     for(int k=0;k<5;k++){
    pixels[k].setPixelColor(i,pixels[k].Color(r,g,b));
    pixels[k].show();
    delay(wait);
     }
  }
}

void rainbow(int wait) {
  for(int j=0; j<256; j++) {
    for(int k=0;k<5;k++){
    for(int i=0; i<NUMPIXELS; i++){
      
      pixels[k].setPixelColor(i,Wheel((i+j)&255,k));
      }
      pixels[k].show();
    }
    //delay(wait);
  }
}

void rainbowCycle(int wait) {
  for(int j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(int k=0;k<5;k++){
      for(int i=0; i<NUMPIXELS; i++) {
         pixels[k].setPixelColor(i,Wheel(((i*256/NUMPIXELS)+j)&255,k));
      }
    pixels[k].show();
    }
    //delay(wait);
  }
}

void theaterChase(int wait,int r,int g,int b) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q<3; q++) {
      for(int k=0;k<5;k++){
      for (int i=0; i<NUMPIXELS; i=i+3) {
        pixels[k].setPixelColor(i+q,pixels[k].Color(r,g,b));    //turn every third pixel on

      }
      
      pixels[k].show();
      delay(wait);
      for (int i=0; i<NUMPIXELS; i=i+3) {
        pixels[k].setPixelColor(i+q,0);  //turn every third pixel off
      }
      delay(wait);
    }
    }
  }
}

void theaterChaseRainbow(int wait){
  for(int j=0; j<256; j++){     // cycle all 256 colors in the wheel
    for(int q=0; q<3; q++){
      for(int k=0;k<5;k++){
      for(int i=0; i < pixels[k].numPixels(); i=i+3){
        pixels[k].setPixelColor(i+q, Wheel((i+j)%255,k));    //turn every third pixel on
      }
      
      pixels[k].show();
      delay(wait);

      for (int i=0; i<pixels[k].numPixels(); i=i+3){
        pixels[k].setPixelColor(i+q,0);   //turn every third pixel off
      }
    }
    }
  }
}

uint32_t Wheel(byte WheelPos, int face) {
  WheelPos=255-WheelPos;
  if(WheelPos<85) {
    return pixels[face].Color(255-WheelPos*3,0,WheelPos*3);
  }
  if(WheelPos<170) {
    WheelPos-=85;
    return pixels[face].Color(0,WheelPos*3,255-WheelPos*3);
  }
  WheelPos-=170;
  return pixels[face].Color(WheelPos*3,255-WheelPos*3,0);
}

// affiche dans l'axe de visualisation, 0 en bas à droite, x horizontal, y vertical, z profondeur
void afficheVisu(int z, int x,int y,int v, int r, int b){
  setpixel(y, x, z, v, r, b); // converti coordonnées visu en coordonnées plaque, avec y no de plaque
}

void nombre(int a, int v, int r, int b){
  int chiffre2=a%10;
  int chiffre1=a/10;
  for (int z=7; z>=0; z--) // affiche le nombre du fond vers l'avant
  {
    //clearZ(0); // efface le nombre precedent au depart. moins joli
    chiffre(chiffre1,true, z, v, r, b);
    chiffre(chiffre2,false, z, v, r, b);
    if (z < 7) clearZ(z+1); // efface plan z précédent
  }
}

// efface le plan de profondeur z
void clearZ(int z)
{
    for (int x=0; x<8; x++)
    {
      for (int y=0; y<5; y++)
      {
        afficheVisu(z, x, y, 0,0,0);
      }
    }
  }

void chiffre(int a, bool zone1, int z, int v, int r, int b){

  int incrementX;
  if (zone1) incrementX=4;
  else incrementX=0;

  for (int y=4;y>=0;y--){
    for (int x=3;x>=0;x--){
      if (cardef[a][y][x]==1){
        //setpixel(3, 3-x+incrementX, 4-y, v,r,b);
        //else setpixel(3, 3-x+incrementX, 4-y, 0,0,0);
        afficheVisu(z,3-x+incrementX,4-y,v,r,b);
        }
       else afficheVisu(z,3-x+incrementX,4-y,0,0,0);
    }
   }

}

void setpixel(int face, int x, int y, int v, int r, int b)
{
    //Adafruit_NeoPixel p = pixels[face];
    bool pair = y%2==0;
    int num;
    if (pair) num = 8*y+x; 
    else num = 8*y+7-x;
    pixels[face].setPixelColor(num,pixels[face].Color(v,r,b));
    pixels[face].show();
    return;
}
