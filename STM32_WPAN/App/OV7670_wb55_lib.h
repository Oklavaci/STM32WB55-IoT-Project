#include "stdint.h"
#include "stm32wbxx_hal.h"


static const uint8_t DevAdrr_T = 0x42;
static const uint8_t DevAdrr_R = 0x43;

extern I2C_HandleTypeDef hi2c3;
extern UART_HandleTypeDef huart1;

#define REG_BATT 0xFF
#define OV7670_QVGA_WIDTH  			320   // 65000
#define OV7670_QVGA_HEIGHT 			240
#define CAMERA_FRAME_BUFFER_SIZE 	38400  // (320 * 240 / 2 = 38400)
#define NEXT_ADDRESS 				9600  // 38400/4
#define VSYNC_PIN					GPIO_PIN_8
#define HSYNC_PIN					GPIO_PIN_9
#define PXCLK_PIN					GPIO_PIN_4
#define DATA_PORT					GPIOA
#define PIN_LOW						0x00u
#define IMG_ROW 	    			240
#define IMG_COL     				320
#define frame_size     				IMG_COL * IMG_ROW * 2 / 4

extern uint8_t frame_buffer0[frame_size];
extern uint8_t frame_buffer1[frame_size];
extern uint8_t frame_buffer2[frame_size];
extern uint8_t frame_buffer3[frame_size];

int ov7670_read(uint8_t regAddr, uint8_t *data);
int ov7670_write(uint8_t regAddr, uint8_t data);
int ov7670_init(void);
int ov7670_config(void);
void get_frame(void);

/*
void get1(void);
void get2(void);
*/
