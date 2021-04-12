
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
uint8_t cor_vermelho=0,cor_azul=0,cor_verde=15;
 
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
  int x=0,y=0,contagem=0;
  
  // variaveis tempo
  
  // variaveis eprom
  int address = 0;
  byte value;
  
  // variaveis de contagem de ciclo

// === --- Configurações Iniciais --- ==============================================
  // --- Configurações Iniciais ---
  void setup() {
      Serial.begin(9600);
      pinMode(bt1, INPUT_PULLUP); // SETA ENTRAR
      pinMode(bt2, INPUT_PULLUP); // SETA DESCER
      pinMode(bt3, INPUT_PULLUP); // X

      matrix.begin();

      Serial.println("V-103-KEY-RADIO-SERVER-COLOR-RGBmatrixpainel:");
      Serial.println("ACENDE O LED 1,1 POR 1 SEGUNDO");

      matrix.setTextColor(matrix.Color333(cor_vermelho,cor_azul,cor_verde));
      matrix.drawPixel(1, 1, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
      delay(1000);
      matrix.fillScreen(0);
      delay(1000);

  } //end setup

// === --- Loop Infinito --- =======================================================
void loop() {
      readKey();
      delay(100);  
}

// === readKey =====================================================================
    void readKey() {
        static boolean flag1 = 0, flag2 = 0, flag3 = 0; // teste local
        /* teste estado botão
            Se botao alto testar se foi a primeira vez.
            Use flag3 para saber se houve mudança de estado se sim flag3 ==1,
            se nao houve mudança de estado flag3==0

            Se botao baixo teste se foi a primeira vez.
            Use flag1 para saber se houve mudança de estado se sim flag1 ==1,
            se nao houve mudança de estado flag1==0 

            Logo, existirá sempre a comparação se ouve mudança de alto para baixo
            por isso usa-se o flag4 como confirmador de mudança de estado. 

            Essa solução busca contornar o uso de interrupção devido a conflitos
            com outras bibliotecas aplicadas em outros projetos, busco padronizar
            a leitura de botões com essa solução.*/
        if (digitalRead(bt1)) {
            if (flag4 == 1) {
                Serial.println("SETA SUBIR!  "); // debug via serial
                matrix.fillScreen(0); // limpa tela pra rescreveer
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
                matrix.fillScreen(0);
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
                matrix.fillScreen(0);
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

      // função movimento seta 
    for (int x = 8; x >= 0; x=x-1)
    {

       //matrix.drawPixel(X, Y, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
      int    buf_verde = cor_verde;
      int    buf_verm = cor_vermelho;
              cor_vermelho=buf_verde;
              cor_verde = buf_verm;
        //----seta lado direito----
      // desenhando ponto a ponto biblioteca bruta

      //---- seta lado esquerdo----
        for (int i = 0; i < 7; ++i)
         {
          matrix.drawPixel(x+2+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
          matrix.drawPixel(x+3+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
          matrix.drawPixel(x+4+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
         }

       for (int i = 0; i < 11; ++i)
       {
          matrix.drawPixel(x+4+i, y+8, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
          matrix.drawPixel(x+4+i, y+9, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
       }
        cor_vermelho=buf_verm;
        cor_verde = buf_verde;
            for (int i = 0; i < 7; ++i)
            {
                matrix.drawPixel(x+2+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
                matrix.drawPixel(x+3+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
                matrix.drawPixel(x+4+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
            }
          for (int i = 0; i < 11; ++i)
          {
   
              matrix.drawPixel(x+4+i, y+6, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+4+i, y+7, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
          }


      delay(50);
      if (x==7||x==5||x==3||x==2)
      {
        matrix.fillScreen(0);
      }

    }
      
      delay(10);
    
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
      int    buf_verde = cor_verde;
      int    buf_verm = cor_vermelho;
            cor_vermelho=buf_verde;
            cor_verde = buf_verm;
        //----seta lado direito----
      // desenhando ponto a ponto biblioteca bruta
            for (int i = 0; i < 7; ++i)
            {
              matrix.drawPixel(x+11-i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+12-i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+13-i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+14-i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+15-i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
            }
            for (int i = 0; i < 7; ++i)
            {
              matrix.drawPixel(x+14+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+15+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+16+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+17+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+18+i, y+7-i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
            }
      cor_verde = buf_verde;
      cor_vermelho = buf_verm;
            for (int i = 0; i < 7; ++i)
            {
              matrix.drawPixel(x+14+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+15+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+16+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+17+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+18+i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
            }
            for (int i = 0; i < 7; ++i)
            {
              matrix.drawPixel(x+11-i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+12-i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+13-i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+14-i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
              matrix.drawPixel(x+15-i, y+8+i, matrix.Color333(cor_vermelho,cor_azul,cor_verde));
            }

      cor_verde = buf_verde;
      cor_vermelho = buf_verm;
    
  }// fim x


// === incrementa ==================================================================
      void incrementa() {
        //matrix.fillScreen(0);
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