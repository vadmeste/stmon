
#include "docea_log.h"
#include "gpio_aflist.h"

/**
 ** @brief  Returns the current time and sub second.
 ** @param  Secondfraction: the sub second fraction.
 ** @param  RTC_TimeStructure : pointer to a RTC_TimeTypeDef structure that 
 **         contains the current time values. 
 ** @retval table : return current time and sub second in a table form
 **/
static Table_TypeDef RTC_Get_Time_StringTbl(uint32_t Secondfraction , RTC_TimeTypeDef* RTC_TimeStructure )
{
    Table_TypeDef table2;

    /* Fill the table2 fields with the current Time*/
    table2.tab[0]  = (((uint8_t)(RTC_TimeStructure->RTC_Hours & 0xF0) >> 0x04) + 0x30);
    table2.tab[1]  = (((uint8_t)(RTC_TimeStructure->RTC_Hours & 0x0F))+ 0x30);
    table2.tab[2]  = 0x3A;

    table2.tab[3]  = (((uint8_t)(RTC_TimeStructure->RTC_Minutes & 0xF0) >> 0x04) + 0x30);
    table2.tab[4]  =(((uint8_t)(RTC_TimeStructure->RTC_Minutes & 0x0F))+ (uint8_t)0x30);
    table2.tab[5]  = 0x3A;

    table2.tab[6]  = (((uint8_t)(RTC_TimeStructure->RTC_Seconds & 0xF0) >> 0x04)+ 0x30);
    table2.tab[7]  = (((uint8_t)(RTC_TimeStructure->RTC_Seconds & 0x0F)) + 0x30);
    table2.tab[8]  = '.';

    table2.tab[9] = (uint8_t)(((Secondfraction % 100000) / 10000) + 0x30);
    table2.tab[10] = (uint8_t)(((Secondfraction % 10000) / 1000) + 0x30);
    table2.tab[11] = (uint8_t)(((Secondfraction % 1000) / 100) + 0x30);
    table2.tab[12] = (uint8_t)(((Secondfraction % 100) / 10) + 0x30);
    table2.tab[13] = (uint8_t)((Secondfraction % 10) + 0x30);

    /* return table2 */
    return table2;
}

/**
 ** @brief  Returns the current time and sub second.
 ** @param  Secondfraction: the sub second fraction.
 ** @param  RTC_TimeStructure : pointer to a RTC_TimeTypeDef structure that 
 **         contains the current time values. 
 ** @retval table : return current time and sub second in a table form
 **/
static Table_TypeDef RTC_Get_Date_StringTbl(RTC_DateTypeDef* RTC_DateStructure )
{
    Table_TypeDef table2;

    /* Fill the table2 fields with the current Time*/
    table2.tab[0]  = (((uint8_t)(RTC_DateStructure->RTC_Date & 0xF0) >> 0x04) + 0x30);
    table2.tab[1]  = (((uint8_t)(RTC_DateStructure->RTC_Date & 0x0F))+ 0x30);
    table2.tab[2]  = 0x2F;

    table2.tab[3]  = (((uint8_t)(RTC_DateStructure->RTC_Month & 0xF0) >> 0x04) + 0x30);
    table2.tab[4]  =(((uint8_t)(RTC_DateStructure->RTC_Month & 0x0F))+ (uint8_t)0x30);
    table2.tab[5]  = 0x2F;

    table2.tab[6]  = (((uint8_t)(RTC_DateStructure->RTC_Year & 0xF0) >> 0x04)+ 0x30);
    table2.tab[7]  = (((uint8_t)(RTC_DateStructure->RTC_Year & 0x0F)) + 0x30);
    table2.tab[8]  = 0xA0;

    /* return table2 */
    return table2;
}


static void RTC_display(Table_TypeDef table, uint8_t max_idx )
{   
      uint8_t index = 0;
      
      for (index = 0;index < max_idx; index++)
        {
         char c = table.tab[index];
         if (c == '\0')
             break;
         // VCP_DataTx (&c, 1);
        }  
}

static void RTC_Time_display(Table_TypeDef table) {
    RTC_display(table, 14);
}

