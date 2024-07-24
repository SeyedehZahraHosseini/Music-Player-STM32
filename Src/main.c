/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.c
  * @brief          : Main program body
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
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

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
ADC_HandleTypeDef hadc1;

TIM_HandleTypeDef htim1;
TIM_HandleTypeDef htim2;
TIM_HandleTypeDef htim6;

UART_HandleTypeDef huart2;

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
static void MX_GPIO_Init(void);
static void MX_TIM6_Init(void);
static void MX_TIM1_Init(void);
static void MX_TIM2_Init(void);
static void MX_ADC1_Init(void);
static void MX_USART2_UART_Init(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
#define ARRAY_LENGTH(arr) (sizeof(arr)/sizeof(arr[0]))
#define DEBOUNCE_TIME 100
#define MAX_MUSIC 4


#define D1_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_SET);
#define D1_LOW HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1, GPIO_PIN_RESET);

#define D2_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_SET);
#define D2_LOW HAL_GPIO_WritePin(GPIOD, GPIO_PIN_2, GPIO_PIN_RESET);

#define D3_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_SET);
#define D3_LOW HAL_GPIO_WritePin(GPIOD, GPIO_PIN_3, GPIO_PIN_RESET);

#define D4_HIGH HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_SET);
#define D4_LOW HAL_GPIO_WritePin(GPIOD, GPIO_PIN_4, GPIO_PIN_RESET);

#define P_HIGH HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_SET);
#define P_LOW HAL_GPIO_WritePin(GPIOC, GPIO_PIN_4, GPIO_PIN_RESET);

typedef struct
{
    uint16_t frequency;
    uint16_t duration;
} Tone;

uint32_t debounce=0;
int press=0;
int pause = 1;
int music_number=0;
int turn_m[MAX_MUSIC];

extern TIM_HandleTypeDef htim2;
TIM_HandleTypeDef *pwm_timer= &htim2;
uint32_t pwm_channel = TIM_CHANNEL_2;

const Tone *volatile melody_ptr;
volatile uint16_t melody_tone_count=0;
volatile uint16_t current_tone_number=1;
volatile uint32_t current_tone_end=0;
int volume =10;
volatile uint16_t pre_volume;
volatile uint32_t last_butten_pressed;

uint32_t startTime=0;
int turn=1;
uint8_t temp1=1, temp2=2, temp3=3, temp4=4;
int mode=1, display=1;

uint8_t data[100], d, letter=0;

int second=0, flag_second=0;
uint32_t second_pause;

int brightness=100;


////////////////////////////////// music



#define NOTE_B0  31
#define NOTE_C1  33
#define NOTE_CS1 35
#define NOTE_D1  37
#define NOTE_DS1 39
#define NOTE_E1  41
#define NOTE_F1  44
#define NOTE_FS1 46
#define NOTE_G1  49
#define NOTE_GS1 52
#define NOTE_A1  55
#define NOTE_AS1 58
#define NOTE_B1  62
#define NOTE_C2  65
#define NOTE_CS2 69
#define NOTE_D2  73
#define NOTE_DS2 78
#define NOTE_E2  82
#define NOTE_F2  87
#define NOTE_FS2 93
#define NOTE_G2  98
#define NOTE_GS2 104
#define NOTE_A2  110
#define NOTE_AS2 117
#define NOTE_B2  123
#define NOTE_C3  131
#define NOTE_CS3 139
#define NOTE_D3  147
#define NOTE_DS3 156
#define NOTE_E3  165
#define NOTE_F3  175
#define NOTE_FS3 185
#define NOTE_G3  196
#define NOTE_GS3 208
#define NOTE_A3  220
#define NOTE_AS3 233
#define NOTE_B3  247
#define NOTE_C4  262
#define NOTE_CS4 277
#define NOTE_D4  294
#define NOTE_DS4 311
#define NOTE_E4  330
#define NOTE_F4  349
#define NOTE_FS4 370
#define NOTE_G4  392
#define NOTE_GS4 415
#define NOTE_A4  440
#define NOTE_AS4 466
#define NOTE_B4  494
#define NOTE_C5  523
#define NOTE_CS5 554
#define NOTE_D5  587
#define NOTE_DS5 622
#define NOTE_E5  659
#define NOTE_F5  698
#define NOTE_FS5 740
#define NOTE_G5  784
#define NOTE_GS5 831
#define NOTE_A5  880
#define NOTE_AS5 932
#define NOTE_B5  988
#define NOTE_C6  1047
#define NOTE_CS6 1109
#define NOTE_D6  1175
#define NOTE_DS6 1245
#define NOTE_E6  1319
#define NOTE_F6  1397
#define NOTE_FS6 1480
#define NOTE_G6  1568
#define NOTE_GS6 1661
#define NOTE_A6  1760
#define NOTE_AS6 1865
#define NOTE_B6  1976
#define NOTE_C7  2093
#define NOTE_CS7 2217
#define NOTE_D7  2349
#define NOTE_DS7 2489
#define NOTE_E7  2637
#define NOTE_F7  2794
#define NOTE_FS7 2960
#define NOTE_G7  3136
#define NOTE_GS7 3322
#define NOTE_A7  3520
#define NOTE_AS7 3729
#define NOTE_B7  3951
#define NOTE_C8  4186
#define NOTE_CS8 4435
#define NOTE_D8  4699
#define NOTE_DS8 4978
#define REST      0


const Tone super_mario_bros[] = {
	{2637,306}, // E7 x2
	{   0,153}, // x3 <-- Silence
	{2637,153}, // E7
	{   0,153}, // x3
	{2093,153}, // C7
	{2637,153}, // E7
	{   0,153}, // x3
	{3136,153}, // G7
	{   0,459}, // x3
	{1586,153}, // G6
	{   0,459}, // x3

	{2093,153}, // C7
	{   0,306}, // x2
	{1586,153}, // G6
	{   0,306}, // x2
	{1319,153}, // E6
	{   0,306}, // x2
	{1760,153}, // A6
	{   0,153}, // x1
	{1976,153}, // B6
	{   0,153}, // x1
	{1865,153}, // AS6
	{1760,153}, // A6
	{   0,153}, // x1

	{1586,204}, // G6
	{2637,204}, // E7
	{3136,204}, // G7
	{3520,153}, // A7
	{   0,153}, // x1
	{2794,153}, // F7
	{3136,153}, // G7
	{   0,153}, // x1
	{2637,153}, // E7
	{   0,153}, // x1
	{2093,153}, // C7
	{2349,153}, // D7
	{1976,153}, // B6
	{   0,306}, // x2

	{2093,153}, // C7
	{   0,306}, // x2
	{1586,153}, // G6
	{   0,306}, // x2
	{1319,153}, // E6
	{   0,306}, // x2
	{1760,153}, // A6
	{   0,153}, // x1
	{1976,153}, // B6
	{   0,153}, // x1
	{1865,153}, // AS6
	{1760,153}, // A6
	{   0,153}, // x1

	{1586,204}, // G6
	{2637,204}, // E7
	{3136,204}, // G7
	{3520,153}, // A7
	{   0,153}, // x1
	{2794,153}, // F7
	{3136,153}, // G7
	{   0,153}, // x1
	{2637,153}, // E7
	{   0,153}, // x1
	{2093,153}, // C7
	{2349,153}, // D7
	{1976,153}, // B6

	{   0,  0}	// <-- Disable PWM
};

const Tone hedwig_theme[] = {
		// 1 -> 1666 			\		-1666 -> 2499 			\	1660 -> 1494, 2499 -> 2249
		// 2 -> 833     ---------\		-833 -> 1249    ---------\	833 -> 750, 1249 -> 1124
		// 4 -> 416     ---------/		-416 -> 624     ---------/	416 -> 374, 624 -> 561
		// 8 -> 208			    /		-208 -> 312			    /	208 -> 187, 312 -> 281

	{REST, 750},
	{NOTE_D4, 374},
	{NOTE_G4, 561},
	{NOTE_AS4, 187},
	{NOTE_A4, 374},
	{NOTE_G4, 750},
	{NOTE_D5, 374},
	{NOTE_C5, 1124},
	{NOTE_A4, 1124},
	{NOTE_G4, 561},
	{NOTE_AS4, 187},
	{NOTE_A4, 374},
	{NOTE_F4, 750},
	{NOTE_GS4, 374},
	{NOTE_D4, 2249},
	{NOTE_D4, 374},

	{NOTE_G4, 561},
	{NOTE_AS4, 187},
	{NOTE_A4, 374},
	{NOTE_G4, 750},
	{NOTE_D5, 374},
	{NOTE_F5, 750},
	{NOTE_E5, 374},
	{NOTE_DS5, 750},
	{NOTE_B4, 374},
	{NOTE_DS5, 561},
	{NOTE_D5, 187},
	{NOTE_CS5, 374},
	{NOTE_CS4, 750},
	{NOTE_B4, 374},
	{NOTE_G4, 2249},
	{NOTE_AS4, 374},

	{NOTE_D5, 750},
	{NOTE_AS4, 374},
	{NOTE_D5, 750},
	{NOTE_AS4, 374},
	{NOTE_DS5, 750},
	{NOTE_D5, 374},
	{NOTE_CS5, 750},
	{NOTE_A4, 374},
	{NOTE_AS4, 561},
	{NOTE_D5, 187},
	{NOTE_CS5, 374},
	{NOTE_CS4, 750},
	{NOTE_D4, 374},
	{NOTE_D5, 1700},
	{REST, 150},
	{NOTE_AS4, 374},

	{NOTE_D5, 750},
	{NOTE_AS4, 374},
	{NOTE_D5, 750},
	{NOTE_AS4, 374},
	{NOTE_F5, 750},
	{NOTE_E5, 374},
	{NOTE_DS5, 750},
	{NOTE_B4, 374},
	{NOTE_DS5, 561},
	{NOTE_D5, 187},
	{NOTE_CS5, 374},
	{NOTE_CS4, 750},
	{NOTE_AS4, 374},
	{NOTE_G4, 2249},

	{      0,    0}
};


const Tone pink_panther[] = {

	// 1 -> 2000 			\		-500 -> 750 (-4)	       \	2000 -> 1800 			(1)
	// 2 -> 1000    ---------\		-250 -> 375 (-8)   ---------\	1000 -> 900 			(2)
	// 4 -> 500     ---------/		-125 -> 187 (-16)  ---------/	500 -> 450, 750 -> 675 	(4), (-4)
	// 8 -> 250			    /		        			       /	250 -> 225, 375 -> 337 	(8), (-8)
	// 16 -> 125		   			 								125 -> 112, 187 -> 168 	(16), (-16)

  {REST, 900},
  {REST, 450},
  {REST, 225},
  {NOTE_DS4, 225},
  {NOTE_E4, 675},
  {REST, 225},
  {NOTE_FS4, 225},
  {NOTE_G4, 675},
  {REST, 225},
  {NOTE_DS4, 225},
  {NOTE_E4, 337},
  {NOTE_FS4, 225},
  {NOTE_G4, 337},
  {NOTE_C5, 225},
  {NOTE_B4, 337},
  {NOTE_B4, 225},
  {NOTE_G4, 337},
  {NOTE_B4, 225},
  {NOTE_AS4, 900},
  {NOTE_A4, 168},
  {NOTE_G4, 168},
  {NOTE_E4, 168},
  {NOTE_D4, 168},
  {NOTE_E4, 900},
  {REST, 450},
  {REST, 225},
  {NOTE_DS4, 450},



  {NOTE_E4, 675},
  {REST, 225},
  {NOTE_FS4, 225},
  {NOTE_G4, 675},
  {REST, 225},
  {NOTE_DS4, 225},
  {NOTE_E4, 337},
  {NOTE_FS4, 225},
  {NOTE_G4, 337},
  {NOTE_C5, 225},
  {NOTE_B4, 337},
  {NOTE_G4, 225},
  {NOTE_B4, 337},
  {NOTE_E5, 225},
  {NOTE_DS5, 1800},
  {NOTE_D5, 900},
  {REST, 450},
  {REST, 225},
  {NOTE_DS4, 225},
  {NOTE_E4, 750},
  {REST, 225},
  {NOTE_FS4, 225},
  {NOTE_G4, 750},
  {REST, 225},
  {NOTE_DS4, 225},
  {NOTE_E4, 337},
  {NOTE_FS4, 225},
  {NOTE_G4, 337},
  {NOTE_C5, 225},
  {NOTE_B4, 337},
  {NOTE_E4, 225},
  {NOTE_G4, 337},
  {NOTE_B4, 225},



  {NOTE_AS4, 900},
  {NOTE_A4, 168},
  {NOTE_G4, 168},
  {NOTE_E4, 168},
  {NOTE_D4, 168},
  {NOTE_E4, 675},
  {REST, 450},
  {REST, 450},
  {NOTE_E5, 337},
  {NOTE_D5, 225},
  {NOTE_B4, 337},
  {NOTE_A4, 225},
  {NOTE_G4, 337},
  {NOTE_E4, 337},
  {NOTE_AS4, 112},
  {NOTE_A4, 337},
  {NOTE_AS4, 112},
  {NOTE_A4, 337},
  {NOTE_AS4, 112},
  {NOTE_A4, 337},
  {NOTE_AS4, 112},
  {NOTE_A4, 337},
  {NOTE_G4, 168},
  {NOTE_E4, 168},
  {NOTE_D4, 168},
  {NOTE_E4, 112},
  {NOTE_E4, 112},
  {NOTE_E4, 900},
  {		 0,   0}

};

const Tone twinkle_twinkle[] = {
    {523, 500}, // C4
    {523, 500}, // C4
    {659, 500}, // E4
    {659, 500}, // E4
    {739, 500}, // F4
    {739, 500}, // F4
    {659, 500}, // E4
    {587, 500}, // D4
    {587, 500}, // D4
    {523, 500}, // C4
    {523, 500}, // C4
    {659, 500}, // E4
    {659, 500}, // E4
    {587, 500}, // D4
    {587, 500}, // D4
    {523, 500}, // C4
    {0, 0}      // End
};

void PWM_Start() {

    HAL_TIM_PWM_Start(pwm_timer, pwm_channel);
}

void PWM_Cange_Tone(uint16_t pwm_freq, uint16_t volume) {
    if (pwm_freq == 0 || pwm_freq > 20000) {
        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, 0);
    } else {
        const uint32_t internal_clock_freq = HAL_RCC_GetSysClockFreq();

        const uint32_t prescaler = 1 + internal_clock_freq / pwm_freq / 60000;
        const uint32_t timer_clock = internal_clock_freq / prescaler;
        const uint32_t period_cycles = timer_clock / pwm_freq;
        const uint32_t pulse_width = volume * period_cycles / 1000 / 2;

        pwm_timer->Instance->PSC = prescaler - 1;
        pwm_timer->Instance->ARR = period_cycles - 1;
        pwm_timer->Instance->EGR = TIM_EGR_UG;

        __HAL_TIM_SET_COMPARE(pwm_timer, pwm_channel, pulse_width);
    }
}

