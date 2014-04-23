
#ifndef __DOCEA_LOG_H_
#define __DOCEA_LOG_H_


#include "stm32f4xx_rcc.h" 
#include "stm32f4xx_rtc.h" 

#include <stdarg.h>
#include <stdlib.h>
#include <stdint.h>
#include "cbuf.h"
#include "rtc.h"

typedef struct 
{
      uint8_t tab[32];
} Table_TypeDef;


CircularBuffer  g_LogCB;
uint8_t g_LogCB_enable;

short st_subsecond;


void msg(char* buf);

void do_log_append(char *fmt, uint8_t show_timestamp, va_list va);
void log_append(char *fmt, ...);
void dirty_log_append(char* fmt, ...);
void log_raw_append(char* fmt, ...);
void dirty_log_raw_append(char* fmt, ...);
void print_log();

void clockupdate_log(RCC_BUS_TYPE bus, uint32_t periph, FunctionalState newstate);
void gpio_af_config_log(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF);
void print_default_gpios();



#endif