static void RTC_Date_display(Table_TypeDef table) {
    RTC_display(table, 8);
}


char* itoa(int val, int base){
        
      static char buf[32] = {0};
      int i = 30;
      for(; val && i ; --i, val /= base)
          buf[i] = "0123456789abcdef"[val % base];
      return &buf[i+1];
}


char* itoa_ms(int val, int base){
        
      static char buf[5] = {'0', '0', '0', '0', 0};
      int i = 3;
      for(; val && i ; --i, val /= base)
          buf[i] = "0123456789abcdef"[val % base];
      return &buf[1];
}

static void print_str(char* buf) {
    
    int i;
    for (i=0 ; buf[i] != NULL ; i++) {
        // VCP_DataTx(&buf[i], 1);
    }
}

static void quick_print(char* buf, int len) {
    
    int i;
    int grp = 32;
    int blks = len / grp;
    int rest = len % grp;

    for (i = 0 ; i < blks * grp ; i++) {
        // VCP_DataTx(&buf[i], grp);
    }

    for (i = len - rest ; i < len ; i++) {
        // VCP_DataTx(&buf[i], 1);
    }
}



void msg(char* buf) {

    uint32_t ss, subs;
    RTC_WaitForSynchro();

    RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
    RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
    ss = RTC_GetSubSecond();
    // subs = 100000 - ((uint32_t)((uint32_t)RTC_GetSubSecond() * 100000) / (uint32_t) RTC_InitStructure.RTC_SynchPrediv);
    subs = st_subsecond * 10;
    RTC_Date_display(RTC_Get_Date_StringTbl(&RTC_DateStructure));
    print_str("T");
    RTC_Time_display(RTC_Get_Time_StringTbl(subs , &RTC_TimeStructure));
    print_str(" ");
    print_str(buf);
    print_str("\r\n");
}

static void log_append_datetime(Table_TypeDef table, uint8_t max_idx )
{   
      uint8_t index = 0;
      
      for (index = 0;index < max_idx; index++) {
          char c = table.tab[index];
          if (c == '\0')
              break;
          cbWrite(&g_LogCB, (ElemType*) &c);
      }  

}


void
do_log_append(char *fmt, uint8_t show_timestamp, va_list va) {

    int i = 0;
    char sep = 'T', space = ' ';
    uint32_t ss, subs;

    if (! g_LogCB_enable) 
	    return;

    if (show_timestamp == 1) { 

        RTC_WaitForSynchro();

        RTC_GetTime(RTC_Format_BCD, &RTC_TimeStructure);
        RTC_GetDate(RTC_Format_BCD, &RTC_DateStructure);
        ss = RTC_GetSubSecond();
        subs = 100000 - ((uint32_t)((uint32_t)ss * 100000) / (uint32_t) RTC_InitStructure.RTC_SynchPrediv);

        Table_TypeDef date_tbl = RTC_Get_Date_StringTbl(&RTC_DateStructure);
        Table_TypeDef time_tbl = RTC_Get_Time_StringTbl(subs, &RTC_TimeStructure); 

        // Put timestamp first

        log_append_datetime(date_tbl, 8);
        cbWrite(&g_LogCB, &sep);
        log_append_datetime(time_tbl, 14);
        cbWrite(&g_LogCB, &space);
    }

    mini_vsnprintf(fmt, va);


}


void
log_append(char *fmt, ...)
{
    va_list va;

    __disable_irq();

    va_start(va, fmt);
    do_log_append(fmt, 1, va);
    va_end(va);

    __enable_irq();
}

void dirty_log_append(char* fmt, ...)
{
    va_list va;
    va_start(va, fmt);
    do_log_append(fmt, 1, va);
    va_end(va);
}


void
log_raw_append(char* fmt, ...) {

    va_list va;

    __disable_irq();

    va_start(va, fmt);
    do_log_append(fmt, 0, va);
    va_end(va);

    __enable_irq();

}

void