void Update_music(int input) {

	if (!input) {
		music_number++;

		if (music_number > MAX_MUSIC)
		music_number=1;
	}

	switch(turn_m[music_number-1]){
	  case 1:
		  Change_Melody(super_mario_bros, ARRAY_LENGTH(super_mario_bros));
		  break;
	  case 2:
		  Change_Melody(hedwig_theme, ARRAY_LENGTH(hedwig_theme));
		  break;
	  case 3:
		  Change_Melody(pink_panther, ARRAY_LENGTH(pink_panther));
		  break;
	  case 4:
	  		  Change_Melody(twinkle_twinkle, ARRAY_LENGTH(twinkle_twinkle));
	  		  break;
	}
}

void Change_Melody (const Tone *melody, uint16_t tone_count) {
	melody_ptr = melody;
	melody_tone_count = tone_count;
	current_tone_number=0;
}

void Update_melody() {
	if ((HAL_GetTick() > current_tone_end) && (current_tone_number < melody_tone_count) ) {
		const Tone active_tone = *(melody_ptr + current_tone_number);
		PWM_Cange_Tone (active_tone.frequency, volume);
		current_tone_end = HAL_GetTick() + active_tone.duration;
		current_tone_number++;
	} else if (current_tone_number == melody_tone_count) {
		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
		Update_music(0);
	}
}

