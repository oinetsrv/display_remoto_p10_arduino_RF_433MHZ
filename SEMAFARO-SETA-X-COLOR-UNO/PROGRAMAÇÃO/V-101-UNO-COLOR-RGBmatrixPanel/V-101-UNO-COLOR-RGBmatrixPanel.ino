
/* === APRESENTAÇÃO: ===============================================================
  Projeto: Display BALANÇA RODOVIARIA
    CONTROLADOR SERVIDOR
    |- 2 DISPLAYS
    |- 
    |- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
    |- CANAL 1  
    |- CANAL 2  
    |- CANAL 3  

    CONTROLADOR CLIENTE
    |- 1 DISPLAYS
    |- 
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
     Servidor com 1 displays e central de rádio com 1 canais.
     Cliente com 1 displays recebe comandos do servidor. 
     Seleção do digito via rádio.
     Adaptado central de rádio de portão eletrônico.


  Autor: Eng. Stenio Rodrigues
  Data:  Março de 2021

      OBJETIVO:
      NOTA:
      // SERVIDOR E CLIENTE ENTRADAS
  #define bt1 11 // SUBIR
  #define bt2 12 // DESCER
  #define bt3 13 // X

      // SERVIDOR SAIDAS
  #define bt1 11 // SUBIR
  #define bt2 12 // DESCER
  #define bt3 13 // X
// CONEXÃO DISPLAY CONTROLADOR
      // display p10-246-RGB4S-32X16(5120)1,0 V3.95   
      // display p10-3216-3528-8S-A0   
PINO DISPLAY    UNO      MEGA
1       RD1       2       24
2       GD1       3       25
3       BD1       4       26  
4       GND       GND     GND 
5       RD2       5       27
6       GD2       6       28
7       BD2       7       29
8       GND       GND     GND
9       A         A0      A0
10      B         A1      A1
11      C         A2      A2
12      GND       GND     GND
13      LAT     10 ou A3  10
14      CLK       8       11
15      OEB       9        9
16      GND       GND     GND


      ________________________
      |1 RD1     2  GD1       |
      |3  BD1    4  GND       |
      |5  RD2    6  GD2       |
      |7  BD2    8  GND       |
      |9  A      10 B         |
      |11 C      12 N/C       |
      |13 CLK    14 LAT       |
      |15 OE     16 GND       |
      ________________________

*/
// === --- Bibliotecas Auxiliares --- ==============================================
  // --- Bibliotecas Auxiliares ---
 #include <RGBmatrixPanel.h>

// === --- Mapeamento de Hardware --- ==============================================
  // --- Mapeamento de Hardware ---
  #define bt1 11 // SUBIR
  #define bt2 12 // DESCER
  #define bt3 13 // X

// === --- Instâncias --- ==========================================================
  // --- Instâncias ---
  #define CLK  8   // USE THIS ON ARDUINO UNO, ADAFRUIT METRO M0, etc.
  //#define CLK A4 // USE THIS ON METRO M4 (not M0)
  //#define CLK 11 // USE THIS ON ARDUINO MEGA
  #define OE   9
  #define LAT 10
  #define A   A0
  #define B   A1
  #define C   A2

RGBmatrixPanel matrix(A, B, C, CLK, LAT, OE, false);

// === --- Protótipo das Funções --- ===============================================
  // --- Protótipo das Funções ---
  void seta_subir();  // DESENHA SETA SENTIDO SUBIR NO DISPLAY
  void seta_descer(); // DESENHA SETA SENTIDO DESCER NO DISPLAY
  void pare_X();      // DESENHA X NO DISPLAY 
  void incrementa(); // INCREMENTA valor EEPROM
  void imprimir(); // IMPRIMI NO DISPLAY O VALOR SALVO NA EEPROM
  void readKey(); // LÊ COMANDOS INCREMENTOS E DIGITO


// === --- Variáveis Globais --- ===================================================
  // --- Variáveis Globais ---
