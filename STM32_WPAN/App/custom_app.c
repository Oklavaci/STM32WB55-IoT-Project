/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    App/custom_app.c
  * @author  MCD Application Team
  * @brief   Custom Example Application (Server)
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2022 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "app_common.h"
#include "dbg_trace.h"
#include "ble.h"
#include "custom_app.h"
#include "custom_stm.h"
#include "stm32_seq.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "OV7670_wb55_lib.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
typedef struct
{
  /* MyP2PS */
  uint8_t               Switch_c_Notification_Status;
  /* USER CODE BEGIN CUSTOM_APP_Context_t */
  uint8_t               SW1_Status;
  uint8_t               SW2_Status;
  uint8_t               val;
  /* USER CODE END CUSTOM_APP_Context_t */

  uint16_t              ConnectionHandle;
} Custom_App_Context_t;

/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private defines ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macros -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/**
 * START of Section BLE_APP_CONTEXT
 */

PLACE_IN_SECTION("BLE_APP_CONTEXT") static Custom_App_Context_t Custom_App_Context;

/**
 * END of Section BLE_APP_CONTEXT
 */

uint8_t UpdateCharData[247];
uint8_t NotifyCharData[247];

/* USER CODE BEGIN PV */
uint8_t i = 0;
uint8_t TIMER_FLAG=0;
uint8_t GET_FRAME_FLAG=0;

uint8_t testo[100]={0};		//test array of zeros
uint16_t header,footer;     //header and footer bits of image
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* MyP2PS */
static void Custom_Switch_c_Update_Char(void);
static void Custom_Switch_c_Send_Notification(void);

/* USER CODE BEGIN PFP */
static void GetFrameAction(void);
/* USER CODE END PFP */

/* Functions Definition ------------------------------------------------------*/
void Custom_STM_App_Notification(Custom_STM_App_Notification_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_1 */

  /* USER CODE END CUSTOM_STM_App_Notification_1 */
  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* USER CODE END CUSTOM_STM_App_Notification_Custom_Evt_Opcode */

    /* MyP2PS */
    case CUSTOM_STM_LED_C_READ_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LED_C_READ_EVT */

      /* USER CODE END CUSTOM_STM_LED_C_READ_EVT */
      break;

    case CUSTOM_STM_LED_C_WRITE_EVT:
      /* USER CODE BEGIN CUSTOM_STM_LED_C_WRITE_EVT */

      /* USER CODE END CUSTOM_STM_LED_C_WRITE_EVT */
      break;

    case CUSTOM_STM_SWITCH_C_NOTIFY_ENABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_SWITCH_C_NOTIFY_ENABLED_EVT */
    	APP_DBG_MSG("\r\n\r** CUSTOM_STM_BUTTON_C_NOTIFY_ENABLED_EVT \n");

    	      Custom_App_Context.Switch_c_Notification_Status = 1;
      /* USER CODE END CUSTOM_STM_SWITCH_C_NOTIFY_ENABLED_EVT */
      break;

    case CUSTOM_STM_SWITCH_C_NOTIFY_DISABLED_EVT:
      /* USER CODE BEGIN CUSTOM_STM_SWITCH_C_NOTIFY_DISABLED_EVT */
    	APP_DBG_MSG("\r\n\r** CUSTOM_STM_BUTTON_C_NOTIFY_DISABLED_EVT \n");

    	      Custom_App_Context.Switch_c_Notification_Status = 0;
      /* USER CODE END CUSTOM_STM_SWITCH_C_NOTIFY_DISABLED_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_STM_App_Notification_default */

      /* USER CODE END CUSTOM_STM_App_Notification_default */
      break;
  }
  /* USER CODE BEGIN CUSTOM_STM_App_Notification_2 */

  /* USER CODE END CUSTOM_STM_App_Notification_2 */
  return;
}

void Custom_APP_Notification(Custom_App_ConnHandle_Not_evt_t *pNotification)
{
  /* USER CODE BEGIN CUSTOM_APP_Notification_1 */

  /* USER CODE END CUSTOM_APP_Notification_1 */

  switch (pNotification->Custom_Evt_Opcode)
  {
    /* USER CODE BEGIN CUSTOM_APP_Notification_Custom_Evt_Opcode */

    /* USER CODE END P2PS_CUSTOM_Notification_Custom_Evt_Opcode */
    case CUSTOM_CONN_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_CONN_HANDLE_EVT */

      /* USER CODE END CUSTOM_CONN_HANDLE_EVT */
      break;

    case CUSTOM_DISCON_HANDLE_EVT :
      /* USER CODE BEGIN CUSTOM_DISCON_HANDLE_EVT */

      /* USER CODE END CUSTOM_DISCON_HANDLE_EVT */
      break;

    default:
      /* USER CODE BEGIN CUSTOM_APP_Notification_default */

      /* USER CODE END CUSTOM_APP_Notification_default */
      break;
  }

  /* USER CODE BEGIN CUSTOM_APP_Notification_2 */

  /* USER CODE END CUSTOM_APP_Notification_2 */

  return;
}

void Custom_APP_Init(void)
{
  /* USER CODE BEGIN CUSTOM_APP_Init */
	//UTIL_SEQ_RegTask(1<< CFG_TASK_SW1_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, Button1Task);
	//UTIL_SEQ_RegTask(1<< CFG_TASK_SW2_BUTTON_PUSHED_ID, UTIL_SEQ_RFU, Button2Task);
	UTIL_SEQ_RegTask(1<< CFG_TASK_TIMER_EVT_ID, UTIL_SEQ_RFU, GetFrameAction);
	UTIL_SEQ_RegTask(1<< CFG_TASK_STREAM, UTIL_SEQ_RFU, Custom_Switch_c_Send_Notification);

	  Custom_App_Context.Switch_c_Notification_Status = 0;
	  Custom_App_Context.SW1_Status = 0;
	  Custom_App_Context.SW2_Status = 0;

  /* USER CODE END CUSTOM_APP_Init */
  return;
}

