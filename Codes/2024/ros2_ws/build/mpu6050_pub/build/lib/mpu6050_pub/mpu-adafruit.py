#!/usr/bin/env python

import rclpy
from sensor_msgs.msg import Imu
from rclpy.node import Node
from rclpy.qos import qos_profile_sensor_data
import board
import adafruit_mpu6050
from ahrs.filters import Madgwick,Mahony
import numpy as np
import time
#calibration
from calibration import calibration

class Mynode(Node):
    def __init__(self,offs_a,offs_g):
        super().__init__("mpu6050_pub")
        self.get_logger().info("mpu6050_pub active")
        self.i2c=board.I2C()
        self.mpu = adafruit_mpu6050.MPU6050(self.i2c)
        self.imu_pub = self.create_publisher(Imu, 'mpu6050_ros',qos_profile_sensor_data)
        self.imu_timer = self.create_timer(0.01, self.publish_imu)
        self.offs_a=offs_a
        self.offs_g=offs_g
        self.madgwick = Madgwick()
        self.mahony = Mahony()
        self.q = np.array([0, 0, 0, 1], dtype=np.float64)
        self.q2 = np.array([0, 0, 0, 1], dtype=np.float64)

    def publish_imu(self):
        imu_msg = Imu()
        # Read the acceleration vals
        accel_data = self.mpu.acceleration
        imu_msg.linear_acceleration.x = accel_data[0]-self.offs_a[0]
        imu_msg.linear_acceleration.y = accel_data[1]-self.offs_a[1]
        imu_msg.linear_acceleration.z = -(accel_data[2]-(self.offs_a[2]-9.81))
        # Read the gyro vals
        gyro_data = self.mpu.gyro
        imu_msg.angular_velocity.x = gyro_data[0]-self.offs_g[0]
        imu_msg.angular_velocity.y = gyro_data[1]-self.offs_g[1]
        imu_msg.angular_velocity.z = gyro_data[2]-self.offs_g[2]
        #quaternions
        self.q=self.madgwick.updateIMU(self.q,gyro_data,accel_data) 
        self.q2=self.mahony.updateIMU(self.q2,gyro_data,accel_data)
        imu_msg.orientation.w = self.q2[0]
        imu_msg.orientation.x = self.q2[1]
        imu_msg.orientation.y = self.q2[2]
        imu_msg.orientation.z = self.q2[3]

        imu_msg.header.stamp = self.get_clock().now().to_msg()
        self.imu_pub.publish(imu_msg)
        print(imu_msg.orientation)


def main(args=None):
    offs_a,offs_g=calibration(5)
    rclpy.init(args=args)
    node = Mynode(offs_a,offs_g)
    try:
        rclpy.spin(node)
    except KeyboardInterrupt:
        node.destroy_node()
        rclpy.shutdown()
    finally:
        rclpy.shutdown()

if __name__ == '__main__':
    main()