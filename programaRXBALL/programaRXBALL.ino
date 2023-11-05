// Programa para el receptor de un sistema de comunicación inalambrica 
#include <SPI.h>  //Incluye libreria SPI para comunicarse con dispositivos externos utilizando el protocolo SPI.
#include <RF24.h> // Incluye la biblioteca RF24, se utiliza para la comunicación inalámbrica utilizando módulos de radio NRF24L01. 

RF24 radio(8, 10); // Define los puertos CE pin 10, CSN pin 8
const byte address[6] = "00001"; // Define una constante tipo byte ("address") que tiene la secuencia "00001".
 
float altitud, accelerometer_x, accelerometer_y, accelerometer_z,gyro_x, gyro_y, gyro_z, temperaturaD, presion;
void setup() 
{
  Serial.begin(115200); 
  radio.begin(); // Inicializa el módulo de radiofrecuencia.
  radio.openReadingPipe(0, address); // Toma dos parametros, 0 y la dirección de destino 
  radio.setChannel(86); 
  radio.setDataRate(RF24_250KBPS); // Establece la tasa de datos de la comunicación (250K, 1M, 2M)
  radio.setPALevel(RF24_PA_MAX); // Establece nivel de potencia del modulo (MAX, MIN, HIGH, LOW)
  radio.startListening();// Pone el modulo en modo escucha, en este momento puede recibir los datos del TX 
}

void loop()
{
  if (radio.available())     
  {  
    float datos[9]; 
    radio.read(datos, sizeof(datos)); 
    
    altitud = datos[0];
    accelerometer_x = datos[1];
    accelerometer_y = datos[2];
    accelerometer_z = datos[3];
    gyro_x = datos[4];
    gyro_y = datos[5];
    gyro_z = datos[6];
    temperaturaD = datos[7];
    presion = datos[8];


    Serial.print(altitud); // Imprime los valores a traves del puerto serial
    Serial.print(",");
    Serial.print(accelerometer_x);
    Serial.print(",");
    Serial.print(accelerometer_y);
    Serial.print(",");
    Serial.print(accelerometer_z);
    Serial.print(",");
    Serial.print(gyro_x);
    Serial.print(",");
    Serial.print(gyro_y);
    Serial.print(",");
    Serial.print(gyro_z);
    Serial.print(",");
    Serial.print(temperaturaD);
    Serial.print(",");
    Serial.println(presion); 
  }
  //delay(50);
}
