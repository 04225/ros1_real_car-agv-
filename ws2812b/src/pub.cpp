#include <ros/ros.h>
#include <sensor_msgs/Joy.h>
#include<vector>
#include<string>
#include<ros/ros.h>
#include<ros/master.h>
#include<std_msgs/String.h>
using namespace std;

ros::Publisher pub;
class led_msgs{
public:
    string off="ws2812_off\n";
    string red="ws2812_red\n";
    string blue="ws2812_blue\n";
};
bool find_topic(const string &topic_name){
    vector<ros::master::TopicInfo> topic_vector;
    if (!ros::master::getTopics(topic_vector))
    {
        ROS_ERROR("can not get rostopic list");
        return false;
    }
    for (vector<ros::master::TopicInfo>::iterator i = topic_vector.begin(); i != topic_vector.end(); i++)
    {
        if (i->name == topic_name)
        {
            return true;
        }
    }
    return false;
}
led_msgs obj;
void callback(const sensor_msgs::Joy::ConstPtr &msg){
    std_msgs::String output_msg;
    if (msg->buttons[6]==1)   //手动关闭
    {
        output_msg.data = obj.off;
    }
    else if(find_topic("/odom")){  //odom存在
        output_msg.data = obj.blue;
    }
    else{
        output_msg.data = obj.red;
    }
    pub.publish(output_msg);
}
int main(int argc, char * argv[])
{
    setlocale(LC_ALL,"");
    ros::init (argc,argv,"led_pub");
    ros::NodeHandle nh;
    pub=nh.advertise<std_msgs::String>("led",20);
    ros::Subscriber sub=nh.subscribe<sensor_msgs::Joy>("/joy",50,callback);
    ROS_INFO("loading--------------");
    ros::spin();
    return 0;
}
