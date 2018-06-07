#include <math.h>
#include  <stdio.h>  //Keil library	
#include  <stdarg.h>

#include "mpu6050.h"

/* Private define ------------------------------------------------------------*/
int16_t i16XAcc_Last=0,i16YAcc_Last=0,i16ZAcc_Last=0;
int16_t i16XAcc_Offset=0,i16YAcc_Offset=0,i16ZAcc_Offset=0;
int16_t i16XGyro_Last=0,i16YGyro_Last=0,i16ZGyro_Last=0;
int16_t i16XGyro_Offset=-36,i16YGyro_Offset=17,i16ZGyro_Offset=-7;
int16_t i16XAccBuffer[FILTER_NUM],i16YAccBuffer[FILTER_NUM],i16ZAccBuffer[FILTER_NUM];
int16_t i16XAccAverage=0,i16YAccAverage=0,i16ZAccAverage=0;
float   f32XAngle=0,f32YAngle=0,f32ZAngle=0;

//��ʼ��MPU6050
void mpu6050_init(void)
{
	uint8_t parm[7]={0x01,0x04,0x02,0x18,0x08,0x00,0x02};
	IIC_Init();
	Delay_Ms(10);
	
	IIC_WriteByte(mpu6050_Address, PWR_MGMT_1, parm[0]);//�˳�˯��ģʽ����ȡ��ʱ��Ϊ������x��
	Delay_Ms(2);
	IIC_WriteByte(mpu6050_Address, SMPLRT_DIV, parm[1]);//�����ǲ�����,25hz,δ��
	Delay_Ms(2);
	IIC_WriteByte(mpu6050_Address, CONFIG, 	   parm[2]);//��ͨ�˲�Ƶ��,δ��
	Delay_Ms(2);
	IIC_WriteByte(mpu6050_Address, GYRO_CONFIG,parm[3]);//�������Լ켰������Χ, +-2000 ��/s
	Delay_Ms(2);
	IIC_WriteByte(mpu6050_Address,ACCEL_CONFIG,parm[4]);//���ټ��Լ졢������Χ����ͨ�˲�Ƶ��, +-4g
	Delay_Ms(2);
	IIC_WriteByte(mpu6050_Address, MPU6050_RA_USER_CTRL,parm[5]);//����MPU6050 ����AUXI2C
	Delay_Ms(2);
	IIC_WriteByte(mpu6050_Address, MPU6050_RA_INT_PIN_CFG, parm[6]);//������ֱ�ӷ���HMC5883L
	Delay_Ms(2);    
}

//**************************************
//MPU6050���ݶ�ȡ
//**************************************
void mpu6050_read(void)
{   
	uint8_t buffer[14];	
	static uint8_t filter_cnt=0;
	int32_t temp1=0,temp2=0,temp3=0;
	uint8_t i; 
	
	IIC_ReadBytes(mpu6050_Address,MPU6050_REG_RAW_DATA,14,buffer);
	
	i16XAcc_Last=(((((int16_t)buffer[0]) << 8) | buffer[1])) - i16XAcc_Offset;
	i16YAcc_Last=(((((int16_t)buffer[2]) << 8) | buffer[3])) - i16YAcc_Offset;
	i16ZAcc_Last=(((((int16_t)buffer[4]) << 8) | buffer[5])) - i16ZAcc_Offset;
	
	i16XGyro_Last=((((int16_t)buffer[8])  << 8) | buffer[9])  - i16XGyro_Offset;
	i16YGyro_Last=((((int16_t)buffer[10]) << 8) | buffer[11]) - i16YGyro_Offset;
	i16ZGyro_Last=((((int16_t)buffer[12]) << 8) | buffer[13]) - i16ZGyro_Offset;
	
	//���»�����������
	i16XAccBuffer[filter_cnt] = i16XAcc_Last;	
	i16YAccBuffer[filter_cnt] = i16YAcc_Last;
	i16ZAccBuffer[filter_cnt] = i16ZAcc_Last;
	
	for(i=0;i<FILTER_NUM;i++)
	{
		temp1 += i16XAccBuffer[i];
		temp2 += i16YAccBuffer[i];
		temp3 += i16ZAccBuffer[i];
	}
	i16XAccAverage = temp1 / FILTER_NUM;
	i16YAccAverage = temp2 / FILTER_NUM;
	i16ZAccAverage = temp3 / FILTER_NUM;
	filter_cnt++;
	if(filter_cnt==FILTER_NUM)	filter_cnt=0;

	IMUupdate(	i16XGyro_Last*Gyro_Gr,
				i16YGyro_Last*Gyro_Gr,
				i16ZGyro_Last*Gyro_Gr,
				i16XAccAverage,
				i16YAccAverage,
				i16ZAccAverage);
	
}

