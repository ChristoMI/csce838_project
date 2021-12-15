#!/usr/bin/env python
import rospy
from std_msgs.msg import String
from subprocess import call
import os

from real_car_lane_follow.msg import CarObjects

OPEN_WDT = 0
GET_WDT_TIMEOUT = 1
SET_WDT_TIMEOUT = 2
KICK_WDT_TIMER = 3

def callback(data):
    encoded_msg = 0

    rospy.loginfo("Objects number - " + str(len(data.objects)))

    for object in data.objects:
        encoded_msg += object.maxY
        encoded_msg *= 1000

        encoded_msg += object.minY
        encoded_msg *= 1000

        encoded_msg += object.maxX
        encoded_msg *= 1000

        encoded_msg += object.minX
        encoded_msg *= 10

        encoded_msg += object.classID

        rospy.loginfo("Message: " + str(encoded_msg))

        call("python3 /home/car/ryan_brown_ws/src/bluetooth_com/scripts/talker_p3.py " + str(encoded_msg), shell = True)
        #rospy.sleep(10000)

    wdt_timer_kick_result = call("/home/car/ryan_brown_ws/src/bluetooth_com/src/wdt " + str(KICK_WDT_TIMER), shell = True)

    rospy.sleep(0.5)
    


def listener():
    rospy.loginfo("NODE INITIALIZATION")
    rospy.init_node('communication_module', anonymous=True)

    call("/home/car/ryan_brown_ws/src/bluetooth_com/src/ErrorLog 0", shell = True)

    rospy.loginfo("SETTING UP WDT")

    wdt_timer_setup_result = call("/home/car/ryan_brown_ws/src/bluetooth_com/src/wdt " + str(SET_WDT_TIMEOUT) + " 1000", shell = True)

    if (wdt_timer_setup_result == -1):
        call("g++ /home/car/ryan_brown_ws/src/bluetooth_com/src/ErrorLog 1 WDT\ failed\ to\ respond", shell = True)
    elif (wdt_timer_setup_result == -2):
        call("g++ /home/car/ryan_brown_ws/src/bluetooth_com/src/ErrorLog 2 Unauthorized\ WDT\ operation", shell = True)

    rospy.loginfo("SUBSCRIBED")

    rospy.Subscriber('/object_detection/objects', CarObjects, callback)

    # spin() simply keeps python from exiting until this node is stopped
    rospy.spin()

if __name__ == '__main__':
    listener()
