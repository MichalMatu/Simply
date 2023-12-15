/*
  Programa de seleção de Menu com telas, adaptado por JailsonBR do código original do olikraus.

  PORFAVOR NÃO REMOVA NENHUMA INFORMAÇÃO DESTE CABEÇARIO, ESTE É UM CÓDIGO OPENSOURCE SOB LICENÇA DO CRIADOR

  Codigo adaptado por JailsonBR do site XProjetos.net em 12-04-2019, o uso deste código é livre porem deve respeitar e mante o nome do criador
  e tambem de quem modificou o código o código está sob Licença Creative Commons Atribuição 4.0 Internacional. http://creativecommons.org/licenses/by/4.0/

  Universal 8bit Graphics Library (https://github.com/olikraus/u8g2/)

  Copyright (c) 2016, olikraus@gmail.com
  All rights reserved.

  Redistribution and use in source and binary forms, with or without modification,
  are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this list
    of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice, this
    list of conditions and the following disclaimer in the documentation and/or other
    materials provided with the distribution.

      THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND
      CONTRIBUTORS "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES,
      INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES OF
      MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
      DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR
      CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
      SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
      NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
      LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
      CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT,
      STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
      ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF
      ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

  Copyright (c) 2019, XProjetos.net
*/
#include <Adafruit_I2CDevice.h>
#include <Arduino.h>
#include <U8g2lib.h>
#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif
#define PinoAnalogico 34 // O compilador substituirá qualquer menção de PinoAnalogico ao valor 34 no momento da compilação.
#define PinoLED 15       // O compilador substituirá qualquer menção de PinoLED ao valor 15 no momento da compilação.

// PWM Setup para ESP32
int freq = 5000; // Frequencia para o PWM
int CanalLED = 0;
int resolution = 8;

U8G2_ST7920_128X64_1_SW_SPI u8g2(U8G2_R0, 18, 19, 21, 22); // Enable, RW, RS, RESET

byte menu_inicial = 1; // Variável para armazenar a variável para chamar o menu principal
byte ValorPWM = 0;     // Variável para armazenar a variável para o PWM

int leituraADC = 0, AnaRead = 0; // Variáveis para armazenar os valores analógicos

// Icones em XBMP para barra de icones (Gerados no GIMP)
static unsigned char bluetooth_bits[] = {
    0x10, 0x32, 0x54, 0x38, 0x38, 0x54, 0x32, 0x10};

static unsigned char wifi_bits[] = {
    0x7e, 0x81, 0x3c, 0x42, 0x18, 0x00, 0x18, 0x18};

// Lista do Menu Principal
const char *string_list = // Lista de Menus, você pode adicionar mais itens se desejar
    "Barra de icones\n"
    "Leitura Analogica\n"
    "LED PWM\n"
    "XProjetos.net\n"
    "5. Item\n"
    "6. Item\n"
    "7. Item\n"
    "Final";

uint8_t current_selection = 1; // Configura o primeiro item estar selecionado

/*******************************************************************
 *                       SETUP
 *****************************************************************/

void setup(void)
{

  // MKR Zero Test Board
  u8g2.begin(/*Select=*/12, /*Right/Next=*/27, /*Left/Prev=*/25, /*Up=*/26, /*Down=*/14, /*Home/Cancel=*/13); // Inicia os botões de navegação para o menu
  u8g2.setFont(u8g2_font_6x12_tr);                                                                            // Configura o tipo de Fonte

  pinMode(PinoAnalogico, INPUT); // Configura o pino PinoAnalogico para IO34 como entrada
  pinMode(PinoLED, OUTPUT);
  //------------------PWM ESP32--------------------
  sigmaDeltaSetup(CanalLED, PinoLED, freq);
  // Inicializa o PWM e anexa o PinoLED para o canal 0
  sigmaDeltaSetup(CanalLED, PinoLED, freq);
  // Configura a frequencia do canal 0 do PWM para 5000Hz
  sigmaDeltaWrite(CanalLED, 0); // Inicia o PWM em zero (pode ser configurado de 0 a 255)
}

