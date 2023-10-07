/*Transmision general*/
//Escrito por: Brayan Romero


#include <OneWire.h>
#include <DallasTemperature.h>

//libreria del MPU - 6050
#include "Wire.h" 

//Librerias de transceptor
#include <SPI.h>
#include <RH_NRF24.h>

//Librerios BMP280
#include<Adafruit_Sensor.h>
#include<Adafruit_BMP280.h>



//Objeto DS18B20
#define ONE_WIRE_BUS 2 //PIN S de DS18B20 conectado al pin digital 2 (cambiar numero si necesario

//Bus de protocolo ONEWIRE
OneWire oneWire(ONE_WIRE_BUS);

// Enviar numero de referencia ONEWire del ds18b20. 
DallasTemperature sensors(&oneWire);

//Objeto transceptor
RH_NRF24 nrf24;

//Objeot BMP280
Adafruit_BMP280 bmp;

//Variables
float temperaturaD = 0;   //DS18B20
String str_temperaturaD;   

const int MPU_ADDR = 0x68; //MPU-6050
int16_t accelerometer_x, accelerometer_y, accelerometer_z; 
int16_t gyro_x, gyro_y, gyro_z; 
int16_t temperature;

String str_accelerometer_x, str_accelerometer_y, str_accelerometer_z;
String str_gyro_x, str_gyro_y, str_gyro_z; 
String str_datos;

float presion = 0; //BMP280
float altitud = 0;


String str_presion;
String str_altitud;




bool bandera = false;

void setup() {
  Serial.begin(9600);

  //Setup del transceptor
  if (!nrf24.init()){Serial.println("Fallo de inicializacion");}
  if (!nrf24.setChannel(2)){Serial.println("No se ha logrado establecer canal");} 
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)){Serial.println("Fallo en opciones RF");}  


  //Setup del MPU - 6050
  Wire.begin();
  Wire.beginTransmission(MPU_ADDR); 
  Wire.write(0x6B); 
  Wire.write(0); 
  Wire.endTransmission(true);


  //Setup del DS18b20
  //Se inicializa el sensor DS18B20 (libreria DallasTemperature)
  sensors.begin();

  //Setup bmp280
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Modo de operación */
  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
  Adafruit_BMP280::FILTER_X16,      /* Filtrado. */
  Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */

  delay(1000); 
}

void dallas(){   //Funcion donde se hace la lectura de temperatura
  sensors.requestTemperatures();
  temperaturaD = sensors.getTempCByIndex(0);
}

void mpu6050(){

  Wire.beginTransmission(MPU_ADDR);
  Wire.write(0x3B); 
  Wire.endTransmission(false);
  Wire.requestFrom(MPU_ADDR, 7*2, true); 
  
  
  accelerometer_x = Wire.read()<<8 | Wire.read(); 
  accelerometer_y = Wire.read()<<8 | Wire.read(); 
  accelerometer_z = Wire.read()<<8 | Wire.read(); 
  temperature = Wire.read()<<8 | Wire.read(); 
  gyro_x = Wire.read()<<8 | Wire.read(); 
  gyro_y = Wire.read()<<8 | Wire.read(); 
  gyro_z = Wire.read()<<8 | Wire.read(); 
}

void bmp280(){

  //Lectura de presion en hPa
  presion = bmp.readPressure() / 100;
  
  //Lectura de altitud el parametro es la presion en hpa al nivel del mar
  altitud = bmp.readAltitude(1013.25);
}

void loop() {

  mpu6050(); //Hacemos los calculos del modulo 
  dallas(); //Se llama la funcion de lectura de temperatura
  bmp280();
   

  //Conviritiendo a string
  str_accelerometer_x = String(accelerometer_x); 
  str_accelerometer_y = String(accelerometer_y); 
  str_accelerometer_z = String(accelerometer_z); 
  str_gyro_x = String(gyro_x);
  str_gyro_y = String(gyro_y);
  str_gyro_z = String(gyro_z);
  str_temperaturaD = String(temperaturaD);
  str_presion = String(presion);
  str_altitud = String(altitud);


  //Uniendo los datos en una sola cadena
  str_datos = str_accelerometer_x + ',' + str_accelerometer_y + ',' + str_accelerometer_z + ',' + str_gyro_x + ',' + str_gyro_y + ',' + str_gyro_z + ',' + str_temperaturaD + ',' + str_presion + ',' + str_altitud;


  static const char *datos = str_datos.c_str();  

  //Envia los datos a la base
  nrf24.send((uint8_t *)datos, strlen(datos));
  nrf24.waitPacketSent();
  delay(10);  //Enviamos datos cada 10 ms

}
