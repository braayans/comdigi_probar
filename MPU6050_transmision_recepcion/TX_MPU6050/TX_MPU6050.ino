/*Transmision de acelerometro y MPU - 6050
transceptor nrf24l01*/
//Escrito por: Brayan Romero


//libreria del MPU - 6050
#include "Wire.h" 

//Librerias de transceptor
#include <SPI.h>
#include <RH_NRF24.h>

//Objeto transceptor
RH_NRF24 nrf24;



//Variables
const int MPU_ADDR = 0x68; 
int16_t accelerometer_x, accelerometer_y, accelerometer_z; 
int16_t gyro_x, gyro_y, gyro_z; 
int16_t temperature;

String str_accelerometer_x, str_accelerometer_y, str_accelerometer_z;
String str_gyro_x, str_gyro_y, str_gyro_z; 
String str_datos;


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

  delay(1000); 
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


void loop() {

  mpu6050(); //Hacemos los calculos del modulo 



  //Conviritiendo a string
  str_accelerometer_x = String(accelerometer_x); 
  str_accelerometer_y = String(accelerometer_y); 
  str_accelerometer_z = String(accelerometer_z); 
  str_gyro_x = String(gyro_x);
  str_gyro_y = String(gyro_y);
  str_gyro_z = String(gyro_z);

  //Uniendo los datos en una sola cadena
  str_datos = str_accelerometer_x + ',' + str_accelerometer_y + ',' + str_accelerometer_z + ',' + str_gyro_x + ',' + str_gyro_y + ',' + str_gyro_z;
  
  static const char *datos = str_datos.c_str();  

  //Enviando los datos
  nrf24.send((uint8_t *)datos, strlen(datos));
  nrf24.waitPacketSent();
  delay(10);  //Enviamos datos cada 10 ms

}

