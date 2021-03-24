/* === APRESENTAÇÃO: ===============================================================
  Projeto: Display BALANÇA RODOVIARIA
    CONTROLADOR SERVIDOR
    |- 6 DISPLAYS
    |- SALVA VALOR EEPROM
    |- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
    |- CANAL 1  
    |- CANAL 2  
    |- CANAL 3  

    CONTROLADOR CLIENTE
    |- 3 DISPLAYS
    |- SALVA VALOR EEPROM
    |- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
    |- CANAL 1  
    |- CANAL 2  
    |- CANAL 3  

Sentido entrada A para B  
A apresenta -> 
B apresenta X
Parada dentro balança
A e B apresenta X
Sentido saida A para B  depois de dentro da balança  
A apresenta -> 
B apresenta X

Sentido entrada B para A  
A apresenta X 
B apresenta <-
Parada dentro balança
A e B apresenta X
Sentido saida B para A  depois de dentro da balança  
A apresenta X 
B apresenta <-
  A|___________________________B|
/                               \

  Descrição do projeto:
     Servidor com 1 displays e central de rádio com 3 canais.
     Cliente com 1 displays recebe comandos do servidor. 
     Seleção do digito via rádio.
     Adaptado central de rádio de portão eletrônico.


  Autor: Eng. Stenio Rodrigues
  Data:  Março de 2021

      OBJETIVO:
      NOTA:
      // SERVIDOR E CLIENTE ENTRADAS
      19  
      20  

      // SERVIDOR SAIDAS
      21  

      // CONECTOR DISPLAY
      ________________________
      |1 pin_noe 2  pin_a     |
      |3  GND    4  pin_b     |
      |5  GND    6  N/C       |
      |7  GND    8  pin_sck  S|
      |9  GND    10 pin_clk  L|
      |11 GND    12 pin_r_datR|
      ________________________

      // PAINEL           MEGA
      //byte panelsWide   N/C
      //byte panelsHigh   N/C
      //byte pin_noe      9
      //byte GND          GND
      //byte pin_a        6
      //byte pin_b        7
      //byte pin_sck      52
      //byte pin_clk      8
      //byte pin_r_data   51

      // PAINEL           UNO
      //byte panelsWide   N/C
      //byte panelsHigh   N/C
      //byte pin_noe      9
      //byte GND          GND
      //byte pin_a        6
      //byte pin_b        7
      //byte pin_sck      12
      //byte pin_clk      8
      //byte pin_r_data   11
      ///////////////////////
 
*/
// === --- Bibliotecas Auxiliares --- ==============================================
  // --- Bibliotecas Auxiliares ---
  #include <SPI.h>
  #include <DMD2.h>
  #include "Arial_Black_16.h"
  #include "SystemFont5x7.h" 
  #include <EEPROM.h>

// === --- Mapeamento de Hardware --- ==============================================
  // --- Mapeamento de Hardware ---
  // definindo botoes

// === --- Instâncias --- ==========================================================
  // --- Instâncias ---
  int QDISPLAY = 1;
  //SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 52, 51); // MEGA DMD controls the entire display
  SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 12, 11); // UNO

// === --- Protótipo das Funções --- ===============================================
  // --- Protótipo das Funções ---
  //void readKey(); // LÊ COMANDOS INCREMENTOS E DIGITO
  void seta_subir();  // DESENHA SETA SENTIDO SUBIR NO DISPLAY
  void seta_descer(); // DESENHA SETA SENTIDO DESCER NO DISPLAY
  void pare_X();      // DESENHA X NO DISPLAY 
  void incrementa(); // INCREMENTA valor EEPROM
  void imprimir(); // IMPRIMI NO DISPLAY O VALOR SALVO NA EEPROM


// === --- Variáveis Globais --- ===================================================
  // --- Variáveis Globais ---
  
  // definindo variaiveis marcadoras de status botoes
  
  // variaveis marcadores de digitos
  int pos_dig = 0;
  int dig_0 = 0;
  int dig_1 = 0;
  int dig_2 = 0;
  int dig_3 = 0;
  
  // variaveis tempo
  
  // variaveis eprom
  int address = 0;
  byte value;
  
  // variaveis de contagem de ciclo
  
// === --- Configurações Iniciais --- ==============================================
  // --- Configurações Iniciais ---
  void setup() {
      Serial.begin(9600);
      dmd.setBrightness(250);
      dmd.selectFont (Arial_Black_16);   
      dmd.begin();

      Serial.println("V-103-VARREDURA-UNO-DMD2:");
 
      pinMode(LED_BUILTIN, OUTPUT);
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      delay(1000);                       // wait for a second
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      delay(1000); 
      digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
      dmd.fillScreen(true);
      delay(300);
      dmd.clearScreen();
  } //end setup

// === --- Loop Infinito --- =======================================================
  // --- Loop Infinito ---
  void loop() {
    /*
        for (int y = 0; y <= 15; y++) {
            for (int x = 0; x <= 31; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON);
                delay(50);
            }
        }
    */
      delay(1000);
      dmd.clearScreen();
      delay(500);
      seta_subir();
      delay(1000);
      dmd.clearScreen();
      delay(500);
      seta_descer();
      delay(1000);
      dmd.clearScreen();
      delay(500);
      pare_X();
      delay(1000);
      dmd.clearScreen();
      incrementa(); 
      delay(500);


      
  } //end loop
