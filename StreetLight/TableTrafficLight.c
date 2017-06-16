// ***** 0. Documentation Section *****
// TableTrafficLight.c for Lab 10
// Runs on LM4F120/TM4C123
// Index implementation of a Moore finite state machine to operate a traffic light.  

// east/west red light connected to PB5
// east/west yellow light connected to PB4
// east/west green light connected to PB3
// north/south facing red light connected to PB2
// north/south facing yellow light connected to PB1
// north/south facing green light connected to PB0
// pedestrian detector connected to PE2 (1=pedestrian present)
// north/south car detector connected to PE1 (1=car present)
// east/west car detector connected to PE0 (1=car present)
// "walk" light connected to PF3 (built-in green LED)
// "don't walk" light connected to PF1 (built-in red LED)

typedef enum {Walk=0, WalkRedOff1, WalkRed1, WalkRedOff2, WalkRed2, WalkRedOff3, Go1, Go2, Yellow1, Yellow2, NumberStates} StateNames;
typedef struct
{
	unsigned long waitTime;
	unsigned long outPB;
	unsigned long outPF;
	StateNames Next[9];
	
}StateStruct;

/*****************************/
/* Sensor Map ****************/
/* Sensor1 | Sensor2 | Walk  */
/*****************************/

#define PB5 0x20
#define PB4 0x10
#define PB3 0x08
#define PB2 0x04
#define PB1 0x02
#define PB0 0x01
#define PF3 0x08
#define PF1 0x02
#define PE2 0x04
#define PE1 0x02
#define PE0 0x01

#define RED1 		PB5
#define YELLOW1	PB4
#define GREEN1	PB3

#define RED2		PB2
#define YELLOW2 PB1
#define GREEN2 	PB0

#define GREEN_WALK PF3
#define RED_WALK	 PF1	
#define ALL_OFF		 0x00

#define SENSOR1       PE0
#define SENSOR2       PE1
#define SENSOR_WALK   PE2

#define OutputPB GPIO_PORTB_DATA_R
#define OutputPF GPIO_PORTF_DATA_R

const StateStruct WalkState 			= { /*Wait Time in 10ms unit = */ 200, /*Output PB = */RED1|RED2, /*Output PF = */GREEN_WALK,
																			/*If sensors, go to State */
																			{/*000*/Walk, /*001*/Walk, /*010*/WalkRedOff1, /*011*/WalkRedOff1, /*100*/WalkRedOff1, /*101*/WalkRedOff1, /*110*/WalkRedOff1, /*111*/WalkRedOff1 }
																		};
const StateStruct WalkRedOff1State 		= { /*Wait Time in 10ms unit = */ 50, /*Output PB = */RED1|RED2, /*Output PF = */ALL_OFF,
																			/*If sensors, go to State */
																			{/*000*/WalkRed1, /*001*/WalkRed1, /*010*/WalkRed1, /*011*/WalkRed1, /*100*/WalkRed1, /*101*/WalkRed1, /*110*/WalkRed1, /*111*/WalkRed1 }
																		};
const StateStruct WalkRed1State 	= { /*Wait Time in 10ms unit = */ 50, /*Output PB = */RED1|RED2, /*Output PF = */RED_WALK,
																			/*If sensors, go to State */
																			{/*000*/WalkRedOff2, /*001*/WalkRedOff2, /*010*/WalkRedOff2, /*011*/WalkRedOff2, /*100*/WalkRedOff2, /*101*/WalkRedOff2, /*110*/WalkRedOff2, /*111*/WalkRedOff2 }
																		};
const StateStruct WalkRedOff2State 		= { /*Wait Time in 10ms unit = */ 50, /*Output PB = */RED1|RED2, /*Output PF = */ALL_OFF,
																			/*If sensors, go to State */
																			{/*000*/WalkRed2, /*001*/WalkRed2, /*010*/WalkRed2, /*011*/WalkRed2, /*100*/WalkRed2, /*101*/WalkRed2, /*110*/WalkRed2, /*111*/WalkRed2 }
																		};
const StateStruct WalkRed2State 	= { /*Wait Time in 10ms unit = */ 50, /*Output PB = */RED1|RED2, /*Output PF = */RED_WALK,
																			/*If sensors, go to State */
																			{/*000*/WalkRedOff3, /*001*/WalkRedOff3, /*010*/WalkRedOff3, /*011*/WalkRedOff3, /*100*/WalkRedOff3, /*101*/WalkRedOff3, /*110*/WalkRedOff3, /*111*/WalkRedOff3 }
																		};
const StateStruct WalkRedOff3State  	= { /*Wait Time in 10ms unit = */ 50, /*Output PB = */RED1|RED2, /*Output PF = */ALL_OFF,
																			/*If sensors, go to State */
																			{/*000*/Walk, /*001*/Walk, /*010*/Go2, /*011*/Go2, /*100*/Go1, /*101*/Go1, /*110*/Go1, /*111*/Go1 }
																		};
const StateStruct Go1State 				= { /*Wait Time in 10ms unit = */ 200, /*Output PB = */GREEN1|RED2, /*Output PF = */RED_WALK,
																			/*If sensors, go to State */
																			{/*000*/Yellow1, /*001*/Yellow1, /*010*/Yellow1, /*011*/Yellow1, /*100*/Go1, /*101*/Yellow1, /*110*/Yellow1, /*111*/Yellow1 }
																		};
const StateStruct Go2State 				= { /*Wait Time in 10ms unit = */ 200, /*Output PB = */GREEN2|RED1, /*Output PF = */RED_WALK,
																			/*If sensors, go to State */
																			{/*000*/Yellow2, /*001*/Yellow2, /*010*/Go2, /*011*/Yellow2, /*100*/Yellow2, /*101*/Yellow2, /*110*/Yellow2, /*111*/Yellow2 }
																		};
