from CMPS12 import Compass
import serial
import time
import numpy as np
from configparser import SectionProxy
from os import waitid_result
import openpyxl
from datetime import datetime

class motor():
    def __init__(self, port, baudrate):
        self.arduino = serial.Serial(port, baudrate)
        self.arduino.close()
        self.arduino.open()
        self.compas = 0
        self.t = 0
    
    def motor_config(self,eror,kecepatan, hold):
        cap_atas,cap_bawah = 1570,1510
        if hold == False:
            n = 0.5 * abs(eror)
            r = 0.2 * abs(eror)
            zn = 5
            xr = 8
            
        else:
            n = 0.5 * abs(eror)
            r = 0.2 * abs(eror)
            zn = 0
            xr = 0
        maju1kn, maju2kn, maju1kr, maju2kr = map(lambda x: max(min(x, cap_atas), cap_bawah), 
        [(n + kecepatan + zn),(-n + kecepatan + zn),(r + kecepatan-xr),(-r + kecepatan-xr)])

        self.send_maju = f"go {kecepatan} {kecepatan} 1499 1499 ;"
        self.send_maju1 = f"go {kecepatan} {kecepatan} 1498 1498 ;"
        self.send_kanan1 = f"go {maju1kr} {maju2kn} 1498 1498 ;"
        self.send_kiri1 = f"go {maju2kr} {maju1kn} 1498 1498 ;"
        self.send_kanan2 = f"go {maju1kr} {maju2kn} 1499 1499 ;"
        self.send_kiri2 = f"go {maju2kr} {maju1kn} 1499 1499 ;"
        self.send_stop = "stop;"

    def serial_depth(self):
        while True:
            self.arduino.flush()
            for data in iter(self.arduino.readline, b''):
                data = data.strip().decode()
                if data.startswith('$'):
                    return data
                    break
        
    def log(self,waktu,target):
        filename = datetime.now().strftime("logging_%Y-%m-%d_%H-%M-%S.xlsx")
        times = waktu
        x=1
        workbook = openpyxl.Workbook()
        worksheet = workbook.active
        worksheet['A1'] = 'Pitch'
        worksheet['B1'] = 'Roll'
        worksheet['C1'] = 'Compass'
        worksheet['D1'] = 'Depth'
        worksheet['E1'] = 'Time'
        worksheet['F1'] = 'Target'
        worksheet['F2'] = target
        A=time.time()
        while True:
            time.sleep(0.1)
            p = Compass.runpitch()
            r = Compass.runroll()
            c = Compass.run()
            d = self.serial_depth()
            x+=1
            allocateA=f"A{x}"
            allocateB=f"B{x}"
            allocateC=f"C{x}"
            allocateD=f"D{x}"
            allocateE=f"E{x}"
            worksheet[allocateA] = p
            worksheet[allocateB] = r
            worksheet[allocateC] = c
            worksheet[allocateD] = d
            B=time.time()-A
            print(B)
            worksheet[allocateE] = B
            if B>=times:
                print("Looping Done!")
                break
        workbook.save(filename)
        print('Writing done!')
             
    def serial_once(self, perintah, hasil):
        self.arduino.flush()
        self.arduino.write(bytes(perintah, 'utf-8'))
        for data in iter(self.arduino.readline, b''):
            data = data.strip().decode()
            if data == hasil:
                self.arduino.flush()
                break

    def co_serial(self, perintah):
        self.arduino.flushInput()
        self.arduino.write(bytes(perintah,'utf-8'))
        self.arduino.flush()

    def hitung_error(self, selisih):
        self.error_kompas = selisih - 360 if selisih > 180 else selisih + 360 if selisih < -180 else selisih
        return self.error_kompas

    def heading_result(self, sel, toleransi):
        #1=maju 2=kiri 3=kanan
        selisih_kompas = self.hitung_error(sel)
        if abs(selisih_kompas) > toleransi:
            condition = 3 if selisih_kompas > 0 else 2
        else:
            condition = 1
        return condition

    def motor_function(self, hold_position, sel, toleransi):
        self.condition_now = self.heading_result(sel, toleransi)
        
        if hold_position:
            if self.condition_now in [2, 3]:
                print('Kiri' if self.condition_now == 2 else 'Kanan', '(True/{})'.format(self.condition_now))
                time.sleep(0.1)
                self.co_serial(self.send_kiri1 if self.condition_now == 2 else self.send_kanan1)
                print(self.send_kiri1 if self.condition_now == 2 else self.send_kanan1)
            elif self.condition_now == 1:
                print('Maju (True/1)')
                time.sleep(0.1)
                self.co_serial(self.send_maju1)
                
        else:
            if self.condition_now in [2, 3]:
                print('Kiri' if self.condition_now == 2 else 'Kanan', '(False/{})'.format(self.condition_now))
                time.sleep(0.1)
                self.co_serial(self.send_kiri2 if self.condition_now == 2 else self.send_kanan2)
                print(self.send_kiri2 if self.condition_now == 2 else self.send_kanan2)
            elif self.condition_now == 1:
                print('Maju (False/1)')
                time.sleep(0.1)
                self.co_serial(self.send_maju)  
        
    def motor_function_timer(self, target, waktu, kecepatan, hold):
        start_time = time.time()
        while True:
            time.sleep(0.1)
            print(Compass.run_())
            selisih = time.time() - start_time
            sel = target - Compass.run_()
            eror = self.hitung_error(sel)
            self.motor_config(eror, kecepatan, hold)
            self.motor_function(hold, sel, 1)
            if selisih >= waktu:
                self.co_serial(self.send_stop)
                print('done!')
                break
                        
    def camera(self, out, cam, waktu):
        start_time = time.time()
        while cam.isOpened() and (time.time() - start_time) < waktu:
            ret, frame = cam.read()
            if ret:
                out.write(frame)
        print('done!')