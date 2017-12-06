

#include "receivefromcan.h"
#include "can.h"
#include "_can_dbc/generated_Viserion.h"
#include <iostream>
#include "bluetoothtransmission.h"
#include "display.hpp"
#include <stdio.h>
#include <math.h>
#include    <stdlib.h>


#include <string.h>
using namespace std;

SEND_COORDINATES_t send_start_co={0};
SEND_GEO_ANGLES_t send_compass_dir={0};
MOTOR_STATUS_t speed ={0};
SENSORS_VALUES_t sensor_values={0};

double slatt;
double slongi;

int sensor_left=0;
int sensor_right=0;
int sensor_middle=0;
int sensor_rear=0;

char slatti[9];
char slongii[11];
char send[21];

int count =0;
int comp=0;
int car_speed=0;
void receiveallcanmsges(Uart3 & u3){
	can_msg_t can_msg;

	while (CAN_rx(can1, &can_msg, 0) && u3.getRxQueueSize()==0)
	{

		//cout<<"check"<<endl;
		dbc_msg_hdr_t can_msg_hdr;
		can_msg_hdr.dlc = can_msg.frame_fields.data_len;
		can_msg_hdr.mid = can_msg.msg_id;

		switch(can_msg_hdr.mid)
		{

		case 101:{
			if(dbc_decode_SENSORS_VALUES(&sensor_values,can_msg.data.bytes,&can_msg_hdr))
			{
				sensor_left = sensor_values.SENSOR_left_in;
				sensor_right=sensor_values.SENSOR_right_in;
				sensor_middle = sensor_values.SENSOR_middle_in;

				setSENSOR_LEFT_INIT(100);


			}

			break;
		}

		case 400:{

			if(dbc_decode_SEND_COORDINATES(&send_start_co,can_msg.data.bytes,&can_msg_hdr))
			{
				slatt=send_start_co.SEND_LATTITUDE;
				slongi=send_start_co.SEND_LONGITUDE;


			   sprintf(slatti,"%f",slatt);
			  sprintf(slongii,"%f",slongi);

			   strcat(send,slatti);
			   strcat(send,",");
			   strcat(send,slongii);
				u3.putline(send,portMAX_DELAY);
				//write function for what to do when u receive coordinates from gps
			}

		}
		break;
		case 401 :{
				if(dbc_decode_SEND_GEO_ANGLES(&send_compass_dir,can_msg.data.bytes,&can_msg_hdr))
					 comp=send_compass_dir.SEND_HEAD;
				setDIRECTION_INIT(comp);
		}
		break;
		case 301 :{
				if(dbc_decode_MOTOR_STATUS(&speed,can_msg.data.bytes,&can_msg_hdr))

				{
				 car_speed=speed.MOTOR_Send_Speed;
				 setSPEED_INIT(car_speed);

					//write function for what to do when u receive degreeval from compass
				}
		}
		break;
		}

	}

}
void sendDetails(Uart3 & u3,bool stats){
	if(stats == 1){

		char sensor_send[15]="O";
		char value_left[3];
		char value_right[3];
		char value_middle[3];

		itoa(sensor_left,value_left,10);
		itoa(sensor_right,value_right,10);
		itoa(sensor_middle,value_middle,10);

		strcat(sensor_send,value_right);
		strcat(sensor_send,",");
		strcat(sensor_send,value_middle);
		strcat(sensor_send,",");
		strcat(sensor_send,value_left);
		strcat(sensor_send,",");
		strcat(sensor_send,"85");
		sendtoapp(sensor_send,u3);


		char compass[4]="D";
		char value[3];
		itoa(comp,value,10);
		strcat(compass,value);
		strcat(compass,"\n");
		sendtoapp(compass,u3);


		char mph[4]="S";
		char value1[3];
		itoa(car_speed,value1,10);
		strcat(mph,value1);
		strcat(mph,"\n");
		sendtoapp(mph,u3);





	}


}
