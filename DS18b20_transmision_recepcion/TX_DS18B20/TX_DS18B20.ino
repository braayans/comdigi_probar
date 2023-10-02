/*Transmision de Temperatura usando DS18B20 y
transceptor nrf24l01*/
//Escrito por: Brayan Romero

// Librerias de DS18B20
#include <OneWire.h>
#include <DallasTemperature.h>

// Librerias de transceptor
#include <SPI.h>
#include <RH_NRF24.h>

#define ONE_WIRE_BUS 2 //PIN S de DS18B20 conectado al pin digital 2 (cambiar numero si necesario)  

//Bus de protocolo ONEWIRE
OneWire oneWire(ONE_WIRE_BUS);

// Enviar numero de referencia ONEWire del ds18b20. 
DallasTemperature sensors(&oneWire);

//Creacion de objeto transceptor
RH_NRF24 nrf24;


float temperaturaD = 0;   //Variable donde se guardara la temperatura
String str_temperaturaD;   //Cadena de caracteres que se enviara por el transceptor


void setup(void)
{

  Serial.begin(9600);

  //Se inicializa el sensor DS18B20 (libreria DallasTemperature)
  sensors.begin();

  //Inicializacion de transceptor
  if (!nrf24.init()){Serial.println("Fallo de inicializacion");}
  if (!nrf24.setChannel(2)){Serial.println("No se ha logrado establecer canal");} //Cambiar el numero si se desea transmitir en otro canal.... Si 2 entonces f = 2400 + 2 MHz
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)){Serial.println("Fallo en opciones RF");}  //Aqui se puede cambiar la velocidad de transmision y la potencia de transmision


  delay(1000);

}



void dallas(){   //Funcion donde se hace la lectura de temperatura
  sensors.requestTemperatures();
  temperaturaD = sensors.getTempCByIndex(0);
}


void loop(void)
{

  dallas(); //Se llama la funcion de lectura de temperatura

  str_temperaturaD = String(temperaturaD); //Se convierte al formato para enviar

  static const char *datos = str_temperaturaD.c_str(); //Reformateo a string de lenguaje C

  //Enviando los datos
  nrf24.send((uint8_t *)datos, strlen(datos)); 
  nrf24.waitPacketSent();
  delay(10);  //Enviamos datos cada 10 ms

}
