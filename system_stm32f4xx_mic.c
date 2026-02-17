#define RCC_BASE   (0x40023800)
#define GPIOA_BASE (0x40020000)
#define GPIOB_BASE (0x40020400)
#define I2C1_BASE  (0x40005400)
#define TIM2_BASE  (0x40000000)

#define RCC_AHB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x30))
#define RCC_APB1ENR (*(volatile unsigned int *)(RCC_BASE + 0x40))

#define GPIOA_MODER (*(volatile unsigned int *)(GPIOA_BASE + 0x00))
#define GPIOA_OTYPER (*(volatile unsigned int *)(GPIOA_BASE + 0x04))
#define GPIOA_ODR   (*(volatile unsigned int *)(GPIOA_BASE + 0x14))

#define GPIOB_MODER (*(volatile unsigned int *)(GPIOB_BASE + 0x00))
#define GPIOB_OTYPER (*(volatile unsigned int *)(GPIOB_BASE + 0x04))
#define GPIOB_PUPDR (*(volatile unsigned int *)(GPIOB_BASE + 0x0C))
#define GPIOB_AFRH  (*(volatile unsigned int *)(GPIOB_BASE + 0x24))

#define I2C1_CR1 (*(volatile unsigned int *)(I2C1_BASE + 0x00))
#define I2C1_CR2 (*(volatile unsigned int *)(I2C1_BASE + 0x04))
#define I2C1_CCR (*(volatile unsigned int *)(I2C1_BASE + 0x1C))
#define I2C1_TRISE (*(volatile unsigned int *)(I2C1_BASE + 0x20))
#define I2C1_SR1 (*(volatile unsigned int *)(I2C1_BASE + 0x14))
#define I2C1_SR2 (*(volatile unsigned int *)(I2C1_BASE + 0x18))
#define I2C1_DR  (*(volatile unsigned int *)(I2C1_BASE + 0x10))

#define TIM2_PSC (*(volatile unsigned int *)(TIM2_BASE + 0x28))
#define TIM2_ARR (*(volatile unsigned int *)(TIM2_BASE + 0x2C))
#define TIM2_CR1 (*(volatile unsigned int *)(TIM2_BASE + 0x00))
#define TIM2_SR  (*(volatile unsigned int *)(TIM2_BASE + 0x10))

void I2C1_Init();

void I2C1_Send(char saddr, int n, char* str);

int main(void) {
	I2C1_Init();
	while(1) {
		I2C1_Send(0x12, 6, "CSE-RU");
	}
}



void I2C1_Init() {

	//bus enable for I2C
	RCC_AHB1ENR |= (1<<1);

	//mode selection
	GPIOB_MODER |= (1<<19);
	GPIOB_MODER &= ~(1<<18);
	GPIOB_MODER |= (1<<17);
	GPIOB_MODER &= ~(1<<16);

	//open drain pin 8 and 9
	GPIOB_OTYPER |= (1<<8);
	GPIOB_OTYPER |= (1<<9);

	//pull up for sda and scl
	GPIOB_PUPDR &= ~(1<<19);
	GPIOB_PUPDR |= (1<<18);
	GPIOB_PUPDR &= ~(1<<17);
	GPIOB_PUPDR |= (1<<16);

	//declare as AF4
	GPIOB_AFRH &= ~(0xFF<<0);
	GPIOB_AFRH |= (1<<6);
	GPIOB_AFRH |= (1<<2);

	//I2C module work
	//bus enable
	RCC_APB1ENR |= (1<<21);

	//first i2c reset then set
	I2C1_CR1 |= (1<<15);
	I2C1_CR1 &= ~(1<<15);

	//16mhz, 4no bit set
	I2C1_CR2 |= (1<<4);

	//output speed
	I2C1_CCR = 80;
	I2C1_TRISE = 17;

	//peripheral i2c module enable
	I2C1_CR1 |= (1<<0);
}

void I2C1_Send(char saddr, int n, char* str) {

	// if the bus bust then wait
	while(I2C1_SR2 & (1<<1)) {}

	//start if the bus is not busy
	I2C1_CR1 |= (1<<8);
	// if the master generated the start bit
	while(!(I2C1_SR1 & (1<<0))) {}  

	//start done now address frame with write bit(0)
	I2C1_DR = (saddr<<1);

	//address matching
	while(!(I2C1_SR1 & (1<<1))) {}

	//master ready for communication, so clear the address flag
	(void)I2C1_SR2;

	//check data reg empty then send
	for(int i = 0; i < n; i++) {
		while(!(I2C1_SR1 & (1<<7))) {}
		I2C1_DR = *str++;
	}

	//last byte transfer finished
	while(!(I2C1_SR1 & (1<<2))) {}
	
	// stop
	I2C1_CR1 |= (1<<9);
}