uint8_t cor_vermelho=15,cor_azul=0,cor_verde=0;
 
// definindo variaiveis marcadoras de status botoes
    unsigned int duty1 = 1;
    static boolean flag4 = 0,
    flag5 = 0,
    flag6 = 0;
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
      matrix.begin();

      Serial.println("V-101-UNO-COLOR-RGBmatrixpainel:");
      
 
      matrix.setTextColor(matrix.Color333(cor_vermelho,cor_azul,cor_verde));
      matrix.drawPixel(1, 1, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
      delay(1000);
      matrix.fillScreen(0);
      delay(1000);
      matrix.fillScreen(15);
      delay(1000);


  uint8_t r=0, g=0, b=0;


  } //end setup

// === --- Loop Infinito --- =======================================================
void loop() {
  // Do nothing -- image doesn't change
}


// === readKey =====================================================================
    void readKey() {

        static boolean flag1 = 0, flag2 = 0, flag3 = 0; // teste local
        /* teste estado botao

            Se botao alto teste se foi a primeira vez
            use flag 3 para saber se houve mudança de estado se sim flag3 ==1
            se nao houve mudança de estado flag3==0

            Se botao baixo teste se foi a primeira vez
            use flag1 para saber se houve mudança de estado se sim flag1 ==1
            se nao houve mudança de estado flag1==0 
        */
        if (digitalRead(bt1)) {
            if (flag4 == 1) {
                Serial.println("SETA SUBIR!  "); 
               // dmd.clearScreen(); limpa tela
                delay(25);
                seta_subir();
            }
            flag1 = 0x01;
            flag4 = 0x00; // teste memoria de outros testes
            delay(10);
        }
        if (!digitalRead(bt1) && flag1) {
            flag1 = 0x00;
            flag4 = 0x01;
            Serial.println("RESET FLAG BOTAO SETA SUBIR");
            delay(10);
        }

        if (digitalRead(bt2)) {
            if (flag5 == 1) {
                Serial.println("SETA DESCER!  "); 
                // dmd.clearScreen(); limpa tela
                delay(25);
               // seta_descer();
                pare_X();
            }
            flag2 = 0x01;
            flag5 = 0x00; // teste memoria de outros testes
            delay(10);
        }
        if (!digitalRead(bt2) && flag2) {
            flag2 = 0x00;
            flag5 = 0x01;
            Serial.println("RESET FLAG BOTAO SETA DESCER");
            delay(10);
        }

        if (digitalRead(bt3)) {
            if (flag6 == 1) {
                Serial.println("PARE_X!  "); 
               // dmd.clearScreen(); limpa tela
                delay(25);
                pare_X();
            }
            flag3 = 0x01;
            flag6 = 0x00; // teste memoria de outros testes
            delay(10);
        }
        if (!digitalRead(bt3) && flag3) {
            flag3 = 0x00;
            flag6 = 0x01;
            Serial.println("RESET FLAG BOTAO PARE_X");
            delay(10);
        }
    } // readKey

 
// === seta_subir =============================================================
 
  void seta_subir(){
    //dmd.drawLine (x1, y1, x2, y2, GRAPHICS_NORMAL);
    /*
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
    */
  }// fim seta subir


// === seta_descer =============================================================
  void seta_descer(){
  
 /* 
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
*/
  }// fim seta descer

// === pare_X =============================================================
  void pare_X(){
    //dmd.drawChar (12, 1, 'X', GRAPHICS_ON);
  }// fim x


// === incrementa ==================================================================
      void incrementa() {
        //dmd.clearScreen();
        //delay(10);

        if (pos_dig == 0) {
            dig_0 = dig_0 + 1;
            //EEPROM.write(0, dig_0);

        }
        if (dig_0 > 3) {
            dig_0 = 0;
           // EEPROM.write(0, dig_0);
        }
        Serial.println("");


    }


// === imprimir ====================================================================
  void imprimir() {
      Serial.println(" ");
      Serial.print("DADOS DENTRO DA EEPROM: ");
  

  }