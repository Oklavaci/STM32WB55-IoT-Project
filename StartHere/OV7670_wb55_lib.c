#include "OV7670_wb55_lib.h"
#include "main.h"

//I2C_HandleTypeDef hi2c3;
//UART_HandleTypeDef huart1;
uint8_t* temp;

uint8_t OV7670_reg[][2] = {
		 {0x12, 0x14},   // QVGA, RGB 14
		  {0x8C, 0x00},   // RGB444 Disable
		  {0x40, 0xD0},   // RGB565, 00 - FF
		  {0x3A, 0x04 + 8},   // UYVY (why?)
	      {0x3D, 0x80 + 0x00},   // gamma enable, UV auto adjust, UYVY
		  {0xB0, 0x84}, // important
		  {0x66, 0x05},
		  {0x76, 0x7f}, // 7f
		  {0x55, 0x11},
		  /* clock related */
		  {0x0C, 0x04},  // DCW enable
		  {0x3E, 0x19},  // manual scaling, pclk/=4
		  {0x70, 0x3A | 0x80},  // scaling_xsc
		  {0x71, 0x35 | 0x80},  // scaling_ysc
		  {0x72, 0x11}, // down sample by 2
		  {0x73, 0xF1}, // DSP clock /= 4


		  /* windowing (empirically decided...) */
		  {0x17, 0x13},   // HSTART1
		  {0x18, 0x01},   // HSTOP
		  {0x32, 0xf7},   // HREF
		  {0x19, 0x02},   // VSTART =  14 ( = 3 * 4 + 2)
		  {0x1a, 0x7a},   // VSTOP  = 494 ( = 123 * 4 + 2)
		  {0x03, 0x0a},   // VREF (VSTART_LOW = 2, VSTOP_LOW = 2)

		  /* color matrix coefficient */
		#if 0
		  {0x4f, 0xb3},
		  {0x50, 0xb3},
		  {0x51, 0x00},
		  {0x52, 0x3d},
		  {0x53, 0xa7},
		  {0x54, 0xe4},
		  {0x58, 0x9e},
		#else
		  {0x4f, 0x80},
		  {0x50, 0x80},
		  {0x51, 0x00},
		  {0x52, 0x22},
		  {0x53, 0x5e},
		  {0x54, 0x80},
		  {0x58, 0x9e},
		#endif

		  /* 3a */
		//  {0x13, 0x84},
		//  {0x14, 0x0a},   // AGC Ceiling = 2x
		//  {0x5F, 0x2f},   // AWB B Gain Range (empirically decided)
		//                  // without this bright scene becomes yellow (purple). might be because of color matrix
		//  {0x60, 0x98},   // AWB R Gain Range (empirically decided)
		//  {0x61, 0x70},   // AWB G Gain Range (empirically decided)
		  {0x41, 0x38},   // edge enhancement, de-noise, AWG gain enabled


		  /* gamma curve */
		#if 1
		  {0x7b, 16},
		  {0x7c, 30},
		  {0x7d, 53},
		  {0x7e, 90},
		  {0x7f, 105},
		  {0x80, 118},
		  {0x81, 130},
		  {0x82, 140},
		  {0x83, 150},
		  {0x84, 160},
		  {0x85, 180},
		  {0x86, 195},
		  {0x87, 215},
		  {0x88, 230},
		  {0x89, 244},
		  {0x7a, 16},
		#else
		  /* gamma = 1 */
		  {0x7b, 4},
		  {0x7c, 8},
		  {0x7d, 16},
		  {0x7e, 32},
		  {0x7f, 40},
		  {0x80, 48},
		  {0x81, 56},
		  {0x82, 64},
		  {0x83, 72},
		  {0x84, 80},
		  {0x85, 96},
		  {0x86, 112},
		  {0x87, 144},
		  {0x88, 176},
		  {0x89, 208},
		  {0x7a, 64},
		#endif

		  /* fps */
		//  {0x6B, 0x4a}, //PLL  x4
		  {0x11, 0x01}, // pre-scalar = 1/1

		  /* others */
		  {0x1E, 0x31}, //mirror flip
		//  {0x42, 0x08}, // color bar

	{REG_BATT, REG_BATT},
};
volatile uint8_t *data_pins;
volatile uint32_t *vsync = &(GPIOB->IDR);
volatile uint32_t *hsync = &(GPIOB->IDR);
volatile uint32_t *pixclk = &(GPIOB->IDR);

uint16_t start_bit = 0xBEBE;
uint16_t stop_bit  = 0xDEDE;

uint8_t frame_buffer0[frame_size];
uint8_t frame_buffer1[frame_size];
uint8_t frame_buffer2[frame_size];
uint8_t frame_buffer3[frame_size];

int ov7670_read(uint8_t regAddr, uint8_t *data)
{
    if(HAL_I2C_Master_Transmit(&hi2c3,DevAdrr_T, &regAddr, 1, 100) == HAL_OK){
    	if(HAL_I2C_Master_Receive(&hi2c3, DevAdrr_R, data, 1, 100) == HAL_OK)
    		return 1;
    	else {
    		HAL_UART_Transmit(&huart1,(uint8_t*) "ov7670_read - You can transmit, but you cannot receive \r\n", sizeof("ov7670_read - You can transmit, but you cannot receive \r\n"), 20);
    		return 0;
    	}
    } else {
    	HAL_UART_Transmit(&huart1,(uint8_t*) "ov7670_read - You cannot even transmit. \r\n", sizeof("ov7670_read - You cannot even transmit. \r\n"), 20);
    	return 0;
    }
}


