/*
 * ==================================================================================================================================
 *                                                         Include Section
 * ===================================================================================================================================
 */

#include "../LIB/BIT_MATHS.h"
#include "../LIB/STD_TYPES.h"
#include "../MCAL/RCC/MRCC_Interface.h"
#include "../MCAL/GPIO/MGPIO_interface.h"
#include "../MCAL/ADC/ADC_interface.h"
#include "../MCAL/UART/UART_interface.h"
#include "../HAL/LM35/LM35_interface.h"
#include "../HAL/LDR/LDR_interface.h"
#include "../HAL/LCD/LCD_int.h"
#include "../MCAL/SysTick/SysTick_interface.h"


/*
 * ==================================================================================================================================
 *                                                         Global Vairables
 * ===================================================================================================================================
 */

u8 Global_u8Temp_value;


u8 recived_buffer[4];
u8 recived_flag=0;
u8 tempAuto_flag =0;
s8 counter=0;

/*
 * ==================================================================================================================================
 *                                                         OS Tasks
 * ===================================================================================================================================
 */



/*
 * ==================================================================================================================================
 *                                                         Main Function
 * ===================================================================================================================================
 */

void UART_Call_back()
{
	//@F0;
	u8 value;

	if(counter>=0)
	{
		value = UART_u8Recive(UART_6, enable);
	    recived_buffer[counter]=value;
	    if(counter==3)
	    {
	    	counter=-1;
	    	recived_flag=1;

	    }
	}
	counter++;
}

void send_data(u8 type,u8 data)
{
	u8 temp = 0;

	/*THE START BYTE OF THE FRAME*/
	UART_vTransmite(UART_6, '@', enable);

	/*SENDING THE TYPE OF DATA*/
	UART_vTransmite(UART_6, type, enable);

	/*SENDING EQUAL SIGN*/
	UART_vTransmite(UART_6, '=', enable);

	/*SENDING THE DATA BASED ON HOW MANY DIGITS OF DATA WILL BE SEND*/
	if(data == 0)
	{
		/*SENDING ONE DIGIT AND MAKE SURE THAT IT IS ZERO*/
		UART_vTransmite(UART_6, '0', enable);
	}
	else if(data < 10)
	{
		/*SENDING DATA WHEN IT IS ONE DIGIT ONLY*/
		UART_vTransmite(UART_6, (data + 48), enable);
	}
	else if (data < 100)
	{
		/*SENDING DATA WHEN IT IS TWO DIGIT ONLY*/
		temp = data / 10;
		UART_vTransmite(UART_6, temp + 48, enable); // SENDING THE TENS AT FIRST
		temp = data % 10;
		UART_vTransmite(UART_6, temp + 48, enable); // SENDING THE ONES SECOND
	}
	else if (data < 1000)
		{
			/*SENDING DATA WHEN IT IS TWO DIGIT ONLY*/
			temp = data / 100;
			UART_vTransmite(UART_6, temp + 48, enable); // SENDING THE TENS AT FIRST
			temp = (data / 10) % 10;
			UART_vTransmite(UART_6, temp + 48, enable); // SENDING THE ONES SECOND
			temp = data  % 10;
			UART_vTransmite(UART_6, temp + 48, enable); // SENDING THE ONES SECOND
		}
	else
	{
		/*DO NOTHING*/
	}

	/*THE END BYTE OF THE FRAME*/
	UART_vTransmite(UART_6, ';', enable);

}

u8 Global_u8Temp_value_old = 0;

void tempSensor()
{
	/*READING THE TEMPRATURE SENSOR THROUGH THE ADC CHANNEL 4*/
	Global_u8Temp_value = LM35_u16_Get_Temp(ADC_CHANNEL_4);



	LCD_enuGoTo(0,0);
	LCD_enuSendString("TEMP = ");
	LCD_enuSendNumber(Global_u8Temp_value-20);


	/*CONTROLLING THE FAN DEPENDING ON THE READING OF THE TEMPRATURE SENSOR AND THE tempAuto_flag */
	if((Global_u8Temp_value > 25) && (tempAuto_flag == 1)) // TURN ON THE FAN
	{
		MGPIO_voidWriteData(_GPIOB_PORT,_PIN_12,HIGH);
	}
	else if((Global_u8Temp_value < 25) && (tempAuto_flag == 1)) // TURN OFF THE FAN
	{
		MGPIO_voidWriteData(_GPIOB_PORT,_PIN_12,LOW);
	}
	else
	{
		/*DO NOTHING*/
	}

	/*CHECKING IF THE VALUE OF THE TEMPRATURE HAS CHANGED OR NOT */
	/*IF THE VALUE HAS CHANGED THEN SEND THE NEW VALUE TO THE GUI*/
	if(Global_u8Temp_value_old != Global_u8Temp_value)
	{
		Global_u8Temp_value_old = Global_u8Temp_value;
		send_data('T', (Global_u8Temp_value -20));
	}
	else
	{
		/*DO NOTHING*/
	}





}

