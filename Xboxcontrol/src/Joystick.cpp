#include <ros/ros.h>
#include<string>
#include<sensor_msgs/Joy.h>
#include<std_msgs/String.h>
#include<geometry_msgs/Twist.h>
using namespace std;
bool button0_pressed = false;
bool button1_pressed = false;
bool button2_pressed = false;
bool last_button1_pressed = false;
bool last_button2_pressed = false;
double last_x_v=0.5;
geometry_msgs::Twist twist_msg;
ros::Publisher pub;
void callback(const sensor_msgs::Joy::ConstPtr &msg){
    button0_pressed = (msg->buttons[0] ==1);    //解锁
    button1_pressed = (msg->buttons[1] ==1);    //加速
    button2_pressed = (msg->buttons[2] ==1);    //减速
    twist_msg.linear.x =0;   //初始话
    twist_msg.angular.z =0 ;
    if (button0_pressed)         //解锁
    {
        if (msg->axes[7] ==1 )
        {
            twist_msg.linear.x = last_x_v;
            ROS_INFO("向上");
        }
        else if (msg->axes[7] == -1)
        {
            twist_msg.linear.x = -last_x_v;
            ROS_INFO("向下");
        }
        if (msg->axes[6] ==1)
        {
            twist_msg.angular.z = 4;   //旋转速度固定
            ROS_INFO("向左旋转");
        }
        else if (msg->axes[6] == -1)
        {
            twist_msg.angular.z = -4;   //旋转速度固定
            ROS_INFO("向右旋转");
        }  
    }
    else if (button1_pressed && !last_button1_pressed)   //加速
    {
        if (last_x_v >= 6)
        {
            last_x_v=6.0;
        }
        else{
        last_x_v +=0.25;   //0.5的步进
        ROS_INFO("速度增加，现在速度为：%.2f",last_x_v);
        }
    }
// ----------------------------------------
    else if (button2_pressed && !last_button2_pressed)   //减速
    {
        if (last_x_v <= 0.25)
        {
            last_x_v=0.25;
        }
        else
        {
            last_x_v -=0.25;  //0.25的步减
            ROS_INFO("速度减少，现在速度为：%.2f",last_x_v);
        }
    } 
    last_button1_pressed = button1_pressed;   //更新
    last_button2_pressed = button2_pressed;
    pub.publish (twist_msg);
}
int main(int argc, char * argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "Xboxcontrol");
    ros::NodeHandle nh;
    pub = nh.advertise<geometry_msgs::Twist>("/cmd_vel",10);
    ros::Subscriber sub=nh.subscribe<sensor_msgs::Joy>("/joy",50,callback);
    ROS_INFO("LOADING.........");
    ros::spin();
    return 0;
}
