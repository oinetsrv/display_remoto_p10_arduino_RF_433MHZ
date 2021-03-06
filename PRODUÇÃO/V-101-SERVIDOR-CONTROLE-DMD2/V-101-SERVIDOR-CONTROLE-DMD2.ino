/*
CONTROLADOR SERVIDOR 
|- 6 DISPLAYS
|- SALVA VALOR EEPROM
|- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
|- CANAL 1 INCREMENTA
|- CANAL 2 SELECIONA DIGITO
|- CANAL 3 LIGA / DESLIGA

CONTROLADOR CLIENTE
|- 3 DISPLAYS
|- SALVA VALOR EEPROM
|- ACIONADO POR CENTRAL DE RADIO 3 CANAIS
|- CANAL 1 INCREMENTA
|- CANAL 2 SELECIONA DIGITO
|- CANAL 3 LIGA / DESLIGA

AUTOR: STENIO RODRIGUES
DATA:02/02/2021

OBJETIVO:
NOTA:
// SERVIDOR E CLIENTE ENTRADAS
19 SELECIONA DIGITO
20 INCREMENTA VALOR

// SERVIDOR SAIDAS
XX INCREMENTA SERVIDOR COMANDA INCREMENTO 32 CLIENTE

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
///////////////////////

*/
#include <SPI.h>

#include <DMD2.h>

#include <fonts/Arial14.h>

#include <EEPROM.h>

const int COUNTDOWN_FROM = 12;
int counter = COUNTDOWN_FROM;
int QDISPLAY = 6;

SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 52, 51); // MEGA DMD controls the entire display
//SoftDMD dmd(QDISPLAY, 1, 9, 6, 7, 8, 12, 11); // UNO
// PAINEL           MEGA      UNO
//byte panelsWide   N/C
//byte panelsHigh   N/C
//byte pin_noe      9         9
//byte GND          GND       GND
//byte pin_a        6         6
//byte pin_b        7         7
//byte pin_sck      52        12    
//byte pin_clk      8         8
//byte pin_r_data   51        11

// BOTÃO SELECIONA DIGITO 
DMD_TextBox box(dmd);
DMD_TextBox box1(dmd, 1, 2); // "box" provides a text box to automatically write to/scroll the display
DMD_TextBox box2(dmd, 31, 2);
DMD_TextBox box3(dmd, 63, 2);
DMD_TextBox box4(dmd, 95, 2);
DMD_TextBox box5(dmd, 127, 2);
DMD_TextBox box6(dmd, 159, 2);

// "box" provides a text box to automatically write to/scroll the display
// the setup routine runs once when you press reset:
int cont = 0;
int x = 0;
int y = 0;
// definindo botoes
#define butUp 19 // seleciona DIGITOSo
#define butP 20 // incrementa 
#define butM 21 // desliga
volatile byte state_digito = LOW;
volatile byte state_increm = LOW;
volatile byte state_liga = LOW;
const int dig_real = 3; // PERMITE INCREMENTO (SELECIONADOR DE DIGITO REAL)
const int inc_serv = 17; // 36 INCREMENTADOR VIA SERVIDOR
const int liga_serv = 18; //38 LIGA/ DESLIGA VIA SERVIDOR
// definindo variaiveis marcadoras de status botoes
boolean t_butUp, t_butP, t_butM;
int cont_press_butUp = 0;
int cont_press_butP = 0;
int cont_press_butM = 0;
// variavel menu
char menu = 1;
// variaveis marcadores de digitos
int pos_dig = 0;
int dig_0 = 0;
int dig_1 = 0;
int dig_2 = 0;
int dig_3 = 0;
// variaveis tempo
unsigned long startMillis; // some global variables available anywhere in the program
unsigned long startMillis_comando;
unsigned long currentMillis;
unsigned long currentMillis_comando;
const unsigned long period = 400; // the value is a number of milliseconds
// fim variaveis tempo
// variaveis eprom
int address = 0;
byte value;
// fim variaveis eprom
// variaveis de contagem de ciclo
int i = 0;
int posI = 0;
int posF = 0;
int tamImp = 0;
// final variaveis de contagem de ciclo
int
var = 0;
int cont1 = 0;
int pos_dig_X = 0; // posicao do pixel no eixo X
int pos_dig_Y = 0; // posicao do pixel no eixo Y
int contX = 0; // diagonal percorrendo X
int contY = 0; // diagonal percorrendo Y
int contXY = -1; // diagonal percorrendo Y
// ///////// digito dezena display SUPERIOR //////////
int m_x_I_d_c = 0; // margem X inicial dezena centavo
int m_x_F_d_c = 0; // margem X final dezena centavo
int m_y_I_d_c = 0; // margem Y inicial dezena centavo
int m_y_F_d_c = 0; // margem Y final dezena centavo
// ///////// digito unidade display SUPERIOR //////////
int m_x_I_u_c = 0; // margem X inicial unidade centavo
int m_x_F_u_c = 0; // margem X final unidade centavo
int m_y_I_u_c = 0; // margem Y inicial unidade centavo
int m_y_F_u_c = 0; // margem Y final unidade centavo
// ///////// digito milhar display SUPERIOR //////////
int m_x_I_m_c = 0; // margem X inicial milhar centavo
int m_x_F_m_c = 0; // margem X final milhar centavo
int m_y_I_m_c = 0; // margem Y inicial milhar centavo
int m_y_F_m_c = 0; // margem Y final milhar centavo
// /////////////////////////////////////////////////////////////////
// ///////// digito dezena display MEIO //////////
int m_x_I_d_c_m = 0; // margem X inicial dezena centavo
int m_x_F_d_c_m = 0; // margem X final dezena
int m_y_I_d_c_m = 0; // margem Y inicial dezena centavo
int m_y_F_d_c_m = 0; // margem Y final dezena centavo
// ///////// digito unidade display MEIO //////////
int m_x_I_u_c_m = 0; // margem X inicial unidade centavo
int m_x_F_u_c_m = 0; // margem X final unidade centavo
int m_y_I_u_c_m = 0; // margem Y inicial unidade centavo
int m_y_F_u_c_m = 0; // margem Y final unidade centavo
// ///////// digito milhar display MEIO //////////
int m_x_I_m_c_m = 0; // margem X inicial milhar centavo
int m_x_F_m_c_m = 0; // margem X final milhar centavo
int m_y_I_m_c_m = 0; // margem Y inicial milhar centavo
int m_y_F_m_c_m = 0; // margem Y final milhar centavo
// /////////////////////////////////////////////////////////////////////////
// ///////// digito dezena display FINAL //////////
int m_x_I_d_c_f = 0; // margem X inicial dezena centavo
int m_x_F_d_c_f = 0; // margem X final dezena centavo
int m_y_I_d_c_f = 0; // margem Y inicial dezena centavo
int m_y_F_d_c_f = 0; // margem Y final dezena centavo
// ///////// digito unidade display FINAL //////////
int m_x_I_u_c_f = 0; // margem X inicial unidade centavo
int m_x_F_u_c_f = 0; // margem X final unidade centavo
int m_y_I_u_c_f = 0; // margem Y inicial unidade centavo
int m_y_F_u_c_f = 0; // margem Y final unidade centavo
// ///////// digito milhar display FINAL //////////
int m_x_I_m_c_f = 0; // margem X inicial milhar centavo
int m_x_F_m_c_f = 0; // margem X final milhar centavo
int m_y_I_m_c_f = 0; // margem Y inicial milhar centavo
int m_y_F_m_c_f = 0; // margem Y final milhar centavo
// /////////////////////////////////////////////////////////////////////////
int diagonal = 0;
int pos_display = 0; // paramentro que passa qual posição de impressao no display
int cont_desliga = 0;
int dig_real_cont = 0;
int desliga_cont = 0;

