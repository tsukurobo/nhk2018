#include "ros/ros.h"
#include "std_msgs/Char.h"
#include "std_msgs/Int8.h"
using namespace std;

std_msgs::Char key_msg;

int main(int argc, char **argv){
  ros::init(argc,argv,"tr_key");
  ros::NodeHandle nh;
  ros::Publisher key_pub = nh.advertise<std_msgs::Char>("tr_key",10);
  ros::Rate rate(10);

  std::cout << "input key a(step.1) to k(step.11)" << std::endl;
  //一応、10進数で　'a'は97, 'b'は98...と送られる

  while (ros::ok()){
    char key;
    
    cin >> key;
    cout << key << "was sent"  << endl;

    key_msg.data = key;

    if(key_msg.data != '\0'){
      key_pub.publish(key_msg);
    }

    ros::spinOnce();

    key_msg.data = '\0';
    rate.sleep();
  }
  return 0;
}