void set_array(int type) {
	if (!type) {
		for(int i = 0; i < MAX_MUSIC; i++) {
			turn_m[i]=i+1;
		}
	} else {

		for (int i = MAX_MUSIC - 1; i > 0; i--) {
			int j = rand() % (i + 1);
			int temp = turn_m[i];
			turn_m[i] = turn_m[j];
			turn_m[j] = temp;
		}

		char s[10];
		snprintf(s, 10, "%d% d% d% d", turn_m[0], turn_m[1], turn_m[2], turn_m[3]);
		HAL_UART_Transmit(&huart2, s, strlen(s), HAL_MAX_DELAY);

		HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_9);
	}


}




////////////////////////////////// 7 segments

uint8_t segmentNumber[10]={
			0x00,
			0x01,
			0x02,
			0x03,
			0x04,
			0x05,
			0x06,
			0x07,
			0x08,
			0x09
};

void SevenSegment_Update(uint8_t number){
	uint8_t segmentPattern = segmentNumber[number];
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0, ((segmentPattern >> 0) & 0x01));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_1, ((segmentPattern >> 1) & 0x01));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_2, ((segmentPattern >> 2) & 0x01));
    HAL_GPIO_WritePin(GPIOC, GPIO_PIN_3, ((segmentPattern >> 3) & 0x01));


}



