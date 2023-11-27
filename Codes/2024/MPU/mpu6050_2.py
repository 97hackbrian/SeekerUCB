import smbus
import math
import time

bus = smbus.SMBus(1)  # 1 indica el número del bus I2C
MPU6050_ADDR = 0x68  # Cambiar según la dirección I2C de tu MPU6050

# Configurar el MPU6050 para activar la lectura de datos
bus.write_byte_data(MPU6050_ADDR, 0x6B, 0)

# Factor de compensación de la gravedad
beta = 0.04

# Inicializar variables
gyro_scale = 131.0  # Factor de escala para el giroscopio
dt = 0.01  # Intervalo de muestreo en segundos
angle_pitch = 0.0
angle_roll = 0.0
angle_yaw = 0.0

def read_sensor_data():
    # Leer datos del acelerómetro y giroscopio
    data = bus.read_i2c_block_data(MPU6050_ADDR, 0x3B, 14)
    
    accel_x = (data[0] << 8) + data[1]
    accel_y = (data[2] << 8) + data[3]
    accel_z = (data[4] << 8) + data[5]

    gyro_x = (data[8] << 8) + data[9]
    gyro_y = (data[10] << 8) + data[11]
    gyro_z = (data[12] << 8) + data[13]

    return accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z

def compute_angles(accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z):
    global angle_pitch, angle_roll, angle_yaw  # Asegúrate de que estas variables sean globales
    
    # Convertir valores brutos a unidades físicas
    accel_x /= 16384.0
    accel_y /= 16384.0
    accel_z /= 16384.0

    gyro_x /= gyro_scale
    gyro_y /= gyro_scale
    gyro_z /= gyro_scale

    # Calcular ángulos de pitch y roll
    accel_angle_pitch = math.atan2(accel_y, math.sqrt(accel_x**2 + accel_z**2))
    accel_angle_roll = math.atan2(-accel_x, accel_z)

    # Compensar los ángulos utilizando los datos del giroscopio
    angle_pitch += gyro_x * dt
    angle_roll += gyro_y * dt
    angle_yaw += gyro_z * dt

    # Filtro complementario para combinar datos del acelerómetro y giroscopio
    angle_pitch = beta * (angle_pitch + gyro_x * dt) + (1.0 - beta) * accel_angle_pitch
    angle_roll = beta * (angle_roll + gyro_y * dt) + (1.0 - beta) * accel_angle_roll

    return math.degrees(angle_pitch), math.degrees(angle_roll), math.degrees(angle_yaw)

# Inicializar las variables de ángulo antes del bucle
pitch, roll, yaw = 0.0, 0.0, 0.0

try:
    while True:
        accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z = read_sensor_data()
        pitch, roll, yaw = compute_angles(accel_x, accel_y, accel_z, gyro_x, gyro_y, gyro_z)

        print(f"Pitch: {pitch:.2f} degrees, Roll: {roll:.2f} degrees, Yaw: {yaw:.2f} degrees")

        time.sleep(dt)

except KeyboardInterrupt:
    pass
