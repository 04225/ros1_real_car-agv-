#include <ros/ros.h>
#include <std_msgs/String.h>
#include<string>
#include<fcntl.h>
#include<termios.h>
#include<unistd.h>
#include<cstring>
using namespace std;

string last_led_msgs="";
int serial_fd;
bool open_serial(const string &port){
    serial_fd=open(port.c_str(),O_WRONLY | O_NOCTTY);   //读取和输入
    if (serial_fd == -1)
    {
        return false;
    }
    struct termios tty;
    {
        memset(&tty ,0, sizeof(tty));
    };
    if (tcgetattr(serial_fd,&tty)!=0)
    {
        return false;
    }
    
    cfsetospeed(&tty,B9600);   //输出波特率
    cfsetispeed(&tty,B9600);   //输入波特率
    tty.c_cflag &=~(PARENB | CSTOPB | CSIZE);
    tty.c_cflag |=CS8 | CLOCAL | CREAD;
    tty.c_oflag &=~OPOST;
    tty.c_cc[VTIME]=15;  //1.5S
    tty.c_cflag &=~CRTSCTS;
    tty.c_cc[VMIN] = 0;
    return tcsetattr(serial_fd,TCSANOW,&tty)==0;
}
void send_msgs(const std_msgs::String::ConstPtr &msg){
    string current_led_msgs=msg->data;   //topic->data
    if (serial_fd !=-1)
    {
        if (current_led_msgs != last_led_msgs)
        {
            write(serial_fd,msg->data.c_str(),msg->data.size());
            last_led_msgs=current_led_msgs;
        }
    }
}
int main(int argc, char * argv[])
{
    setlocale(LC_ALL,"");
    ros::init(argc, argv, "led_sub");
    ros::NodeHandle nh;
    sleep(2);
    if (!open_serial("/dev/ttyUSB0"))
    {
        ROS_ERROR("open serial error");
        return -1;
    }
    ros::Subscriber sub=nh.subscribe<std_msgs::String>("led",20,send_msgs);
    ros::spin();
    close(serial_fd);
    return 0;
}