void loop(void)
{
  leituraADC = analogRead(PinoAnalogico); // Leitura de porta analógica e armazenamento na variável
  u8g2.setCursor(0, 0);                   // Configura a posição do proximo item a ser escrito na tela
  u8g2.print(leituraADC);                 // Escreve o valor armazenado da porta analogica

  //=========================NAVEGACÃO============================
  if (menu_inicial == 1)
  {
    current_selection = u8g2.userInterfaceSelectionList( // Caso estiver no Menu Princinpal
        "Menu Principal",                                // Titulo do Menu
        current_selection,
        string_list);
  }
  if (current_selection == 0)
  {                            // Caso o botão voltar seja pressionado
    u8g2.userInterfaceMessage( // Exibir mensagem abaixo
        "Nada Selecionado.",
        "",
        "",
        " ok ");
  }
  //=====================FIM NAVEGACÃO============================

  //----------------TELA1 -  TELA DE EXEMPLO DE BARRA DE ICONES-----------------
  if (current_selection == 1)
  {
    menu_inicial = 0;
    u8g2.firstPage();
    do
    {
      int8_t event2 = u8g2.getMenuEvent();
      u8g2.drawRFrame(0, 11, 127, 51, 4);       // Desenha um quadro vazio com bordas  u8g2.drawRFrame(x,y,comprimento,altura,borda);
      u8g2.drawLine(0, 9, 127, 9);              // Desenha uma linha
      u8g2.drawXBM(0, 0, 8, 8, bluetooth_bits); // Desenha uma figura tipo XBM para o bluetooth
      u8g2.drawXBM(10, 0, 8, 8, wifi_bits);     // Desenha uma figura tipo XBM para o wifi
      if (event2 == U8X8_MSG_GPIO_MENU_HOME)
      {                   // Verifica se o botão de voltar/home foi pressionado.
        menu_inicial = 1; // Caso pressionado chama o menu principal
        current_selection = 0;
      }

    } while (u8g2.nextPage()); // Condição para manter a tela
    current_selection = 1;     // se caso current_selection estiver com o mesmo ID da tela a tela fica mantida (travada)
  }                            // Fim Tela1

  //----------------TELA2-----------------

  else if (current_selection == 2)
  {
    int8_t event2 = u8g2.getMenuEvent();
    menu_inicial = 0;
    u8g2.firstPage();
    do
    {
      leituraADC = analogRead(PinoAnalogico);     // Leitura de porta analógica e armazenamento na variável
      AnaRead = map(leituraADC, 0, 4095, 0, 120); // Faz uma equivalencia entre o maximo da porta analogica para a barra grafica
      u8g2.drawFrame(3, 10, 120, 8);              // Desenha um retangulo vazio para a barra gráfica
      u8g2.drawBox(3, 10, AnaRead, 8);            // Desenha um retangulo preenchido para a barra gráfica apartir do valor gerado do map
      u8g2.setCursor(0, 50);                      // Configura a posição do proximo item a ser escrito na tela
      u8g2.print(leituraADC);                     // Escreve o valor armazenado da porta analogica
      if (event2 == U8X8_MSG_GPIO_MENU_HOME)
      {                   // Verifica se o botão de voltar/home foi pressionado.
        menu_inicial = 1; // Caso pressionado chama o menu principal
        current_selection = 0;
      }
    } while (u8g2.nextPage()); // Condição para manter a tela
    current_selection = 2;     // se caso current_selection estiver com o mesmo ID da tela a tela fica mantida (travada)
  }                            // Fim Tela2

  //----------------TELA3-----------------

  else if (current_selection == 3)
  {
    menu_inicial = 0;
    u8g2.firstPage(); // buffer para exibição gráfica
    do
    {
      u8g2.drawFrame(1, 10, 126, 8);        // Desenha um retangulo vazio para a barra gráfica
      u8g2.drawBox(3, 10, ValorPWM / 2, 8); // Desenha um retangulo preenchido para a barra gráfica apartir do valor gerado do ValorPWM
      sigmaDeltaWrite(0, ValorPWM);         // Valor dado para a variável ValorPWM é enviado ao canal 0 de PWM
      u8g2.setCursor(0, 50);                // Configura a posição do proximo item a ser escrito na tela
      u8g2.print(ValorPWM);                 // Escreve o valor armazenado do PWM

      int8_t event2 = u8g2.getMenuEvent();
      if (event2 == U8X8_MSG_GPIO_MENU_NEXT || event2 == U8X8_MSG_GPIO_MENU_UP)
      {             // Verifica se o botão de subir/avançar foi pressionado.
        ValorPWM++; // Incrementa o ValorPWM aumentando o brilho do LED
      }
      if (event2 == U8X8_MSG_GPIO_MENU_PREV || event2 == U8X8_MSG_GPIO_MENU_DOWN)
      {             // Verifica se o botão de subir/avançar foi pressionado.
        ValorPWM--; // Incrementa o ValorPWM aumentando o brilho do LED
      }
      if (event2 == U8X8_MSG_GPIO_MENU_HOME)
      {                   // Verifica se o botão de voltar/home foi pressionado.
        menu_inicial = 1; // Caso pressionado chama o menu principal
        current_selection = 0;
      }
    } while (u8g2.nextPage()); // Condição para manter a tela
    current_selection = 3;     // se caso current_selection estiver com o mesmo ID da tela a tela fica mantida (travada)
  }                            // Fim Tela3
  else
  {
    u8g2.userInterfaceMessage(     // Caso o menu não possua nada para se exibir
        "A tela não possui nada:", // Exibir mensagem abaixo
        u8x8_GetStringLineStart(current_selection - 1, string_list),
        "",
        "OK \n Cancelar");
  }
}