// definindo botoes
#define bt1 15 // SELECIONA
#define bt2 16 // INCREMENTA
#define bt3 17 // ATIVA DIGITO REAIS
// FIM definindo botoes

void readKey();
unsigned int duty1 = 1;
static boolean flag2 = 0, flag4 = 0;

void setup() {
    Serial.begin(9600);
    dmd.setBrightness(255);
    dmd.selectFont(Arial14);
    dmd.begin();
    Serial.println("V-101-SERVIDOR-CONTROLE-MONO-DMD2:");


    pinMode(bt1, INPUT);
    pinMode(bt2, INPUT);
    //pinMode(bt3, INPUT);  // ATIVAR NA VERSÃO ESCRAVO

    pinMode(bt3, OUTPUT);   // ATIVAR NA VERSÃO SERVIDOR
    digitalWrite(bt3, LOW); // ATIVAR NA VERSÃO SERVIDOR

    startMillis = millis(); // initial start time

}

// the loop routine runs over and over again forever:
void loop() {
    readKey();
    //menu_controle();
    imprimir();
    //imprimir_6();
    currentMillis = millis(); 
    if (currentMillis - startMillis >= 4*period){
        startMillis = currentMillis;
        //currentMillis = millis(); 
        marcador_digito();
    }

}
void marcador_digito(){

        dmd.setPixel(3, 3 , GRAPHICS_OFF);
        dmd.setPixel(25, 3 , GRAPHICS_OFF);
        dmd.setPixel(46, 3 , GRAPHICS_OFF);
         

}
void readKey() {
    static boolean flag1 = 0, flag3 = 0;
    if (digitalRead(bt1)) {
        if (flag2 == 1) {
            Serial.println("SELECIONA DIGITO!  ");
            digito();
            imprimir();
        }
        flag1 = 0x01;
        flag2 = 0x00;
        delay(10);
    }
    if (!digitalRead(bt1) && flag1) {
        flag1 = 0x00;
        flag2 = 0x01;
        
        Serial.println("RESET FLAG BOTAO DIGITO");
        delay(10);
    }
    if (digitalRead(bt2)) {
        if (flag4 == 1) {
            
            counter = counter + 1;
            Serial.println("INCREMENTA!  ");
            Serial.print(counter);
            Serial.println(F("..."));
            
            incrementa();
            imprimir();
        }
            if (counter>9)
            {
              counter = -1;
            }
        flag3 = 0x01;
        flag4 = 0x00;
        delay(10);
    }
    if (!digitalRead(bt2) && flag3) {
        flag3 = 0x00;
        flag4 = 0x01;
       
        Serial.println("RESET FLAG BOTAO INCREMENTA");
        delay(10);
    }






    
}

void imprimir_6() {
    dmd.fillScreen(true);
    delay(5000);
    dmd.clearScreen();
    delay(500);
    box1.print(' ');
    box1.print(counter);
    box1.println(F("..."));

    box2.print(' ');
    box2.print(counter);
    box2.println(F("..."));

    box3.print(' ');
    box3.print(counter);
    box3.println(F("..."));

    box4.print(' ');
    box4.print(counter);
    box4.println(F("..."));

    box5.print(' ');
    box5.print(counter);
    box5.println(F("..."));

    box6.print(' ');
    box6.print(counter);
    box6.println(F("..."));
}