dirty_log_raw_append(char* fmt, ...) {
    
    va_list va;
    va_start(va, fmt);
    do_log_append(fmt, 0, va);
    va_end(va);
}

      
const char* af_list[] = { "RTC_50Hz/MCO/TAMPER/SWJ/TRACE",
    "TIM1/TIM2", 
    "TIM3/TIM4/TIM5",
    "TIM6/TIM8/TIM9/TIM10/TIM11"
    "I2C1/I2C2/I2C3", 
    "SPI1/SPI2/SPI4/SPI5/SPI6", 
    "SAI1/SPI3", 
    "I2S3ext/USART1/USART2/USART3"
    "UART4/UART5/USART6/UART7/UART8", 
    "CAN1/CAN2/TIM12/TIM13/TIM14", 
    "OTG_FS/OTG_HS",
    "ETH", 
    "FMC/OTG_HS_FS/SDIO",
    "DMCI", "LTDC", "EVENTOUT" };

void gpio_af_config_log(GPIO_TypeDef* GPIOx, uint16_t GPIO_PinSource, uint8_t GPIO_AF) {
    
    char* gpio_name = NULL;

    __disable_irq();

    dirty_log_append("docea_monitoring ");

#define print_gpio_conn(gp) \ 
    if ( GPIOx == gp) \
        dirty_log_raw_append("conn GPIO%c%d with %s", \
            #gp[4], GPIO_PinSource, gpio_aflist[#gp[4] - 'A'][GPIO_PinSource][GPIO_AF]);
 
    print_gpio_conn(GPIOA);
    print_gpio_conn(GPIOB);
    print_gpio_conn(GPIOC);
    print_gpio_conn(GPIOD);
    print_gpio_conn(GPIOE);
    print_gpio_conn(GPIOF);
    print_gpio_conn(GPIOG);
    print_gpio_conn(GPIOH);
    print_gpio_conn(GPIOI);
    print_gpio_conn(GPIOJ);
    print_gpio_conn(GPIOK);

    dirty_log_raw_append(" ");
    dirty_log_raw_append("\n"); 

    __enable_irq();
}

void print_default_gpios() {
    
    int i, af_i, af_level, pin;
    __IO uint32_t tmp;
    uint8_t af_val;

    __disable_irq();

    dirty_log_append("Already activated PIN config:\n");

#define gpio_current_conf(gp) \
        for (af_level = 0; af_level < 2 ; af_level++) { \
            if (gp == GPIOK && af_level > 0) break; \
            tmp = gp->AFR[af_level]; \
            for (af_i = 0 ; af_i < 8 ; af_i++) { \
              pin = af_level*8+af_i; \
              af_val = tmp & 0xF; \
              if (af_val != 0) \
                  dirty_log_append("[GPIO%c%d(%d,%d) => %s\n", #gp[4], pin, af_level, af_i, gpio_aflist[#gp[4]-'A'][pin][af_val]); \
              tmp = tmp >> 4; \
            } \
        } 

                  // dirty_log_append("GPIO%c, pin = %d, AF = %s", #gp[4], pin, gpio_aflist[#gp[4] - 'A'][pin][af_level*8+i]); 
    gpio_current_conf(GPIOA);
    gpio_current_conf(GPIOB);
    gpio_current_conf(GPIOC);
    gpio_current_conf(GPIOD);
    gpio_current_conf(GPIOE);
    gpio_current_conf(GPIOF);
    gpio_current_conf(GPIOG);
    gpio_current_conf(GPIOH);
    gpio_current_conf(GPIOI);
    gpio_current_conf(GPIOJ);
    gpio_current_conf(GPIOK);

    dirty_log_raw_append("\n");

   __enable_irq(); 

}

void clockupdate_log(RCC_BUS_TYPE bus, uint32_t periph, FunctionalState newstate) {

    char* bus_name = NULL;

    __disable_irq();

    dirty_log_append("docea_monitoring %s ", newstate == ENABLE ? "enabling" : "disabling");

    switch (bus) {
        case RCC_AHB1_BUS:
            dirty_log_raw_append("AHB1 ");
            if (periph & RCC_AHB1Periph_GPIOA) dirty_log_raw_append("GPIOA ");
            if (periph & RCC_AHB1Periph_GPIOB) dirty_log_raw_append("GPIOB ");
            if (periph & RCC_AHB1Periph_GPIOC) dirty_log_raw_append("GPIOC ");
            if (periph & RCC_AHB1Periph_GPIOD) dirty_log_raw_append("GPIOD ");
            if (periph & RCC_AHB1Periph_GPIOE) dirty_log_raw_append("GPIOE ");
            if (periph & RCC_AHB1Periph_GPIOF) dirty_log_raw_append("GPIOF ");
            if (periph & RCC_AHB1Periph_GPIOG) dirty_log_raw_append("GPIOG ");
            if (periph & RCC_AHB1Periph_GPIOH) dirty_log_raw_append("GPIOH ");
            if (periph & RCC_AHB1Periph_GPIOI) dirty_log_raw_append("GPIOI ");
            if (periph & RCC_AHB1Periph_GPIOJ) dirty_log_raw_append("GPIOJ ");
            if (periph & RCC_AHB1Periph_GPIOK) dirty_log_raw_append("GPIOK ");
            if (periph & RCC_AHB1Periph_CRC) dirty_log_raw_append("CRC ");
            if (periph & RCC_AHB1Periph_BKPSRAM) dirty_log_raw_append("BKPSRAM interface ");
            if (periph & RCC_AHB1Periph_CCMDATARAMEN) dirty_log_raw_append("CCM data RAM interface ");
            if (periph & RCC_AHB1Periph_DMA1) dirty_log_raw_append("DMA1 ");
            if (periph & RCC_AHB1Periph_DMA2) dirty_log_raw_append("DMA2 ");
            if (periph & RCC_AHB1Periph_DMA2D) dirty_log_raw_append("DMA2D ");
            if (periph & RCC_AHB1Periph_ETH_MAC) dirty_log_raw_append("Ethernet MAC ");
            if (periph & RCC_AHB1Periph_ETH_MAC_Tx) dirty_log_raw_append("Ethernet Transmission ");
            if (periph & RCC_AHB1Periph_ETH_MAC_Rx ) dirty_log_raw_append("Ethernet Reception ");
            if (periph & RCC_AHB1Periph_ETH_MAC_PTP) dirty_log_raw_append("Ethernet PTP ");
            if (periph & RCC_AHB1Periph_OTG_HS) dirty_log_raw_append("USB OTG HS ");
            if (periph & RCC_AHB1Periph_OTG_HS_ULPI) dirty_log_raw_append("USB OTG HS ULPI ");
           break; 

        case RCC_AHB2_BUS:
           dirty_log_raw_append("AHB2 ");
           if (periph & RCC_AHB2Periph_DCMI) dirty_log_raw_append("DCMI ");
           if (periph & RCC_AHB2Periph_CRYP) dirty_log_raw_append("CRYP ");
           if (periph & RCC_AHB2Periph_HASH) dirty_log_raw_append("HASH ");
           if (periph & RCC_AHB2Periph_RNG) dirty_log_raw_append("RNG ");
           if (periph & RCC_AHB2Periph_OTG_FS) dirty_log_raw_append("USB OTG FS ");
           break;

        case RCC_AHB3_BUS:
           dirty_log_raw_append("AHB3 ");
// if (periph & RCC_AHB3Periph_FSMC) dirty_log_raw_append("FSMC ");
           if (periph & RCC_AHB3Periph_FMC) dirty_log_raw_append("FMC ");
           break;

        case RCC_APB1_BUS:
           dirty_log_raw_append("APB1 ");
           if (periph & RCC_APB1Periph_TIM2) dirty_log_raw_append("TIM2 ");
           if (periph & RCC_APB1Periph_TIM3) dirty_log_raw_append("TIM3 ");
           if (periph & RCC_APB1Periph_TIM4) dirty_log_raw_append("TIM4 ");
           if (periph & RCC_APB1Periph_TIM5) dirty_log_raw_append("TIM5 ");
           if (periph & RCC_APB1Periph_TIM6) dirty_log_raw_append("TIM6 ");
           if (periph & RCC_APB1Periph_TIM7) dirty_log_raw_append("TIM7 ");
           if (periph & RCC_APB1Periph_TIM12) dirty_log_raw_append("TIM12 ");
           if (periph & RCC_APB1Periph_TIM13) dirty_log_raw_append("TIM13 ");
           if (periph & RCC_APB1Periph_TIM14) dirty_log_raw_append("TIM14 ");
           if (periph & RCC_APB1Periph_WWDG) dirty_log_raw_append("WWDG ");
           if (periph & RCC_APB1Periph_SPI2) dirty_log_raw_append("SPI2 ");
           if (periph & RCC_APB1Periph_SPI3) dirty_log_raw_append("SPI3 ");
           if (periph & RCC_APB1Periph_USART2) dirty_log_raw_append("USART2 ");
           if (periph & RCC_APB1Periph_USART3) dirty_log_raw_append("USART3 ");
           if (periph & RCC_APB1Periph_UART4) dirty_log_raw_append("UART4 ");
           if (periph & RCC_APB1Periph_UART5) dirty_log_raw_append("UART5 ");
           if (periph & RCC_APB1Periph_I2C1) dirty_log_raw_append("I2C1 ");
           if (periph & RCC_APB1Periph_I2C2) dirty_log_raw_append("I2C2 ");
           if (periph & RCC_APB1Periph_I2C3) dirty_log_raw_append("I2C3 ");
           if (periph & RCC_APB1Periph_CAN1) dirty_log_raw_append("CAN1 ");
           if (periph & RCC_APB1Periph_CAN2) dirty_log_raw_append("CAN2 ");
           if (periph & RCC_APB1Periph_PWR) dirty_log_raw_append("PWR ");
           if (periph & RCC_APB1Periph_DAC) dirty_log_raw_append("DAC ");
           if (periph & RCC_APB1Periph_UART7) dirty_log_raw_append("UART7 ");
           if (periph & RCC_APB1Periph_UART8) dirty_log_raw_append("UART8 ");
           break;

        case RCC_APB2_BUS:
           dirty_log_raw_append("APB2 ");
           if (periph & RCC_APB2Periph_TIM1) dirty_log_raw_append("TIM1 ");
           if (periph & RCC_APB2Periph_TIM8) dirty_log_raw_append("TIM8 ");
           if (periph & RCC_APB2Periph_USART1) dirty_log_raw_append("USART1 ");
           if (periph & RCC_APB2Periph_USART6) dirty_log_raw_append("USART6 ");
           if (periph & RCC_APB2Periph_ADC1) dirty_log_raw_append("ADC1 ");
           if (periph & RCC_APB2Periph_ADC2) dirty_log_raw_append("ADC2 ");
           if (periph & RCC_APB2Periph_ADC3) dirty_log_raw_append("ADC3 ");
           if (periph & RCC_APB2Periph_SDIO) dirty_log_raw_append("SDIO ");
           if (periph & RCC_APB2Periph_SPI1) dirty_log_raw_append("SPI1 ");
           if (periph & RCC_APB2Periph_SPI4) dirty_log_raw_append("SPI4 ");
           if (periph & RCC_APB2Periph_SYSCFG) dirty_log_raw_append("SYSCFG ");
           if (periph & RCC_APB2Periph_TIM9) dirty_log_raw_append("TIM9 ");
           if (periph & RCC_APB2Periph_TIM10) dirty_log_raw_append("TIM10 ");
           if (periph & RCC_APB2Periph_TIM11) dirty_log_raw_append("TIM11 ");
           if (periph & RCC_APB2Periph_SPI5) dirty_log_raw_append("SPI5 ");
           if (periph & RCC_APB2Periph_SPI6) dirty_log_raw_append("SPI6 ");
           if (periph & RCC_APB2Periph_SAI1) dirty_log_raw_append("SAI1 ");
           if (periph & RCC_APB2Periph_LTDC) dirty_log_raw_append("LTDC ");
           break;
    }
    dirty_log_raw_append("clock\n");
    
    __enable_irq();
} 


void print_log() {

    char nl = '\n';
    char c = '.';

    while (!cbIsEmpty(&g_LogCB)) {
        cbRead(&g_LogCB, &c); 
        // VCP_DataTx(&c, 1);   
    }
    // VCP_DataTx(&nl, 1);   
}

