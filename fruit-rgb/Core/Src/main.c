/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
#include <string.h>
#include "lcd.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
typedef struct{
	uint16_t R;
	uint16_t G;
	uint16_t B;
	uint16_t C;
}RGB;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define RS_PORT GPIOF
#define EN_PORT GPIOE

#define D4_PORT GPIOF
#define D5_PORT GPIOE
#define D6_PORT GPIOE
#define D7_PORT GPIOF

//4 pin mode -> pins
#define D4_PIN GPIO_PIN_14
#define D5_PIN GPIO_PIN_11
#define D6_PIN GPIO_PIN_9
#define D7_PIN GPIO_PIN_13

#define RS_PIN GPIO_PIN_15
#define EN_PIN GPIO_PIN_13

#define TCS34725_CMD_Read_Word    0x20
#define TCS34725_CMD_BIT          0x80
#define IIC_Addr_t				  0x29<<1
//#define I2C_MEMADD_SIZE_8BIT      0x00000001U
//#define I2C_MEMADD_SIZE_16BIT     0x00000010U

#define TCS34725_ATIME            0x01    /* Integration time */
#define TCS34725_CONTROL          0x0F    /* Set the gain level for the sensor */

#define TCS34725_ENABLE           0x00
#define TCS34725_ENABLE_AIEN      0x10    /* RGBC Interrupt Enable */
#define TCS34725_ENABLE_WEN       0x08     /* Wait enable - Writing 1 activates the wait timer */
#define TCS34725_ENABLE_AEN       0x02     /* RGBC Enable - Writing 1 actives the ADC, 0 disables it */
#define TCS34725_ENABLE_PON       0x01    /* Power on - Writing 1 activates the internal oscillator, 0 disables it */

#define TCS34725_CDATAL           0x14    /* Clear channel data */
#define TCS34725_CDATAH           0x15
#define TCS34725_RDATAL           0x16    /* Red channel data */
#define TCS34725_RDATAH           0x17
#define TCS34725_GDATAL           0x18    /* Green channel data */
#define TCS34725_GDATAH           0x19
#define TCS34725_BDATAL           0x1A    /* Blue channel data */
#define TCS34725_BDATAH           0x1B

#define TCS34725_AILTL            0x04    /* Clear channel lower interrupt threshold */
#define TCS34725_AILTH            0x05
#define TCS34725_AIHTL            0x06    /* Clear channel upper interrupt threshold */
#define TCS34725_AIHTH            0x07
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

I2C_HandleTypeDef hi2c2;

TIM_HandleTypeDef htim3;

UART_HandleTypeDef huart3;

/* USER CODE BEGIN PV */
LCD_t lcd;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_I2C2_Init(void);
static void MX_USART3_UART_Init(void);
static void MX_TIM3_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void display_message ( char *title,  char *message ) {
//	lcd_clear(&lcd);
	lcd_setCursor(&lcd, 0, 0);
	lcd_print(&lcd, title);
	lcd_setCursor(&lcd, 0, 1);
	lcd_print(&lcd, message);
}


uint16_t DEV_I2C_ReadWord(uint8_t add_)
{
    uint8_t Buf[2]={0, 0};
		HAL_I2C_Mem_Read(&hi2c2, IIC_Addr_t, add_, I2C_MEMADD_SIZE_8BIT, Buf, 2, 0x10);
    return ((Buf[1] << 8) | (Buf[0] & 0xff));
} // DEV_I2C_ReadWord END


static uint16_t TCS34725_ReadWord(uint8_t add)
{
    add = add | TCS34725_CMD_BIT;
    return DEV_I2C_ReadWord(add);
} // TCS34725_ReadWord END

void DEV_I2C_WriteByte(uint8_t add_, uint8_t data_)
{
	uint8_t Buf[1] = {0};
	Buf[0] = data_;
	HAL_I2C_Mem_Write(&hi2c2, IIC_Addr_t, add_, I2C_MEMADD_SIZE_8BIT, Buf, 1, 0x10);
}

