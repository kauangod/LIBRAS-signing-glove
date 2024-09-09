
#include <Wire.h>
#include <MPU6050_tockn.h>
#include "BluetoothSerial.h"
#include <string.h>
#include <time.h>

#if !defined(CONFIG_BT_ENABLED) || !defined(CONFIG_BLUEDROID_ENABLED)
#error Bluetooth is not enabled! Please run `make menuconfig` to and enable it
#endif

#define POLEGAR 14
#define NIVEL1_POLEGAR 10

#define INDICADOR 26
#define NIVEL1_INDICADOR 100

#define MEDIO 25
#define NIVEL1_MEDIO 100

#define ANELAR 12
#define NIVEL1_ANELAR 100

#define MINIMO 32
#define NIVEL1_MINIMO 100
 
#define VALOR_MIN_CONTATO 4000

#define VALOR_POSIT_MIN_X 30
#define VALOR_POSIT_MAX_X 140
#define VALOR_MIN_Y -15
#define VALOR_MAX_Y 30
#define VALOR_NEG_MAX_X -30
#define VALOR_NEG_MIN_X -140
#define FATOR_MULTIPLICATIVO 100;

//Variáveis Globais:
int PinoContato1 = 36;
int PinoContato2 = 39;
int PinoContato3 = 34;
int PinoBotao = 13;
int Contato1 = 0;
int Contato2 = 0;
int Contato3 = 0;
int Botao = 0;
int nivel2_polegar = -1;
int nivel2_indicador = -1;
int nivel2_medio = -1;
int nivel2_anelar = -1;
int nivel2_minimo = -1;
int calibrado = 0;

//Variáveis para armazenar valores do módulo.
MPU6050 mpu6050(Wire);
float AccX = 0.0, AccY = 0.0, GyrX = 0.0, GyrY = 0.0, AngleX = 0.0, AngleY = 0.0, 
displ_x = 0.0, displ_y = 0.0, current_gyrX = 0.0, current_gyrY = 0.0,
previous_gyrX = 0.0, previous_gyrY = 0.0, 
var_gyrX = 0.0, var_gyrY = 0.0, 
current_angY = 0.0, current_accx = 0.0,
previous_accx = 0.0, current_accy = 0.0, previous_accy =0.0, 
previous_angY = 0.0, var_angY = 0.0;

//Bluetooth:
BluetoothSerial SerialBT;
int reps = 0;
String letra = "-1";
String palavra = "";
char recebido = '\n';
String comando = "";

void pause(int wait_time){ 
  double elapsed_time = -1.0, start_time = 0.0, current_time = 0.0;

  start_time = millis();

  while (elapsed_time < wait_time) {
		current_time = millis();
    elapsed_time = (current_time - start_time) / 1000;
  }
}

