/**
  ******************************************************************************
  * @file    LTDC_AnimatedPictureFromUSB/stm32fxxx_it.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   This file includes the interrupt handlers for the application
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
#include "usb_bsp.h"
#include "usb_hcd_int.h"
#include "usbh_core.h"
#include "stm32fxxx_it.h"

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/

extern USB_OTG_CORE_HANDLE          USB_OTG_Core;
extern USBH_HOST                    USB_Host;
 
/* Private function prototypes -----------------------------------------------*/
extern void USB_OTG_BSP_TimerIRQ (void);

/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*             Cortex-M Processor Exceptions Handlers                         */
/******************************************************************************/
/**
  * @brief  NMI_Handler
  *         This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  HardFault_Handler
  *         This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
  /* Go to infinite loop when Hard Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  MemManage_Handler
  *         This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
  /* Go to infinite loop when Memory Manage exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  BusFault_Handler
  *         This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
  /* Go to infinite loop when Bus Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  UsageFault_Handler
  *         This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
  /* Go to infinite loop when Usage Fault exception occurs */
  while (1)
  {
  }
}

/**
  * @brief  SVC_Handler
  *         This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  DebugMon_Handler
  *         This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}


/**
  * @brief  PendSV_Handler
  *         This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  EXTI1_IRQHandler
  *         This function handles External line 1 interrupt request.
  * @param  None
  * @retval None
  */
void EXTI1_IRQHandler(void)
{
  if(EXTI_GetITStatus(EXTI_Line1) != RESET)
  {
      USB_Host.usr_cb->OverCurrentDetected();
      EXTI_ClearITPendingBit(EXTI_Line1);
  }
}

/**
 * @brief FLASH handler
 **/

void FLASH_IRQHandler(void)
{
    if (FLASH_GetStatus() == FLASH_COMPLETE)
    {
        log_append("Flash operation successful\n");
    }
}

void SPI5_IRQHandler(SPI_TypeDef* SPIChannel) {

    log_append("SPI : activity\n");
    // if (SPI_I2S_GetITStatus(SPIChannel, SPI_I2S_IT_TXE) != RESET) {
    //    SPI_I2S_ClearITPendingBit(SPIChannel, SPI_I2S_IT_TXE);
    // } 
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
  * @brief  OTG_FS_IRQHandler
  *          This function handles USB-On-The-Go FS global interrupt request.
  *          requests.
  * @param  None
  * @retval None
  */

void OTG_HS_IRQHandler(void)
{
  USBH_OTG_ISR_Handler(&USB_OTG_Core);
}

void generic_dma_handler(void) {
    log_append("DMA_activity\n");
}

void DMA1_Stream0_IRQHandler(void) 
{
    generic_dma_handler();
}
void DMA1_Stream1_IRQHandler(void) {
    generic_dma_handler();
}
void DMA1_Stream2_IRQHandler(void) {
    generic_dma_handler();
}
void DMA1_Stream3_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA1_Stream4_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA1_Stream5_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA1_Stream6_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA1_Stream7_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream0_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream1_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream2_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream3_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream4_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream5_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream6_IRQHandler(void) {
    generic_dma_handler();
}
void   DMA2_Stream7_IRQHandler(void){
    generic_dma_handler();
}

void SysTick_Handler(void)
{
	// STM_EVAL_LEDToggle(LED3);
	// log_append("Timer INTR\n");
	TimingDelay_Decrement();
}
    
void DMA2D_IRQHandler(void) {

    if (DMA2D_GetITStatus(DMA2D_IT_TC) != RESET)
    {
        log_append("DMA2D_completed\n");
        DMA2D_ClearITPendingBit(DMA2D_IT_TC);
    }

}


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
