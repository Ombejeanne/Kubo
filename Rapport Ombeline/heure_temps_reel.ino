#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "RTClib.h"
RTC_DS1307 RTC;

#define intensite 127
#define PIN 6
#define NUMPIXELS 64
Adafruit_NeoPixel pixels0 = Adafruit_NeoPixel(NUMPIXELS, 3, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels1 = Adafruit_NeoPixel(NUMPIXELS, 4, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels2 = Adafruit_NeoPixel(NUMPIXELS, 5, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels3 = Adafruit_NeoPixel(NUMPIXELS, 6, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels4 = Adafruit_NeoPixel(NUMPIXELS, 7, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels5 = Adafruit_NeoPixel(NUMPIXELS, 8, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels6 = Adafruit_NeoPixel(NUMPIXELS, 9, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels7 = Adafruit_NeoPixel(NUMPIXELS, 10, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels[8]={pixels0, pixels1 ,pixels2 ,pixels3 ,pixels4 ,pixels5 ,pixels6 ,pixels7}; //tableau des 8 faces

int cardef[10][5][4] // tableau à trois dimentions
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

void setup() {
  for(int i=0;i<8;i++){
  pixels[i].begin();
  pixels[i].show();
  Serial.begin(9600);
  }
  
   Wire.begin();
   RTC.begin();

  if (! RTC.isrunning()) { //Si on enlève tout ce qui est en rapport avec le module, la fonction nombre fonctionne
    Serial.println("RTC is NOT running!");
    RTC.adjust(DateTime(__DATE__, __TIME__));   // Mets le RTC à la date et à l'heure de l'ordinateur
  } 
 
}

void loop() {
  DateTime now = RTC.now(); //probleme
  delay(100);
  int h = now.hour();
  int m= now.minute();
  Serial.print(h,DEC);
  Serial.print(":");
  Serial.println(m,DEC);
  delay(500);
  nombre(12,intensite,0,0); //remplacer 12 par h, ici le nombre 12 s'affiche en vert
  delay(500);
  nombre(52,0,intensite,0); //remplacer 52 par m, ici le chiffre 52 s'affiche en rouge
  delay(500);
  chiffre(7,true,3,0,0,intensite); //le chiffre 7 sur la zone 1 (gauche) de la face liée à la sortie 3
  for (int i=0; i<100; i++) {  //tous les chiffres défilent (voir ce qu'il faut changer dans la fonction dans ses commentaires pour changer l'affichage)
    nombre(i,intensite,intensite,0); //les nombres deffilent en jaune
    delay(100);
  }
}

// affiche dans l'axe de visualisation, 0 en bas à droite (première led de la série), x horizontal, y vertical, z profondeur
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
    //if (z < 7) clearZ(z+1); // efface plan z précédent, enlever le commentaire pour faire défiler en profondeur sur le cube entier (1/5)
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

  int incrementX; //variable pour placer le chiffre à gauche ou à droite de la face
  if (zone1) incrementX=4;
  else incrementX=0;

  for (int y=4;y>=0;y--){
    for (int x=3;x>=0;x--){
      if (cardef[a][y][x]==1)
        setpixel(3, 3-x+incrementX, 4-y, v,r,b); //mettre en commentaire pour faire défiler en profondeur sur le cube entier (2/5)
        else setpixel(3, 3-x+incrementX, 4-y, 0,0,0); //mettre en commentaire pour faire défiler en profondeur sur le cube entier (3/5)
        //afficheVisu(z,3-x+incrementX,4-y,v,r,b); //enlever le commentaire pour faire défiler en profondeur sur le cube entier (4/5)
        //else afficheVisu(z,3-x+incrementX,4-y,0,0,0); //enlever le commentaire pour faire défiler en profondeur sur le cube entier (5/5)
    }
   }

}

void setpixel(int face, int x, int y, int v, int r, int b){ //fonction qui permet d'acceder à n'importe quelle led d'une face, x horizontale, y profondeur, première led devant à droite
    bool pair = y%2==0;
    int num;
    if (pair) num = 8*y+x; 
    else num = 8*y+7-x;
    pixels[face].setPixelColor(num,pixels[face].Color(v,r,b));
    pixels[face].show();
    return;
}
