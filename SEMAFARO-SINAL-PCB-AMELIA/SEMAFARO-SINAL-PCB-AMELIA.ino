/* === APRESENTAÇÃO: ===============================================================
Projeto: Semáforo BALANÇA RODOVIÁRIA 4 unidades
  CONTROLADOR SERVIDOR
    |- 1 SEMÁFARO
    |- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
    |- CANAL 1  
    |- CANAL 2  
    |- CANAL 3  

  Descrição do projeto:
     Semáforo e central de rádio com 3 canais.
     Adaptado central de rádio de portão eletrônico.
     Funções implementadas:
     Ao pressionar botão 1:
     Aciona sinal verde (A), sinal Vermelho (b),(B)e(a)
     Ao pressionar botão 1 duas vezes seguidas.
     Aciona sinal verde (a), sinal Vermelho (b)e(B)e(A)
     Ao pressionar botão 1 a terceira vez repete o ciclo.
     ____________________________________________________

     Ao pressionar botão 3:
     Aciona sinal verde (B), sinal Vermelho (a),(A)e(b)
     Ao pressionar botão 1 duas vezes seguidas.
     Aciona sinal verde (b), sinal Vermelho (a)e(A)e(B)
     Ao pressionar botão 1 a terceira vez repete o ciclo.
     ____________________________________________________

     Ao pressionar botão 2:
     Aciona sinal Vermelho (A),(a),(B),(b)
     Ao pressionar botão 2 duas vezes seguidas.
     Desliga todos os led


  A|b___________________________a|B
/                               \



  Autor: Eng. Stenio Rodrigues
  Data:  Agosto de 2021 
  versão: 1.0   2 Displays  LED 16x32 + 2 arduinos + radio 3 vias
          1.1   2 Displays  RGB 16x32 + 2 arduinos + radio 3 vias
          1.2   2 Semáforos 35 leds   + 1 PCB+328P + radio 3 vias
          1.3   4 semáforos 16 leds   + 1 PCB+328P + radio 3 vias

*/
// === --- Bibliotecas Auxiliares --- ==============================================
// --- Bibliotecas Auxiliares ---

// === --- Mapeamento de Hardware --- ==============================================
// --- Mapeamento de Hardware ---
// definindo botoes
#define bt1 5 //  
#define bt2 6 //  
#define bt3 7 //  

#define VM_ext_SAI 3 // VERDE-3
#define VM_ext_ENT 4 // VERDE-4
#define VD_ext_SAI 8 // VERDE-3
#define VD_ext_ENT 9 // VERDE-4
#define VM_int_SAI 10 // VERMELHO 2
#define VM_int_ENT 11 // vermelho interno entrada
#define VD_int_ENT 12 // VERDE-2
#define VD_int_SAI 13 // VERDE-1

// === --- Instâncias --- ==========================================================
// --- Instâncias ---

// === --- Variáveis Globais --- ===================================================
// --- Variáveis Globais ---
int entrada_1 = 0;

// definindo variaiveis marcadoras de status botoes
unsigned int duty1 = 1;

static boolean  flag1 = 0,
                flag2 = 0,
                flag3 = 0; // Variáveis globais que memorizam mudança de extado
 
// === --- Protótipo das Funções --- ===============================================
// --- Protótipo das Funções ---
void read_bts();
void texta_saidas();
void desliga_tudo();

// === --- Configurações Iniciais --- ==============================================
// --- Configurações Iniciais ---
void setup() {
    Serial.begin  (9600);
    Serial.println("V-SEMAFARO-SINAL-PCB-RADIO-3-CANAIS:");
    Serial.print  ("NOME ARQUIVO:                       ");
    Serial.print  ("SEMAFARO-SINAL-PCB-AMELIA V1.3      ");
    Serial.println("                                    ");

    pinMode(bt1, INPUT_PULLUP );
    pinMode(bt2, INPUT_PULLUP );
    pinMode(bt3, INPUT_PULLUP );

    pinMode(VM_int_SAI, OUTPUT);
    pinMode(VM_int_ENT, OUTPUT);
    pinMode(VD_int_ENT, OUTPUT);
    pinMode(VD_int_SAI, OUTPUT);
    pinMode(VD_ext_SAI, OUTPUT);
    pinMode(VD_ext_ENT, OUTPUT);
    pinMode(VM_ext_SAI, OUTPUT);
    pinMode(VM_ext_ENT, OUTPUT);
    // parte tudo desligado
    digitalWrite(VD_ext_SAI, LOW );
    digitalWrite(VD_ext_ENT, LOW );
    digitalWrite(VM_ext_SAI, LOW );
    digitalWrite(VM_ext_ENT, LOW );
    digitalWrite(VM_int_SAI, HIGH);
    digitalWrite(VM_int_ENT, HIGH);
    digitalWrite(VD_int_ENT, HIGH);
    digitalWrite(VD_int_SAI, HIGH);
    delay(10);
} //end setup