/////////////////////
void imprimir() {
    Serial.println(" ");
    Serial.print("DADOS DENTRO DO DIGITO: ");
    for (int i = 0; i <= 3; i++) {
        value = EEPROM.read(i);
        Serial.print(value);
    }

    // endereço dezena 
    if (EEPROM.read(1) == 0) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_0(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 1) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_1(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 2) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_2(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 3) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_3(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 4) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_4(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 5) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_5(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 6) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_6(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 7) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_7(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 8) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_8(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(1) == 9) {
        m_x_I_d_c = 1; // X maximo 4 minimo 1 atual 2
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final real
        m_y_I_d_c = 5; // Y maximo 7 minimo 3 altura digito atual 5
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final real
        pos_display = 64; // posicao para 6 displays 32 para 3
        numero_9(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }

    // endereço digito unidade
    if (EEPROM.read(2) == 0) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_0(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 1) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_1(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 2) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_2(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 3) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_3(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 4) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_4(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 5) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_5(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 6) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_6(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 7) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_7(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 8) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_8(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(2) == 9) {
        m_x_I_d_c = 23; // maximo 46  // margem X inicial unidade centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final unidade centavo
        m_y_I_d_c = 5; // margem Y inicial unidade centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final unidade centavo
        pos_display = 64; // posicao unidade
        numero_9(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }

    // endereço digito milhar
    if (EEPROM.read(3) == 0) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_0(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 1) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_1(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 2) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_2(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 3) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_3(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 4) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_4(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 5) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_5(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 6) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_6(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 7) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_7(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 8) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_8(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    if (EEPROM.read(3) == 9) {
        m_x_I_d_c = 45; // maximo 46  // margem X inicial milhar centavo
        m_x_F_d_c = m_x_I_d_c + 16; // margem X final milhar centavo
        m_y_I_d_c = 5; // margem Y inicial milhar centavo
        m_y_F_d_c = m_y_I_d_c + 15; // margem Y final milhar  centavo
        pos_display = 64; // posicao milhar
        numero_9(m_x_I_d_c, m_x_F_d_c, m_y_I_d_c, m_y_F_d_c, pos_display);
        cont = 1;
    }
    /// condicionais impressão  //
    if (EEPROM.read(0) == 1) {
        //numero_1 ();
        cont = 2;
    }
    if (EEPROM.read(0) == 2) {
        //numero_2 ();
        cont = 3;
    }
    if (EEPROM.read(0) == 3) {
        //numero_3 ();
        cont = 4;
    }
    if (EEPROM.read(0) == 4) {
        //numero_4 ();
        cont = 5;
    }
    if (EEPROM.read(0) == 5) {
        //numero_5 ();
        cont = 6;
    }
    if (EEPROM.read(0) == 6) {
        //numero_6 ();
        cont = 7;
    }
    if (EEPROM.read(0) == 7) {
        //numero_7 ();
        cont = 8;
    }
    if (EEPROM.read(0) == 8) {
        //numero_8 ();
        cont = 9;
    }
    if (EEPROM.read(0) == 9) {
        //numero_9 ();
        cont = 10;
    }
}
/////////////////////
void incrementa() {
    dmd.clearScreen();
    delay(10);
    if (pos_dig == 0) {
        dig_0 = dig_0 + 1;
        EEPROM.write(0, dig_0);
    }
    if (pos_dig == 1) {
        dig_1 = dig_1 + 1;
        EEPROM.write(1, dig_1);
    }
    if (pos_dig == 2) {
        dig_2 = dig_2 + 1;
        EEPROM.write(2, dig_2);
    }
    if (pos_dig == 3) {
        dig_3 = dig_3 + 1;
        EEPROM.write(3, dig_3);
    }
    if (dig_0 > 9) {
        dig_0 = 0;
        EEPROM.write(0, dig_0);
    }
    if (dig_1 > 9) {
        dig_1 = 0;
        EEPROM.write(1, dig_1);
    }
    if (dig_2 > 9) {
        dig_2 = 0;
        EEPROM.write(2, dig_2);
    }
    if (dig_3 > 9) {
        dig_3 = 0;
        EEPROM.write(3, dig_3);
    }
   // imprimir(); // INFORMA VIA SERIAL A INFORMAÇAO ATUALIZADA
}
/////////////////////

/////////////////////
void digito() {
    pos_dig = pos_dig + 1;
    //pos_dig = 0;
    address = pos_dig;
    contXY = -4; // posicao no display
    if (pos_dig > 3) {
        pos_dig = 0;
        contXY = -4; // posicao no display
        address = pos_dig;
    }
    startMillis = currentMillis;
    if (pos_dig == 0) {
        digitalWrite(bt3, HIGH); //ATIVA EDITAR VALOR NO ESCRAVO
        dmd.setPixel(3, 3 , GRAPHICS_OFF);
        dmd.setPixel(25, 3 , GRAPHICS_OFF);
        dmd.setPixel(46, 3 , GRAPHICS_OFF);
    } 
    if (pos_dig == 1) {
        digitalWrite(bt3, LOW);
        dmd.setPixel(3, 3 , GRAPHICS_ON);
 
        dmd.setPixel(25, 3 , GRAPHICS_OFF);
        dmd.setPixel(46, 3 , GRAPHICS_OFF);
         
    } 
    if (pos_dig == 2) {
        digitalWrite(bt3, LOW);
        dmd.setPixel(25, 3 , GRAPHICS_ON);

        dmd.setPixel(3, 3 , GRAPHICS_OFF);
        dmd.setPixel(46, 3 , GRAPHICS_OFF);
         
    } 
    if (pos_dig == 3) {
        digitalWrite(bt3, LOW);
        dmd.setPixel(46, 3 , GRAPHICS_ON);

        dmd.setPixel(3, 3 , GRAPHICS_OFF);
        dmd.setPixel(25, 3 , GRAPHICS_OFF);

    }

    Serial.println(" ");
    Serial.print("endereco DIGITO: ");
    Serial.print(address + 1);
    Serial.println(" ");
}
/////////////////////

void numero_0(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 5) {
            for (int y = 0; y <= 11; y++) {
                if (x <= m_x_I_d_c + 4) {
                    dmd.setPixel(x + 1, m_y_I_d_c + 2 + y, GRAPHICS_ON);
                    dmd.setPixel(x + 10, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON);
            }
        }
    }
    // FIM painel superior
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    m_x_F_d_c = m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 5) {
            for (int y = 0; y <= 19; y++) {
                if (x <= m_x_I_d_c + 5) {
                    dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                    dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
    }
    // FIM painel meio
    // **********************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 16; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 4) {
            for (int y = 0; y <= 8; y++) {
                if (x <= m_x_I_d_c + 2 && y <= 7) {
                    dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                    dmd.setPixel(x + 13, m_y_I_d_c + y, GRAPHICS_ON);
                }
                dmd.setPixel(x + 1, m_y_I_d_c + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 6 + y, GRAPHICS_ON);
            }
        }
    }
    // FIM Painel final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_0 digito   ///

void numero_1(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 10; x++) {
        diagonal = diagonal + 1;
        //dmd.setPixel(x, m_y_I_d_c + 10, GRAPHICS_ON);
        for (int y = m_y_I_d_c + 0; y <= 15; y++) {
            if (x >= m_x_I_d_c + 5 && y >= 6) {
                dmd.setPixel(x, y, GRAPHICS_ON);
            }
            if (x >= m_x_I_d_c + 3 && x <= m_x_I_d_c + 5 && y == 6) {
                dmd.setPixel(x, y, GRAPHICS_ON);
            }
            if (x >= m_x_I_d_c + 2 && x <= m_x_I_d_c + 5 && y == 7) {
                dmd.setPixel(x, y, GRAPHICS_ON);
            }
            if (x >= m_x_I_d_c + 1 && x <= m_x_I_d_c + 5 && y >= 8 && y <= 12) {
                dmd.setPixel(x, y, GRAPHICS_ON);
            }
        }
    }
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    m_x_F_d_c = m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 5; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    for (int y = 0; y <= 15; y++) {
        for (int x = m_x_I_d_c + 5; x <= m_x_I_d_c + 10; x++) {
            dmd.setPixel(x, y, GRAPHICS_ON);
        }
        dmd.setPixel(m_x_I_d_c + 5, y, GRAPHICS_ON); // contorno meio
        dmd.setPixel(m_x_I_d_c + 10, y, GRAPHICS_ON);
    }
    // FIM painel meio
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 7; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int y = 0; y <= m_y_F_d_c; y++) {
        dmd.setPixel(m_x_I_d_c + 5, y, GRAPHICS_ON);
        dmd.setPixel(m_x_I_d_c + 10, y, GRAPHICS_ON);
        for (int x = m_x_I_d_c + 5; x <= m_x_I_d_c + 10; x++) {
            dmd.setPixel(x, y, GRAPHICS_ON); // contorno inferior display de baixo
        }
    }
    for (int x = m_x_I_d_c + 5; x <= m_x_I_d_c + 10; x++) {
        dmd.setPixel(x, m_y_F_d_c, GRAPHICS_ON); // contorno inferior display de baixo
    }
    // FIM painel final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_1 digito   ///

void numero_2(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real

    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        for (int y = 0; y <= 3; y++) {
            if (x <= m_x_I_d_c + 9) {
                dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON);
            }
        }
        if (x + 2 <= m_x_I_d_c + 11) {
            dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON); //
        }
        if (x <= m_x_I_d_c + 5) {
            if (15 == m_y_I_d_c + 8) {
                dmd.setPixel(x + 1, 15, GRAPHICS_ON);
            }
            for (int y = 0; y <= 5; y++) {
                dmd.setPixel(x + 1, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 1, m_y_I_d_c + 4 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 8, m_y_I_d_c + 6 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 9, m_y_I_d_c + 5 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 9, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + 4 + y, GRAPHICS_ON); //
            }
            dmd.setPixel(x + 1, m_y_I_d_c + 9 + y, GRAPHICS_ON); //
        }
        if (x <= m_x_I_d_c + 4) {
            dmd.setPixel(x + 5, m_y_I_d_c + 5 + y, GRAPHICS_ON); //
        }
    }
    for (int y = m_y_I_d_c + 1; y <= m_y_I_d_c + 15; y++) {
        if (y <= m_y_I_d_c + 7) {
            for (int x = m_x_I_d_c + 9; x <= m_x_I_d_c + 14; x++) {}
            dmd.setPixel(m_x_I_d_c + 1, y + 2, GRAPHICS_ON); //
        }
    }
    // FIM painel superior
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    // dezena centavo  // PASSO DE 22 PARA O PROXIMO
    // DISPLAY
    m_x_F_d_c =
    m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    diagonal = 0;
    for (int y = m_y_I_d_c; y <= m_y_I_d_c + 19; y++) {
        if (y <= m_y_I_d_c + 2) {
            for (int x = m_x_I_d_c + 8; x <= m_x_I_d_c + 14; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON);
            }
            //dmd.setPixel(m_x_I_d_c + 8, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 14, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 3 && y <= m_y_I_d_c + 3) {
            for (int x = m_x_I_d_c + 8; x <= m_x_I_d_c + 13; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            //dmd.setPixel(m_x_I_d_c + 7, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 13, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 4 && y <= m_y_I_d_c + 5) {
            for (int x = m_x_I_d_c + 7; x <= m_x_I_d_c + 12; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            //dmd.setPixel(m_x_I_d_c + 6, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 12, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 6 && y <= m_y_I_d_c + 8) {
            for (int x = m_x_I_d_c + 6; x <= m_x_I_d_c + 11; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            //dmd.setPixel(m_x_I_d_c + 5, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 11, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 9 && y <= m_y_I_d_c + 11) {
            for (int x = m_x_I_d_c + 5; x <= m_x_I_d_c + 10; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            //dmd.setPixel(m_x_I_d_c + 4, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 10, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 12 && y <= m_y_I_d_c + 13) {
            for (int x = m_x_I_d_c + 4; x <= m_x_I_d_c + 9; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            dmd.setPixel(m_x_I_d_c + 4, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 9, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 14 && y <= m_y_I_d_c + 15) {
            for (int x = m_x_I_d_c + 3; x <= m_x_I_d_c + 8; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            //dmd.setPixel(m_x_I_d_c + 3, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 8, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 16) {
            for (int x = m_x_I_d_c + 2; x <= m_x_I_d_c + 7; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            //dmd.setPixel(m_x_I_d_c + 1, y, GRAPHICS_ON); //
            dmd.setPixel(m_x_I_d_c + 7, y, GRAPHICS_ON);
        }
    }
    // delay(500);
    // FIM painel meio
    // **********************************************
    m_x_I_d_c = m_x_I_d_c + dig - 1; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 9; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int y = 0; y <= m_y_F_d_c; y++) {
        for (int x = m_x_I_d_c + 2; x <= m_x_I_d_c + 7; x++) {
            dmd.setPixel(x, y, GRAPHICS_ON); //
        }
        dmd.setPixel(m_x_I_d_c + 2, y, GRAPHICS_ON); //
        if (y <= m_y_I_d_c + 3) {
            dmd.setPixel(m_x_I_d_c + 8, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 5) {
            for (int x = m_x_I_d_c + 7; x <= m_x_I_d_c + 15; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            dmd.setPixel(m_x_I_d_c + 16, y, GRAPHICS_ON);
        }
    }
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        dmd.setPixel(x + 1, m_y_F_d_c, GRAPHICS_ON); //
        if (x <= m_x_I_d_c + 9) {
            dmd.setPixel(x + 7, m_y_I_d_c + 4, GRAPHICS_ON);
        }
    }
    // Painel fim final
    // ************************************************
    // devolve os valores das  variaveis
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_2 digito   ///

void numero_3(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 5) {
            for (int y = 0; y <= 11; y++) {
                if (x <= m_x_I_d_c + 4 && y <= 5) {
                    dmd.setPixel(x + 1, m_y_I_d_c + 2 + y, GRAPHICS_ON);
                    dmd.setPixel(x + 10, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                }
                if (y <= 5) {
                    dmd.setPixel(x, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x + 10, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON);
            }
        }
    }
    // Painel fim superior
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    // dezena centavo  // PASSO DE 22 PARA O PROXIMO
    // DISPLAY
    m_x_F_d_c =
    m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 2) {
            for (int y = 0; y <= 19; y++) {
                if (x <= m_x_I_d_c + 5 && y <= 7) {
                    if (x <= m_x_I_d_c + 5 && y <= 4) {
                        for (int z = 4; z <= 8; z = z + 2) {
                            dmd.setPixel(x + z, m_y_I_d_c + y + 6, GRAPHICS_ON);
                        }
                    }
                    dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y + 1, GRAPHICS_ON); //
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + 11 + z, m_y_I_d_c + y - 1 - z, GRAPHICS_ON); //
                    }
                }
                if (x <= m_x_I_d_c + 5 && y >= 11) {
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + 13 - z, m_y_I_d_c + y - z, GRAPHICS_ON); //
                    }
                    dmd.setPixel(x + 10, m_y_I_d_c + y - 2, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y + 6, GRAPHICS_ON); //
                    dmd.setPixel(x + 11, m_y_I_d_c + y + 6, GRAPHICS_ON); //
                }
            }
        }
    }
    // FIM painel meio
    // **********************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 16; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 4) {
            for (int y = 0; y <= 8; y++) {
                if (x <= m_x_I_d_c + 2 && y <= 7) {
                    if (y >= 4) {
                        dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                    }
                    dmd.setPixel(x + 13, m_y_I_d_c + y, GRAPHICS_ON);
                }
                if (y >= 4) {
                    dmd.setPixel(x + 1, m_y_I_d_c + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 6 + y, GRAPHICS_ON);
            }
        }
    }
    // Painel fim final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_3 digito   ///