void refresh_7segments () {


	switch (mode) {
	case 1:
		display = turn_m[music_number-1];
	case 2:
		display = turn_m[music_number-1];
		break;
	case 3:
		display = volume;
		break;
	case 4:
			display = brightness;
			break;
	}

	temp1 = display / 1000;
	temp2 = (display / 100) % 10;
	temp3 = (display / 10) % 10;
	temp4 = display % 10;





	if (turn==1 && ((HAL_GetTick() - startTime) > 5)) {
		D4_LOW;
		SevenSegment_Update(temp1);  // Display the thousands digit
		D1_HIGH;
		turn++;
		startTime = HAL_GetTick();
		P_HIGH;
	}

	if (turn==2 && ((HAL_GetTick() - startTime) > 5)) {
		D1_LOW;
		SevenSegment_Update(temp2);  // Display the thousands digit
		D2_HIGH;
		turn++;
		startTime = HAL_GetTick();
		P_LOW;
	}

	if (turn==3 && ((HAL_GetTick() - startTime) > 5)) {
		D2_LOW;
		SevenSegment_Update(temp3);  // Display the hundreds digit
		D3_HIGH;
		turn++;
		startTime = HAL_GetTick();
		P_HIGH;
	}

	if (turn==4 && ((HAL_GetTick() - startTime) > 5)) {
		D3_LOW;
		SevenSegment_Update(temp4);  // Display the ones digit
		D4_HIGH;
		turn=1;
		startTime = HAL_GetTick();
		P_HIGH;
	}
}