// === seta_subir =============================================================
 
  void seta_subir(){
    //dmd.drawLine (x1, y1, x2, y2, GRAPHICS_NORMAL);
        //----seta lado direito----
      dmd.drawLine (2, 7, 3, 6, GRAPHICS_ON);
      dmd.drawLine (4, 5, 5, 4, GRAPHICS_ON);
      dmd.drawLine (6, 3, 7, 2, GRAPHICS_ON);
      dmd.drawLine (8, 1, 8, 1, GRAPHICS_ON);
    // cada linha no desenho aparece 3 linhas
      dmd.drawLine (3, 7, 4, 6, GRAPHICS_ON);
      dmd.drawLine (5, 5, 6, 4, GRAPHICS_ON);
      dmd.drawLine (7, 3, 8, 2, GRAPHICS_ON);
      dmd.drawLine (9, 1, 9, 1, GRAPHICS_ON);
    
      dmd.drawLine (4, 7, 5, 6, GRAPHICS_ON);
      dmd.drawLine (6, 5, 7, 4, GRAPHICS_ON);
      dmd.drawLine (8, 3, 9, 2, GRAPHICS_ON);
      dmd.drawLine (10, 1, 10, 1, GRAPHICS_ON);
      //---- fim seta lado direito----
      //---- seta lado esquerdo----
      dmd.drawLine (2, 8, 3, 9, GRAPHICS_ON);
      dmd.drawLine (4, 10, 5, 11, GRAPHICS_ON);
      dmd.drawLine (6, 12, 7, 13, GRAPHICS_ON);
      dmd.drawLine (8, 14, 8, 14, GRAPHICS_ON);
        
      dmd.drawLine (3, 8, 4, 9, GRAPHICS_ON);
      dmd.drawLine (5, 10, 6, 11, GRAPHICS_ON);
      dmd.drawLine (7, 12, 8, 13, GRAPHICS_ON);
      dmd.drawLine (9, 14, 9, 14, GRAPHICS_ON);
        
      dmd.drawLine (4, 8, 5, 9, GRAPHICS_ON);
      dmd.drawLine (6, 10, 7, 11, GRAPHICS_ON);
      dmd.drawLine (8, 12, 9, 13, GRAPHICS_ON);
      dmd.drawLine (10, 14, 10, 14, GRAPHICS_ON);
        //---- fim seta lado esquerdo----
    
        // ---- corpo ------
      dmd.drawLine (4, 6, 4, 7, GRAPHICS_ON);
      dmd.drawLine (4, 8, 4, 9, GRAPHICS_ON);
      dmd.drawLine (5, 6, 5, 7, GRAPHICS_ON);
      dmd.drawLine (5, 8, 5, 9, GRAPHICS_ON);
      dmd.drawLine (6, 6, 6, 7, GRAPHICS_ON);
      dmd.drawLine (6, 8, 6, 9, GRAPHICS_ON);
      dmd.drawLine (7, 6, 7, 7, GRAPHICS_ON);
      dmd.drawLine (7, 8, 7, 9, GRAPHICS_ON);  
      dmd.drawLine (8, 6, 8, 7, GRAPHICS_ON);
      dmd.drawLine (8, 8, 8, 9, GRAPHICS_ON);
      dmd.drawLine (9, 6, 9, 7, GRAPHICS_ON);
      dmd.drawLine (9, 8, 9, 9, GRAPHICS_ON); 
      dmd.drawLine (10, 6, 10, 7, GRAPHICS_ON);
      dmd.drawLine (10, 8, 10, 9, GRAPHICS_ON);
      dmd.drawLine (11, 6, 11, 7, GRAPHICS_ON);
      dmd.drawLine (11, 8, 11, 9, GRAPHICS_ON);   
      dmd.drawLine (12, 6, 12, 7, GRAPHICS_ON);
      dmd.drawLine (12, 8, 12, 9, GRAPHICS_ON);
      dmd.drawLine (13, 6, 13, 7, GRAPHICS_ON);
      dmd.drawLine (13, 8, 13, 9, GRAPHICS_ON); 
      dmd.drawLine (14, 6, 14, 7, GRAPHICS_ON);
      dmd.drawLine (14, 8, 14, 9, GRAPHICS_ON);
      dmd.drawLine (15, 6, 15, 7, GRAPHICS_ON);
      dmd.drawLine (15, 8, 15, 9, GRAPHICS_ON);
      delay(1);
    
  }// fim seta subir

