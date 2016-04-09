import  RPi.GPIO as GPIO
import time
import Adafruit_CharLCD as LCD
import subprocess
import os
import signal

lcd = LCD.Adafruit_CharLCDPlate()

pathToProcess = "../robot/"

# Make list of button value, text, and backlight color.
buttons = ( (LCD.SELECT, 'Select', (1,1,1)),
            (LCD.LEFT,   'Left'  , (1,0,0)),
            (LCD.UP,     'Up'    , (0,0,1)),
            (LCD.DOWN,   'Down'  , (0,1,0)),
            (LCD.RIGHT,  'Right' , (1,0,1)) )

class Ultra: # For Ultrasonic, values from Matt's Arduino Code
	l = 0
	f = 1
	r = 2
	b = 3

class Encoder: # For Encoders
	l = 0
	r = 1

def dLCD(but):
	if (not hasattr(dLCD, "screen")): dLCD.screen = 0
    if (not hasattr(dLCD, "p")): dLCD.p = None
	print(dLCD.screen)
	print(but)
	prevScreen = 0
	if (dLCD.screen == 0): # 0 Select Run Robot
		lcd.message('   Run Robot    \n<-            ->')
		if (but == LCD.LEFT): dLCD.screen = 2
		elif (but == LCD.RIGHT): dLCD.screen = 1
		elif (but == LCD.DOWN): dLCD.screen = 3
	elif (dLCD.screen == 1): # 1 Select Sensor Data
		lcd.message('  Sensor Data   \n<-            ->')
		if (but == LCD.LEFT): dLCD.screen = 0
		elif (but == LCD.RIGHT): dLCD.screen = 2
		elif (but == LCD.DOWN): dLCD.screen = 5
	elif (dLCD.screen == 2): # Power Off
        lcd.message('   Shut Down')
        if (but == LCD.LEFT): dLCD.screen = 1
        elif (bu == LCD.RIGHT): dLCD.screen = 0
        elif (but == LCD.DOWN): os.system("poweroff")
	elif (dLCD.screen == 3): # 3 Traverse Maze
		lcd.message(' Traverse Maze  \n<-            ->')
		if (but == LCD.UP): dLCD.screen = 0
		elif (but == LCD.DOWN):
			dLCD.p = subprocess.Popen(pathToProcess + "./bruh",shell=True);
			dLCD.screen = 7;
		elif (but == LCD.LEFT): dLCD.screen = 4
		elif (but == LCD.RIGHT): dLCD.screen = 4
	elif (dLCD.screen == 4): # 4 Solve Maze
		lcd.message('   Solve Maze   \n<-            ->')
		if (but == LCD.UP): dLCD.screen = 0
		elif (but == LCD.DOWN): pass # Whatever Method to Solve Maze
		elif (but == LCD.LEFT): dLCD.screen = 3
		elif (but == LCD.RIGHT): dLCD.screen = 3
	elif (dLCD.screen == 5): # 5 Ultrasonic
		if (not prevScreen == 5): lcd.message(getUltra(Ultra.l) + ' ' + getUltra(Ultra.f) + ' ' + getUltra(Ultra.r) + ' ' + getUltra(Ultra.b) + ' \n L   F   R   B  ')
		else: lcd.message(getUltra(Ultra.l) + ' ' + getUltra(Ultra.f) + ' ' + getUltra(Ultra.r) + ' ' + getUltra(Ultra.b))
		if (but == LCD.UP): dLCD.screen = 1
		elif (but == LCD.LEFT): dLCD.screen = 6
		elif (but == LCD.RIGHT): dLCD.screen = 6
	elif (dLCD.screen == 6): # 6 Encoders
		if (not prevScreen == 6): lcd.message(' ' + getEncoder(Encoder.l) + '    ' + getEncoder(Encoder.r) + '\n L  Encoders  R ')
		else: lcd.message(' ' + getEncoder(Encoder.l) + '    ' + getEncoder(Encoder.r))
		if (but == LCD.UP): dLCD.screen = 1
		elif (but == LCD.LEFT): dLCD.screen = 5
		elif (but == LCD.RIGHT): dLCD.screen = 5
	elif (dLCD.screen == 7): # Kill Screen
		if (not prevScreen == 7): prev = prevScreen
		lcd.message('Press any Button\n     To Stop    ');
		if (but != -1):
			# dLCD.p.kill()
            os.killpg(os.getpgid(dLCD.p.pid), signal.SIGINT)
			dLCD.screen = prev
	if (not but == -1): lcd.clear()
	prevScreen = dLCD.screen

def getUltra(n): # Get Ultrasonic Readings
	value = str(n)
	return '0'*(3-len(value)) + value

def getEncoder(n):
	value = str(n)
	return '0'*(5-len(value)) + value

buttonArray = [True, True, True, True, True]
buttonArrayPrev = [True, True, True, True, True]
change = -1
while True:
	for i in range(0, len(buttonArray)):
		#print(buttons[i])
		buttonArray[i] = lcd.is_pressed(buttons[i][0])
		if (buttonArray[i] and not buttonArrayPrev[i]):
			change = buttons[i][0]
			break
	print(buttonArray)
	print(buttonArrayPrev)
	dLCD(change)
	change = -1
	buttonArrayPrev = buttonArray[:]
	time.sleep(.1)