void numero_4(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        for (int y = 0; y <= 19; y++) {
            if (x <= m_x_I_d_c + 2) {
                dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                dmd.setPixel(x + 1, m_y_I_d_c + y, GRAPHICS_ON);
                dmd.setPixel(x + 3, m_y_I_d_c + y, GRAPHICS_ON);
                dmd.setPixel(x + 13, m_y_I_d_c + y + 3, GRAPHICS_ON);
                dmd.setPixel(x + 12, m_y_I_d_c + y + 3, GRAPHICS_ON);
                dmd.setPixel(x + 10, m_y_I_d_c + y + 3, GRAPHICS_ON);
            }
        }
    } // fim for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
    // FIM painel superior
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    // dezena centavo  // PASSO DE 22 PARA O PROXIMO
    // DISPLAY
    m_x_F_d_c =
    m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 2) {
            for (int y = 0; y <= 19; y++) {
                if (x <= m_x_I_d_c + 5 && y <= 7) {
                    if (x <= m_x_I_d_c + 4 && y <= 4) {
                        for (int z = 0; z <= 10; z = z + 2) {
                            dmd.setPixel(x + z, m_y_I_d_c + y + 5, GRAPHICS_ON);
                        }
                    }
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + z, m_y_I_d_c + y - 3 + z, GRAPHICS_ON);
                        //dmd.setPixel(x + z, m_y_I_d_c + y - 3 + z, GRAPHICS_ON);
                    }
                    dmd.setPixel(x + 3, m_y_I_d_c + y + 1, GRAPHICS_ON);
                }
                for (int z = 0; z <= 3; z++) {
                    dmd.setPixel(x + 10 + z, m_y_I_d_c + y, GRAPHICS_ON); //
                }
            }
        }
    }
    // FIM painel meio
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 16; // margem Y final dezena centavo
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 4) {
            for (int y = 0; y <= 8; y++) {
                if (x <= m_x_I_d_c + 2 && y <= 7) {
                    dmd.setPixel(x + 13, m_y_I_d_c + y, GRAPHICS_ON);
                }
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {}
        }
    }
    // FIM painel inferior
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_4 digito   ///

