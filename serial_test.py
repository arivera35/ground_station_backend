import serial, time

my_port = serial.Serial('/dev/ttyUSB0', baudrate=9600, parity=serial.PARITY_NONE, stopbits=serial.STOPBITS_ONE, bytesize=serial.EIGHTBITS, xonxoff=True)

message = "R1n;"

my_port.write(message.encode())

print("Message sent")

time.sleep(0.5)

response = my_port.readline().decode().strip()
print("Response received:", response)

my_port.close()