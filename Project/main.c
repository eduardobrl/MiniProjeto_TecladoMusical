/**************************************************
* Centro Universitario FEI
* Disciplina EL9630 / NEA630
* Mini Projeto Dirigido 02 – Timer / Interrupções – Agosto / 2017 rev03
* Silvana Gasparotto de Souza
**************************************************/
#include "ior5f100le.h"
#include "ior5f100le_ext.h"
#include "intrinsics.h"
#include "myRL78.h"
#include "Modules.h"
#include "CTRL_Button.h"
#include "Hardware.h"

#define SW(x)        !( 1 << x)
#define NUM_DE_BOTOES    7

// Configura watchdog = desligado
#pragma location = "OPTBYTE"
__root __far const char opbyte0 = WDT_OFF;
// Configura detector de baixa tensão = desligado
#pragma location = "OPTBYTE"
__root __far const char opbyte1 = LVD_OFF;
// oscilador 32MHz flash high speed
#pragma location = "OPTBYTE"
__root __far const char opbyte2 = FLASH_HS | CLK_32MHZ;
// debug ativado, com apagamento em caso de falha de autenticação
#pragma location = "OPTBYTE"
__root __far const char opbyte3 = DEBUG_ON_ERASE;
// Configura security ID
#pragma location = "SECUID"
__root __far const char senha[10] = {0,0,0,0,0,0,0,0,0,0};
#pragma vector = INTIT_vect // escreve função de interrupção no vetor do IT

__interrupt void trata_IT(void)
{
P7_bit.no7 = !P7_bit.no7; // inverte o estado do led
}
void nota_musical (int CNT);

typedef enum{
  SW0 = 0xFE,
  SW1 = 0XFD,
  SW2 = 0XFB,
  SW3 = 0XF7,
  SW4 = 0XEF,
  SW5 = 0XDF,
  SW6 = 0XBF,
  NONE = 0xFF
} BUTTON_STATE_MACHINE;



typedef struct {
  BUTTON_STATE_MACHINE SWX;
  int freq;
} notas;

const notas Notas[NUM_DE_BOTOES] = {
  {SW0, 30},
  {SW1, 35},
  {SW2, 40},
  {SW3, 53},
  {SW4, 29},
  {SW5, 63},
  {SW6, 15},
};


void init_oscilator(){
  CMC = 0; // desativa osciladores X1 e XT1
  OSMC = 0x10; // configura o LOCO (15kHz) como fonte de clock do IT/RTC
  RTCEN = 1; // habilita o RTC e o IT
  
}

#define BUTOES_PINOS    P7    
#define BUZZER          P7_bit.no7

void main( void )
{
  init_oscilator();
  PU7 = P7_PULL;
  PU5 = P5_PULL;
  
  PM7 = P7_MODE;
  PM5 = P5_MODE;
  
  P7 = P7_PORT;
  P5 = P5_PORT;
  
  while (1)
  {
    int botao;
    int read;
    read = (P5_bit.no0 << 6);
    read = (BUTTON_STATE_MACHINE) ( (P7 | 128 | 64) & ( read | 191 ));
    
    
    for (botao = 0; botao < NUM_DE_BOTOES; botao++){
      
      if( read == Notas[botao].SWX ){
        nota_musical(Notas[botao].freq);

        while(read == Notas[botao].SWX ) read = (BUTTON_STATE_MACHINE)(P7 | 128 | 64);
      }
      else{
        BUZZER = 0;
        __disable_interrupt();// desabilita as interrupções do RL78
      }
      
      
    }
    
      }
  
}



  void nota_musical (int CNT)
  {
    ITMC = 0x8000 | CNT;
    /* RINTE=1 p habilitar a interrupção do o IT e configura o timer p gerar interrupções na frequência especificada pelo valor de CNT */
    ITMK = 0; // habilita a interrupção do IT
    __enable_interrupt(); // habilita as interrupções do RL78
  }