void numero_5(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 5) {
            for (int y = 0; y <= 11; y++) {
                if (x <= m_x_I_d_c + 4) {
                    dmd.setPixel(x + 1, m_y_I_d_c + 2 + y, GRAPHICS_ON);
                    if (y <= 1) {
                        dmd.setPixel(x + 10, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                    }
                }
                dmd.setPixel(x, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                if (y <= 1) {
                    dmd.setPixel(x + 10, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                }
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON);
            }
        }
    }
    // FIM painel superior
    // ************************************************
    // Painel meio
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    // dezena centavo  // PASSO DE 22 PARA O PROXIMO
    // DISPLAY
    m_x_F_d_c =
    m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 2) {
            for (int y = 0; y <= 19; y++) {
                if (x <= m_x_I_d_c + 5 && y <= 7) {
                    if (x <= m_x_I_d_c + 5 && y <= 4) {
                        for (int z = 0; z <= 8; z = z + 2) {
                            if (z <= 1 && y == 4) {
                                dmd.setPixel(x + 1 + z, m_y_I_d_c + y + 4, GRAPHICS_ON);
                            }
                            if (z <= 1) {
                                dmd.setPixel(x + z, m_y_I_d_c + y + 3, GRAPHICS_ON);
                            } else {

                                dmd.setPixel(x + z, m_y_I_d_c + y + 5, GRAPHICS_ON);
                            }
                        }
                    }
                    for (int z = 0; z <= 3; z++) {
                        dmd.setPixel(x + z, m_y_I_d_c + y - 3 + z, GRAPHICS_ON);
                    }
                }
                if (x <= m_x_I_d_c + 5 && y >= 9) {
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + 13 - z, m_y_I_d_c + y - z, GRAPHICS_ON); //
                    }
                    dmd.setPixel(m_x_I_d_c + 14, m_y_I_d_c + y - 2, GRAPHICS_ON); //
                    dmd.setPixel(x + 11, m_y_I_d_c + y - 2, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y - 3, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y + 6, GRAPHICS_ON); //
                }
            }
        }
    }
    // FIM painel meio
    // ************************************************
    // Painel final
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 16; // margem Y final dezena centavo
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 4) {
            for (int y = 0; y <= 8; y++) {
                if (x <= m_x_I_d_c + 2 && y <= 7) {
                    if (y >= 6) {
                        dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                    }
                    dmd.setPixel(x + 13, m_y_I_d_c + y, GRAPHICS_ON);
                }
                if (y >= 6) {
                    dmd.setPixel(x + 1, m_y_I_d_c + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 6 + y, GRAPHICS_ON);
            }
        }
    }
    // FIM painel final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_5 digito   ///

