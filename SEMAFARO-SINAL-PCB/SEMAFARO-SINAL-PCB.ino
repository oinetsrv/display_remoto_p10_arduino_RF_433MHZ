/* === APRESENTAÇÃO: ===============================================================
Projeto: Display BALANÇA RODOVIÁRIA
  CONTROLADOR SERVIDOR
    |- 1 SEMÁFARO
    |- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
    |- CANAL 1  
    |- CANAL 2  
    |- CANAL 3  

  CONTROLADOR CLIENTE
    |- 2 SEMÁFARO
    |- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
    |- CANAL 1  
    |- CANAL 2  
    |- CANAL 3 

Sentido A para B  
B apresenta ->  VERDE
A apresenta X   VERMELHO
Parada dentro balança
A e B apres X   VERMELHO
Sentido saida A para B  depois de dentro da balança  
B apresenta ->  VERDE
A apresenta X   VERMELHO

Sentido B para A  
B apresenta X   VERDE
A apresenta <-  VERMELHO
Parada dentro balança
A e B apres X   VERMELHO
Sentido saida B para A  depois de dentro da balança  
B apresenta X   VERMELHO
A apresenta <-  VERDE
  A|___________________________B|
/                               \

  Descrição do projeto:
     Servidor com 1 semafáro e central de rádio com 3 canais.
     Cliente com 1 semáfaro recebe comandos do servidor. 
     Seleção do digito via rádio.
     Adaptado central de rádio de portão eletrônico.


  Autor: Eng. Stenio Rodrigues
  Data:  Abril de 2021 
  versão: 1.0   2 Displays  LED 16x32 + 2 arduinos + radio 3 vias
          1.1   2 Displays  RGB 16x32 + 2 arduinos + radio 3 vias
          1.2   2 Semáforos 35 leds   + 1 PCB+328P + radio 3 vias

      OBJETIVO:
      NOTA:
      // SERVIDOR E CLIENTE ENTRADAS
      bt1  // ENTRADA VERDE SAIDA VERMELHO
      bt2  // ENTRADA VERMELHO SAIDA VERDE
      bt3  // VERNELHO GERAL
   2X bt3  // VERDE GERAL
   3X bt3  // DESLIGADO

*/
// === --- Bibliotecas Auxiliares --- ==============================================
  // --- Bibliotecas Auxiliares ---

// === --- Mapeamento de Hardware --- ==============================================
  // --- Mapeamento de Hardware ---
  // definindo botoes
  #define bt1 5 // SUBIR
  #define bt2 6 // DESCER
  #define bt3 7 // X
  
  #define VM2 10 // VERMELHO 2
  #define VM1 11 // VERMELHO 1
  #define VD2 12 // VERDE-2
  #define VD1 13 // VERDE-1

// === --- Instâncias --- ==========================================================
  // --- Instâncias ---

// === --- Protótipo das Funções --- ===============================================
  // --- Protótipo das Funções ---
  void livre_V();     // acende todos leds verdes
  void seta_subir();  // acende leds vm1 e vd 2
  void seta_descer(); // acende leds vm2 e vd 1
  void pare_X();     // acende todos leds vermelhos
  void off_X();     // acende todos leds vermelhos

// === --- Variáveis Globais --- ===================================================
  // --- Variáveis Globais ---
  int cont=0; // vairiavel que conta quantas vezes precionado botao 3
  // definindo variaiveis marcadoras de status botoes
    unsigned int duty1 = 1;
    static boolean flag4 = 0, // Variáveis globais que memorizam mudança de estado
    flag5 = 0,
    flag6 = 0;                // dos canais de entrada da central de rádio
  // variaveis marcadores de digitos
  int pos_dig = 0;
  int dig_0 = 0;
  
  // variaveis tempo
  
  // variaveis eprom
 
  // variaveis de contagem de ciclo
  
// === --- Configurações Iniciais --- ==============================================
  // --- Configurações Iniciais ---
  void setup() {
      Serial.begin(9600);
       
      Serial.println("V-SEMAFARO-SINAL-PCB-RADIO-3-CANAIS:");
      Serial.print("NOME ARQUIVO:  ");
      Serial.print("SEMAFARO-SINAL-PCB");
      Serial.println(" ");

      pinMode(bt1, INPUT_PULLUP); // SETA SUBIR
      pinMode(bt2, INPUT_PULLUP); // SETA DESCER
      pinMode(bt3, INPUT_PULLUP); // X

      pinMode(VM2, OUTPUT); // 
      pinMode(VM1, OUTPUT); // 
      pinMode(VD1, OUTPUT); // 
      pinMode(VD2, OUTPUT); // 

      digitalWrite(VM2, LOW);
      digitalWrite(VM1, LOW);
      digitalWrite(VD1, LOW);
      digitalWrite(VD2, LOW);
        delay(100); 
                            // wait for a second
      
  } //end setup

// === --- Loop Infinito --- =======================================================
  // --- Loop Infinito ---
  void loop() {
      readKey();
      delay(1000);  
      seta_subir();
      delay(1000); 
      seta_descer();
      delay(1000); 
  } //end loop

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
                delay(25);
                seta_subir();
                cont = 0;
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
                delay(25);
                seta_descer();
                cont = 0;
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
                delay(25);
                pare_X();
                cont=cont+1;
            }
            if (flag6 == 1 && cont >= 2) {
                Serial.println("livre_V!  "); 
                delay(25);
                livre_V();
                cont=0;
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
    delay(1);
      digitalWrite(VM2, LOW);
      digitalWrite(VM1, HIGH);
      digitalWrite(VD1, LOW);
      digitalWrite(VD2, HIGH);
      delay(100);

  }// fim seta subir

// === seta_descer =============================================================
  void seta_descer(){
   delay(1);
      digitalWrite(VM2, HIGH);
      digitalWrite(VM1, LOW);
      digitalWrite(VD1, HIGH);
      digitalWrite(VD2, LOW);
      delay(100);

  }// fim seta descer

// === pare_X =============================================================
  void pare_X(){
   delay(1);
      digitalWrite(VM2, HIGH);
      digitalWrite(VM1, HIGH);
      digitalWrite(VD1, LOW);
      digitalWrite(VD2, LOW);
      delay(100);
  }// fim pare_X
// === pare_X =============================================================
  void livre_V(){
   delay(1);
      digitalWrite(VM2, LOW);
      digitalWrite(VM1, LOW);
      digitalWrite(VD1, HIGH);
      digitalWrite(VD2, HIGH);
      delay(100);
  }// fim livre_V
  // === off_X =============================================================
  void off_X(){
   delay(1);
      digitalWrite(VM2, LOW);
      digitalWrite(VM1, LOW);
      digitalWrite(VD1, LOW);
      digitalWrite(VD2, LOW);
      delay(100);
  }// fim off_X

