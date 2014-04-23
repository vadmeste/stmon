/**
  ******************************************************************************
  * @file    stm32xxx_it.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Exceptions Handlers
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2013 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software 
  * distributed under the License is distributed on an "AS IS" BASIS, 
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "global_includes.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"

#include "usb_core.h"
#include "usbd_core.h"
// #include "usbd_hid_core.h"
#include "usb_conf.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
extern __IO uint32_t TS_Pressed;
extern uint32_t demo_mode;
extern USB_OTG_CORE_HANDLE           USB_OTG_Core;
extern USB_OTG_CORE_HANDLE           USB_OTG_dev;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
__IO uint32_t ButtonPressed = 0x00;
extern uint32_t USBD_OTG_ISR_Handler (USB_OTG_CORE_HANDLE *pdev);
extern __IO uint32_t PeriodValue;
extern __IO uint32_t CaptureNumber;
uint16_t tmpCC4[2] = {0, 0};
/* Private function prototypes -----------------------------------------------*/
extern void USB_OTG_BSP_TimerIRQ (void);
extern void xPortSysTickHandler( void );
extern void LTDC_ISR_Handler(void);
extern void DMA2D_ISR_Handler(void);
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {}
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{}

/**
  * @brief  This function handles PendSV_Handler exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
    
    log_append("docea_monitoring systick event\n");
    if(demo_mode == 0)
        xPortSysTickHandler();
    else  
        TimingDelay_Decrement();
}

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
void EXTI0_IRQHandler(void)
{ 
  ButtonPressed = 0x01;
  
  EXTI_ClearITPendingBit(USER_BUTTON_EXTI_LINE);
}
/**
  * @brief  TIM2_IRQHandler
  *         This function handles Timer2 Handler.
  * @param  None
  * @retval None
  */

void TIM2_IRQHandler(void)
{
  USB_OTG_BSP_TimerIRQ();
}

/**
  * @brief  This function handles OTG_HS Handler.
  * @param  None
  * @retval None
  */
void OTG_HS_IRQHandler(void)
{
  {
    USBH_OTG_ISR_Handler (&USB_OTG_Core);
  }
}

/**
  * @brief  This function handles TIM5 global interrupt request.
  * @param  None
  * @retval None
  */
void TIM5_IRQHandler(void)
{
 /* if (TIM_GetITStatus(TIM5, TIM_IT_CC4) != RESET)
  {    
    tmpCC4[CaptureNumber++] = TIM_GetCapture4(TIM5);
   
    TIM_ClearITPendingBit(TIM5, TIM_IT_CC4);

    if (CaptureNumber >= 2)
    {
      PeriodValue = (uint16_t)(0xFFFF - tmpCC4[0] + tmpCC4[1] + 1);
    }
  } */
}

/**
  * @brief  This function handles DMA2 Stream1 global interrupt request.
  * @param  None
  * @retval None
  */

void DMA2_Stream1_IRQHandler(void)
{
}

/**
  * @brief  This function handles LTDC global interrupt request.
  * @param  None
  * @retval None
  */
void LTDC_IRQHandler(void)
{
  LTDC_ISR_Handler();
}

/**
  * @brief  This function handles DMA2D global interrupt request.
  * @param  None
  * @retval None
  */
void DMA2D_IRQHandler(void)
{
  if (DMA2D_GetITStatus(DMA2D_IT_TC) != RESET)
  {
    log_append("docea_monitoring graphic_state idle\n");
  }

  DMA2D_ISR_Handler();
}

void RTC_WKUP_IRQHandler(void)
{
    if(RTC_GetITStatus(RTC_IT_WUT) != RESET)
    {
        /* Toggle LED4 */
        STM_EVAL_LEDToggle(LED4);
        RTC_ClearITPendingBit(RTC_IT_WUT);
        EXTI_ClearITPendingBit(EXTI_Line22);
    }
}


static super(char * buf) {
	log_append(buf);
}


