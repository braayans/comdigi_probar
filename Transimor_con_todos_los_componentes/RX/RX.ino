//Programa de recepcion para todo el modelo
//Escrito por: Brayan Romero

//Librerias
#include <SPI.h>	
#include <RH_NRF24.h>	

//Objeto de transceptor
RH_NRF24 nrf24;		

// cadenas de caracteres que esperamos recibir
String str_datos;	
String recibido;

//Variables recibidas
String str_temperaturaD;   
String str_accelerometer_x, str_accelerometer_y, str_accelerometer_z;
String str_gyro_x, str_gyro_y, str_gyro_z; 
String str_presion;
String str_altitud;

//interruptor
bool bandera = false;


void setup() 
{
  Serial.begin(9600);		
  if (!nrf24.init()){Serial.println("fallo de inicializacion");}   
  else{
    Serial.println("Antena funcionando!!!!");
    delay(2000);
  }   
  if (!nrf24.setChannel(2)) {Serial.println("fallo en establecer canal");} //Se puede cambiar el canal
    
  if (!nrf24.setRF(RH_NRF24::DataRate250kbps, RH_NRF24::TransmitPower0dBm)){Serial.println("fallo en opciones RF");} //Las opciones deben ser iguales a la del transmisor
  Serial.println("Base iniciada");  //Si es el primer print entonces todo OK
}

void loop()
{
  //Mandamos una I desde el teclado para que la base le pida datos al balon 
  
  if (Serial.available() > 0) {
  // Lee el primer byte disponible
    char incomingByte = Serial.read();
    
    // Verifica si el byte recibido es igual a 'R'
    if (incomingByte == 'I') {
      // Activa la bandera cuando se recibe 'I'
      bandera = true;
      Serial.println("Comunicacion con Balon iniciada");
    }

    if (incomingByte == 'R') {
      // Desactiva la bandera cuando se recibe 'R'
      bandera = false;
      Serial.println("Finalizacion de comunicacion");
      delay(1000);
    }
  }
  
  uint8_t buf[RH_NRF24_MAX_MESSAGE_LEN];			
  uint8_t buflen = sizeof(buf);	

  if (nrf24.recv(buf, &buflen)) 
  {
    str_datos = String((char*)buf);

    // Divide la cadena usando ',' como delimitador
    int pos = 0;
    while ((pos = str_datos.indexOf(',')) != -1) 
    {
      // Encuentra la posición de la próxima coma en la cadena
      // Si no se encuentra más ninguna coma, la función indexOf() devuelve -1

      String token = str_datos.substring(0, pos); 

      if (str_accelerometer_x.length() == 0) {
        str_accelerometer_x = token;
      } else if (str_accelerometer_y.length() == 0) 
      {
        str_accelerometer_y = token;
      } else if (str_accelerometer_z.length() == 0) 
      {
        str_accelerometer_z = token;
      } else if (str_gyro_x.length() == 0) 
      {
        str_gyro_x = token;
      } else if (str_gyro_y.length() == 0) 
      {
        str_gyro_y = token;
      } else if (str_gyro_z.length() == 0) 
      {
        str_gyro_z = token;
      } else if (str_temperaturaD.length() == 0) 
      {
      str_temperaturaD = token;
      } else if (str_presion.length() == 0) 
      {
        str_presion = token;
      } else if (str_altitud.length() == 0) 
      {
        str_altitud = token;
      }
    // Actualiza str_datos para excluir el token que ya se procesó, avanzando más allá de la coma
    str_datos = str_datos.substring(pos + 1);
  }

  recibido = str_accelerometer_x + ',' + str_accelerometer_y + ',' + str_accelerometer_z + ',' + str_gyro_x + ',' + str_gyro_y + ',' + str_gyro_z + ',' + str_temperaturaD + ',' + str_presion + ',' + str_altitud;

  Serial.println(recibido);
  }

}

