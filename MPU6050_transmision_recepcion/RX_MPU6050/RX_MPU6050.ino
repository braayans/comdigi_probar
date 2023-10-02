//Programa de recepcion para BMP280
//Escrito por: Brayan Romero 

//Librerias
#include <SPI.h>	
#include <RH_NRF24.h>	

//Objeto de transceptor
RH_NRF24 nrf24;		

// cadenas de caracteres que esperamos recibir
String str_datos;
String str_accelerometer_x;
String str_accelerometer_y;
String str_accelerometer_z;
String str_gyro_x;
String str_gyro_y;
String str_gyro_z;

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

    int primera_coma = str_datos.indexOf(',');  // Busca la primera coma
    if (primera_coma != -1) {
      str_accelerometer_x = str_datos.substring(0, primera_coma);
      int segunda_coma = str_datos.indexOf(',', primera_coma + 1); // Busca la segunda coma
      if (segunda_coma != -1) {
        str_accelerometer_y = str_datos.substring(primera_coma + 1, segunda_coma);
        int tercera_coma = str_datos.indexOf(',', segunda_coma + 1); // Busca la tercera coma
        if (tercera_coma != -1) {
          str_accelerometer_z = str_datos.substring(segunda_coma + 1, tercera_coma);
          int cuarta_coma = str_datos.indexOf(',', tercera_coma + 1); // Busca la cuarta coma
          if (cuarta_coma != -1) {
            str_gyro_x = str_datos.substring(tercera_coma + 1, cuarta_coma);
            int quinta_coma = str_datos.indexOf(',', cuarta_coma + 1); // Busca la quinta coma
            if (quinta_coma != -1) {
              str_gyro_y = str_datos.substring(cuarta_coma + 1, quinta_coma);
              str_gyro_z = str_datos.substring(quinta_coma + 1);
            }
          }
        }
      }
    }

    Serial.print("Accelerometer X: ");
    Serial.println(str_accelerometer_x);

    Serial.print("Accelerometer Y: ");
    Serial.println(str_accelerometer_y);

    Serial.print("Accelerometer Z: ");
    Serial.println(str_accelerometer_z);

    Serial.print("Gyro X: ");
    Serial.println(str_gyro_x);

    Serial.print("Gyro Y: ");
    Serial.println(str_gyro_y);

    Serial.print("Gyro Z: "); 
    Serial.println(str_gyro_z);

  }
}
