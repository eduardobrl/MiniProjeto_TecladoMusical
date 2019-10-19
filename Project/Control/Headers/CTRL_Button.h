#ifndef __CTRL_LEDS_H__
#define __CTRL_LEDS_H__

    void CTRL_Button_Task(void);
    void CTRL_Button_Init(void);

  #define  P7_PINS              (P7 | 192)
  //#define  (P5_PINS)               ( (P5_bit.no0 << 7) | (63) )
    
    
  #define   BUTTON_BITS         ( P7_PINS & P5_PINS) | 128
    
#endif