void numero_6(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 5) {
            for (int y = 0; y <= 11; y++) {
                if (x <= m_x_I_d_c + 4) {
                    dmd.setPixel(x + 1, m_y_I_d_c + 2 + y, GRAPHICS_ON);
                    if (y <= 3) {
                        dmd.setPixel(x + 10, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                    }
                }
                dmd.setPixel(x, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                if (y <= 3) {
                    dmd.setPixel(x + 10, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                }
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON);
            }
        }
    }
    // FIM painel superior
    // ************************************************
    // Painel meio
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    // dezena centavo  // PASSO DE 22 PARA O PROXIMO
    // DISPLAY
    m_x_F_d_c =
        m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 2) {
            for (int y = 0; y <= 19; y++) {
                for (int z = 0; z <= 3; z++) {
                    dmd.setPixel(x + z, m_y_I_d_c + y + 5, GRAPHICS_ON);
                }
                if (x <= m_x_I_d_c + 5 && y <= 7) {
                    if (x <= m_x_I_d_c + 5 && y <= 4) {
                        for (int z = 4; z <= 8; z = z + 2) {
                            dmd.setPixel(x + z, m_y_I_d_c + y + 6, GRAPHICS_ON);
                            if (z <= 8) {
                                dmd.setPixel(x + 10, m_y_I_d_c + y + 7, GRAPHICS_ON);
                            }

                        }
                    }
                    for (int z = 0; z <= 3; z++) {
                        dmd.setPixel(x + z, m_y_I_d_c + y - 3 + z, GRAPHICS_ON);
                    }
                    dmd.setPixel(x + 3, m_y_I_d_c + y + 1, GRAPHICS_ON);
                    dmd.setPixel(x + 2, m_y_I_d_c + y + 12, GRAPHICS_ON); //
                    dmd.setPixel(x + 3, m_y_I_d_c + y + 12, GRAPHICS_ON); //
                }
                if (x <= m_x_I_d_c + 5 && y >= 10) {
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + 13 - z, m_y_I_d_c + y - z, GRAPHICS_ON); //
                    }
                    dmd.setPixel(x + 10, m_y_I_d_c + y - 2, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y + 6, GRAPHICS_ON); //
                    dmd.setPixel(x + 11, m_y_I_d_c + y + 6, GRAPHICS_ON); //
                }
            }
        }
    }
    // FIM painel meio
    // **********************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 16; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 4) {
            for (int y = 0; y <= 8; y++) {
                if (x <= m_x_I_d_c + 2 && y <= 7) {
                    dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                    dmd.setPixel(x + 13, m_y_I_d_c + y, GRAPHICS_ON);
                }
                dmd.setPixel(x + 1, m_y_I_d_c + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 6 + y, GRAPHICS_ON);
            }
        }
    }
    // Painel Fim final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_6 digito   ///