u8 Global_u8GAS_value;
u8 Global_u8Gas_value_old = 1;


void gasSensor()
{
	Global_u8GAS_value = MGPIO_u8ReadData(_GPIOA_PORT,_PIN_10);


	/*CHECKING IF THE VALUE OF THE TEMPRATURE HAS CHANGED OR NOT */
	/*IF THE VALUE HAS CHANGED THEN SEND THE NEW VALUE TO THE GUI*/
	if(Global_u8Gas_value_old != Global_u8GAS_value)
	{
		Global_u8Gas_value_old = Global_u8GAS_value;
		send_data('F', Global_u8GAS_value);


	}
	else
	{
		/*DO NOTHING*/

	}

}



u16 Global_u8LDR_value = 0;
u16 Global_u8LDR_value_old = 0;


void LDR_Sensor()
{
	Global_u8LDR_value = LDR_u32_Get_value(ADC_CHANNEL_5);
	//Global_u8LDR_value = ((Global_u8LDR_value - 100) / 870) * 100;


	//Global_u8LDR_value = ((float)(Global_u8LDR_value - 105) / 700) * 100;





	/*CHECKING IF THE VALUE OF THE LDR HAS CHANGED OR NOT */
	/*IF THE VALUE HAS CHANGED THEN SEND THE NEW VALUE TO THE GUI*/
	if(Global_u8LDR_value != Global_u8LDR_value_old)
	{
		Global_u8LDR_value_old = Global_u8LDR_value;
		send_data('L', Global_u8LDR_value);
	}
	else
	{
		/*DO NOTHING*/
	}

}





int main()
{

/************************************** RCC *************************************/
	MRCC_voidInitClock(HSI,_HSE_CRYSTAL,_HSE_PLL);
	MRCC_voidEnablePeripheralClock(AHB1_BUS,_PERIPHERAL_EN_GPIOA);
	MRCC_voidEnablePeripheralClock(AHB1_BUS,_PERIPHERAL_EN_GPIOB);
	MRCC_voidEnablePeripheralClock(AHB1_BUS,_PERIPHERAL_EN_GPIOC);

	MRCC_voidEnablePeripheralClock(APB2_BUS,PERIPHERAL_EN_ADC1);
	MRCC_voidEnablePeripheralClock(APB2_BUS,PERIPHERAL_EN_USART6);

/****************************************** GPIO PINS **************************/
    //FOR LCD
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_0,_MODE_OUTPUT);
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_1,_MODE_OUTPUT);
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_2,_MODE_OUTPUT);
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_3,_MODE_OUTPUT);
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_6,_MODE_OUTPUT);
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_7,_MODE_OUTPUT);


	MGPIO_voidSetPinMode(_GPIOB_PORT,_PIN_12,_MODE_OUTPUT);//FAN

	//For DOOR

	MGPIO_voidSetPinMode(_GPIOC_PORT,_PIN_14,_MODE_OUTPUT); //MOTOR M1
	MGPIO_voidSetPinMode(_GPIOC_PORT,_PIN_15,_MODE_OUTPUT);//MOTOR M2

	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_10,_MODE_INPUT); //smoke
	MGPIO_voidSetPullType(_GPIOA_PORT,_PIN_10,_PULL_UP);

	//For Garage
	MGPIO_voidSetPinMode(_GPIOB_PORT,_PIN_5,_MODE_OUTPUT);//LED
	MGPIO_voidSetPinMode(_GPIOB_PORT,_PIN_6,_MODE_OUTPUT);//LED


	// ADC Pins
    MGPIO_voidSetPinMode(_GPIOA_PORT , _PIN_4 , _MODE_ANALOG );//LM35

    MGPIO_voidSetPinMode(_GPIOA_PORT , _PIN_5 , _MODE_ANALOG );//GARAGE

    //Uart Pins
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_11,_MODE_ALTF);
	MGPIO_voidSetPinMode(_GPIOA_PORT,_PIN_12,_MODE_ALTF);
    MGPIO_voidSetPinAltFn(_GPIOA_PORT , _PIN_11, _ALTFN_8 );
    MGPIO_voidSetPinAltFn(_GPIOA_PORT , _PIN_12, _ALTFN_8 );