// === --- Loop Infinito                      --- =======================================================
// --- Loop Infinito ---
void loop() {
    read_bts();
    delay(150);

} //end loop
// === --- Desenvolvimento funções auxiliares --- =======================================================
void read_bts() {
    static bool bt1_flag = 0,
                bt2_flag = 0,
                bt3_flag = 0;

    if (!digitalRead(bt1)) bt1_flag = 0x01;
    if (!digitalRead(bt2)) bt2_flag = 0x01;
    if (!digitalRead(bt3)) bt3_flag = 0x01;
    
    if (digitalRead(bt1) && bt1_flag && !flag1) {
        bt1_flag = 0x00;
        flag1 = !flag1; //inverter flag
        desliga_tudo();
        // interno entrada
        digitalWrite(VM_int_ENT, LOW);  // vermelho interno entrada  LIGADO
        digitalWrite(VD_int_ENT, HIGH); // verde interno entrada     desligado
        // externo entrada
        digitalWrite(VM_ext_ENT, LOW);  // vermelho externo entrada desligado
        digitalWrite(VD_ext_ENT, HIGH); // verde externo entrada    LIGADO
        // interno saida
        digitalWrite(VM_int_SAI, LOW); // vermelho interno saida      LIGADO
        digitalWrite(VD_int_SAI, HIGH); // verde interno saida        desligado
        // externo saida
        digitalWrite(VM_ext_SAI, HIGH);  // vermelho externo saida    LIGADO
        digitalWrite(VD_ext_SAI, LOW);  // verde externo saida        desligado
    } //end if
    // segunda vez botão 1
    if (digitalRead(bt1) && bt1_flag && flag1) {
        bt1_flag = 0x00;
        flag1 = !flag1; //inverter flag
        desliga_tudo();
        // interno entrada
        digitalWrite(VM_int_ENT, HIGH);  // vermelho interno entrada  desligado
        digitalWrite(VD_int_ENT, LOW); // verde interno entrada     LIGADO
        // externo entrada
        digitalWrite(VM_ext_ENT, HIGH);  // vermelho externo entrada LIGADO
        digitalWrite(VD_ext_ENT, LOW); // verde externo entrada    desligado
        // interno saida
        digitalWrite(VM_int_SAI, LOW); // vermelho interno saida      LIGADO
        digitalWrite(VD_int_SAI, HIGH); // verde interno saida        desligado
        // externo saida
        digitalWrite(VM_ext_SAI, HIGH);  // vermelho externo saida    LIGADO
        digitalWrite(VD_ext_SAI, LOW);  // verde externo saida        desligado
    } // end if

    if (digitalRead(bt2) && bt2_flag && !flag2) {
        bt2_flag = 0x00;
        flag2 = !flag2; //inverter flag
        flag1 = 0;
        flag3 = 0;
        desliga_tudo();
    } //end if
    if (digitalRead(bt2) && bt2_flag && flag2) {
        bt2_flag = 0x00;
        flag2 = !flag2; //inverter flag
        desliga_tudo();
        // interno entrada
        digitalWrite(VM_int_ENT, LOW);  // vermelho interno entrada   LIGADO
        digitalWrite(VD_int_ENT, HIGH); // verde interno entrada      desligado
        // externo entrada
        digitalWrite(VM_ext_ENT, HIGH);  // vermelho externo entrada  LIGADO
        digitalWrite(VD_ext_ENT, LOW); // verde externo entrada       desligado
        //-----------------------
        // interno saida
        digitalWrite(VM_int_SAI, LOW); // vermelho interno saida      LIGADO
        digitalWrite(VD_int_SAI, HIGH); // verde interno saida        desligado
        // externo saida
        digitalWrite(VM_ext_SAI, HIGH);  // vermelho externo saida    LIGADO
        digitalWrite(VD_ext_SAI, LOW);  // verde externo saida        desligado

          
    } //end if

    if (digitalRead(bt3) && bt3_flag && !flag3) {
        bt3_flag = 0x00;
        flag3 = !flag3; //inverter flag
        desliga_tudo();
        // interno entrada
        digitalWrite(VM_int_ENT, LOW);  // vermelho interno entrada   LIGADO
        digitalWrite(VD_int_ENT, HIGH); // verde interno entrada      desligado
        // externo entrada
        digitalWrite(VM_ext_ENT, HIGH);  // vermelho externo entrada  LIGADO
        digitalWrite(VD_ext_ENT, LOW); // verde externo entrada       desligado
        // interno saida
        digitalWrite(VM_int_SAI, LOW); // vermelho interno saida   LIGADO
        digitalWrite(VD_int_SAI, HIGH); // verde interno saida     desligado
        // externo saida
        digitalWrite(VM_ext_SAI, LOW);  // vermelho externo saida  desligado
        digitalWrite(VD_ext_SAI, HIGH);  // verde externo saida    LIGADO
    } //end if 
    if (digitalRead(bt3) && bt3_flag  && flag3) {
        bt3_flag = 0x00;
        flag3 = !flag3; //inverter flag
        desliga_tudo();
        // interno entrada
        digitalWrite(VM_int_ENT, LOW);  // vermelho interno entrada   LIGADO
        digitalWrite(VD_int_ENT, HIGH); // verde interno entrada      desligado
        // externo entrada
        digitalWrite(VM_ext_ENT, HIGH);  // vermelho externo entrada  LIGADO
        digitalWrite(VD_ext_ENT, LOW); // verde externo entrada       desligado
        // interno saida
        digitalWrite(VM_int_SAI, HIGH); // vermelho interno saida   desligado
        digitalWrite(VD_int_SAI, LOW); // verde interno saida       LIGADO
        // externo saida
        digitalWrite(VM_ext_SAI, HIGH);  // vermelho externo saida  LIGADO
        digitalWrite(VD_ext_SAI, LOW);  // verde externo saida      desligado
    } //end if 

} //end read_bts
void texta_saidas() {
    digitalWrite(VM_int_ENT, LOW); // vermelho interno entrada
    delay(1000);
    digitalWrite(VM_int_ENT, HIGH);
    delay(1000);
    digitalWrite(VM_int_SAI, LOW); // vermelho interno saida
    delay(1000);
    digitalWrite(VM_int_SAI, HIGH);
    delay(1000);
    digitalWrite(VD_int_SAI, LOW); // verde interno saida 
    delay(1000);
    digitalWrite(VD_int_SAI, HIGH);
    delay(1000);
    digitalWrite(VD_int_ENT, LOW); // verde interno entrada
    delay(1000);
    digitalWrite(VD_int_ENT, HIGH);
    delay(1000);
    digitalWrite(VD_ext_SAI, HIGH); // verde externo saida 
    delay(1000);
    digitalWrite(VD_ext_SAI, LOW);
    delay(1000);
    digitalWrite(VD_ext_ENT, HIGH); // verde externo entrada
    delay(1000);
    digitalWrite(VD_ext_ENT, LOW);
    delay(1000);
    digitalWrite(VM_ext_SAI, HIGH); //  vermelho externo saida 
    delay(1000);
    digitalWrite(VM_ext_SAI, LOW);
    delay(1000);
    digitalWrite(VM_ext_ENT, HIGH); // vermelho externo entrada
    delay(1000);
    digitalWrite(VM_ext_ENT, LOW);
} // end texta saidas
void desliga_tudo(){
        digitalWrite(VM_int_ENT, HIGH); // vermelho interno entrada 
        digitalWrite(VM_int_SAI, HIGH); // vermelho interno saida  
        digitalWrite(VD_int_SAI, HIGH); // verde interno saida
        digitalWrite(VD_int_ENT, HIGH); // verde interno entrada  
        digitalWrite(VD_ext_SAI, LOW);  // verde externo saida  
        digitalWrite(VD_ext_ENT, LOW);  // verde externo entrada  
        digitalWrite(VM_ext_SAI, LOW);  // vermelho externo saida  
        digitalWrite(VM_ext_ENT, LOW);  // vermelho externo entrada  
}// end desliga_tudo