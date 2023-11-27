import smbus
import time

bus = smbus.SMBus(1)  # 1 indica el número del bus I2C

MPU6050_ADDR = 0x68  # Cambiar según la dirección I2C de tu MPU6050

# Configurar el MPU6050 para activar la lectura de datos
bus.write_byte_data(MPU6050_ADDR, 0x6B, 0)
time.sleep(2)
while True:
    # Leer datos del acelerómetro y giroscopio
    data = bus.read_i2c_block_data(MPU6050_ADDR, 0x3B, 6)
    accel_x = (data[0] << 8) + data[1]
    accel_y = (data[2] << 8) + data[3]
    accel_z = (data[4] << 8) + data[5]

    print(f"Acelerómetro: X={accel_x}, Y={accel_y}, Z={accel_z}")

    time.sleep(1)  # Espera 1 segundo antes de la próxima lectura