void calibracao(){
	int i = 0, read_polegar = 0, read_indicador = 0, read_medio = 0, read_anelar = 0, read_minimo = 0, fim = 0;
  double elapsed_time = -1.0, max_time = 2.0, start_time, current_time;

  Serial.print("\nCalibração do módulo iniciada!\nNão mova a luva.");
  SerialBT.print("\nCalibração do módulo iniciada!\nNão mova a luva.");

  pause(2.0);

  mpu6050.calcGyroOffsets(true);
  
  Serial.print("\nCalibração do módulo finalizada, iniciando calibração dos sensores.\nFaça a letra 'O'."); 
  SerialBT.print("\nCalibração do módulo finalizada, iniciando calibração dos sensores.\nFaça a letra 'O'.");

  pause(4.0);

  start_time = millis();
		
  while (elapsed_time < max_time){
		current_time = millis();
    elapsed_time = (current_time - start_time) / 1000;
    
    read_polegar = analogRead(POLEGAR);
    if(read_polegar < NIVEL1_POLEGAR){
      Serial.print("\nNão está sendo reconhecida flexão no polegar!");
      SerialBT.print("\nNão está sendo reconhecida flexão no polegar!");
      start_time = current_time;
    }
    else
      nivel2_polegar = read_polegar+100;
        
		read_indicador = analogRead(INDICADOR);
    if(read_indicador < NIVEL1_INDICADOR){
      Serial.print("\nNão está sendo reconhecida flexão no indicador!");
      SerialBT.print("\nNão está sendo reconhecida flexão no indicador!");
      start_time = current_time;
    }
    else
      nivel2_indicador = read_indicador + 100;
        
    read_medio = analogRead(MEDIO);
    if(read_medio < NIVEL1_MEDIO){
      Serial.print("\nNão está sendo reconhecida flexão no dedo médio!");
      SerialBT.print("\nNão está sendo reconhecida flexão no dedo médio!");
      start_time = current_time;
    }
    else
      nivel2_medio = read_medio + 100;
      
    read_anelar = analogRead(ANELAR);
    if(read_anelar < NIVEL1_ANELAR){
      Serial.print("\nNão está sendo reconhecida flexão no anelar!");
      SerialBT.print("\nNão está sendo reconhecida flexão no anelar!");
      start_time = current_time;
    }
    else
      nivel2_anelar = read_anelar + 100;
        
    read_minimo=analogRead(MINIMO);
    if(read_minimo < NIVEL1_MINIMO){
      Serial.print("\nNão está sendo reconhecida flexão no dedo mínimo!");
      SerialBT.print("\nNão está sendo reconhecida flexão no dedo mínimo!");
      start_time = current_time;
    }
    else
      nivel2_minimo = read_minimo + 100;
  }
  
  Serial.print("\nCalibração finalizada.\n"); 
  SerialBT.print("\nCalibração finalizada.\n");

  calibrado = 1;

  pause(1.0);

  Serial.print("\n\nPolegar:");
  Serial.print("    Indicador:");
  Serial.print("    Médio:");
  Serial.print("    Anelar:");
  Serial.print("    Mínimo:");
  Serial.print("\n  ");
  Serial.print(nivel2_polegar);
  Serial.print("            ");
  Serial.print(nivel2_indicador);
  Serial.print("           ");
  Serial.print(nivel2_medio);
  Serial.print("          ");
  Serial.print(nivel2_anelar);
  Serial.print("          ");
  Serial.print(nivel2_minimo);

  SerialBT.print("\nNível 2 Polegar: ");
  SerialBT.print(nivel2_polegar);
  SerialBT.print("\nNível 2 Indicador: ");
  SerialBT.print(nivel2_indicador);
  SerialBT.print("\nNível 2 Médio: ");
  SerialBT.print(nivel2_medio);
  SerialBT.print("\nNível 2 Anelar: ");
  SerialBT.print(nivel2_anelar);
  SerialBT.print("\nNível 2 Mínimo: ");
  SerialBT.print(nivel2_minimo);
}


int flexao(int dedo){
  int nivel = 0;
  int valor_leitura = analogRead(dedo);
   
  switch (dedo){
    case POLEGAR:
      if(valor_leitura >= NIVEL1_POLEGAR && valor_leitura <= nivel2_polegar)
        nivel = 1;
      else if(valor_leitura >= nivel2_polegar)
        nivel = 2;
      break;
    case INDICADOR:
      if(valor_leitura >= NIVEL1_INDICADOR && valor_leitura <= nivel2_indicador)
        nivel = 1;
      else if(valor_leitura >= nivel2_indicador)
        nivel = 2;
      break;
    case MEDIO:
      if(valor_leitura >= NIVEL1_MEDIO && valor_leitura <= nivel2_medio)
        nivel = 1;
      else if(valor_leitura >= nivel2_medio)
        nivel = 2;
      break;
    case ANELAR:
      if(valor_leitura >= NIVEL1_ANELAR && valor_leitura <= nivel2_anelar)
        nivel = 1;
      else if(valor_leitura >= nivel2_anelar)
        nivel = 2;
      break;
    case MINIMO:
      if(valor_leitura >= NIVEL1_MINIMO && valor_leitura <= nivel2_minimo)
        nivel = 1;
      else if(valor_leitura >= nivel2_minimo)
        nivel = 2;
      break;
    default:
      nivel = -1;
  }
  
  return nivel;
}

