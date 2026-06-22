from pymodbus.client import ModbusSerialClient
import time

class VFD:
    # READ ONLY
    OPERATING_FREQ_REG              = 0x1001
    OUTPUT_VOLTAGE_REG              = 0x1003
    OUTPUT_CURRENT_REG              = 0x1004
    OUTPUT_POWER_REG                = 0x1005
    OUTPUT_TORQUE_PERCENTAGE_REG    = 0x1005

    # WRITE
    # TODO

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
            print("Nije moguce povezati se na uredjaj")
        else:
            print("Uspesno ste se povezali na uredjaj")

    def getReadings(self):
        value = self.instance.read_input_registers(address = self.OPERATING_FREQ_REG)
        print(f"Operating frequency: {value.registers[0]}")
        value = self.instance.read_input_registers(address = self.OUTPUT_VOLTAGE_REG)
        print(f"Output voltage: ")
        value = self.instance.read_input_registers(address = self.OUTPUT_CURRENT_REG)
        print(f"Output current: ")
        value = self.instance.read_input_registers(address = self.OUTPUT_POWER_REG)
        print(f"Output power: ")
        value = self.instance.read_input_registers(address = self.OUTPUT_TORQUE_PERCENTAGE_REG)
        print(f"Output torque percentage: ")

if __name__== "__main__":
    vfd = VFD()
    vfd.connect()
    vfd.getReadings()
    time.sleep(1000)
