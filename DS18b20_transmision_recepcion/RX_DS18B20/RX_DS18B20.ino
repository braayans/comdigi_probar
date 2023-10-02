//Programa de recepcion para DS18B20
//Escrito por: Brayan Romero

//Librerias
#include <SPI.h>	
#include <RH_NRF24.h>	

//Objeto de transceptor
RH_NRF24 nrf24;		

// cadenas de caracteres que esperamos recibir
String str_datos;	

void setup() 
{
  Serial.begin(9600);		
  if (!nrf24.init()){Serial.println("fallo de inicializacion");}      
  if (!nrf24.setChannel(2)) {Serial.println("fallo en establecer canal");} //Se puede cambiar el canal
    
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)){Serial.println("fallo en opciones RF");} //Las opciones deben ser iguales a la del transmisor
  				    
     
  Serial.println("Base iniciada");  //Si es el primer print entonces todo OK
}

void loop()
{
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];			
  uint8_t buflen = sizeof(buf);	
    
  if (nrf24.recv(buf, &buflen))	// si recibe datos validos
  { 
    str_datos = String((char*)buf);	// almacena en str_datos datos recibidos

    Serial.print("Temperatura: ");	// muestra texto
    Serial.println(str_datos);	// muestra valor de la variable

  }
}