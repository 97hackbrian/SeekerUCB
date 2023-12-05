import board
import adafruit_mpu6050
import time
def calibration(cal_time):
    i2c=board.I2C()
    mpu = adafruit_mpu6050.MPU6050(i2c)
    n=0
    start=time.time()
    accel_total=[0,0,0]
    gyr_total=[0,0,0]
    while(time.time()-start<cal_time):
        accel_data = mpu.acceleration
        gyro_data = mpu.gyro
        n+=1
        for index in range (len(accel_data)):
            accel_total[index]+=accel_data[index]
            gyr_total[index]+=gyro_data[index]
    prom_a=[x/n for x in accel_total]
    prom_g=[x/n for x in gyr_total]
    return (prom_a,prom_g)