/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

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
  MX_TIM6_Init();
  MX_TIM1_Init();
  MX_TIM2_Init();
  MX_ADC1_Init();
  MX_USART2_UART_Init();
  /* USER CODE BEGIN 2 */
  HAL_TIM_Base_Start_IT(&htim6);
  HAL_UART_Receive_IT(&huart2, &d, 1);
  HAL_ADC_Start_IT(&hadc1);

  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_1);
  __HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, brightness*10);
  set_array(0);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
	 
  }
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
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSI;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.HSICalibrationValue = RCC_HSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSI;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL12;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_1) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USART2|RCC_PERIPHCLK_TIM1
                              |RCC_PERIPHCLK_ADC12;
  PeriphClkInit.Usart2ClockSelection = RCC_USART2CLKSOURCE_PCLK1;
  PeriphClkInit.Adc12ClockSelection = RCC_ADC12PLLCLK_DIV1;
  PeriphClkInit.Tim1ClockSelection = RCC_TIM1CLK_HCLK;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/**
  * @brief ADC1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_ADC1_Init(void)
{

  /* USER CODE BEGIN ADC1_Init 0 */

  /* USER CODE END ADC1_Init 0 */

  ADC_MultiModeTypeDef multimode = {0};
  ADC_ChannelConfTypeDef sConfig = {0};

  /* USER CODE BEGIN ADC1_Init 1 */

  /* USER CODE END ADC1_Init 1 */

  /** Common config
  */
  hadc1.Instance = ADC1;
  hadc1.Init.ClockPrescaler = ADC_CLOCK_ASYNC_DIV1;
  hadc1.Init.Resolution = ADC_RESOLUTION_12B;
  hadc1.Init.ScanConvMode = ADC_SCAN_DISABLE;
  hadc1.Init.ContinuousConvMode = DISABLE;
  hadc1.Init.DiscontinuousConvMode = DISABLE;
  hadc1.Init.ExternalTrigConvEdge = ADC_EXTERNALTRIGCONVEDGE_NONE;
  hadc1.Init.ExternalTrigConv = ADC_SOFTWARE_START;
  hadc1.Init.DataAlign = ADC_DATAALIGN_RIGHT;
  hadc1.Init.NbrOfConversion = 1;
  hadc1.Init.DMAContinuousRequests = DISABLE;
  hadc1.Init.EOCSelection = ADC_EOC_SINGLE_CONV;
  hadc1.Init.LowPowerAutoWait = DISABLE;
  hadc1.Init.Overrun = ADC_OVR_DATA_OVERWRITTEN;
  if (HAL_ADC_Init(&hadc1) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure the ADC multi-mode
  */
  multimode.Mode = ADC_MODE_INDEPENDENT;
  if (HAL_ADCEx_MultiModeConfigChannel(&hadc1, &multimode) != HAL_OK)
  {
    Error_Handler();
  }

  /** Configure Regular Channel
  */
  sConfig.Channel = ADC_CHANNEL_3;
  sConfig.Rank = ADC_REGULAR_RANK_1;
  sConfig.SingleDiff = ADC_SINGLE_ENDED;
  sConfig.SamplingTime = ADC_SAMPLETIME_181CYCLES_5;
  sConfig.OffsetNumber = ADC_OFFSET_NONE;
  sConfig.Offset = 0;
  if (HAL_ADC_ConfigChannel(&hadc1, &sConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN ADC1_Init 2 */

  /* USER CODE END ADC1_Init 2 */

}

/**
  * @brief TIM1 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM1_Init(void)
{

  /* USER CODE BEGIN TIM1_Init 0 */

  /* USER CODE END TIM1_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};
  TIM_BreakDeadTimeConfigTypeDef sBreakDeadTimeConfig = {0};

  /* USER CODE BEGIN TIM1_Init 1 */

  /* USER CODE END TIM1_Init 1 */
  htim1.Instance = TIM1;
  htim1.Init.Prescaler = 48-1;
  htim1.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim1.Init.Period = 1000-1;
  htim1.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim1.Init.RepetitionCounter = 0;
  htim1.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim1, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim1) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterOutputTrigger2 = TIM_TRGO2_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim1, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCNPolarity = TIM_OCNPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  sConfigOC.OCIdleState = TIM_OCIDLESTATE_RESET;
  sConfigOC.OCNIdleState = TIM_OCNIDLESTATE_RESET;
  if (HAL_TIM_PWM_ConfigChannel(&htim1, &sConfigOC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sBreakDeadTimeConfig.OffStateRunMode = TIM_OSSR_DISABLE;
  sBreakDeadTimeConfig.OffStateIDLEMode = TIM_OSSI_DISABLE;
  sBreakDeadTimeConfig.LockLevel = TIM_LOCKLEVEL_OFF;
  sBreakDeadTimeConfig.DeadTime = 0;
  sBreakDeadTimeConfig.BreakState = TIM_BREAK_DISABLE;
  sBreakDeadTimeConfig.BreakPolarity = TIM_BREAKPOLARITY_HIGH;
  sBreakDeadTimeConfig.BreakFilter = 0;
  sBreakDeadTimeConfig.Break2State = TIM_BREAK2_DISABLE;
  sBreakDeadTimeConfig.Break2Polarity = TIM_BREAK2POLARITY_HIGH;
  sBreakDeadTimeConfig.Break2Filter = 0;
  sBreakDeadTimeConfig.AutomaticOutput = TIM_AUTOMATICOUTPUT_DISABLE;
  if (HAL_TIMEx_ConfigBreakDeadTime(&htim1, &sBreakDeadTimeConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM1_Init 2 */

  /* USER CODE END TIM1_Init 2 */
  HAL_TIM_MspPostInit(&htim1);

}

/**
  * @brief TIM2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM2_Init(void)
{

  /* USER CODE BEGIN TIM2_Init 0 */

  /* USER CODE END TIM2_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM2_Init 1 */

  /* USER CODE END TIM2_Init 1 */
  htim2.Instance = TIM2;
  htim2.Init.Prescaler = 0;
  htim2.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim2.Init.Period = 4294967295;
  htim2.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim2.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim2, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim2) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim2, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 0;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_HIGH;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim2, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM2_Init 2 */

  /* USER CODE END TIM2_Init 2 */
  HAL_TIM_MspPostInit(&htim2);

}

/**
  * @brief TIM6 Initialization Function
  * @param None
  * @retval None
  */
static void MX_TIM6_Init(void)
{

  /* USER CODE BEGIN TIM6_Init 0 */

  /* USER CODE END TIM6_Init 0 */

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  /* USER CODE BEGIN TIM6_Init 1 */

  /* USER CODE END TIM6_Init 1 */
  htim6.Instance = TIM6;
  htim6.Init.Prescaler = 240-1;
  htim6.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim6.Init.Period = 100-1;
  htim6.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim6) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim6, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM6_Init 2 */

  /* USER CODE END TIM6_Init 2 */

}

/**
  * @brief USART2 Initialization Function
  * @param None
  * @retval None
  */
static void MX_USART2_UART_Init(void)
{

  /* USER CODE BEGIN USART2_Init 0 */

  /* USER CODE END USART2_Init 0 */

  /* USER CODE BEGIN USART2_Init 1 */

  /* USER CODE END USART2_Init 1 */
  huart2.Instance = USART2;
  huart2.Init.BaudRate = 115200;
  huart2.Init.WordLength = UART_WORDLENGTH_8B;
  huart2.Init.StopBits = UART_STOPBITS_1;
  huart2.Init.Parity = UART_PARITY_NONE;
  huart2.Init.Mode = UART_MODE_TX_RX;
  huart2.Init.HwFlowCtl = UART_HWCONTROL_NONE;
  huart2.Init.OverSampling = UART_OVERSAMPLING_16;
  huart2.Init.OneBitSampling = UART_ONE_BIT_SAMPLE_DISABLE;
  huart2.AdvancedInit.AdvFeatureInit = UART_ADVFEATURE_NO_INIT;
  if (HAL_UART_Init(&huart2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN USART2_Init 2 */

  /* USER CODE END USART2_Init 2 */

}

/**
  * @brief GPIO Initialization Function
  * @param None
  * @retval None
  */
static void MX_GPIO_Init(void)
{
  GPIO_InitTypeDef GPIO_InitStruct = {0};

  /* GPIO Ports Clock Enable */
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOF_CLK_ENABLE();
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOC, GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOD, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4, GPIO_PIN_RESET);

  /*Configure GPIO pins : DRDY_Pin MEMS_INT3_Pin MEMS_INT4_Pin MEMS_INT2_Pin */
  GPIO_InitStruct.Pin = DRDY_Pin|MEMS_INT3_Pin|MEMS_INT4_Pin|MEMS_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : CS_I2C_SPI_Pin LD4_Pin LD3_Pin LD5_Pin
                           LD7_Pin LD9_Pin LD10_Pin LD8_Pin
                           LD6_Pin */
  GPIO_InitStruct.Pin = CS_I2C_SPI_Pin|LD4_Pin|LD3_Pin|LD5_Pin
                          |LD7_Pin|LD9_Pin|LD10_Pin|LD8_Pin
                          |LD6_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /*Configure GPIO pins : PC0 PC1 PC2 PC3
                           PC4 */
  GPIO_InitStruct.Pin = GPIO_PIN_0|GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3
                          |GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /*Configure GPIO pin : PA0 */
  GPIO_InitStruct.Pin = GPIO_PIN_0;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : SPI1_SCK_Pin SPI1_MISO_Pin SPI1_MISOA7_Pin */
  GPIO_InitStruct.Pin = SPI1_SCK_Pin|SPI1_MISO_Pin|SPI1_MISOA7_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI1;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD8 PD6 PD7 */
  GPIO_InitStruct.Pin = GPIO_PIN_8|GPIO_PIN_6|GPIO_PIN_7;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : DM_Pin DP_Pin */
  GPIO_InitStruct.Pin = DM_Pin|DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF14_USB;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pins : PD1 PD2 PD3 PD4 */
  GPIO_InitStruct.Pin = GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3|GPIO_PIN_4;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : I2C1_SCL_Pin I2C1_SDA_Pin */
  GPIO_InitStruct.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /*Configure GPIO pin : PB9 */
  GPIO_InitStruct.Pin = GPIO_PIN_9;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING_FALLING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* EXTI interrupt init*/
  HAL_NVIC_SetPriority(EXTI0_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI0_IRQn);

  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);

}

/* USER CODE BEGIN 4 */

int musicNumber=0;

int extractMusic(const char *received) {
    // Define the expected prefix and suffix for the command
    const char prefix[] = "MUSIC_SET(";
    const char suffix[] = ")";

    // Check if the received string starts with the prefix
    if (strncmp(received, prefix, strlen(prefix)) != 0) {
        return 0; // Prefix doesn't match
    }

    // Find the start and end of the music number
    const char *musicStart = received + strlen(prefix);
    const char *musicEnd = strstr(musicStart, suffix);

    // If suffix not found or music number length exceeds the maximum allowed
    if (musicEnd == NULL) {
    	send_message(0, 1);
        return 0; // Invalid format or music number too long
    }

    for (const char *ptr = musicStart; ptr < musicEnd; ++ptr) {
		if (!isdigit(*ptr)) {
			send_message(0, 1);
			return 0; // Not a digit
		}
	}

    char str[5]; // Assuming volume can be at most 4 digits
	strncpy(str, musicStart, musicEnd - musicStart);
	str[musicEnd - musicStart] = '\0';

	if (atoi(str) < 1 || atoi(str) > MAX_MUSIC) {
		send_message(0, 1);
		return 0; // Volume out of range
	}


    // Extract and convert the music number
	music_number = atoi(str);


    return 1; // Music number extracted successfully
}

// Function to extract the volume from the received string
int extractVolume(const char *received) {
    // Define the expected prefix and suffix for the command
    const char prefix[] = "Change_Volume(";
    const char suffix[] = ")";

    // Check if the received string starts with the prefix
    if (strncmp(received, prefix, strlen(prefix)) != 0) {
        return 0; // Prefix doesn't match
    }

    // Find the start and end of the volume
    const char *volumeStart = received + strlen(prefix);
    const char *volumeEnd = strstr(volumeStart, suffix);

    // If suffix not found or volume length exceeds the maximum allowed
    if (volumeEnd == NULL) {
    	send_message(0, 2);
        return 0; // Invalid format or volume too long
    }

    for (const char *ptr = volumeStart; ptr < volumeEnd; ++ptr) {
		if (!isdigit(*ptr)) {
			send_message(0, 2);
			return 0; // Not a digit
		}
	}



    char str[5]; // Assuming volume can be at most 4 digits
	strncpy(str, volumeStart, volumeEnd - volumeStart);
	str[volumeEnd - volumeStart] = '\0';

	if (atoi(str) < 1 || atoi(str) > 100) {
		send_message(0, 2);
		return 0; // Volume out of range
	}

	// Extract and convert the music number
	volume = atoi(str);

    return 1; // Volume extracted successfully
}

int extractLed(const char *received) {
    // Define the expected prefix and suffix for the command
    const char prefix[] = "Change_LED(";
    const char suffix[] = ")";

    // Check if the received string starts with the prefix
    if (strncmp(received, prefix, strlen(prefix)) != 0) {
        return 0; // Prefix doesn't match
    }

    // Find the start and end of the volume
    const char *ledStart = received + strlen(prefix);
    const char *ledEnd = strstr(ledStart, suffix);

    // If suffix not found or volume length exceeds the maximum allowed
    if (ledEnd == NULL) {
    	send_message(0, 6);
        return 0; // Invalid format or volume too long
    }

    for (const char *ptr = ledStart; ptr < ledEnd; ++ptr) {
		if (!isdigit(*ptr)) {
			send_message(0, 6);
			return 0; // Not a digit
		}
	}



    char str[5]; // Assuming volume can be at most 4 digits
	strncpy(str, ledStart, ledEnd - ledStart);
	str[ledEnd - ledStart] = '\0';

	if (atoi(str) < 30 || atoi(str) > 100) {
		send_message(0, 6);
		return 0; // Volume out of range
	}

	// Extract and convert the music number
	brightness = atoi(str);

    return 1; // Volume extracted successfully
}

int extractarray(const char *received) {
    // Define the expected prefix and suffix for the command
    const char prefix[] = "Play_Mode(";
    const char suffix[] = ")";

    // Check if the received string starts with the prefix
    if (strncmp(received, prefix, strlen(prefix)) != 0) {
        return 0; // Prefix doesn't match
    }

    // Find the start and end of the mode
    const char *modeStart = received + strlen(prefix);
    const char *modeEnd = strstr(modeStart, suffix);

    // If suffix not found or mode length exceeds the maximum allowed
    if (modeEnd == NULL) { // Adjusted to fit the 7-char array + null terminator
    	send_message(0, 4);
        return 0; // Invalid format or mode too long
    }

    // Extract the mode string
    char str[8]; // 7 chars + null terminator
    strncpy(str, modeStart, modeEnd - modeStart);
    str[modeEnd - modeStart] = '\0'; // Null-terminate the string

    // Transmit the extracted string
    HAL_UART_Transmit(&huart2, str, strlen(str), 0xFFFFFFFF);

    // Check the extracted mode and perform actions
    if (strcmp(str, "SHUFFLE") == 0) {
        set_array(1);
        send_message(1, 4);
        return 1;
    } else if (strcmp(str, "ORDERED") == 0) {
        set_array(0);
        HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_10);
        send_message(1, 5);
        return 1;
    }

    send_message(0, 4);
    return 0;
}