/* USER CODE BEGIN FD */

/* USER CODE END FD */

/*************************************************************
 *
 * LOCAL FUNCTIONS
 *
 *************************************************************/

/* MyP2PS */
void Custom_Switch_c_Update_Char(void) /* Property Read */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Switch_c_UC_1*/

  /* USER CODE END Switch_c_UC_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SWITCH_C, (uint8_t *)UpdateCharData);
  }

  /* USER CODE BEGIN Switch_c_UC_Last*/

  /* USER CODE END Switch_c_UC_Last*/
  return;
}

void Custom_Switch_c_Send_Notification(void) /* Property Notification */
{
  uint8_t updateflag = 0;

  /* USER CODE BEGIN Switch_c_NS_1*/

   header = 0xBEBE;
   footer = 0xDEDE;
   APP_DBG_MSG("\r\nNOTIFYING \n");

  	  if(Custom_App_Context.Switch_c_Notification_Status)
  		{
  		      P2PS_NOTIFY_INT16(P2P_NOTIFY_CHAR_UUID, &header, 2);
  		      HAL_Delay(10);
  		              for(uint16_t k = 0; k<320*120;k=k+100){
  		            	  P2PS_NOTIFY_INT8(P2P_NOTIFY_CHAR_UUID, (uint8_t *)frame_buffer0+k, 100);
  		  				//HAL_Delay(1);
  		              }
  		      APP_DBG_MSG("\r\n OK");
  		              for(uint16_t k = 0; k<320*120;k=k+100){
  		            	  P2PS_NOTIFY_INT8(P2P_NOTIFY_CHAR_UUID, (uint8_t *)frame_buffer1+k, 100);
  		                  //HAL_Delay(1);
  		                  }
  		      APP_DBG_MSG("\r\n OK");
  		              for(uint16_t k = 0; k<320*120;k=k+100){
  		            	  P2PS_NOTIFY_INT8(P2P_NOTIFY_CHAR_UUID, (uint8_t *)frame_buffer2+k, 100);
  		                  //HAL_Delay(1);
  		                  }
  		      APP_DBG_MSG("\r\n OK");
  		              for(uint16_t k = 0; k<320*120;k=k+100){
  		            	  P2PS_NOTIFY_INT8(P2P_NOTIFY_CHAR_UUID, (uint8_t *)frame_buffer3+k, 101);
  		                  //HAL_Delay(1);
  		                  }
  		      APP_DBG_MSG("\r\n OK \r\n END");
  		      HAL_Delay(10);
  		      P2PS_NOTIFY_INT16(P2P_NOTIFY_CHAR_UUID, &footer, 2);

  		      GET_FRAME_FLAG=1;
  		}

  	 else
  	 {
  	   APP_DBG_MSG("-- CUSTOM APPLICATION : CAN'T INFORM CLIENT -  NOTIFICATION DISABLED\n");
  	 }
  /* USER CODE END Switch_c_NS_1*/

  if (updateflag != 0)
  {
    Custom_STM_App_Update_Char(CUSTOM_STM_SWITCH_C, (uint8_t *)NotifyCharData);
  }

  /* USER CODE BEGIN Switch_c_NS_Last*/
  /* USER CODE END Switch_c_NS_Last*/

  return;
}

/* USER CODE BEGIN FD_LOCAL_FUNCTIONS*/
void GetFrameAction(void) /* Property Notification */
{
  APP_DBG_MSG("\r\n Image has been capturing \n");
  //get_frame();
  GET_FRAME_FLAG=0;
  UTIL_SEQ_PauseTask(1<<CFG_TASK_TIMER_EVT_ID);
  UTIL_SEQ_SetTask(1<<CFG_TASK_STREAM, CFG_SCH_PRIO_0);
  return;
}


void P2PS_APP_SW1_Button_Action(void){
	UTIL_SEQ_PauseTask(1<<CFG_TASK_TIMER_EVT_ID);
	APP_DBG_MSG("\r\nPAUSED \n");
	/*
    UTIL_SEQ_SetTask(1<<CFG_TASK_SW1_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
    flag=0 ;
    APP_DBG_MSG("\r\nFLAG = 0\t Stream Halt value = %d \n", testo[0]);
*/
    return;
  }
void P2PS_APP_SW2_Button_Action(void){
	UTIL_SEQ_ResumeTask(1<<CFG_TASK_TIMER_EVT_ID);
	APP_DBG_MSG("\r\nRESUME \n");
/*
    UTIL_SEQ_SetTask(1<<CFG_TASK_SW2_BUTTON_PUSHED_ID, CFG_SCH_PRIO_0);
    flag=1;
	APP_DBG_MSG("\r\nFLAG = 1\t Streaming \n");
*/
    return;
  }
void P2PS_APP_SW3_Button_Action(void){ //in app_entry.c disabled right now
	TIMER_FLAG=1;
	APP_DBG_MSG("\r\nTIMER_FLAG = 1\n");
    return;
  }

void P2PS_APP_Client_Action(void){ //in app_entry.c disabled right now
	TIMER_FLAG=1;
	APP_DBG_MSG("\r\nTIMER_FLAG = 1\n");
    return;
  }

void P2PS_APP_Tim_Action(void){
	if (TIMER_FLAG==1){
	UTIL_SEQ_SetTask(1<<CFG_TASK_TIMER_EVT_ID, CFG_SCH_PRIO_0);
	HAL_GPIO_TogglePin(GPIOB, GPIO_PIN_5);
	}
	return;
}

/* USER CODE END FD_LOCAL_FUNCTIONS*/