void numero_7(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        for (int y = 0; y <= 5; y++) {
            dmd.setPixel(x, m_y_I_d_c + 1 + y, GRAPHICS_ON); //
        }
        dmd.setPixel(x, m_y_I_d_c + 1, GRAPHICS_ON); //
        if (x <= m_x_I_d_c + 6) {
            if (m_y_I_d_c + 9 >= 15) {
                dmd.setPixel(x, 15, GRAPHICS_ON);
            }
            for (int y = m_y_I_d_c + 1; y <= 15; y++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
            dmd.setPixel(x, m_y_I_d_c + 9, GRAPHICS_ON);
            if (x <= m_x_I_d_c + 4) {
                dmd.setPixel(x + 5, m_y_I_d_c + 5, GRAPHICS_ON);
            }
        }
    }
    for (int y = m_y_I_d_c + 1; y <= m_y_I_d_c + 15; y++) {
        if (y <= m_y_I_d_c + 9) {
            for (int x = m_x_I_d_c + 9; x <= m_x_I_d_c + 14; x++) {
                dmd.setPixel(x, y + 1, GRAPHICS_ON); //
            }
            dmd.setPixel(m_x_I_d_c + 1, y, GRAPHICS_ON);
        }
        if (y >= m_y_I_d_c + 9) {
            dmd.setPixel(m_x_I_d_c + 14, y + 1, GRAPHICS_ON);
        } else {
            for (int x = m_x_I_d_c + 9; x <= m_x_I_d_c + 14; x++) {
                dmd.setPixel(x, y + 1, GRAPHICS_ON); //
            }
            dmd.setPixel(m_x_I_d_c + 15, y + 1, GRAPHICS_ON); //
        }
        if (y >= m_y_I_d_c + 5) {
            if (y >= m_y_I_d_c + 5 && y <= m_y_I_d_c + 9) {
                dmd.setPixel(m_x_I_d_c + 6, y, GRAPHICS_ON);
            }
            dmd.setPixel(m_x_I_d_c + 9, y + 1, GRAPHICS_ON);
            if (y >= m_y_I_d_c + 10) {
                dmd.setPixel(m_x_I_d_c + 8, y + 1, GRAPHICS_ON); //
                for (int x = m_x_I_d_c + 9; x <= m_x_I_d_c + 14; x++) {
                    //dmd.setPixel(x, y + 1, GRAPHICS_ON); //
                }
            }
        }
    }
    // Painel fim superior
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    // dezena centavo  // PASSO DE 22 PARA O PROXIMO
    // DISPLAY
    m_x_F_d_c =
        m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    for (int y = m_y_I_d_c; y <= m_y_I_d_c + 19; y++) {
        if (y <= m_y_I_d_c + 2) {
            for (int x = m_x_I_d_c + 9; x <= m_x_I_d_c + 14; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
        }
        if (y >= m_y_I_d_c + 3 && y <= m_y_I_d_c + 6) {
            for (int x = m_x_I_d_c + 8; x <= m_x_I_d_c + 13; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
        }
        if (y >= m_y_I_d_c + 7 && y <= m_y_I_d_c + 9) {
            for (int x = m_x_I_d_c + 7; x <= m_x_I_d_c + 12; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
        }
        if (y >= m_y_I_d_c + 10 && y <= m_y_I_d_c + 13) {
            for (int x = m_x_I_d_c + 6; x <= m_x_I_d_c + 11; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
        }
        if (y >= m_y_I_d_c + 14) {
            for (int x = m_x_I_d_c + 5; x <= m_x_I_d_c + 11; x++) {
                dmd.setPixel(x, y, GRAPHICS_ON); //
            }
        }
    }
    // Painel fim meio
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig - 1; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 9; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int y = 0; y <= m_y_F_d_c; y++) {
        for (int x = m_x_I_d_c + 6; x <= m_x_I_d_c + 12; x++) {
            dmd.setPixel(x, y, GRAPHICS_ON); //
        }
    }
    // Painel final
    // ************************************************
    // Painel fim final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_7 digito   ///

void numero_8(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 5) {
            for (int y = 0; y <= 11; y++) {
                if (x <= m_x_I_d_c + 4) {
                    dmd.setPixel(x + 1, m_y_I_d_c + 2 + y, GRAPHICS_ON);
                    dmd.setPixel(x + 10, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON);
            }
        }
    }
    diagonal = 0;
    // FIM painel superior
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    m_x_F_d_c = m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 2) {
            for (int y = 0; y <= 19; y++) {
                if (x <= m_x_I_d_c + 5 && y <= 7) {
                    if (x <= m_x_I_d_c + 5 && y <= 4) {
                        for (int z = 4; z <= 8; z = z + 2) {
                            dmd.setPixel(x + z, m_y_I_d_c + y + 6, GRAPHICS_ON);
                        }
                    }
                    for (int z = 0; z <= 3; z++) {
                        dmd.setPixel(x + z, m_y_I_d_c + y - 3 + z, GRAPHICS_ON);
                    }
                    dmd.setPixel(x + 3, m_y_I_d_c + y + 1, GRAPHICS_ON);
                    dmd.setPixel(x + 2, m_y_I_d_c + y + 12, GRAPHICS_ON); //
                    dmd.setPixel(x + 3, m_y_I_d_c + y + 12, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y + 1, GRAPHICS_ON); //
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + 11 + z, m_y_I_d_c + y - 1 - z, GRAPHICS_ON); //
                    }
                }
                if (x <= m_x_I_d_c + 5 && y >= 11) {
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + z, m_y_I_d_c + y - z, GRAPHICS_ON);
                    }
                    dmd.setPixel(x + 3, m_y_I_d_c + y - 2, GRAPHICS_ON);
                    for (int z = 0; z <= 2; z++) {
                        dmd.setPixel(x + 13 - z, m_y_I_d_c + y - z, GRAPHICS_ON); //
                    }
                    dmd.setPixel(x + 10, m_y_I_d_c + y - 2, GRAPHICS_ON); //
                    dmd.setPixel(x + 10, m_y_I_d_c + y + 6, GRAPHICS_ON); //
                    dmd.setPixel(x + 11, m_y_I_d_c + y + 6, GRAPHICS_ON); //
                }
            }
        }
    }
    // FIM painel meio
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 16; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 4) {
            for (int y = 0; y <= 8; y++) {
                if (x <= m_x_I_d_c + 2 && y <= 7) {
                    dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                    dmd.setPixel(x + 13, m_y_I_d_c + y, GRAPHICS_ON);
                }
                dmd.setPixel(x + 1, m_y_I_d_c + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 6 + y, GRAPHICS_ON);
            }
        }
    }
    // FIM painel final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_8 digito   ///