void TCS34725_WriteByte(uint8_t add, uint8_t data)
{
    add = add | TCS34725_CMD_BIT;
    DEV_I2C_WriteByte(add, data);
}

void TCS34725_Init(void)
{
	TCS34725_WriteByte(TCS34725_ENABLE, TCS34725_ENABLE_PON);
	HAL_Delay(3);
	TCS34725_WriteByte(TCS34725_ENABLE, TCS34725_ENABLE_PON | TCS34725_ENABLE_AEN);
	HAL_Delay(3);
}

void TCS34725_Set_Integration_Time(uint16_t time)
{
    /* Update the timing register */
    TCS34725_WriteByte(TCS34725_ATIME, time);
//    IntegrationTime_t = time;
}

void TCS34725_Set_Gain(uint16_t gain)
{
	TCS34725_WriteByte(TCS34725_CONTROL, gain);
//    Gain_t = gain;
}

RGB TCS34725_Get_RGBData()
{
    RGB temp;
    temp.C = TCS34725_ReadWord(TCS34725_CDATAL | TCS34725_CMD_Read_Word);
    temp.R = TCS34725_ReadWord(TCS34725_RDATAL | TCS34725_CMD_Read_Word);
    temp.G = TCS34725_ReadWord(TCS34725_GDATAL | TCS34725_CMD_Read_Word);
    temp.B = TCS34725_ReadWord(TCS34725_BDATAL | TCS34725_CMD_Read_Word);

    HAL_Delay(3);
    return temp;
} // TCS34725_Get_RGBData END

/******************************************************************************
function:   Convert raw RGB values to RGB888 format
parameter	:
     rgb    : RGBC Numerical value
******************************************************************************/
uint32_t TCS34725_GetRGB888(RGB rgb)
{
    float i=1;
    //Limit data range
    if(rgb.R >= rgb.G && rgb.R >= rgb.B){
        i = rgb.R / 255 + 1;
    }
    else if(rgb.G >= rgb.R && rgb.G >= rgb.B){
        i = rgb.G / 255 + 1;
    }
    else if(rgb.B >=  rgb.G && rgb.B >= rgb.R){
        i = rgb.B / 255 + 1;
    }

    if(i!=0)
    {
        rgb.R = (rgb.R) / i;
        rgb.G = (rgb.G) / i;
        rgb.B = (rgb.B) / i;
    }

    if(rgb.R > 30)
        rgb.R = rgb.R - 30;
    if(rgb.G > 30)
        rgb.G = rgb.G - 30;
    if(rgb.B > 30)
        rgb.B = rgb.B - 30;
    rgb.R = rgb.R * 255 / 225;
    rgb.G = rgb.G * 255 / 225;
    rgb.B = rgb.B * 255 / 225;

    if(rgb.R>255)
           rgb.R = 255;
    if(rgb.G>255)
           rgb.G = 255;
    if(rgb.B>255)
           rgb.B = 255;
    return (rgb.R << 16) | (rgb.G << 8) | (rgb.B);
}

/******************************************************************************
function:   Convert raw RGB values to RGB565 format
parameter	:
     rgb    : RGBC Numerical value
******************************************************************************/

static void TCS34725_Set_Interrupt_Threshold(uint16_t Threshold_H, uint16_t Threshold_L)
{
    TCS34725_WriteByte(TCS34725_AILTL, Threshold_L & 0xff);
    TCS34725_WriteByte(TCS34725_AILTH, Threshold_L >> 8);
    TCS34725_WriteByte(TCS34725_AIHTL, Threshold_H & 0xff);
    TCS34725_WriteByte(TCS34725_AIHTH, Threshold_H >> 8);
}

static void TCS34725_Set_Interrupt_Persistence_Reg(uint8_t TCS34725_PER)
{
    if(TCS34725_PER < 0x10)
        TCS34725_WriteByte(0x0C, TCS34725_PER);
    else
        TCS34725_WriteByte(0x0C, 0x0f);
}

