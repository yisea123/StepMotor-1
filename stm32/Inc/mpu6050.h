#ifndef __mpu6050_h__
#define __mpu6050_h__

#include "stm32f10x.h"
#include "driver.h"
// ����MPU6050 �ڲ���ַ

#define SMPLRT_DIV 		0x19 			//�����ǲ����ʣ�����ֵ:0x07(125Hz)
#define CONFIG 			0x1A 			//��ͨ�˲�Ƶ�ʣ�����ֵ:0x06(5Hz)
#define GYRO_CONFIG 	0x1B  			//�������Լ켰������Χ������ֵ:0x18(���Լ�,2000deg/s)
#define ACCEL_CONFIG 	0x1C 			//���ټ��Լ졢������Χ����ͨ�˲�Ƶ�ʣ�����ֵ:0x01(���Լ�,2G,5Hz)
#define MPU6050_REG_RAW_DATA 0x3B
#define PWR_MGMT_1 		0x6B 			//��Դ��������ֵ:0x00(��������)
#define MPU6050_RA_USER_CTRL        0x6A
#define MPU6050_RA_INT_PIN_CFG      0x37

#define mpu6050_Address 0xd0 			//IIC д��ʱ�ĵ�ַ�ֽ����ݣ�+1Ϊ��ȡ

#define FILTER_NUM 20					//���������˲�������ֵ
#define Gyro_G 	0.0610351				//���ٶȱ�ɶ�    �˲�����Ӧ����2000��ÿ��	x/65535*4000
#define Gyro_Gr	0.0010653				//���ٶȱ�ɻ���  �˲�����Ӧ����2000��ÿ��	x/65535*4000/180*3.14

//******���ٶ������ǲ���************
extern int16_t i16XGyro_Last,i16YGyro_Last,i16ZGyro_Last;
extern float  f32XAngle,f32YAngle,f32ZAngle;
void mpu6050_init(void);
void mpu6050_read(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif // __mpu6050_h__