int extractPause(const char *received) {
    // Define the expected prefix and suffix for the command
    const char prefix[] = "Pause_After(";
    const char suffix[] = ")";

    // Check if the received string starts with the prefix
    if (strncmp(received, prefix, strlen(prefix)) != 0) {
        return 0; // Prefix doesn't match
    }

    // Find the start and end of the volume
    const char *secondStart = received + strlen(prefix);
    const char *secondEnd = strstr(secondStart, suffix);

    // If suffix not found or volume length exceeds the maximum allowed
    if (secondEnd == NULL) {
    	send_message(0, 3);
        return 0; // Invalid format or volume too long
    }

    for (const char *ptr = secondStart; ptr < secondEnd; ++ptr) {
		if (!isdigit(*ptr)) {
			send_message(0, 3);
			return 0; // Not a digit
		}
	}



    char str[5]; // Assuming volume can be at most 4 digits
	strncpy(str, secondStart, secondEnd - secondStart);
	str[secondEnd - secondStart] = '\0';

	// Extract and convert the music number
	second = atoi(str);

    return 1; // Volume extracted successfully
}

void send_message(int state,int type) {
	char buffer[100];
	char massage[50];
	uint32_t currentTime = HAL_GetTick();
	uint32_t currentSeconds = (currentTime / 1000) % 60; // Get current seconds
	uint32_t currentMinutes = (currentTime / 60000) % 60; // Get current minutes

	if (state){
		if (type>3 && type<6) {
			snprintf(massage, 50, "Play mode changed to %s", (type==4) ? "SHUFFLE" :  "ORDERED");
		} else {
			snprintf(massage, 50, "%s changed to %d", (type==1) ? "Music" : ((type==2) ? "Volume" : ((type==3) ? "Pause time" : "Brightness")), (type==1) ? music_number : ((type==2) ? volume : ((type==3) ? second : brightness)));
		}
	}else
		snprintf(massage, 50, "%s", (type==1) ? "Music not found" : ((type==2) ? "Volume not valid" : ( (type==3) ? "Pause time not valid" : ( (type==4 || type==5) ? "Play mode not valid" : "Brightness not valid"))));
	
	 snprintf(buffer, 100, "\n[%s][%s][%02d:%02d]",
	             (state) ? "INFO" : "ERROR", massage, currentMinutes, currentSeconds);
	HAL_UART_Transmit_IT(&huart2, buffer, strlen(buffer));
}