uint8_t TCS34725_GetLux_Interrupt(uint16_t Threshold_H, uint16_t Threshold_L)
{
    TCS34725_Set_Interrupt_Threshold(Threshold_H, Threshold_L);
    if(HAL_GPIO_ReadPin(TIM3, TIM_CHANNEL_4) == 0){
    	TCS34725_WriteByte(0x66, 0x00);
        TCS34725_Set_Interrupt_Persistence_Reg(0x02);
        return 1;
    }
    return 0;
}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
	RGB rgb;
	uint32_t RGB888;
	uint8_t r_data;
	uint8_t g_data;
	uint8_t b_data;

	char buffer[50];
	int buffer_len;

	char title[] = "Banana Grade:";
	char rx_buffer[50] = "";
//	uint8_t rx_index = 0;
//	uint32_t rx_index = 0;
	uint8_t lcd_flag = 0;

	HAL_StatusTypeDef status;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_I2C2_Init();
  MX_USART3_UART_Init();
  MX_TIM3_Init();
  /* USER CODE BEGIN 2 */
  TCS34725_Init();
//    TCS34725_INTEGRATIONTIME_2_4MS  = 0xFF,   /**<  2.4ms - 1 cycle    - Max Count: 1024  */
//    TCS34725_INTEGRATIONTIME_24MS   = 0xF6,   /**<  24ms  - 10 cycles  - Max Count: 10240 */
//    TCS34725_INTEGRATIONTIME_50MS   = 0xEB,   /**<  50ms  - 20 cycles  - Max Count: 20480 */
//    TCS34725_INTEGRATIONTIME_101MS  = 0xD5,   /**<  101ms - 42 cycles  - Max Count: 43008 */
//    TCS34725_INTEGRATIONTIME_154MS  = 0xC0,   /**<  154ms - 64 cycles  - Max Count: 65535 */
//    TCS34725_INTEGRATIONTIME_700MS  = 0x00    /**<  700ms - 256 cycles - Max Count: 65535 */
  TCS34725_Set_Integration_Time(0xD5);

//    TCS34725_GAIN_1X                = 0x00,   /**<  No gain  */
//    TCS34725_GAIN_4X                = 0x01,   /**<  4x gain  */
//    TCS34725_GAIN_16X               = 0x02,   /**<  16x gain */
//    TCS34725_GAIN_60X               = 0x03    /**<  60x gain */
  TCS34725_Set_Gain(0x01);

