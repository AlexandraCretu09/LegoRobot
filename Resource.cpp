///*
// * Resource.cpp
// *
// *  Created on: 26 Jul 2024
// *      Author: alexa
// */
//#include <BrickPi3.h>
//#include <iostream>
//#include <unistd.h>
//#include <cstring>
//#include <cmath>
//using namespace std;
//
//struct intersection{
//	intersection *left, *right,*forward, *backwards;
//	bool v_left, v_right, v_forward;
//	bool f_left, f_right, f_forward;
//
//};
//
//
//enum way{
//	way_left,
//	way_right,
//	way_forward,
//	way_backwards,
//	null
//};
//
//
//int getPortNumber1(int x){
//	switch(x){
//	case 1:
//		return PORT_1;
//	case 2:
//		return PORT_2;
//	case 3:
//		return PORT_3;
//	case 4:
//		return PORT_4;
//	default:
//		printf("Invalid port number");
//		return -1;
//	}
//}
//
//
//
//bool killButton(BrickPi3 BP){
//	sensor_touch_t Button;
//	if(!BP.get_sensor(PORT_4, &Button)){
//		return Button.pressed;
//	}
//}
//
//
//float returnUltrasonicValue1(int x,  BrickPi3 BP){
//
//	int PORT = getPortNumber1(x);
//
//	sensor_ultrasonic_t Ultra;
//
//	if(BP.get_sensor(PORT, &Ultra)){
//		;
//	}else{
//		//printf("Port %d: cm: %f\n", x, Ultra.cm);
//		return Ultra.cm;
//	}
//	return NULL;
//}
//
//int16_t returnGyroValue(BrickPi3 BP){
//	sensor_gyro_t Gyro;
//	if(!BP.get_sensor(PORT_4, &Gyro)){
//		return Gyro.abs;
//	}
//}
//
//void setRightWheelSpeed(int speed, BrickPi3 BP){
//	BP.set_motor_power(PORT_B, speed);
//}
//
//void setRightWheelDPS1(int degrees, BrickPi3 BP){
//	BP.set_motor_dps(PORT_B, degrees);
//}
//
//void setLeftWheelSpeed(int speed, BrickPi3 BP){
//	BP.set_motor_power(PORT_C, speed);
//}
//
//void setLeftWheelDPS1(int degrees, BrickPi3 BP){
//	BP.set_motor_dps(PORT_C, degrees);
//}
//
//
//
//
//
//void PID_Left(BrickPi3 BP, float base_speed_left, float error){
//	float kp = 0.1;
//
//	if(error != 0.0){
//		float correction = error * kp;
//		base_speed_left += correction;
//		setLeftWheelSpeed(base_speed_left, BP);
//	}
//}
//
//void PID_Right(BrickPi3 BP, float base_speed_right, float error_correction){
//	float kp = 0.1;
//
//	if(error_correction != 0.0){
//		float correction = error_correction * kp;
//		base_speed_right += correction;
//		setRightWheelSpeed(base_speed_right, BP);
//	}
//}
//
//
//float PID_DPS_Left(BrickPi3 BP, float error, float base_dps){
//
//	float kp = 8;
//
//	error = -error;
//
//	float correction = 0;
//	correction = error * kp;
//	base_dps += correction;
//	printf("\nBase dps left: %f", base_dps);
//	setLeftWheelDPS1(base_dps, BP);
//
//	return base_dps;
//
//}
//
//float PID_DPS_Right(BrickPi3 BP, float error, float base_dps){
//
//	float kp = 8;
//
//	float correction = 0;
//	correction = error * kp;
//	base_dps += correction;
//	printf("Base dps right: %f\n", base_dps);
//	setRightWheelDPS1(base_dps, BP);
//
//	return base_dps;
//
//}
//
//
//bool checkPositionForPID(BrickPi3 BP, float positionB, float positionC){
//
//	uint8_t StateB, StateC;
//	int8_t PowerB, PowerC;
//	int32_t PositionB, PositionC;
//	int16_t DPSB, DPSC;
//
//	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//	BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//
//	if(positionC < PositionC ||  positionB < PositionB){
//		return true;
//	}
//	return  false;
//}
//
//
//
//intersection *createIntersectionNode(bool left, bool right, bool forward, intersection *previous, way previous_way){
//	intersection *i = new intersection;
//
//	if(left){
//		i->f_left = true;
//	}
//	if(right){
//		i->f_right = true;
//	}
//	if(forward){
//		i->f_forward = true;
//	}
//	i->backwards = previous;
//
//	switch(previous_way){
//		case way_left:
//			previous->left = i;
//			break;
//		case way_right:
//			previous->right = i;
//			break;
//		case way_forward:
//			previous->forward = i;
//			break;
//		case null:
//			break;
//	}
//	return i;
//
//}
//
//
//bool checkForIntersection(bool &left, bool &right, bool &forward, BrickPi3 BP){
//	float sen_left, sen_right, sen_forward;
//
//	sen_left = returnUltrasonicValue1(1,  BP);
//	sen_right = returnUltrasonicValue1(2,  BP);
//	sen_forward = returnUltrasonicValue1(3, BP);
//
//	if(sen_left > 20){
//		left = true;
//	}
//	if(sen_right > 20){
//		right = true;
//	}
//	if((left || right) && sen_forward > 20){
//		forward = true;
//	}
//	if(left || right)
//		return true;
//	return false;
//}
//
//
//
//float positionB, positionC;
//
//void moveForward(BrickPi3 BP){
//
//	bool ok = false;
//
//	uint8_t StateB, StateC;
//	int8_t PowerB, PowerC;
//	int32_t PositionB, PositionC;
//	int16_t DPSB, DPSC;
//
//	float base_dps_left;
//	float base_dps_right;
//
//	float new_base_dps_left, new_base_dps_right;
//
//	base_dps_left = base_dps_right = 180;
//
//	setLeftWheelDPS1(base_dps_left, BP);
//	setRightWheelDPS1(base_dps_right, BP);
//
//	//printf("Base dps left: %f, Base dps right: %f\n", base_dps_left, base_dps_right);
//
//	float error = returnUltrasonicValue1(1,  BP) - returnUltrasonicValue1(2,  BP);
//
//
//	if(error != 0){
//		if(ok){
//			float procent = 20;
//			float x, y;
//			x = new_base_dps_left * procent / 100;
//			y = new_base_dps_right * procent / 100;
//			if(new_base_dps_left > base_dps_left){
//
//				setLeftWheelDPS1(base_dps_left - x, BP);
//				setRightWheelDPS1(base_dps_right + y, BP);
//			}
//			else{
//				setLeftWheelDPS1(base_dps_left + x, BP);
//				setRightWheelDPS1(base_dps_right - y, BP);
//			}
//			ok = false;
//		}
//		if(checkPositionForPID(BP, positionB, positionC)){
//
//			new_base_dps_left = PID_DPS_Left(BP, error, base_dps_left );
//			new_base_dps_right = PID_DPS_Right(BP, error, base_dps_right);
//
//			BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//			BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//			positionC = PositionC +   pow(error, 2);
//			positionB = PositionB +   pow(error, 2);
//
//			printf("One pid made.\n");
//
//			ok = true;
//		}
//
//
//	}
//}
//
//
//bool rotationImplementation(way direction, BrickPi3 BP);
//void rotateRight(BrickPi3 BP);
//void rotateLeft(BrickPi3 BP);
//void rotateBackwards(BrickPi3 BP);
//int getBackwardsDirection(intersection *i);
//
//
//uint8_t StateB, StateC;
//int8_t PowerB, PowerC;
//int32_t PositionB, PositionC;
//int16_t DPSB, DPSC;
//
//
//void checkForRotation(intersection *i, BrickPi3 BP){
//	setRightWheelSpeed(0, BP);
//	setLeftWheelSpeed(0, BP);
//	if(i->f_right && !i->v_right){ //if way to the right exists
//
//		rotationImplementation(way_right, BP);
//		i->v_right = true;
//	}
//	else if(i->forward && !i->v_forward && i->f_right){ //if way forward and way right exists
//
//		rotationImplementation(way_right, BP);
//		i->v_forward = true;
//	}
//	else if(i->forward && !i->v_forward && !i->f_right){ //if way forward exists and way right does not exist
//
//		//go forward
//		i->v_forward = true;
//	}
//	else if(i->f_left && !i->v_left && !i->f_right && !i->v_forward){
//
//		rotationImplementation(way_left, BP);
//		i->v_left = true;
//	}
//	else if(i->f_left && !i->v_left && i->f_right && !i->v_forward){
//
//		//go forward
//		i->v_left = true;
//	}
//	else if(i->f_left && !i->v_left && i->v_forward){
//
//		rotationImplementation(way_right, BP);
//		i->v_left = true;
//	}
//
//
//	else{
//		int direction = getBackwardsDirection(i);
//		switch(direction){
//			case 90:
//				//rotate to the left
//				rotationImplementation(way_left, BP);
//				break;
//			case 180:
//				//rotate 180 degrees
//				rotationImplementation(way_backwards, BP);
//				break;
//			case -90:
//				//rotate to the right
//				rotationImplementation(way_right, BP);
//				break;
//		}
//	}
//}
//
//
//bool rotationImplementation(way direction, BrickPi3 BP){
//
//	BP.reset_motor_encoder(PORT_B + PORT_C);
//
//	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//	BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//
//	int16_t initial_gyro_value;
//
//	initial_gyro_value = returnGyroValue(BP);
//
//
//	int16_t present_gyro_value;
//	int16_t expected_gyro_value;
//
//
//	switch(direction){
//		case way_right:
//			rotateRight(BP);
//			return true;
//
//		case way_left:
//			rotateLeft(BP);
//			return true;
//
//		case way_backwards:
//			rotateBackwards(BP);
//			return true;
//	}
//	BP.reset_motor_encoder(PORT_B + PORT_C);
//	return false;
//}
//
//
//void rotateRight(BrickPi3 BP){
//	BP.reset_motor_encoder(PORT_C);
//	printf("\nCase right\n");
//
//	while(PositionB > -50){
//		if(killButton(BP))
//			break;
//		BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//		//printf("PoditionB: %d StateB: %d\n", PositionB, StateB);
//		setRightWheelSpeed(-15, BP);
//		setLeftWheelSpeed(-15, BP);
//	}
//
//	BP.reset_motor_encoder(PORT_C);
//	BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//
//	while(PositionC < 515){
//		if(killButton(BP))
//			break;
//		printf("Position left wheel: %d\n", PositionC);
//		setRightWheelSpeed(5, BP);
//		setLeftWheelSpeed(25, BP);
//		BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//
//	}
//	setLeftWheelSpeed(0, BP);
//	setRightWheelSpeed(0, BP);
//
//}
//
//void rotateLeft(BrickPi3 BP){
//	printf("\n\n\n\nCase left\n");
//
//	BP.reset_motor_encoder(PORT_B);
//
//	while(PositionB > -50){
//		if(killButton(BP))
//			break;
//		BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//		//printf("PoditionB: %d StateB: %d\n", PositionB, StateB);
//		setRightWheelSpeed(-15, BP);
//		setLeftWheelSpeed(-15, BP);
//	}
//
//
//
//	BP.reset_motor_encoder(PORT_B);
//	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//
//	while(PositionB < 480){
//		if(killButton(BP))
//			break;
//		printf("Position right wheel%d\n", PositionB);
//		BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//		setRightWheelSpeed(25, BP);
//		setLeftWheelSpeed(5, BP);
//
//	}
//
//	setRightWheelSpeed(0, BP);
//	setLeftWheelSpeed(0, BP);
//}
//
//void rotateBackwards(BrickPi3 BP){
//	BP.reset_motor_encoder(PORT_B);
//	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//
//
//	printf("\nCase Backwards\n");
//
//	while(PositionB > -550){
//		if(killButton(BP))
//			break;
//		BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//		printf("PoditionB: %d StateB: %d\n", PositionB, StateB);
//		setRightWheelSpeed(-15, BP);
//		setLeftWheelSpeed(-15, BP);
//	}
//
//	BP.reset_motor_encoder(PORT_B);
//	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//
//	while(PositionB < 450 || DPSB == 0){
//		if(killButton(BP))
//			break;
//		BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//		//printf("PoditionB: %d\n", PositionB);
//		printf("1st While, DPS: %d\n", DPSB);
//		setRightWheelSpeed(20, BP);
//		setLeftWheelSpeed(0, BP);
//	}
//
//	BP.reset_motor_encoder(PORT_B);
//	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//	BP.reset_motor_encoder(PORT_C);
//	BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//
//	while(PositionC > -400 || DPSC == 0){
//		if(killButton(BP))
//			break;
//		printf("2nd While\n");
//		printf("PoditionC: %d\n", PositionC);
//		BP.get_motor_status(PORT_C, StateC, PowerC, PositionC, DPSC);
//		setRightWheelSpeed(0, BP);
//		setLeftWheelSpeed(-20, BP);
//	}
//
//	BP.reset_motor_encoder(PORT_B);
//	BP.get_motor_status(PORT_B, StateB, PowerB, PositionB, DPSB);
//
//	setRightWheelSpeed(0, BP);
//	setLeftWheelSpeed(0, BP);
//}
//
//
//int getBackwardsDirection(intersection *i){
//	if(i->f_right && !i->f_forward && !i->f_left)
//		return 90;
//	if(i->f_forward && !i->f_left)
//			return 180;
//	if(i->f_left)
//			return -90;
//}
//
//way getPreviousDirection(intersection *i){
//	intersection *p = i->backwards;
//	way direction;
//	if(p->v_right && !p->v_forward && !p->v_left){
//		direction = way_right;
//	}
//	if(p->v_forward && !p->v_left){
//		direction = way_forward;
//	}
//	if(p->v_left){
//		direction = way_left;
//	}
//	return direction;
//}
//
//
//
//
//void startLabyrinth(intersection *root, BrickPi3 BP){
//
//
//	uint8_t StateB, StateC;
//	int8_t PowerB, PowerC;
//	int32_t PositionB, PositionC;
//	int16_t DPSB, DPSC;
//
//	bool left, right, forward;
//	float sen_left, sen_right, sen_forward;
//
//
//	bool ok_left = false;
//	bool ok_right = false;
//
//
//	//float absoluteGyroValue = returnGyroValue(BP);
//
//	way previous_way;
//
//	bool canUseSensors;
//
//
//	while(true && !killButton(BP)){
//
//				left = right = forward = false;
//
//
//				if(!checkForIntersection(left, right, forward, BP)){
//					moveForward(BP);
//					canUseSensors = true;
//
//					//printf("\n");
//				}
//				else if(canUseSensors){
//					if(right){
//						canUseSensors = !rotationImplementation(way_right, BP);
//					}
//					else if(left){
//						canUseSensors = !rotationImplementation(way_left, BP);
//					}
//					setRightWheelSpeed(0, BP);
//					setLeftWheelSpeed(0, BP);
//
//
//				}else{
//					moveForward(BP);
//				}
//
//
//
//
//					//previous_way = getPreviousDirection(root);
//					//intersection *i = createIntersectionNode(left, right, forward, root, previous_way);
//
//					//checkForRotation(root, BP);
//
//
//
//
//
////					sen_left = returnUltrasonicValue(1,  BP);
////					sen_right = returnUltrasonicValue(2,  BP);
////					sen_forward = returnUltrasonicValue(3, BP);
//
//
//
//
////				else{
////
////					if (root->backwards == NULL)
////						previous_way = way_forward;
////					else
////						previous_way = getPreviousDirection(root);
////
////
////					intersection *new_i = 	createIntersectionNode(left, right, forward, root, previous_way);
////					checkForRotation(new_i, BP);
////					startLabyrinth(new_i, BP);
//
////					if(!checkForIntersection(left, right, forward, BP))
////		//					PID(BP);
////					else
////
////
////
//
//
//
//////
////				setRightWheelSpeed(0, BP);
////				setLeftWheelSpeed(0, BP);
////				break;
//
////				rotationImplementation(way_left, BP);
////				rotationImplementation(way_right, BP);
//
//
//
////				rotationImplementation(way_backwards, BP);
////
////
//
////
////
////
////				break;
//
//	}
//	setRightWheelDPS1(0, BP);
//	setLeftWheelDPS1(0, BP);
//
//}
//
//
// int main() {
//     BrickPi3 BP;
//     Sensor sensorObj(BP);  // Create Sensor object
//     SpecialCases specialCases(BP);
//
//     specialCases.toCloseToTheLeft(sensorObj, BP); // Handle left case
//
//     return 0;
// }