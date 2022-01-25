#include <avr/io.h>
#include <util/delay.h> 
#include <Wire.h> 
//**Motor Lavado**
int IN1 = DDB0;      // pin digital 8 de Arduino a IN1 de modulo controlador
int IN2 = DDB1;      // pin digital 9 de Arduino a IN2 de modulo controlador
int IN3 = DDB2;     // pin digital 10 de Arduino a IN3 de modulo controlador
int IN4 = DDB3;     // pin digital 11 de Arduino a IN4 de modulo controlador
int demora = 10;     //TIEMPO ESPERA


//**Motor secado**
int IN5 = DDD4;      // pin digital 8 de Arduino a IN1 de modulo controlador
int IN6 = DDD5;      // pin digital 9 de Arduino a IN2 de modulo controlador
int IN7 = DDD6;     // pin digital 10 de Arduino a IN3 de modulo controlador
int IN8 = DDD7;     // pin digital 11 de Arduino a IN4 de modulo controlador


//**LEDS FUNCIONES DE LA LAVADORA
int lavado = DDA0;
int enjuague = DDA2;
int secado = DDA4;
int listo = DDA6;


//**LEDS ENTRADAS Y SALIDAS
int agua_in = DDC7;
int agua_out = DDC5;
int aire_caliente = DDC3;

//lcd
#include<LiquidCrystal_I2C.h>
#include<LiquidMenu.h>
LiquidCrystal_I2C lcd(0x27,16,2);

//Encoder 

int outputA = DDD3;
int outputB = DDD2;
int sw = DDB4;


int aState;
int aLastState;

//Menu principal 
LiquidLine linea1(1, 0, "Lavado");
LiquidLine linea2(1, 1, "Secado");
LiquidLine linea3(1, 0, "Conectar dispositivo");
LiquidScreen pantalla1(linea1,linea2,linea3);

//Menu lavado
LiquidLine linea1_2(1, 0, "Lavar");
LiquidLine linea2_2(1, 1, "Enjuagar");
LiquidLine linea3_2(1, 0, "Lavado Completo");
LiquidLine linea4_2(1, 1, "atras");
LiquidScreen pantalla2(linea1_2,linea2_2,linea3_2,linea4_2);


//Menu Secado
LiquidLine linea1_3(1, 0, "10 min");
LiquidLine linea2_3(1, 1, "20 min");
LiquidLine linea3_3(1, 0, "30 min");
LiquidLine linea4_3(1, 1, "Atras");
LiquidScreen pantalla3(linea1_3,linea2_3,linea3_3,linea4_3);


LiquidMenu menu(lcd,pantalla1,pantalla2,pantalla3);


