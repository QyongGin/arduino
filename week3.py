import serial
from influxdb_client import InfluxDBClient
import time

serial_port = 'COM14'
baud_rate = 9600
timeout = 2

# InfluxDB v2 설정
influxdb_url = "http://localhost:8086"
influxdb_token = "vi16lBrtZnz3fZVXZTyNoOdfFaRYrwFnPcM3Qo0r1V1lj0KcqUrZEP5Kq9aRGXuNx4trUpYYj5HPZqkaEiMRcA=="
influxdb_org = "test"
influxdb_bucket = "dust"


client = InfluxDBClient(url=influxdb_url, token=influxdb_token, org=influxdb_org)
write_api = client.write_api()

try:
    ser = serial.Serial(serial_port, baud_rate, timeout = timeout)
    print(f"Connected to {serial_port} at {baud_rate} baud")
except:
    print("Failed to connect to serial port")
    exit()
try:
    while True:
        if ser.in_waiting > 0:
            line = ser.readline().decode('utf-8').strip()

            if "=" in line:
                key, value = line.split("=")
                try:
                    value = float(value)
                    data=f"sensor_data,device=arduino {key}={value}"
                    write_api.write(bucket=influxdb_bucket, record=data)
                    print(f"Data written to influxDB: {key}={value}")
                except ValueError:
                    print("Invalid data formay")
            time.sleep(1)

except KeyboardInterrupt:
    print("프로그램이 종료되었습니다.")

finally:
    ser.close()
    