////////////////////////////////////////////////////////////////////////////////
//**************************************
//��Ԫ���㷨
//**************************************
#define Kp 10.0f                        // proportional gain governs rate of convergence to accelerometer/magnetometer
#define Ki 0.008f                          // integral gain governs rate of convergence of gyroscope biases
#define halfT 0.005f                   // half the sample period�������ڵ�һ��

float q0 = 1, q1 = 0, q2 = 0, q3 = 0;    // quaternion elements representing the estimated orientation
float exInt = 0, eyInt = 0, ezInt = 0;    // scaled integral error
void IMUupdate(float gx, float gy, float gz, float ax, float ay, float az)
{
	float norm;
	//float hx, hy, hz, bx, bz;
	float vx, vy, vz;// wx, wy, wz;
	float ex, ey, ez;

	// �Ȱ���Щ�õõ���ֵ���
	float q0q0 = q0*q0;
	float q0q1 = q0*q1;
	float q0q2 = q0*q2;
	//  float q0q3 = q0*q3;
	float q1q1 = q1*q1;
	//  float q1q2 = q1*q2;
	float q1q3 = q1*q3;
	float q2q2 = q2*q2;
	float q2q3 = q2*q3;
	float q3q3 = q3*q3;
	
	if(ax*ay*az==0)
 		return;
		
	norm = my_sqrt(ax*ax + ay*ay + az*az);       //acc���ݹ�һ��
	ax = ax /norm;
	ay = ay / norm;
	az = az / norm;

	// estimated direction of gravity and flux (v and w)              �����������������/��Ǩ
	vx = 2*(q1q3 - q0q2);												//��Ԫ����xyz�ı�ʾ
	vy = 2*(q0q1 + q2q3);
	vz = q0q0 - q1q1 - q2q2 + q3q3 ;

	// error is sum of cross product between reference direction of fields and direction measured by sensors
	ex = (ay*vz - az*vy) ;                           					 //�������������õ���־������
	ey = (az*vx - ax*vz) ;
	ez = (ax*vy - ay*vx) ;

	exInt = exInt + ex * Ki;								  //�������л���
	eyInt = eyInt + ey * Ki;
	ezInt = ezInt + ez * Ki;

	// adjusted gyroscope measurements
	gx = gx + Kp*ex + exInt;					   							//�����PI�󲹳��������ǣ����������Ư��
	gy = gy + Kp*ey + eyInt;
	gz = gz + Kp*ez + ezInt;				   							//�����gz����û�й۲��߽��н��������Ư�ƣ����ֳ����ľ��ǻ����������Լ�

	// integrate quaternion rate and normalise						   //��Ԫ�ص�΢�ַ���
	q0 = q0 + (-q1*gx - q2*gy - q3*gz)*halfT;
	q1 = q1 + (q0*gx + q2*gz - q3*gy)*halfT;
	q2 = q2 + (q0*gy - q1*gz + q3*gx)*halfT;
	q3 = q3 + (q0*gz + q1*gy - q2*gx)*halfT;

	// normalise quaternion
	norm = my_sqrt(q0*q0 + q1*q1 + q2*q2 + q3*q3);
	q0 = q0 / norm;
	q1 = q1 / norm;
	q2 = q2 / norm;
	q3 = q3 / norm;
	
	f32XAngle = asin(-2 * q1 * q3 + 2 * q0* q2)* 57.3; // pitch
	f32YAngle = fast_atan2(2 * q2 * q3 + 2 * q0 * q1, -2 * q1 * q1 - 2 * q2* q2 + 1)* 57.3; // roll
	f32ZAngle = fast_atan2(2 * q1 * q2 + 2 * q0 * q3, -2 * q2*q2 - 2 * q3* q3 + 1)* 57.3; // yaw
    

}

