void setup() {
  //Motor Lavado
  
  DDRB |= (1<<IN1);
  DDRB |= (1<<IN2);
  DDRB |= (1<<IN3);
  DDRB |= (1<<IN4);
  
  //Motor Secado
  
  DDRD |= (1<<IN5);
  DDRD |= (1<<IN6);
  DDRD |= (1<<IN7);
  DDRD |= (1<<IN8);
  
  //Declaracion de los leds como salidas
  
               
  DDRA |= (1<<lavado);
  DDRA |= (1<<enjuague);
  DDRA |= (1<<secado);
  DDRA |= (1<<listo);
  
  DDRC |= (1<<agua_in);
  DDRC |= (1<<agua_out);
  DDRC |= (1<<aire_caliente);
  

  //LCD
  lcd.init();
  lcd.backlight();
  DDRB |= (1<<sw);
  menu.init();


  //Menu principal
  linea1.set_focusPosition(Position::LEFT); 
  linea2.set_focusPosition(Position::LEFT); 
  linea3.set_focusPosition(Position::LEFT); 

  linea1.attach_function(1, lavado_final); //IMPLEMENTAR FUNCION
  linea2.attach_function(1, secado_final);//IMPLEMENTAR FUNCION
  linea3.attach_function(1, conectar_dispositivo); //IMPLEMENTAR FUNCION
  menu.add_screen(pantalla1);

  //Menu lavado
  linea1_2.set_focusPosition(Position::LEFT); 
  linea2_2.set_focusPosition(Position::LEFT); 
  linea3_2.set_focusPosition(Position::LEFT); 
  linea4_2.set_focusPosition(Position::LEFT); 
   
  linea1_2.attach_function(1, lavar); //IMPLEMENTAR FUNCION
  linea2_2.attach_function(1, enjuagado);//IMPLEMENTAR FUNCION
  linea3_2.attach_function(1, lavado_completo);//IMPLEMENTAR FUNCION
  linea4_2.attach_function(1, atras);//IMPLEMENTAR FUNCION
  menu.add_screen(pantalla2);

  //Menu Enjuagado 
  linea1_3.set_focusPosition(Position::LEFT); 
  linea2_3.set_focusPosition(Position::LEFT); 
  linea3_3.set_focusPosition(Position::LEFT); 
  linea4_3.set_focusPosition(Position::LEFT); 
   
  linea1_3.attach_function(1, secar); //IMPLEMENTAR FUNCION
  linea2_3.attach_function(1, secar);//IMPLEMENTAR FUNCION
  linea3_3.attach_function(1, secar);//IMPLEMENTAR FUNCION
  linea4_3.attach_function(1, atras);//IMPLEMENTAR FUNCION
  menu.add_screen(pantalla3);   

  pantalla1.set_displayLineCount(2);
  pantalla2.set_displayLineCount(2);
  pantalla3.set_displayLineCount(2);
  menu.set_focusedLine(0);

  menu.update();
}
int incremento=0;
void loop() {
    selectOption();

  aState = (PIND & (1<<outputA)); 
  
  
    if (aState != aLastState){     
      if ((PIND & (1<<outputB)) != aState) { 
        incremento++;
        if(incremento>1){
          incremento=0; 
          menu.switch_focus(false);}
      } else {
        incremento++;
        if(incremento>1){
          incremento=0; 
          menu.switch_focus(true);}
      }
      menu.update();
      aLastState = aState;
  }
}

void conectar_dispositivo(){}




void selectOption(){
  if((PIND & (1<<sw)) == LOW){
      menu.call_function(1);
      delay(500);
  }
}

void lavado_final(){
  menu.change_screen(2);
  menu.set_focusedLine(0);
  }

void secado_final(){
  menu.change_screen(3);
  menu.set_focusedLine(0);
  }
void atras(){
  menu.change_screen(1);
  menu.set_focusedLine(0);
}
void lavar() {
  lcd.clear();
  lcd.print("Lavando");
  //DEBERIA ENCENDERSE CON EL ENCODER***FUNCION DEL ENCODER ROTATIVO
  PORTA = (1<<lavado); 
  //Simula el llenado de agua
  PORTC = (1<<agua_in); 
  delay(15000);
  PORTC = (0<<agua_in);
  
  lavar_motor();
  PORTA = (0<<lavado); 
  PORTA = (1<<listo); 
  lcd.clear();
  lcd.print("**LAVADO LISTO**");
  delay(10000);
  PORTA = (0<<listo); 
  atras();
}
void enjuagado() {
  lcd.clear();
  lcd.print("Enjuagando...");
  //DEBERIA ENCENDERSE CON EL ENCODER***FUNCION DEL ENCODER ROTATIVO
  PORTA = (1<<enjuague);
  //El agua está siendo expulsada
  PORTC = (1<<agua_out);
  delay(10000);
  enjuagar_motor();
  PORTA = (0<<enjuague);
  PORTC = (0<<agua_out);
  PORTA = (1<<listo);
  lcd.clear();
  lcd.print("**ENJUAGUE LISTO**");
  delay(10000);
  PORTA = (0<<listo);
  atras();


}
void lavado_completo() {
  //DEBERIA ENCENDERSE CON EL ENCODER***FUNCION DEL ENCODER ROTATIVO
  PORTA = (1<<lavado);
  PORTA = (1<<enjuague);//DEBERIA ENCENDERSE CON EL ENCODER***FUNCION DEL ENCODER ROTATIVO
  lavar();
  enjuagado();

}
void secar() { //los ciclos seran 10 ,20 o 30 min
  lcd.clear();
  lcd.print("SECANDO...");
  PORTA = (1<<secado); //ESTO VA EN EL ENCODER
  for (int i = 0; i < 10 / 2; i++)
  {
    
    PORTC = (1<<aire_caliente);
    secado_motor();
    delay(3000);
    PORTC = (0<<aire_caliente);
    secado_motor();
    delay(3000);
    i++;
  }
  PORTA = (0<<secado);
  PORTA = (1<<listo);
  
  lcd.clear();
  lcd.print("**SECADO LISTO**");
  delay(10000);
  PORTA = (0<<listo);
  atras();

}
//El motor1 Gira 90 grados en sentido horario y luego 90 en sentido antihorario por 10 ciclos
void lavar_motor() {
  int contador = 0;
  while (contador < 5) {
    //Giro de 90 grados en sentido horario
    for (int i = 0; i < 128; i++)
    {
      // paso 1
      PORTB = (1<<IN1);
      PORTB = (0<<IN2);
      PORTB = (0<<IN3);
      PORTB = (0<<IN4);
      
      PORTA = (1<<listo);
      delay(demora);

      // paso 2
      
      PORTB = (0<<IN1);
      PORTB = (1<<IN2);
      PORTB = (0<<IN3);
      PORTB = (0<<IN4);
      delay(demora);

       // paso 3
     
      PORTB = (0<<IN1);
      PORTB = (0<<IN2);
      PORTB = (1<<IN3);
      PORTB = (0<<IN4);
      delay(demora);

      // paso 4
      
      PORTB = (0<<IN1);
      PORTB = (0<<IN2);
      PORTB = (0<<IN3);
      PORTB = (1<<IN4);
      delay(demora);
    }
    delay(1000);

    //Giro de 90 grados en sentido antihorario
    for (int i = 0; i < 128; i++)
    {
      // paso 4
      
      PORTB = (0<<IN1);
      PORTB = (0<<IN2);
      PORTB = (0<<IN3);
      PORTB = (1<<IN4);
      delay(demora);

       // paso 3
      PORTB = (0<<IN1);
      PORTB = (0<<IN2);
      PORTB = (1<<IN3);
      PORTB = (0<<IN4);
      delay(demora);

      // paso 2
      PORTB = (0<<IN1);
      PORTB = (1<<IN2);
      PORTB = (0<<IN3);
      PORTB = (0<<IN4);
      delay(demora);

      // paso 1
      PORTB = (1<<IN1);
      PORTB = (0<<IN2);
      PORTB = (0<<IN3);
      PORTB = (0<<IN4);
      delay(demora);
    }
    delay(1000);
    contador++;
  }
}


