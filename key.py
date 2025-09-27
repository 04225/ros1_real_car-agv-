#!/usr/bin/env python3
# -*- coding: utf-8 -*-
import rospy
from sensor_msgs.msg import Joy
from geometry_msgs.msg import Twist
# import pygame
import time
import serial


# axes: [0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0]
# buttons: [0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0]


pub = None
twist_msg = Twist()
button0_pressed = False
button1_pressed = False
button2_pressed = False
now_v=1

# ser =serial.Serial(          //ws2812
#     port='/dev/ttyUSB0',
#     baudrate=9600,
#     timeout=1,
#     stopbits=1,
#     bytesize=8
# )
# time.sleep(2)

# def play_music(file_path):  //play music
#     pygame.mixer.init()
#     try:
#         pygame.mixer.music.load(file_path)
#         print("loading......")
#         pygame.mixer.music.play(0)

#     except pygame.error as e:
#         print("error......")



def joy_callback(msg):
    global button0_pressed, button1_pressed, button2_pressed, twist_msg
    
    # 更新按钮状态
    button0_pressed = (msg.buttons[0] == 1)  # 解锁
    button1_pressed = (msg.buttons[1] == 1)  # 加速
    button2_pressed = (msg.buttons[2] == 1)  # 减速
    
    if button0_pressed:
        # 处理方向控制
        if msg.axes[7] == 1:
            rospy.loginfo_once("向上")
            twist_msg.linear.x = 0.5
            # ser.write("ws2812_red\n")

        elif msg.axes[7] == -1:
            rospy.loginfo_once("向下")          
            twist_msg.linear.x = -0.5
            # ser.write("ws2812_green\n")

        else:
            twist_msg.linear.x = 0.0

        if msg.axes[6] == 1:
            rospy.loginfo_once("向左")
            twist_msg.angular.z = 6
            # ser.write("ws2812_off\n")

        elif msg.axes[6] == -1:
            rospy.loginfo_once("向右")
            twist_msg.angular.z = -6
            # ser.write("ws2812_blue\n")

        else:
            twist_msg.angular.z = 0.0
            
    elif button1_pressed and msg.axes[7]==1:
        rospy.loginfo_once("向前加速")
        twist_msg.linear.x = 1.0
        twist_msg.angular.z = 0.0
    
    elif button1_pressed and msg.axes[7]==-1:
        rospy.loginfo_once("向后加速")
        twist_msg.linear.x = -1.0
        twist_msg.angular.z = 0.0
        
    elif button2_pressed and msg.axes[7]==1:
        rospy.loginfo_once("向前减速")
        twist_msg.linear.x = 0.25
        twist_msg.angular.z = 0.0

    elif button2_pressed and msg.axes[7]==-1:
        rospy.loginfo_once("向后减速")
        twist_msg.linear.x = -0.25
        twist_msg.angular.z = 0.0

    # elif msg.buttons[5] == 1:
    #      ser.write("ws2812_off\n")


    else:
        twist_msg.linear.x = 0.0
        twist_msg.angular.z = 0.0



def timer_callback(event):
    global pub, twist_msg
    if pub is not None:
        pub.publish(twist_msg)

def main(): 
    global pub
    rospy.init_node("joy_control_node")
    pub = rospy.Publisher('/cmd_vel', Twist, queue_size=10)
    rospy.Subscriber("/joy", Joy, joy_callback)
    rospy.Timer(rospy.Duration(0.1), timer_callback)
    rospy.loginfo("游戏手柄控制节点已启动，支持持续控制")
    # play_music("/home/yongzhi/car_ws/src/MP3/short.mp3")
    rospy.spin()

if __name__ == '__main__':
    try:
        main()
    except rospy.ROSInterruptException:
        rospy.loginfo("节点已停止")

