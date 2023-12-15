from motorconfig import motor
from CMPS12 import Compass
import time
import threading

try:
    arduino = motor('/dev/ttyACM0', 115200)
except:
    arduino = motor('/dev/ttyACM1', 115200)
    
kecepatan = 1540
depth1 = str('dt '+'20'+';')
time1 = 30
heading1 = 345+10

print(Compass.run())
OK = (input('Oke?'))

main_loop = True
sec_loop = 'loop_turun'

while main_loop:
    if sec_loop == 'loop_turun':
        time.sleep(20)
        print(sec_loop)
        arduino.serial_once(depth1,'target_accepted')
        arduino.co_serial('da 1;')
        #arduino.wait_result('depth_ok')
        sec_loop = 'loop_maju1'

    if sec_loop == 'loop_maju1':
        print(sec_loop)
        time.sleep(0.1)
        t1=threading.Thread(target=arduino.motor_function_timer,args=(heading1,time1,kecepatan,False))
        t1.start()
        sec_loop = 'loop_logging'
        
    if sec_loop == 'loop_logging':
        print(sec_loop)
        time.sleep(0.1)
        t2=threading.Thread(target=arduino.log,args=(time1,heading1))
        t2.start()
        t1.join()
        t2.join()
        sec_loop='loop_stop'
    
    if sec_loop == 'loop_stop':
        #print(sec_loop)
        time.sleep(0.1)
        arduino.co_serial('stop;')
        time.sleep(0.1)
        arduino.co_serial('stop;')
        time.sleep(2)
        time.sleep(0.1)
        arduino.co_serial('da 0;')
        time.sleep(0.1)
        arduino.co_serial('da 0;')
            