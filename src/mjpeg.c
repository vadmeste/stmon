/**
  ******************************************************************************
  * @file    demo_video.c
  * @author  MCD Application Team
  * @version V1.0.1
  * @date    11-November-2013
  * @brief   Video Player functions
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
#include "Global.h"
#include "GUI_Type.h"
#include "GUI.h"
// #include "DIALOG.h"
#include "GUI_JPEG_Private.h"
// #include "file_utils.h"
// #include "str_utils.h"
// #include "Res/common_res.h"
#include "string.h"

/* External variables --------------------------------------------------------*/
extern __IO uint32_t TS_Orientation;

/* Private typedef -----------------------------------------------------------*/
/* Private defines -----------------------------------------------------------*/
#define ID_FRAMEWIN_VIDEO   (GUI_ID_USER + 0x00)
#define PLAY_BUTTON_VIDEO   (GUI_ID_USER + 0x03)
#define PREV_BUTTON_VIDEO   (GUI_ID_USER + 0x04)
#define NEXT_BUTTON_VIDEO   (GUI_ID_USER + 0x05)
#define STOP_BUTTON_VIDEO   (GUI_ID_USER + 0x06)
#define ID_VIDEO_FILE       (GUI_ID_USER + 0x07)
#define ID_VIDEO_FPS        (GUI_ID_USER + 0x08)
#define ID_VIDEO_PROGRESS   (GUI_ID_USER + 0x09)
#define ID_VIDEO_DURATION   (GUI_ID_USER + 0x0A)
#define ID_VIDEO_EXIT       (GUI_ID_USER + 0x0B)

#define VIDEO_IDLE      0
#define VIDEO_PLAY      1
#define VIDEO_STOP      2
#define VIDEO_PAUSE     3

#define VIDEO_BUFFER_SIZE   (512)

/* Private macros ------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
__IO uint32_t VIDEO_Enlarge = 0;

FIL Video_File;
FATFS fatfs;

char _acVideoBuffer[VIDEO_BUFFER_SIZE];
uint8_t flag = 0;
__IO int ImageOffset = 0;
uint8_t VideoPlayer_State = VIDEO_IDLE;
int t0, frame_average_period, frame_speed;
int s0, frame_average_size;
static int progress_bar = 0;
int duration = 0;
int do_clear = 0;
int time_start, current_time, time_pause, time_seek;

/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

// res = f_open(&Video_File, (char *)FILEMGR_FileList.file[FILEMGR_FileList.idx ].line, FA_OPEN_EXISTING | FA_READ);

/**
  * @brief  called one time at the beginning to retrieve overhead
  *         information and, after this, several times to retrieve the actual MJPEG data
  * @param  p: application defined void pointer
  *         ppData: pointer to the location the requested data resides in
  *         NumBytesReq: number of requested bytes.
  *         Off: defines the offset to use for reading the source data
  * @retval The number of bytes which were actually read (int)
  */
static int _GetData(void * p, const uint8_t ** ppData, unsigned NumBytesReq, uint32_t Off)
{
  unsigned int NumBytesRead;
  FIL * phFile;
  
  phFile = (FIL *)p;
  /*
  * Check buffer size
  */
  if (NumBytesReq > sizeof(_acVideoBuffer)) {
    NumBytesReq = sizeof(_acVideoBuffer);
  }
  /*
  * Set file pointer to the required position
  */
  f_lseek(phFile, Off + ImageOffset);
  /*
  * Read data into buffer
  */
  f_read(phFile, _acVideoBuffer, NumBytesReq, &NumBytesRead); 
  /*
  * Set data pointer to the beginning of the buffer
  */
  *ppData =(unsigned char *) _acVideoBuffer;
  /*
  * Return number of available bytes
  */
  return NumBytesRead;  
}


/**
  * @brief  draw the image after scaling it
  * @param  hFile: pointer to the MJPEG file
  * @retval None
  */
int _PlayMJPEG(FIL *hFile) 
{
  unsigned int  NumBytesRead;
  char *ptr;
  int timeout = 0xFFFF;
  static int counter = 0;
  GUI_JPEG_INFO video_info;

  f_lseek(hFile, ImageOffset);
  
  t0 = GUI_GetTime();
  s0 = hFile->fptr;
  
 
  if(VIDEO_Enlarge == 1)
  {
    GUI_JPEG_DrawEx(_GetData, hFile, 0, 0);
  }
  else
  {
    GUI_JPEG_DrawScaledEx(_GetData, hFile, 14, 35, 2, 3);  
  }
  
  if(counter ++ > 20 )
  {
    frame_average_size =  (hFile->fptr - s0 + frame_average_size)/2; 
    frame_average_period = (GUI_GetTime() - t0 + frame_average_period)/2;
    counter = 0;
  }
  
  if(frame_average_size != 0)
    frame_speed = frame_average_size / frame_average_period ;
  else
    frame_speed = 70;
  
  ImageOffset = hFile->fptr;
  
  while ( --timeout > 0 )
  {
    f_lseek(hFile, ImageOffset - VIDEO_BUFFER_SIZE);
    f_read(hFile, (char *)_acVideoBuffer, VIDEO_BUFFER_SIZE, &NumBytesRead); 
    
    /* Get image offset */
    ptr = _acVideoBuffer;
    while (NumBytesRead --)
    {
      if((*ptr == 0xFF) && ((*(ptr + 1)) == 0xd8))
      {
        ImageOffset -=  (NumBytesRead + 1) ;
        GUI_JPEG_GetInfoEx   (_GetData, hFile, &video_info);
        if( (video_info.XSize != 320) || (video_info.YSize != 240)  )
        {
          return -1; 
        }
        else
        {
          return 0;
        }
      }
      ptr++;
    }
    ImageOffset +=  2 * VIDEO_BUFFER_SIZE; 
    
    if (ImageOffset >= hFile->fsize)
    {
      return -1;
    }
    
  }
  return -1;
}

void gui_init() {

	int res;
	GPIO_InitTypeDef GPIO_InitStructure;

	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);

	/* Enable the BUTTON Clock */
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_GPIOA, ENABLE);

	/* Configure Button pin as Output */
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_OUT;
	GPIO_InitStructure.GPIO_OType = GPIO_OType_PP;
	GPIO_InitStructure.GPIO_PuPd = GPIO_PuPd_NOPULL;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0;
	GPIO_Init(GPIOA, &GPIO_InitStructure);

	/* Force capacity to be charged quickly */
	GPIO_WriteBit(GPIOA, GPIO_Pin_0, Bit_RESET);
	Delay (25);

	/* Check whether the test mode should be started */
	STM_EVAL_PBInit(BUTTON_USER, BUTTON_MODE_GPIO); 

	LowLevel_Init();
	GUI_Init();
	
}