//El motor2 gira en sentido horario sin detenerse por 5 min
void enjuagar_motor() {
  int contador = 0;
  int y = 4;
  while (contador < 5) {
    //Giro de 90 grados en sentido horario
    for (int i = 0; i < 512; i++)
    {
       // paso 1
      PORTB = (1<<IN1);
      PORTB = (0<<IN2);
      PORTB = (0<<IN3);
      PORTB = (0<<IN4);
      delay(demora / y);

       // paso 2
      PORTB = (0<<IN1);
      PORTB = (1<<IN2);
      PORTB = (0<<IN3);
      PORTB = (0<<IN4);
      delay(demora / y);

     // paso 3
      PORTB = (0<<IN1);
      PORTB = (0<<IN2);
      PORTB = (1<<IN3);
      PORTB = (0<<IN4);
      delay(demora / y);

      // paso 4
      PORTB = (0<<IN1);
      PORTB = (0<<IN2);
      PORTB = (0<<IN3);
      PORTB = (1<<IN4);
      delay(demora / y);
    }
    contador++;
  }
}

//el motor girara sin detenerse durante los ciclos determinados
void secado_motor() {
  int contador = 0;
  int y = 3;
  while (contador < 2) {
    //Giro de 90 grados en sentido horario
    for (int i = 0; i < 512; i++)
    {
      // paso 1
      
      PORTD = (1<<IN5);
      PORTD = (0<<IN6);
      PORTD = (0<<IN7);
      PORTD = (0<<IN8);
      
      delay(demora / y);

       // paso 2
      PORTD = (0<<IN5);
      PORTD = (1<<IN6);
      PORTD = (0<<IN7);
      PORTD = (0<<IN8);
      delay(demora / y);

       // paso 3
      PORTD = (0<<IN5);
      PORTD = (0<<IN6);
      PORTD = (1<<IN7);
      PORTD = (0<<IN8);
      delay(demora / y);

      // paso 4
      PORTD = (0<<IN5);
      PORTD = (0<<IN6);
      PORTD = (0<<IN7);
      PORTD = (1<<IN8);
      delay(demora / y);
    }
    contador++;
  }
}