//  TCS34725_Set_Integration_Time(0x00);
//  TCS34725_Set_Gain(0x01);

  lcd.RS_port = RS_PORT;
  lcd.RS_pin = RS_PIN;

  lcd.EN_port = EN_PORT;
  lcd.EN_pin = EN_PIN;

  lcd.D4_port = D4_PORT;
  lcd.D4_pin = D4_PIN;
  lcd.D5_port = D5_PORT;
  lcd.D5_pin = D5_PIN;
  lcd.D6_port = D6_PORT;
  lcd.D6_pin = D6_PIN;
  lcd.D7_port = D7_PORT;
  lcd.D7_pin = D7_PIN;

  lcd_begin(&lcd, 16, 2, LCD_5x8DOTS);
  lcd_clear(&lcd);
  HAL_Delay(1);
  display_message(title, "pending...");
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {

	  uint32_t pin_state = HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_3);	// read button in PA3

	  if (pin_state != GPIO_PIN_SET) {

		  lcd_flag = 1;

		  rgb=TCS34725_Get_RGBData();		// read sensor
		  RGB888=TCS34725_GetRGB888(rgb);	// convert sensor data
		  r_data = RGB888>>16;
		  g_data = (RGB888>>8) & 0xff;
		  b_data = (RGB888) & 0xff;

		  buffer_len = sprintf(buffer, "%u,%u,%u\n", r_data, g_data, b_data);
		  status = HAL_UART_Transmit(&huart3, (uint8_t *) buffer, buffer_len, 1000);	// send sensor data to client

		  HAL_Delay(500);

	  } else {

		  if (lcd_flag) {

			  memset(rx_buffer, '\0', 50);

			  status = HAL_UART_Receive(&huart3, (uint8_t *) rx_buffer, 10, 1000); // Receive grade from client

			  HAL_Delay(1500);

			  lcd_clear(&lcd);
			  HAL_Delay(1);
			  display_message(title, rx_buffer); // Display message

			  if (status == HAL_TIMEOUT) {
				  lcd_flag = 0;
			  }

		    }
	  }

  }
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

  /** Configure the main internal regulator output voltage
  */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE3);

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_NONE;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_HSI;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_0) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief I2C2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_I2C2_Init(void)
{

  /* USER CODE BEGIN I2C2_Init 0 */

  /* USER CODE END I2C2_Init 0 */

  /* USER CODE BEGIN I2C2_Init 1 */

  /* USER CODE END I2C2_Init 1 */
  hi2c2.Instance = I2C2;
  hi2c2.Init.Timing = 0x00303D5B;
  hi2c2.Init.OwnAddress1 = 0;
  hi2c2.Init.AddressingMode = I2C_ADDRESSINGMODE_7BIT;
  hi2c2.Init.DualAddressMode = I2C_DUALADDRESS_DISABLE;
  hi2c2.Init.OwnAddress2 = 0;
  hi2c2.Init.OwnAddress2Masks = I2C_OA2_NOMASK;
  hi2c2.Init.GeneralCallMode = I2C_GENERALCALL_DISABLE;
  hi2c2.Init.NoStretchMode = I2C_NOSTRETCH_DISABLE;
  if (HAL_I2C_Init(&hi2c2) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Analogue filter
  */
  if (HAL_I2CEx_ConfigAnalogFilter(&hi2c2, I2C_ANALOGFILTER_ENABLE) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Digital filter
  */
  if (HAL_I2CEx_ConfigDigitalFilter(&hi2c2, 0) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN I2C2_Init 2 */

  /* USER CODE END I2C2_Init 2 */

}

/**
  * @brief TIM3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 36-1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 2000-1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_4) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */

  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}

/**
  * @brief USART3 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART3_UART_Init(void)
{

  /* USER CODE BEGIN USART3_Init 0 */

  /* USER CODE END USART3_Init 0 */

  /* USER CODE BEGIN USART3_Init 1 */

  /* USER CODE END USART3_Init 1 */
  huart3.Instance = USART3;
  huart3.Init.BaudRate = 115200;
  huart3.Init.WordLength = UART_WORDLENGTH_8B;
  huart3.Init.StopBits = UART_STOPBITS_1;
  huart3.Init.Parity = UART_PARITY_NONE;
  huart3.Init.Mode = UART_MODE_TX_RX;
  huart3.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart3.Init.OverSampling = UART_OVERSAMPLING_16;
  huart3.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart3.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart3) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART3_Init 2 */

  /* USER CODE END USART3_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};
/* USER CODE BEGIN MX_GPIO_Init_1 */
/* USER CODE END MX_GPIO_Init_1 */

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOH_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOF, GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_0, GPIO_PIN_RESET);

  /*Configure GPIO pin : PA3 */
  GPIO_InitStruct.Pin = GPIO_PIN_3;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PF13 PF14 PF15 */
  GPIO_InitStruct.Pin = GPIO_PIN_13|GPIO_PIN_14|GPIO_PIN_15;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOF, &GPIO_InitStruct);

  /*Configure GPIO pins : PE9 PE11 PE13 PE0 */
  GPIO_InitStruct.Pin = GPIO_PIN_9|GPIO_PIN_11|GPIO_PIN_13|GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

/* USER CODE BEGIN MX_GPIO_Init_2 */
/* USER CODE END MX_GPIO_Init_2 */
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
