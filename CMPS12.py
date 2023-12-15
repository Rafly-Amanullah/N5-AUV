from datetime import datetime
import smbus
import time
import math 
import openpyxl

bus = smbus.SMBus(1)

class Compass:
    def __init__(self, offset=0.0, bus=1, device=0x60):
        self.offset = offset
        self.bus = smbus.SMBus(bus)
        self.device = device

    def azimuth(self):
        high = self.bus.read_byte_data(self.device, 0x2) << 8
        low = self.bus.read_byte_data(self.device, 0x3)
        angle = (high + low) / 10.0
        hasil = int((angle + self.offset) % 360)
        hasil2 = hasil+0
        return hasil2
    def pitch(self):
        p = self.bus.read_byte_data(self.device, 0x04)
        return p
    def roll(self):
        r = self.bus.read_byte_data(self.device, 0x05)
        return r
    def runpitch():
        cmps = Compass()
        ptch = cmps.pitch()
        return ptch
    def runroll():
        cmps = Compass()
        rll = cmps.roll()
        return rll
    def run_():
        cmps = Compass()
        com = cmps.azimuth()
        return com
    def run ():
        while True:
            cmps = Compass()
            com = cmps.azimuth()
            return com
        

if __name__ == "__main__":
    while True:
        comcom = Compass.run()
        print ("   kompas = %f" % ( comcom ))