const StateStruct Yellow1State 		= { /*Wait Time in 10ms unit = */ 50, /*Output PB = */YELLOW1|RED2, /*Output PF = */RED_WALK,
																			/*If sensors, go to State */
																			{/*000*/Walk, /*001*/Walk, /*010*/Go2, /*011*/Go2, /*100*/Go1, /*101*/Walk, /*110*/Go2, /*111*/Go2 }
																		};
const StateStruct Yellow2State 		= { /*Wait Time in 10ms unit = */ 50, /*Output PB = */YELLOW2|RED1, /*Output PF = */RED_WALK,
																			/*If sensors, go to State */
																			{/*000*/Walk, /*001*/Walk, /*010*/Go2, /*011*/Walk, /*100*/Go1, /*101*/Walk, /*110*/Go1, /*111*/Walk }
																		};

const StateStruct *State[NumberStates] = { 	&WalkState, &WalkRedOff1State, &WalkRed1State, &WalkRedOff2State, &WalkRed2State, &WalkRedOff3State,
																						&Go1State, &Go2State, &Yellow1State, &Yellow2State};

// ***** 1. Pre-processor Directives Section *****
#include "TExaS.h"
#include "tm4c123gh6pm.h"

// ***** 2. Global Declarations Section *****

// FUNCTION PROTOTYPES: Each subroutine defined
void DisableInterrupts(void); // Disable interrupts
void EnableInterrupts(void);  // Enable interrupts

// ***** 3. Subroutines Section *****

unsigned long mapSensorsToDecimal(void);
void SysTick_Wait10ms(unsigned long p_waitTime);
void SysTick_Init(void);
void Init(void);
void SysTick_Wait(unsigned long delay);																						
																						
int main(void){ 
	StateNames currentState = Walk;
	unsigned long sensor;
  TExaS_Init(SW_PIN_PE210, LED_PIN_PB543210,ScopeOff); // activate grader and set system clock to 80 MHz
  Init();
  EnableInterrupts();
  while(1){
     OutputPB = State[currentState]->outPB;
		 OutputPF = State[currentState]->outPF;
		 SysTick_Wait10ms(State[currentState]->waitTime);
		 sensor = mapSensorsToDecimal();
		 currentState = State[currentState]->Next[sensor];
  }
}

unsigned long mapSensorsToDecimal()
{
/*****************************/
/* Sensor Map ****************/
/* Sensor1 | Sensor2 | Walk  */
/*****************************/
	unsigned long returnValue=0;
	if(GPIO_PORTE_DATA_R&SENSOR_WALK)
		returnValue |= 0x01;
	if(GPIO_PORTE_DATA_R&SENSOR2)
		returnValue |= 0x02;
	if(GPIO_PORTE_DATA_R&SENSOR1)
		returnValue |= 0x04;
	return returnValue;
}

// The delay parameter is in units of the 80 MHz core clock. (12.5 ns)
void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;  // number of counts to wait
  NVIC_ST_CURRENT_R = 0;       // any value written to CURRENT clears
  while((NVIC_ST_CTRL_R&0x00010000)==0){ // wait for count flag
  }
}

void SysTick_Wait10ms(unsigned long p_waitTime)
{
	unsigned long i;
  for(i=0; i<p_waitTime; i++){
    SysTick_Wait(800000);  // wait 10ms
  }
}

void Init()
{
	volatile unsigned long delayForHW;
	SysTick_Init();
	SYSCTL_RCGC2_R |= 0xFF;      // 1) B E F
  delayForHW = SYSCTL_RCGC2_R;      // 2) no need to unlock
  
	GPIO_PORTE_AMSEL_R &= ~(PE2|PE1|PE0);  							// disable analog function
	GPIO_PORTF_AMSEL_R &= ~(PF3|PF1);  									// disable analog function
	GPIO_PORTB_AMSEL_R &= ~(PB5|PB4|PB3|PB2|PB1|PB0);		// disable analog function
  
	GPIO_PORTE_PCTL_R &= ~0xFFFFFFFF; // 4) enable regular GPIO
	GPIO_PORTF_PCTL_R &= ~0xFFFFFFFF; // 4) enable regular GPIO
	GPIO_PORTB_PCTL_R &= ~0xFFFFFFFF; // 4) enable regular GPIO
	
  GPIO_PORTE_DIR_R &= ~(PE2|PE1|PE0);   							// 5) inputs
  GPIO_PORTF_DIR_R |= (PF3|PF1);   								// 5) inputs
	GPIO_PORTB_DIR_R |= (PB5|PB4|PB3|PB2|PB1|PB0);   // 5) inputs
  
	GPIO_PORTE_AFSEL_R &= ~(PE2|PE1|PE0); // 6) regular function on PE1-0
	GPIO_PORTF_AFSEL_R &= ~(PF3|PF1); // 6) regular function on PE1-0
	GPIO_PORTB_AFSEL_R &= ~(PB5|PB4|PB3|PB2|PB1|PB0); // 6) regular function on PE1-0	
  
	GPIO_PORTE_DEN_R |= (PE2|PE1|PE0);    // 7) enable digital on PE1-0
	GPIO_PORTF_DEN_R |= (PF3|PF1);    // 7) enable digital on PE1-0	
	GPIO_PORTB_DEN_R |= (PB5|PB4|PB3|PB2|PB1|PB0);    // 7) enable digital on PE1-0
}

void SysTick_Init()
{
	NVIC_ST_CTRL_R = 0;               // disable SysTick during setup
  NVIC_ST_CTRL_R = 0x00000005;      // enable SysTick with core clock
}
