//Librerias
#include <SPI.h> 
#include <RF24.h> 
//#include <Wire.h> 

#include <OneWire.h>
#include <DallasTemperature.h>

#include "Wire.h" 
#include <Adafruit_Sensor.h> 
#include <Adafruit_BMP280.h> 
#include <Adafruit_MPU6050.h>


RF24 radio(8,10); 
const byte address[6] = "00001"; // Define una constante tipo byte ("address") que tiene la secuencia "00001".

#define ONE_WIRE_BUS 4 //Pin digital dallas18b20
OneWire oneWire(ONE_WIRE_BUS); //Bus de protocolo ONEWIRE
DallasTemperature sensors(&oneWire); // Enviar numero de referencia ONEWire del ds18b20.

Adafruit_MPU6050 mpu;


Adafruit_BMP280 bmp;

//Variables
float temperaturaD = 0;   //DS18B20
String str_temperaturaD;   

const int MPU_ADDR = 0x68; //MPU-6050
float accelerometer_x, accelerometer_y, accelerometer_z; 
float gyro_x, gyro_y, gyro_z; 
float temperature;



float presion = 0; //BMP280
float altitud = 0;
float P0 = 1013.5;

void setup()
{
  Serial.begin(115200);  // Inicializar la comunicación serial a 9600 baudios


  radio.begin(); 
  radio.openWritingPipe(address); // Establece la dirección de escritura del módulo de radi
  radio.setChannel(86); // Configura el canal 101 (2501 MHz) como canal de comunicación (Mismo canal en ambos casos)
  radio.setDataRate(RF24_250KBPS); // Establece la tasa de datos de la comunicación (250K, 1M, 2M)
  radio.setPALevel(RF24_PA_MAX); // Establece nivel de potencia del modulo (MAX, MIN, HIGH, LOW)

  if(mpu.begin()){
    Serial.println("MPU OK");
  }
  else{
    Serial.println("ERROR MPU");
  }

  mpu.setAccelerometerRange(MPU6050_RANGE_8_G);  //Medicion hasta 8G
  mpu.setGyroRange(MPU6050_RANGE_500_DEG);  //Mediciones de 500°/ seg
  mpu.setFilterBandwidth(MPU6050_BAND_184_HZ); // 184 Hz ancho de banda

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

  P0 = bmp.readPressure()/100; // Lee la presión del BMP, la divide en 100 y almacena el resultado en P0
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

  altitud = bmp.readAltitude(P0);
  presion = bmp.readPressure() / 100;


}


void loop()
{

  altitud = 2000;
  accelerometer_x = 1000;
  accelerometer_y = 3000;
  accelerometer_z = 4000;
  gyro_x = 6000;
  gyro_y = 7000;
  gyro_z = 90;
  temperaturaD = 24.4;
  presion = 3000;

  float datos[9] = {altitud, accelerometer_x, accelerometer_y, accelerometer_z,gyro_x, gyro_y, gyro_z, temperaturaD, presion}; // Declara una variable "datos" como un arreglo de 8 elementos de tipo float (números decimales).
  radio.write(datos, sizeof(datos)); // Envia los datos almancenados en la variable datos a traves del objeto radio. El tamaño de los datos se determina utilizando la función "sizeof(datos)"

  for(int i = 0; i < 9; i++){
    if(i < 8){
      Serial.print(datos[i]);
      Serial.print(",");
    }
    else{
      Serial.println(datos[i]);
    }
  }

delay(50); 
}