void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
	if (huart->Instance == USART2) {

		HAL_UART_Receive_IT(&huart2, &d, 1);

		data[letter++] = d;
		data[letter]='\0';




		if (d== '\n' || d== '\r') {


			if (extractMusic(data)) {
				Update_music(music_number);
				send_message(1, 1);
			} else if (extractVolume(data)) {
				send_message(1, 2);
			}else if (extractPause(data)) {
				flag_second=1;
				second_pause=HAL_GetTick();
				send_message(1, 3);
			} else if (extractarray(data)) {

			} else if (extractLed(data)) {
				__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, brightness*10);
				send_message(1, 6);
			}

			letter=0; data[0]='\0';
		}

	}
}

void HAL_GPIO_EXTI_Callback (uint16_t GPIO_Pin) {
	if (GPIO_Pin == GPIO_PIN_0) {
		if (HAL_GPIO_ReadPin(GPIOA, GPIO_PIN_0)) {
			if (HAL_GetTick() > last_butten_pressed +200) {
				last_butten_pressed = HAL_GetTick();
				PWM_Start();
				Update_music(0);

				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
			}
		}
	}else if (GPIO_Pin == GPIO_PIN_6) {

		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_6)) {
			static uint32_t last_enter_time = 0;
			if (HAL_GetTick()- last_enter_time < DEBOUNCE_TIME){
				return;
			}

			press=1;
			debounce = HAL_GetTick();

		} else {

			if (press) {
				pause=!pause;
				HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_12);

			}
			press=0;

		}

	}

	else if (GPIO_Pin == GPIO_PIN_7) {
		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_7)) {
			mode=2;
		} else {
			mode=1;
			Update_music(music_number);
		}

	}
	else if (GPIO_Pin == GPIO_PIN_8) {

		if (HAL_GPIO_ReadPin(GPIOD, GPIO_PIN_8)) {
		
			mode=3;
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);

		} else {
			mode=1;
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_15);
		
		}
	} else if (GPIO_Pin == GPIO_PIN_9) {

		if (HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_9)) {

			mode=4;
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);

		} else {
			mode=1;
			HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_14);

		}
	}
}

