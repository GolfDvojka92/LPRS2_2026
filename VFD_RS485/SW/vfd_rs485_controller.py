from typing import cast

from pymodbus.client import ModbusSerialClient
import time

class VFD:
    # READ ONLY
    OPERATING_FREQ_REG              = 0x1001
    OUTPUT_VOLTAGE_REG              = 0x1003
    OUTPUT_CURRENT_REG              = 0x1004
    OUTPUT_POWER_REG                = 0x1005
    OUTPUT_TORQUE_PERCENTAGE_REG    = 0x1006

    # WRITE
    SET_FREQUENCY_REG               = 0x1000
    CONTROL_COMMAND_REG             = 0x2000

    VALUE_FOR                       = 0x0001
    VALUE_REV                       = 0x0002
    VALUE_STOP                      = 0x0006

    def __init__(self):
        self.instance = ModbusSerialClient(
            port = '/dev/ttyUSB0',
            baudrate = 9600,
            bytesize = 8,
            parity = 'N',
            stopbits = 1,
            timeout = 1
        )

    def connect(self):
        if self.instance.connect() == False:
            print("Could not connect")
        else:
            print("Connected successfully")

    def run(self):
        self.instance.write_register(address = self.CONTROL_COMMAND_REG, value = self.VALUE_FOR)

    def stop(self):
        self.instance.write_register(address = self.CONTROL_COMMAND_REG, value = self.VALUE_STOP)

    def setFrequency(self, v):
        self.instance.write_register(address = self.SET_FREQUENCY_REG, value = v / 100)

    def getReadings(self):
        value = self.instance.read_holding_registers(address = self.OPERATING_FREQ_REG)
        print(f"Operating frequency: {value.registers[0]}Hz")
        value = self.instance.read_holding_registers(address = self.OUTPUT_VOLTAGE_REG)
        print(f"Output voltage: {value.registers[0]}V")
        value = self.instance.read_holding_registers(address = self.OUTPUT_CURRENT_REG)
        print(f"Output current: {value.registers[0]}A")
        value = self.instance.read_holding_registers(address = self.OUTPUT_POWER_REG)
        print(f"Output power: {value.registers[0]}W")
        value = self.instance.read_holding_registers(address = self.OUTPUT_TORQUE_PERCENTAGE_REG)
        print(f"Output torque percentage: {value.registers[0]}%")

if __name__== "__main__":
    vfd = VFD()
    vfd.connect()
    vfd.run()
    vfd.setFrequency(0)
    vfd.getReadings()
    while True:
        try:
            v = int(input("Target frequency (-1 -> stop):"))
            if v == -1:
                vfd.stop()
                break
            else:
                vfd.setFrequency(v)
                value = vfd.instance.read_holding_registers(address = vfd.OPERATING_FREQ_REG)
                target = value.registers[0]
                if target < v:
                    while value.registers[0] < v:
                        print(f"Operating frequency: {value.registers[0]}Hz")
                        time.sleep(1)
                        value = vfd.instance.read_holding_registers(address = vfd.OPERATING_FREQ_REG)
                else:
                    while value.registers[0] > v:
                        print(f"Operating frequency: {value.registers[0]}Hz")
                        time.sleep(1)
                        value = vfd.instance.read_holding_registers(address = vfd.OPERATING_FREQ_REG)
                vfd.getReadings()
        except ValueError:
            print("Invalid input")
