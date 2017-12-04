#include <LiquidCrystal.h>
#include <Adafruit_BMP085.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_HMC5883_U.h>
#include <Wire.h>

//Initialisation du lcd
LiquidCrystal lcd(7,6,5,4,3,2);

//Initialisation du capteur de pression, temperature et altitude.
Adafruit_BMP085 bmp;

//Initialisation du magnetometre.
Adafruit_HMC5883_Unified mag = Adafruit_HMC5883_Unified(12345);

//Variables d'affichages
String afftemp;   //esthetique température
String affpress;  //esthetique préssion
String affvent;   //esthetique vent

String DataLine1; //Ligne à afficher
String DataLine2; //Seconde ligne à afficher
String Message_previsionel;

//Variables capteurs
int pressure; //reçois la valeur du capteur
int temp;     //reçois la valeur du capteur

void setup() 
{
  Serial.begin(9600); //Debuggage
  
  //Initialisation de l'écran LCD
  lcd.begin(16,2);  //sur 16 colonnes et 2 lignes
  lcd.clear();      //Efface le contenu deja présent sur l'écran
  
  //Verification de l'initialisation du capteur
  if (!bmp.begin()) 
  {  
  Serial.println("Capteur BMP085 non détecté");
  
  lcd.setCursor(0,0);
  lcd.print("Erreur démarrage");

  delay(1500);
  
  //Réinitialise l'écran
  lcd.clear();
  lcd.setCursor(0,0);
  
  while (1) {}
  
  }
  else
  {
  Serial.println("Capteur BMP085 détecté");
  lcd.setCursor(1,0);
  lcd.print("Station Meteo");

   delay(1500);
   
  //Réinitialise l'écran
  lcd.clear();
  lcd.setCursor(0,0);
  }
}

void loop () 
{
  
////////////////////////////////////////////////////////////////////
//                          Magnetometre                          //
////////////////////////////////////////////////////////////////////

//Initialisation magnetometre
sensors_event_t event;
mag.getEvent(&event);

float heading = atan2(event.magnetic.y, event.magnetic.x);

float headingDegrees = (heading * 180/M_PI) + 180;

DirectionDuVent(headingDegrees);

Serial.print("Heading (degrees): "); Serial.println(headingDegrees);

Serial.println(affvent);

////////////////////////////////////////////////////////////////////
//              Capteur de pression et temperature                //
////////////////////////////////////////////////////////////////////

temp=bmp.readTemperature();
pressure=bmp.readPressure()/100;

Serial.println(temp);
Serial.println(pressure);

////////////////////////////////////////////////////////////////////
//                          Affichage                             //
////////////////////////////////////////////////////////////////////

Message_previsionel = "Il fera beau temps, les temperatures seront moderees pour la saison. Attention a la chute brutale des temperatures cette nuit";
  
afftemp = "T=";
affpress = "P=";

DataLine1= afftemp+temp+"C  "+affpress+pressure+"hPa";
DataLine2= "Vent " + affvent;

lcd.setCursor(0,0);
lcd.print(DataLine1);

delay(1000);

Affichage_LCD(Message_previsionel, 1);

ClearLine(0);
lcd.clear();

lcd.setCursor(0,0);
lcd.print(DataLine2);

delay(1000);

Affichage_LCD(Message_previsionel, 1);

ClearLine(0);
lcd.clear();

}

void Affichage_LCD(String To_Display,int Line){

int i,j,k;
int taille = To_Display.length();

  for(i=0;i<=To_Display.length()-15;i++)    //augmentation de i pour décaler dans le tableau du texte
  {
    for (j=0;j<15;j++)
    {
      lcd.setCursor(j,Line);                //décalage sur le lcd vers la droite
      lcd.print(To_Display[j+i]);           // écrire une à une les lettres du tableau
    }
    
    delay(300);
  }
    delay(3000);
  }

void ClearLine (int Line){

int compt;

for (compt=15;compt>=0;compt--)
    {
      lcd.setCursor(compt,Line);
      lcd.print(" ");
      delay(100);
    }
}

void DirectionDuVent(float DirDegrees)
{
  if (DirDegrees <= 22.5 || DirDegrees >= 337.5)
    {
      affvent = "Nord";
    }
  
  if (DirDegrees < 67.5 && DirDegrees > 22.5 )
    {
      affvent = "Nord-Est";
    }

  if (DirDegrees <= 112.5 && DirDegrees >= 67.5 )
    {
      affvent = "Est";
    }

    if (DirDegrees < 157.5 && DirDegrees > 112.5 )
    {
      affvent = "Sud-Est";
    }

    if (DirDegrees <= 202.5 && DirDegrees >= 157.5 )
    {
      affvent = "Sud";
    }

    if (DirDegrees < 247.5 && DirDegrees > 202.5 )
    {
      affvent = "Sud-Ouest";
    }

    if (DirDegrees <= 292.5 && DirDegrees >= 247.5 )
    {
      affvent = "Ouest";
    }

    if (DirDegrees < 337.5 && DirDegrees > 292.5 )
    {
      affvent = "Nord-Ouest";
    }
   
}
