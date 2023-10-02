/*Transmision de Temperatura presion y altitud usando BMP280 y
transceptor nrf24l01*/
//Escrito por: Brayan Romero

//Librerias de transceptor
#include <SPI.h>
#include <RH_NRF24.h>


//Librerias para bmp280
#include<Wire.h>
#include<Adafruit_Sensor.h>
#include<Adafruit_BMP280.h>

//Objetos de BMP280 y transceptor

Adafruit_BMP280 bmp;
RH_NRF24 nrf24;



//Variables que calcular
float temperatura = 0;
float presion = 0;
float altitud = 0;


//Variables en forma de caracter para ser transmitidas
String str_temperatura;
String str_presion;
String str_altitud;
String str_datos;

void setup() {
  Serial.begin(9600);

  //Probar si sensor BMP esta disponible  
  if (!bmp.begin()) {
    Serial.println(F("Could not find a valid BMP280 sensor, check wiring!"));
    while (1);
  }

  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Modo de operación */
    Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
    Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
    Adafruit_BMP280::FILTER_X16,      /* Filtrado. */
    Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */

  //Comprobar si la antena esta funcionando 

  if (!nrf24.init()){Serial.println("Fallo de inicializacion");}
  if (!nrf24.setChannel(2)){Serial.println("No se ha logrado establecer canal");} //Cambiar el numero si se desea transmitir en otro canal.... Si 2 entonces f = 2400 + 2 MHz
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)){Serial.println("Fallo en opciones RF");}  //Aqui se puede cambiar la velocidad de transmision y la potencia de transmision

 
  delay(1000); //Por si las moscas, no es obligatorio pero recomendable
}

//Medicion de las variables que se envian 
void bmp280(){

  //Lectura de temperatura
  temperatura = bmp.readTemperature();

  //Lectura de presion en hPa
  presion = bmp.readPressure() / 100;
  
  //Lectura de altitud el parametro es la presion en hpa al nivel del mar
  altitud = bmp.readAltitude(1013.25);
}

void loop() {
  
  bmp280(); //Se llama la funcion para medir los datos


  //Convertimos los datos float a string para enviarlos por el transceptor
  str_temperatura = String(temperatura);
  str_presion = String(presion);
  str_altitud = String(altitud);

  //Unimos los strings creados en una unica linea separadas por comas(,)
  str_datos = str_temperatura + ","  + str_presion + "," + str_altitud; 

  static const char *datos = str_datos.c_str();  //Cambiar formato str_datos y guardarlo en un puntero (dejar quieto)


  //Enviando los datos
  nrf24.send((uint8_t *)datos, strlen(datos));
  nrf24.waitPacketSent();
  delay(10);  //Enviamos datos cada 10 ms

}