void rep(String letra_atual){
  if(letra == "-1" || letra == letra_atual)
    reps += 1;
  else if(letra != letra_atual)
    reps = 1;
  letra = letra_atual;
}

int apont_cima(){
  return (AngleX >= VALOR_POSIT_MIN_X && AngleX <= VALOR_POSIT_MAX_X) ? 1 : 0;
}

int apont_baixo(){
  return (AngleX >= VALOR_NEG_MIN_X && AngleX <= VALOR_NEG_MAX_X) ? 1 : 0;
}

int mov_y(){
  return (var_gyrY > -4 && var_gyrY < 4) ? 0 : 1;
}

int movimentando (){
  return (displ_y > -6 && displ_y < 6) ? 0 : 1; 
}

void setup()
{
  Serial.begin(115200);
 
  Wire.begin();
  mpu6050.begin();

  SerialBT.begin("LUBRAS");
  Serial.print("\nO dispositivo bluetooth está pronto para parear!");

  // Sensores de Contato e Botão
  pinMode(PinoContato1, INPUT);
  pinMode(PinoContato2, INPUT);
  pinMode(PinoContato3, INPUT);
  pinMode(PinoBotao, INPUT);

  // Sensores Flex
  pinMode(MINIMO, INPUT);
  pinMode(ANELAR, INPUT);
  pinMode(MEDIO, INPUT);
  pinMode(INDICADOR, INPUT);
  pinMode(POLEGAR, INPUT);
}