// === seta_descer =============================================================
  void seta_descer(){
  
  
    //----seta lado direito----
      dmd.drawLine (23, 1, 24, 2, GRAPHICS_ON);
      dmd.drawLine (25, 3, 26, 4, GRAPHICS_ON);
      dmd.drawLine (27, 5, 28, 6, GRAPHICS_ON);
      dmd.drawLine (29, 7, 29, 7, GRAPHICS_ON);
    // cada linha no desenho aparece 3 linhas
      dmd.drawLine (24, 1, 25, 2, GRAPHICS_ON);
      dmd.drawLine (26, 3, 27, 4, GRAPHICS_ON);
      dmd.drawLine (28, 5, 29, 6, GRAPHICS_ON);
      dmd.drawLine (30, 7, 30, 7, GRAPHICS_ON);
    
      dmd.drawLine (25, 1, 26, 2, GRAPHICS_ON);
      dmd.drawLine (27, 3, 28, 4, GRAPHICS_ON);
      dmd.drawLine (29, 5, 30, 6, GRAPHICS_ON);
      dmd.drawLine (31, 7, 31, 7, GRAPHICS_ON);
      //---- fim seta lado direito----
      //---- seta lado esquerdo----
      dmd.drawLine (23, 14, 24, 13, GRAPHICS_ON);
      dmd.drawLine (25, 12, 26, 11, GRAPHICS_ON);
      dmd.drawLine (27, 10, 28, 9, GRAPHICS_ON);
      dmd.drawLine (29, 8, 29, 8, GRAPHICS_ON); 
    // cada linha no desenho aparece 3 linhas
      dmd.drawLine (24, 14, 25, 13, GRAPHICS_ON);
      dmd.drawLine (26, 12, 27, 11, GRAPHICS_ON);
      dmd.drawLine (28, 10, 29, 9, GRAPHICS_ON);
      dmd.drawLine (30, 8, 30, 8, GRAPHICS_ON); 
    
      dmd.drawLine (25, 14, 26, 13, GRAPHICS_ON);
      dmd.drawLine (27, 12, 28, 11, GRAPHICS_ON);
      dmd.drawLine (29, 10, 30, 9, GRAPHICS_ON);
      dmd.drawLine (31, 8, 31, 8, GRAPHICS_ON);
        //---- fim seta lado esquerdo----
    
        // ---- corpo ------
      dmd.drawLine (18, 6, 18, 7, GRAPHICS_ON);
      dmd.drawLine (18, 8, 18, 9, GRAPHICS_ON);
      dmd.drawLine (19, 6, 19, 7, GRAPHICS_ON);
      dmd.drawLine (19, 8, 19, 9, GRAPHICS_ON);  
      dmd.drawLine (20, 6, 20, 7, GRAPHICS_ON);
      dmd.drawLine (20, 8, 20, 9, GRAPHICS_ON);
      dmd.drawLine (21, 6, 21, 7, GRAPHICS_ON);
      dmd.drawLine (21, 8, 21, 9, GRAPHICS_ON); 
      dmd.drawLine (22, 6, 22, 7, GRAPHICS_ON);
      dmd.drawLine (22, 8, 22, 9, GRAPHICS_ON);
      dmd.drawLine (23, 6, 23, 7, GRAPHICS_ON);
      dmd.drawLine (23, 8, 23, 9, GRAPHICS_ON);   
      dmd.drawLine (24, 6, 24, 7, GRAPHICS_ON);
      dmd.drawLine (24, 8, 24, 9, GRAPHICS_ON);
      dmd.drawLine (25, 6, 25, 7, GRAPHICS_ON);
      dmd.drawLine (25, 8, 25, 9, GRAPHICS_ON); 
      dmd.drawLine (26, 6, 26, 7, GRAPHICS_ON);
      dmd.drawLine (26, 8, 26, 9, GRAPHICS_ON);
      dmd.drawLine (27, 6, 27, 7, GRAPHICS_ON);
      dmd.drawLine (27, 8, 27, 9, GRAPHICS_ON);
      dmd.drawLine (28, 6, 28, 7, GRAPHICS_ON);
      dmd.drawLine (28, 8, 28, 9, GRAPHICS_ON);

  }// fim seta descer

// === pare_X =============================================================
  void pare_X(){
    dmd.drawChar (12, 1, 'X', GRAPHICS_ON);
  }// fim x

// === incrementa ==================================================================
      void incrementa() {
        //dmd.clearScreen();
        //delay(10);

        if (pos_dig == 0) {
            dig_0 = dig_0 + 1;
            EEPROM.write(0, dig_0);
            if (dig_0==1)
            {
             dmd.drawChar (12, 1, '1', GRAPHICS_ON);
            }
            if (dig_0==2)
            {
             dmd.drawChar (12, 1, '2', GRAPHICS_ON);
            }
            if (dig_0==3)
            {
             dmd.drawChar (12, 1, '3', GRAPHICS_ON);
             pos_dig = 1;
            }
        }
        if (dig_0 > 9) {
            dig_0 = 0;
            EEPROM.write(0, dig_0);
        }
        Serial.println("");


    }
// === imprimir ====================================================================
  void imprimir() {
      Serial.println(" ");
      Serial.print("DADOS DENTRO DA EEPROM: ");
      for (int i = 0; i <= 3; i++) {
          value = EEPROM.read(i);
          Serial.print(value);
      }
      // ENDEREÇO REAL
      if (EEPROM.read(0) == 0) {

      }
       

  }