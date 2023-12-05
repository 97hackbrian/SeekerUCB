#!/usr/bin/env python

import rclpy
from sensor_msgs.msg import Imu
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
import board
import adafruit_mpu6050
import time

class Mynode(Node):
    def __init__(self):
        super().__init__("mpu6050_pub")
        self.get_logger().info("mpu6050_pub active")
        self.i2c=board.I2C()
        self.mpu = adafruit_mpu6050.MPU6050(self.i2c)
        self.offs_a,self.offs_g=self.calibration(5)
        self.imu_pub = self.create_publisher(Imu, 'mpu6050_ros',qos_profile_sensor_data)
        self.imu_timer = self.create_timer(0.05, self.publish_imu)

    def publish_imu(self):
        imu_msg = Imu()
        # Read the acceleration vals
        accel_data = self.mpu.acceleration
        imu_msg.linear_acceleration.x = accel_data[0]-self.offs_a[0]
        imu_msg.linear_acceleration.y = accel_data[1]-self.offs_a[1]
        imu_msg.linear_acceleration.z = accel_data[2]-(self.offs_a[2]-9.81)
        # Read the gyro vals
        gyro_data = self.mpu.gyro
        imu_msg.angular_velocity.x = gyro_data[0]-self.offs_g[0]
        imu_msg.angular_velocity.y = gyro_data[1]-self.offs_g[1]
        imu_msg.angular_velocity.z = gyro_data[2]-self.offs_g[2]

        imu_msg.header.stamp = self.get_clock().now().to_msg()
        self.imu_pub.publish(imu_msg)

    def calibration(self,cal_time):
        n=0
        start=time.time()
        accel_total=[0,0,0]
        gyr_total=[0,0,0]
        while(time.time()-start<cal_time):
            accel_data = self.mpu.acceleration
            gyro_data = self.mpu.gyro
            n+=1
            for index in range (len(accel_data)):
                accel_total[index]+=accel_data[index]
                gyr_total[index]+=gyro_data[index]
        prom_a=[x/n for x in accel_total]
        prom_g=[x/n for x in gyr_total]
        return (prom_a,prom_g)

def main(args=None):
    rclpy.init(args=args)
    node = Mynode()
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.destroy_node()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()