void loop(){
  if (SerialBT.available()){
    char recebido = SerialBT.read();
    if (recebido != '\n')
      comando += String(recebido);
    else{
      comando = "";
    }
  }
  if(comando == "Calibração"){
    nivel2_polegar = -1;
    nivel2_indicador = -1;
    nivel2_medio = -1;
    nivel2_anelar = -1;
    nivel2_minimo = -1;
    calibracao();
    comando == "";
  }
  else if(calibrado){
    mpu6050.update();
    AngleX = mpu6050.getAngleX();
    AngleY = mpu6050.getAngleY();
    Contato1 = digitalRead(PinoContato1);
    Botao = digitalRead(PinoBotao);
    GyrX = mpu6050.getGyroAngleX();
    GyrY = mpu6050.getGyroAngleY();
    AccX = mpu6050.getAccX() * FATOR_MULTIPLICATIVO;
    AccY = mpu6050.getAccY() * FATOR_MULTIPLICATIVO;

    Serial.print("\n\n\n\n\n");
    Serial.print("   var_gyrX: ");
    Serial.print(var_gyrX);
    Serial.print("   var_gyrY: ");
    Serial.print(var_gyrY);
    Serial.print("   var_angY: ");
    Serial.print(var_angY);
    Serial.print("   displ_x: ");
    Serial.print(displ_x);
    Serial.print("   displ_y: ");
    Serial.print(displ_y);
    Serial.print("\n\n\n");
    Serial.print ("   Nível Polegar:   ");
    Serial.print (flexao(POLEGAR));
    Serial.print ("   Nível Indicador:   ");
    Serial.print (flexao(INDICADOR));
    Serial.print ("   Nível Médio:   ");
    Serial.print (flexao(MEDIO));
    Serial.print ("   Nível Anelar:   ");
    Serial.print (flexao(ANELAR));
    Serial.print ("   Nível Mínimo:   ");
    Serial.print(flexao(MINIMO));
    Serial.print("\n\n\n   Contato: ");
    Serial.print(Contato1);
    Serial.print("        \n\n");
    Serial.print(palavra);
    
    if (comando == "Espaço"){
      palavra = String(palavra + " ");
      comando == "";
    }
    else if (comando == "Apagar"){
      if (palavra.length() > 0){
        palavra.remove(palavra.length() - 1);
        Serial.println(palavra);
        SerialBT.println(palavra);
      } 
      comando == "";
    }
    
    if(reps > 20){
      palavra=String(palavra + letra);
      Serial.println(palavra);
      SerialBT.println(palavra);
      reps = 0;
      current_accx = AccX;
      previous_accx = AccX;
      current_accy = AccY;
      previous_accy = AccY;
      current_gyrX = GyrX;
      previous_gyrX = GyrX;
      current_gyrY = GyrY;
      previous_gyrY = GyrY;
      current_angY = AngleY;
      previous_angY = AngleY;
    }
    previous_accx = current_accx;
    current_accx = AccX;
    previous_accy = current_accy;
    current_accy = AccY;
    previous_gyrX = current_gyrX;
    current_gyrX = GyrX;
    previous_gyrY = current_gyrY;
    current_gyrY = GyrY;
    previous_angY = current_angY;
    current_angY = AngleY;
    
    
    var_gyrX = (current_gyrX-previous_gyrX);
    var_gyrY = (current_gyrY-previous_gyrY);
    var_angY = (current_angY-previous_angY);
    displ_x = (current_accx - previous_accx);
    displ_y = (current_accy - previous_accy);

    if (mov_y()){
      if(flexao(MEDIO) == 2){
        if(flexao(INDICADOR)){
          rep("J");
          reps += 5;
        }
        else{
          rep("Z");
          reps += 8;
        }
      }
      else{
        if(flexao(INDICADOR)){
          rep("Ç");
          reps += 5;
        }
        else if(flexao(ANELAR)){
          rep("H");
          reps+=5;
        }
      }
    }
    else if (Botao)
      rep ("R");
    else if (flexao(MEDIO)){
      if (flexao(INDICADOR)){
        if (flexao(MINIMO)){
          if (flexao(POLEGAR)){
            if(flexao(ANELAR)==2){
              if(flexao(POLEGAR)==2)
                rep("S");
              else if (movimentando()){
                rep("X");
                reps += 6;
              }
              else
                rep("E");
            }
            else{
              rep("O");
            }
          }
          else{        
            if(flexao(INDICADOR)==2)
              rep("A");
            else
              rep("C");
          }  
        }
        else{
          if (flexao(POLEGAR)){
              rep("I");
          }
          else{
            rep("Y");
          }
        }
      }
      else{
        if (flexao(MEDIO)==2){
          if(flexao(POLEGAR)){
            if (apont_cima()){
              rep("G");
            }
            else{
              rep("Q");
            }
          }
          else
            rep("L");
        }
        else {//
          if (flexao(MEDIO)==1){
              rep("D");
          }
        }
      }
    }

    else{
      if (flexao(ANELAR)){
        if (apont_baixo()){ // Mão para baixo.
          rep("N");
        }
        else {
          if (Contato1 > VALOR_MIN_CONTATO){
            rep("U");
          }
          else{
            if(flexao(POLEGAR))
              rep ("V");
            else{
              if(apont_cima()){
                rep("K");
                reps+=4;
              }
              else
                rep("P");
            }
          }
        }
      }
      else{
        if (!flexao(MINIMO)){
          if (flexao(POLEGAR)){ 
            if (!flexao(INDICADOR)){
              rep("B");
            }
            else{
              rep("T");
            }
          }
          else{ 
            if(flexao(INDICADOR))
              rep("F");
          }
        }
        else{
          if (apont_cima()){ // Mão para cima.
            rep("W");
          }
          else if (apont_baixo()){
            rep("M");
          }
        }
      }
    }
  }

  delay(100);
}