void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim) {
	if (htim->Instance == TIM6) {

		if (!pause) {

			Update_melody();
			refresh_7segments();
		} else {
			PWM_Cange_Tone(0, 0);

			D1_LOW;
			D2_LOW;
			D3_LOW;
			D4_LOW;
		}

		if (flag_second && HAL_GetTick() - second_pause >= second*1000 ) {
			flag_second=0;
			pause=1;
		}
	}
}

void HAL_ADC_ConvCpltCallback(ADC_HandleTypeDef* hadc) {
	if (hadc->Instance == ADC1) {

		if (!pause && mode==2) {


			music_number = ((HAL_ADC_GetValue(&hadc1)* (MAX_MUSIC)) / 4000)+1;

			if (music_number <1)
				music_number=1;
			if (music_number > MAX_MUSIC)
				music_number=MAX_MUSIC;

		}

		if (!pause && mode==3) {
			volume = ((HAL_ADC_GetValue(&hadc1)* 120) / 4000)-10;

			if (volume <0)
				volume=0;
			if (volume > 100)
				volume=100;

		}

		if (!pause && mode==4) {
			brightness = ((HAL_ADC_GetValue(&hadc1)* 80) / 4000)+25;

			if (brightness <30)
				brightness=30;
			if (brightness > 100)
				brightness=100;

			__HAL_TIM_SET_COMPARE(&htim1, TIM_CHANNEL_1, brightness*10);

		}

		HAL_ADC_Start_IT(&hadc1);
	}
}

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
