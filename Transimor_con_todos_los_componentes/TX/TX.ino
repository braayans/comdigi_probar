/*Transmision general*/
//Escrito por: Brayan Romero




#include <OneWire.h>
#include <DallasTemperature.h>

//libreria del MPU - 6050
#include "Wire.h" 
#include <Adafruit_MPU6050.h>
#include <Adafruit_Sensor.h>

//Librerias de transceptor
#include <SPI.h>
#include <RH_NRF24.h>

//Librerios BMP280
#include<Adafruit_BMP280.h>



//Objeto DS18B20
#define ONE_WIRE_BUS 4 //PIN S de DS18B20 conectado al pin digital 2 (cambiar numero si necesario)

//Bus de protocolo ONEWIRE
OneWire oneWire(ONE_WIRE_BUS);

// Enviar numero de referencia ONEWire del ds18b20. 
DallasTemperature sensors(&oneWire);

//Objeto transceptor
RH_NRF24 nrf24;

//Objeto MPU6050
Adafruit_MPU6050 mpu;

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
  Serial.begin(115200);

  //Setup del transceptor
  if (!nrf24.init()){Serial.println("Fallo de inicializacion");}
  if (!nrf24.setChannel(2)){Serial.println("No se ha logrado establecer canal");} 
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)){Serial.println("Fallo en opciones RF");}  

  Serial.println("Todo funcionando ant");

  //Setup del MPU - 6050
  if(mpu.begin()){
    Serial.println("MPU OK");
  }
  else{
    Serial.println("ERROR MPU");
  }
  
  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);  //Medicion hasta 8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);  //Mediciones de 500°/ seg
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ); // 184 Hz ancho de banda



  //Setup del DS18b20
  //Se inicializa el sensor DS18B20 (libreria DallasTemperature)
  sensors.begin();

  //Setup bmp280
  if(bmp.begin()){
    Serial.println("BMP OK");
  }
  else{
    Serial.println("ERROR BMP");
  }
  bmp.setSampling(Adafruit_BMP280::MODE_NORMAL,     /* Modo de operación */
  Adafruit_BMP280::SAMPLING_X2,     /* Temp. oversampling */
  Adafruit_BMP280::SAMPLING_X16,    /* Presion oversampling */
  Adafruit_BMP280::FILTER_X16,      /* Filtrado. */
  Adafruit_BMP280::STANDBY_MS_500); /* Tiempo Standby. */



  Serial.println("Todo funcionando");
  delay(1000); 
}

void dallas(){   //Funcion donde se hace la lectura de temperatura
  sensors.requestTemperatures();
  temperaturaD = sensors.getTempCByIndex(0);
}

void mpu6050(){

  sensors_event_t a, g, temp;
  mpu.getEvent(&a, &g, &temp);

  accelerometer_x = a.acceleration.x;
  accelerometer_y = a.acceleration.y;
  accelerometer_z = a.acceleration.z;

  gyro_x = g.gyro.x;
  gyro_y = g.gyro.y;
  gyro_z = g.gyro.z;

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
  delay(2000);
  Serial.println(str_datos); //Borrar para cuando se implemente en el balon para no gastar espacio innecesareamente
  static const char *datos = str_datos.c_str();  

  //Envia los datos a la base
  nrf24.send((uint8_t *)datos, strlen(datos));
  nrf24.waitPacketSent();
  delay(10);  //Enviamos datos cada 10 ms

}
