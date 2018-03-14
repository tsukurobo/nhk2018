#include <stdio.h>
#include <stdlib.h>
#include <vector>
#include <iostream>

#include "ros/ros.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
//int型の配列Arrを使うよー
int Arr[90];
//コールバック関数（arrayって言う名前のstd_msgs（スタンダードメッセージズ）のInt32Multiarray型の情報（トピック）が変化した時に呼ばれる関数）の宣言
void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& array);

int main(int argc, char **argv)
{
//init なんかいつもいるやつ。多分初期化か定義かなんか。
    ros::init(argc, argv, "arraySubscriber");
//これもいつもいる。ノードハンドル。ちょくちょくこのノードハンドルを使う。
    ros::NodeHandle n;  
//サブスクライバーの宣言っぽい　ROS::Subscriber サブスクライバーだよー
//　　　　　　　　　　　　　　　　　　　　　　　　　　sub3 んで、そのサブスクライバーをsub3って呼ぶよー
//　　　　　　　　　　　　　　　　　　　　　　　　　　n.advertise   なんかいる
//			　　　　"array"　　arrayっていう場所（トピック）の情報を読むよー
//                          arrayCallback　情報が変化したらarrayCallbackっていう関数よ割り込んで読むよー
    ros::Subscriber sub3 = n.subscribe("array", 100, arrayCallback);
//なんかよくいる　ros::spin  多分”繰り返す”的な何か
	ros::spin();
    return 0;
}
//arrayCallbackの中身　arrayという場所（情報）の中のstd_msga（スタンダードメッセージズ）のInt32MultiArray型の情報を読むよー）
void arrayCallback(const std_msgs::Int32MultiArray::ConstPtr& array)
{

    int i = 0;
    // print all the remaining number
    //データの始まりから、終わりまで
    for(std::vector<int>::const_iterator it = array->data.begin(); it != array->data.end(); ++it)
    {   //Arr[i]に*it(array.data[i]の中身)を入れる
        Arr[i] = *it;
        i++;
    }
//Arr[]の中身を表示
	for(int j = 0; j < 4; j++)
    {
        printf("%d, ", Arr[j]);
    }

    printf("\n");

    return;
}
