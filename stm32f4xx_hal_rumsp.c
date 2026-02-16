/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file         stm32f4xx_hal_msp.c
  * @brief        This file provides code for the MSP Initialization
  *               and de-Initialization codes.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2026 STMicroelectronics.
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
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */

#define RCC_BASE (0x40023800)
#define GPIOA_BASE (0x40020000)
#define UART1_BASE (0x40011000)

#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x30))
#define RCC_APB2ENR (*(volatile unsigned int *)(RCC_BASE + 0x44))

#define GPIOA_MODER (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_AFRH (*(volatile unsigned int *)(GPIOA_BASE + 0x24))

#define UART1_SR (*(volatile unsigned int *)(UART1_BASE + 0x00))
#define UART1_BRR (*(volatile unsigned int *)(UART1_BASE + 0x08))
#define UART1_CR1 (*(volatile unsigned int *)(UART1_BASE + 0x0C))
#define UART1_DR (*(volatile unsigned int *)(UART1_BASE + 0x04))

#define SYSCLK 16000000U
#define BAUDRATE 9600U

void uart1_tx_init(void);
void uart1_receive(int n, char *str);

int main(void) {
	uart1_tx_init();
	char str[6];
	while(1) {
		uart1_receive(6, str);
		(void)str;
	}
}

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN TD */

/* USER CODE END TD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN Define */

/* USER CODE END Define */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN Macro */

/* USER CODE END Macro */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* External functions --------------------------------------------------------*/
/* USER CODE BEGIN ExternalFunctions */

/* USER CODE END ExternalFunctions */

/* USER CODE BEGIN 0 */

/* USER CODE END 0 */
/**
  * Initializes the Global MSP.
  */

void uart1_tx_init() {

	RCC_AHB1ENR |= (1<<0);

	GPIOA_MODER &= ~(3 << 20);  // Clear bits 20:21
	GPIOA_MODER |=  (2 << 20);  // Set to Alternate Function mode (10)

	GPIOA_AFRH &= ~(0xF<<8); 
	GPIOA_AFRH |= (7<<8);

	RCC_APB2ENR |= (1<<4);

	UART1_BRR = (SYSCLK + (BAUDRATE / 2)) / BAUDRATE;

	UART1_CR1 = (1 << 2);

	UART1_CR1 |= (1<<13);
}

void uart1_receive(int n, char* str) {
	for(int i = 0; i < n; i++) {
		while(!(UART1_SR & (1<<5))) {}
		str[i] = UART1_DR;
	}
}