void numero_9(int m_x_I, int m_x_F, int m_y_I, int m_y_F, int dig) {
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
    // Painel superior
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 5) {
            for (int y = 0; y <= 11; y++) {
                if (x <= m_x_I_d_c + 4) {
                    dmd.setPixel(x + 1, m_y_I_d_c + 2 + y, GRAPHICS_ON);
                    dmd.setPixel(x + 10, m_y_I_d_c + 2 + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
                dmd.setPixel(x + 10, m_y_I_d_c + 3 + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 1 + y, GRAPHICS_ON);
            }
        }
    }
    diagonal = 0;
    // FIM painel superior
    // ************************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 71 inicio letramaximo 46  // margem X inicial
    m_x_F_d_c = m_x_I_d_c + 16; // 7  de largura letra margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c - 7; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 15; // margem Y final dezena centavo
    // Painel meio
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 2) {
            for (int y = 0; y <= 19; y++) {
                if (x <= m_x_I_d_c + 5 && y <= 7) {
                    if (x <= m_x_I_d_c + 5 && y <= 4) {
                        for (int z = 4; z <= 8; z = z + 2) {
                            dmd.setPixel(x + z, m_y_I_d_c + y + 6, GRAPHICS_ON);
                        }
                    }
                    for (int z = 0; z <= 3; z++) {
                        dmd.setPixel(x + z, m_y_I_d_c + y - 1 + z, GRAPHICS_ON);
                    }
                    //dmd.setPixel(x + 3, m_y_I_d_c + y + 1, GRAPHICS_ON);
                }
                for (int z = 0; z <= 3; z++) {
                    dmd.setPixel(x + 10 + z, m_y_I_d_c + y, GRAPHICS_ON); //
                }
            }
        }
    }
    // FIM painel meio
    // **********************************************
    m_x_I_d_c = m_x_I_d_c + dig; // 130  // margem X inicial dezena centavo
    m_x_F_d_c = m_x_I_d_c + 15; // margem X final dezena centavo
    m_y_I_d_c = m_y_I_d_c + 0; // margem Y inicial dezena centavo
    m_y_F_d_c = m_y_I_d_c + 16; // margem Y final dezena centavo
    // Painel final
    // ************************************************
    for (int x = m_x_I_d_c + 1; x <= m_x_I_d_c + 15; x++) {
        if (x <= m_x_I_d_c + 4) {
            for (int y = 0; y <= 8; y++) {
                if (x <= m_x_I_d_c + 2 && y <= 7) {
                    if (y >= 4) {
                        dmd.setPixel(x, m_y_I_d_c + y, GRAPHICS_ON);
                    }
                    dmd.setPixel(x + 13, m_y_I_d_c + y, GRAPHICS_ON);
                }
                if (y >= 4) {
                    dmd.setPixel(x + 1, m_y_I_d_c + y, GRAPHICS_ON); //
                }
                dmd.setPixel(x + 10, m_y_I_d_c + y, GRAPHICS_ON); //
            }
        }
        for (int y = 0; y <= 7; y++) {
            if (x <= m_x_I_d_c + 9 && y <= 3) {
                dmd.setPixel(x + 3, m_y_I_d_c + 6 + y, GRAPHICS_ON);
            }
        }
    }
    // FIM painel final
    // ************************************************
    m_x_I_d_c = m_x_I; // maximo 46  // margem X inicial real
    m_x_F_d_c = m_x_F; // margem X final real
    m_y_I_d_c = m_y_I; // margem Y inicial real
    m_y_F_d_c = m_y_F; // margem Y final real
}// / fim numero_9 digito   ///