/********************************* Initilizations *******************************/
    SysTick_vInit();
    LM35_vidInit();
    LCD_VidInit();


	UART_tstrConfiguration object;
	object.UART_BaudRate = UART_BAUDRATE_9600;
	object.UART_Interrupt_Enable = UART_INT_ENABLE_RXNEIE;
	object.p_IRQ_CallBack = UART_Call_back;
	object.UART_ParityBit = UART_NONE_PARITY;
	object.UART_PStopBits =  UART_ONE_STOP_BIT;
	object.UART_WordLength =UART_EIGHT_DATA_BITS;
	object.UART_Mode = UART_FULL_DUPLEX;
    UART_vInit(UART_6, &object);


 while (1)
 {
	tempSensor();
	Systic_vDelay_ms(100);

	gasSensor();
	Systic_vDelay_ms(100);

	LDR_Sensor();
	Systic_vDelay_ms(100);


	 if(recived_flag==1)
	 {
		 recived_flag=0;

		 switch(recived_buffer[1])
		 {
		 case 'F' :
			 	 	 if (recived_buffer[2]=='0')
			 	 	 {
			 	 		 MGPIO_voidWriteData(_GPIOB_PORT,_PIN_12,LOW);
						 tempAuto_flag = 0;
			 	 	 }
			 	 	 else if(recived_buffer[2]=='1')
			 	 	 {
			 	 		 MGPIO_voidWriteData(_GPIOB_PORT,_PIN_12,HIGH);
						 tempAuto_flag = 0;
			 	 	 }
					 else if(recived_buffer[2]=='A') // AUTO TURN ON/OFF THE FAN BASED ON THE TEMPRATURE
			 	 	 {
			 	 		 tempAuto_flag = 1;
			 	 	 }

			 	 	 break;
		 case 'D' :
			 	 	 if (recived_buffer[2]=='0')
					 {
			 	 		MGPIO_voidWriteData(_GPIOC_PORT,_PIN_14,LOW);//MOTOR M1
			 	 		MGPIO_voidWriteData(_GPIOC_PORT,_PIN_15,LOW);//MOTOR M2
				 	 }
					 else if(recived_buffer[2]=='1')
					 {

							MGPIO_voidWriteData(_GPIOC_PORT,_PIN_14,HIGH);//MOTOR M1
						    MGPIO_voidWriteData(_GPIOC_PORT,_PIN_15,LOW);//MOTOR M2

						 //DELAY
						    Systic_vDelay_ms(1000);

							MGPIO_voidWriteData(_GPIOC_PORT,_PIN_14,LOW);//MOTOR M1
							MGPIO_voidWriteData(_GPIOC_PORT,_PIN_15,HIGH);//MOTOR M2

							Systic_vDelay_ms(1000);

							MGPIO_voidWriteData(_GPIOC_PORT,_PIN_14,LOW);//MOTOR M1
							MGPIO_voidWriteData(_GPIOC_PORT,_PIN_15,LOW);//MOTOR M2

				 	 }

			 	 	 break;
		 case 'L' :

			 	 	 if (recived_buffer[2]=='0')
			 	 	 {
				 		 MGPIO_voidWriteData(_GPIOB_PORT,_PIN_5,LOW);
						 MGPIO_voidWriteData(_GPIOB_PORT,_PIN_6,LOW);
				 	 }
			 	 	 else if(recived_buffer[2]=='1')
				 	 {
				 		 MGPIO_voidWriteData(_GPIOB_PORT,_PIN_5,HIGH);
						 MGPIO_voidWriteData(_GPIOB_PORT,_PIN_6,LOW);
		    	 	 }
					 else if(recived_buffer[2]=='2')
					 {
						MGPIO_voidWriteData(_GPIOB_PORT,_PIN_5,HIGH);
						MGPIO_voidWriteData(_GPIOB_PORT,_PIN_6,HIGH);
					 }
			 	 	break;

		 default  :
				    break;
	 }

	 }







  }
return 0;
}