int ov7670_write(uint8_t regAddr, uint8_t data)
{
  if(HAL_I2C_Mem_Write(&hi2c3, DevAdrr_T, regAddr, I2C_MEMADD_SIZE_8BIT, &data, 1, 100) == HAL_OK)
	  return 1;
  else{
	  HAL_UART_Transmit(&huart1,(uint8_t*) "ov7670_write has FAILED \r\n", sizeof("ov7670_write has FAILED \r\n"), 20);
	  return 0;
  };
}


int ov7670_init()
{
  if(ov7670_write(0x12, 0x80)){   // Resets the camera
	  //HAL_Delay(50);
	  HAL_UART_Transmit(&huart1,(uint8_t*) "ov7670_init - RESET Operation of the Camera is completed. \r\n", sizeof("ov7670_init - RESET Operation of the Camera is completed. \r\n"), 20);
	  return 1;
  } else{
	  HAL_UART_Transmit(&huart1, (uint8_t*) "ov7670_init - RESET operation is FAILED. \r\n", sizeof("ov7670_init - RESET operation is FAILED. \r\n"), 20);
	  return 0;
  }
}


int ov7670_config()
{
  char aTxBuffer[1] = {0xAA};

  for(int i = 0; OV7670_reg[i][0] != REG_BATT; i++) {
	//HAL_Delay(10);
    if(ov7670_write(OV7670_reg[i][0], OV7670_reg[i][1]) != 1){
    	//HAL_Delay(1);
    	HAL_UART_Transmit(&huart1,(uint8_t*) "ov7670_config has FAILED when i = ", sizeof("ov7670_config has FAILED when i = "), 20);
    	HAL_UART_Transmit(&huart1, (uint8_t*)aTxBuffer, sprintf(aTxBuffer, "%d", i), 20);
    	HAL_UART_Transmit(&huart1, (uint8_t*) "\r\n", 2, 20);
    	return 0;
    }
  }
  return 1;
}


void get_frame(){
	uint8_t  buffer_flag = 0;
	temp = frame_buffer0;
	uint32_t i = 0;
	uint8_t *data_pins = &(GPIOA -> IDR);
	 // Sync with VSYNC //
	 while (((*vsync) & (VSYNC_PIN)) == PIN_LOW);
	 while (((*vsync) & (VSYNC_PIN)) != PIN_LOW);
	 while (((*vsync) & (VSYNC_PIN)) == PIN_LOW)
	 {
	   // Sync with HSYNC //
	   while (((*hsync) & (HSYNC_PIN)) != PIN_LOW)
	   {
		 // Sync with PXCLK - Detect the positive edge //
		 // We do it 4 times to improve sampling speed //
		 // We will enter the loop 80 times (4 * 80 = 320) //

		 while (((*pixclk) & (PXCLK_PIN)) == PIN_LOW);

		 *(temp+i) = *data_pins;
		 i++;

		 while (((*pixclk) & (PXCLK_PIN)) != PIN_LOW);


		 while (((*pixclk) & (PXCLK_PIN)) == PIN_LOW);

		 *(temp+i) = *data_pins;
		 i++;

		 while (((*pixclk) & (PXCLK_PIN)) != PIN_LOW);

		 while (((*pixclk) & (PXCLK_PIN)) == PIN_LOW);

		 *(temp+i) = *data_pins;
		 i++;

		 while (((*pixclk) & (PXCLK_PIN)) != PIN_LOW);

		 while (((*pixclk) & (PXCLK_PIN)) == PIN_LOW);

		 *(temp+i) = *data_pins;
		 i++;

		 while (((*pixclk) & (PXCLK_PIN)) != PIN_LOW);
	   }
	   while(i >= frame_size){
		   buffer_flag++;

		   switch(buffer_flag) {
		   	      case 1:
		   	           temp = frame_buffer1;
		   	           break;
		   	      case 2:
		   	    	   temp = frame_buffer2;
		   	           break;
		   	      case 3:
		   	    	   temp = frame_buffer3;
		   	           break;
		   	      default:
		   	           ;
		   	   }
		   i = 0;
	   }
	 }


/*
	 HAL_UART_Transmit(&huart1,(uint8_t *) &start_bit, 2, 10);

	 HAL_UART_Transmit(&huart1,(uint8_t *) frame_buffer0, frame_size, HAL_MAX_DELAY);
	 HAL_UART_Transmit(&huart1,(uint8_t *) frame_buffer1, frame_size, HAL_MAX_DELAY);
	 HAL_UART_Transmit(&huart1,(uint8_t *) frame_buffer2, frame_size, HAL_MAX_DELAY);
	 HAL_UART_Transmit(&huart1,(uint8_t *) frame_buffer3, frame_size, HAL_MAX_DELAY);

	 HAL_UART_Transmit(&huart1,(uint8_t *) &stop_bit, 2, 10);
*/
}

