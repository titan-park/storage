#!/usr/bin/env python
import wiringpi2 as wiringpi
import time
import rospy
from std_msgs.msg import String

pin = 18
pin_dir = [23,24]
dt = 0
dtMax = 1024
dtMin = 0

def callback(data):
	rospy.loginfo(rospy.get_caller_id() + "I heard %s", data.data)
	
	if data.data == 'a' :
		print "Left"
		wiringpi.digitalWrite(pin_dir[0], 1)
		wiringpi.digitalWrite(pin_dir[1], 0)
	elif data.data == 'w':
		print "Forward"
		wiringpi.digitalWrite(pin_dir[0], 1)
		wiringpi.digitalWrite(pin_dir[1], 1)
	elif data.data == 'd':
		print "Right"
		wiringpi.digitalWrite(pin_dir[0], 0)
		wiringpi.digitalWrite(pin_dir[1], 1)
	elif data.data == 's':
		print "Stop"
		wiringpi.digitalWrite(pin_dir[0], 0)
		wiringpi.digitalWrite(pin_dir[1], 0)

	global dt

	dt = dt + 100
	if dt > dtMax:
		dt = dtMin

	print(dt)
	wiringpi.pwmWrite(pin, dt)

def listener():
	rospy.init_node('listener', anonymous=True)

	rospy.Subscriber("chatter", String, callback)

	wiringpi.wiringPiSetupGpio()
	wiringpi.pinMode(pin,2)
	wiringpi.pwmSetMode(0)
	wiringpi.pwmSetClock(400)
	wiringpi.pwmSetRange(1024)
	wiringpi.pwmWrite(pin,0)

	wiringpi.pinMode(pin_dir[0],1)
	wiringpi.pinMode(pin_dir[1],1)

    # spin() simply keeps python from exiting until this node is stopped
	rospy.spin()

if __name__ == '__main__':
	listener()
