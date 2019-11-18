import serial
import time

# This is a rough sketch realizing the serial communication with the microcontroller.
# Each button on the snes controller is mapped to a string that represents two bytes.
# For details about the encoding please refer to the communication protocol between snes and snes controller.
# Because of time constrains no function for combining button presses is provided.
# Even thoug the snes supports this, instead some buttons are mapped to two button pressen (e.g. 'b').
# This is because we had to jump in the right direction for this demo a lot.
def mapKeyToBitString(key):
    return {
        'b'     :  '10000001' + '00000000',
        'y'     :  '01000000' + '00000000',
        'sl'    :  '00100000' + '00000000',
        'st'    :  '00010000' + '00000000',
        'up'    :  '00001000' + '00000000',
        'down'  :  '00000100' + '00000000',
        'left'  :  '00000010' + '00000000',
        'right' :  '00000001' + '00000000',
        'a'     :  '00000000' + '10000000',
        'x'     :  '00000000' + '01000000',
        'l'     :  '00000000' + '00100000',
        'r'     :  '00000000' + '00010000',
        'stop'  :  '00000000' + '00000000',
        'up_without_jump' : '10000000' + '00000000',
        'bl'    :  '10000010' + '00000000',
    }.get(key, '00000000' + '00000000')

# The main function starts a simple procedure for playing the prolog-level in 'Yoyhis Islan'.
# The program sends button configurations over serial.
# The microcontroller is programed in a way that the last button configuration will be 
# hold until overridden.
# The time list defines the time periods a specific button configuration should
# remain 'pressed'.
def main(args):
    portName = '/dev/ttyUSB0'
    ser = serial.Serial(portName, 115200)
    
    # This is the sequence of controller inputs that should be simulated.
    commands = ['right', 'b', 'right', 'b', 'left', 'b', 'right', 'b', 'stop', 'b', 'b' 'stop', 'right', 'stop']
    # This are the time periods a certain button configuration should be held.
    times =    [1, 2, 1, 2, 0.8, 2, 0.7, 2, 1, 2, 0.5, 5, 0.2, 1, 2]
    
    for i in range(len(commands)):
        arg = mapKeyToBitString(commands[i])
        print('Key: {} | Byte: {} | Time: {}'.format(commands[i], arg, times[i]))
        ser.write(arg.encode('utf-8'))
        time.sleep(times[i])
    
    return 0

if __name__ == '__main__':
    import sys
    sys.exit(main(sys.argv))
