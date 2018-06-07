#ifndef __mpu6050_h__
#define __mpu6050_h__

#include "stm32f10x.h"
#include "driver.h"
// 定义MPU6050 内部地址

#define SMPLRT_DIV 		0x19 			//陀螺仪采样率，典型值:0x07(125Hz)
#define CONFIG 			0x1A 			//低通滤波频率，典型值:0x06(5Hz)
#define GYRO_CONFIG 	0x1B  			//陀螺仪自检及测量范围，典型值:0x18(不自检,2000deg/s)
#define ACCEL_CONFIG 	0x1C 			//加速计自检、测量范围及高通滤波频率，典型值:0x01(不自检,2G,5Hz)
#define MPU6050_REG_RAW_DATA 0x3B
#define PWR_MGMT_1 		0x6B 			//电源管理，典型值:0x00(正常启动)
#define MPU6050_RA_USER_CTRL        0x6A
#define MPU6050_RA_INT_PIN_CFG      0x37

#define mpu6050_Address 0xd0 			//IIC 写入时的地址字节数据，+1为读取

#define FILTER_NUM 20					//滑动窗口滤波，窗口值
#define Gyro_G 	0.0610351				//角速度变成度    此参数对应陀螺2000度每秒	x/65535*4000
#define Gyro_Gr	0.0010653				//角速度变成弧度  此参数对应陀螺2000度每秒	x/65535*4000/180*3.14

//******加速度陀螺仪参数************
extern int16_t i16XGyro_Last,i16YGyro_Last,i16ZGyro_Last;
extern float  f32XAngle,f32YAngle,f32ZAngle;
void mpu6050_init(void);
void mpu6050_read(void);
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az);

#endif // __mpu6050_h__