void WWDG_IRQHandler(void) { super("WWDG_IRQHandler\n"); }
void PVD_IRQHandler(void) { super("PVD_IRQHandler\n"); }
void TAMP_STAMP_IRQHandler(void) { super("TAMP_STAMP_IRQHandler\n"); }
void FLASH_IRQHandler(void) { super("FLASH_IRQHandler\n"); }
void RCC_IRQHandler(void) { super("RCC_IRQHandler\n"); }
void EXTI1_IRQHandler(void) { super("EXTI1_IRQHandler\n"); }
void EXTI2_IRQHandler(void) { super("EXTI2_IRQHandler\n"); }
void EXTI3_IRQHandler(void) { super("EXTI3_IRQHandler\n"); }
void DMA1_Stream0_IRQHandler(void) { super("DMA1_Stream0_IRQHandler\n"); }
void DMA1_Stream1_IRQHandler(void) { super("DMA1_Stream1_IRQHandler\n"); }
void DMA1_Stream2_IRQHandler(void) { super("DMA1_Stream2_IRQHandler\n"); }
void DMA1_Stream3_IRQHandler(void) { super("DMA1_Stream3_IRQHandler\n"); }
void DMA1_Stream4_IRQHandler(void) { super("DMA1_Stream4_IRQHandler\n"); }
void DMA1_Stream5_IRQHandler(void) { super("DMA1_Stream5_IRQHandler\n"); }
void DMA1_Stream6_IRQHandler(void) { super("DMA1_Stream6_IRQHandler\n"); }
void ADC_IRQHandler(void) { super("ADC_IRQHandler\n"); }
void CAN1_TX_IRQHandler(void) { super("CAN1_TX_IRQHandler\n"); }
void CAN1_RX0_IRQHandler(void) { super("CAN1_RX0_IRQHandler\n"); }
void CAN1_RX1_IRQHandler(void) { super("CAN1_RX1_IRQHandler\n"); }
void CAN1_SCE_IRQHandler(void) { super("CAN1_SCE_IRQHandler\n"); }
void EXTI9_5_IRQHandler(void) { super("EXTI9_5_IRQHandler\n"); }
void TIM1_BRK_TIM9_IRQHandler(void) { super("TIM1_BRK_TIM9_IRQHandler\n"); }
void TIM1_UP_TIM10_IRQHandler(void) { super("TIM1_UP_TIM10_IRQHandler\n"); }
void TIM1_TRG_COM_TIM11_IRQHandler(void) { super("TIM1_TRG_COM_TIM11_IRQHandler\n"); }
void TIM1_CC_IRQHandler(void) { super("TIM1_CC_IRQHandler\n"); }
void TIM3_IRQHandler(void) { super("TIM3_IRQHandler\n"); }
void TIM4_IRQHandler(void) { super("TIM4_IRQHandler\n"); }
void I2C1_EV_IRQHandler(void) { super("I2C1_EV_IRQHandler\n"); }
void I2C1_ER_IRQHandler(void) { super("I2C1_ER_IRQHandler\n"); }
void I2C2_EV_IRQHandler(void) { super("I2C2_EV_IRQHandler\n"); }
void I2C2_ER_IRQHandler(void) { super("I2C2_ER_IRQHandler\n"); }
void SPI1_IRQHandler(void) { super("SPI1_IRQHandler\n"); }
void SPI2_IRQHandler(void) { super("SPI2_IRQHandler\n"); }
void USART1_IRQHandler(void) { super("USART1_IRQHandler\n"); }
void USART2_IRQHandler(void) { super("USART2_IRQHandler\n"); }
void USART3_IRQHandler(void) { super("USART3_IRQHandler\n"); }
void EXTI15_10_IRQHandler(void) { super("EXTI15_10_IRQHandler\n"); }
void RTC_Alarm_IRQHandler(void) { super("RTC_Alarm_IRQHandler\n"); }
void OTG_FS_WKUP_IRQHandler(void) { super("OTG_FS_WKUP_IRQHandler\n"); }
void TIM8_BRK_TIM12_IRQHandler(void) { super("TIM8_BRK_TIM12_IRQHandler\n"); }
void TIM8_UP_TIM13_IRQHandler(void) { super("TIM8_UP_TIM13_IRQHandler\n"); }
void TIM8_TRG_COM_TIM14_IRQHandler(void) { super("TIM8_TRG_COM_TIM14_IRQHandler\n"); }
void TIM8_CC_IRQHandler(void) { super("TIM8_CC_IRQHandler\n"); }
void DMA1_Stream7_IRQHandler(void) { super("DMA1_Stream7_IRQHandler\n"); }
void FSMC_IRQHandler(void) { super("FSMC_IRQHandler\n"); }
void SDIO_IRQHandler(void) { super("SDIO_IRQHandler\n"); }
void SPI3_IRQHandler(void) { super("SPI3_IRQHandler\n"); }





/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
