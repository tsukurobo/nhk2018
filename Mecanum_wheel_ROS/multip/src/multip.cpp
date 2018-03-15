//programmer syatch
#include <stdio.h>
#include <stdlib.h>

#include "ros/ros.h"

#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"

#include "std_msgs/Int32MultiArray.h"

int main(int argc, char **argv)
{

//init なんかいつもいるやつ。多分初期化か定義かなんか。
    ros::init(argc, argv, "arrayPublisher");
//これもいつもいる。ノードハンドル。ちょくちょくこのノードハンドルを使う。
    ros::NodeHandle n;
//パブリッシャーの宣言っぽい　ROS::Publisher パブリッシャーだよー
//                        pub んで、そのパブリッシャーの名前はpubねー
//                        n.advertise　　　なんかいる
//                        <std_msgs::Int32MultiArray> パブリッシュするのはstd_msgs（スタンダードメッセージズ)の、Int32MultiArray型のデータよー
//　　　　　　　　　　　　　　　　　　　　　　　　"array" そのデータを"array"っていう場所（トピック）に送るよー
//                        100 (ここはググった)任意の時間に保持しておく情報量は100までねー情報の出し入れが早すぎると溢れちゃうよー
    ros::Publisher pub = n.advertise<std_msgs::Int32MultiArray>("array", 100);

    while (ros::ok())//ROSがちゃんと動いている間？
    {
//std_msgs(スタンダードメッセージ)のうち、Int32MultiArray型のデータ（変数あるいは枠）のarrayを使うよっていう宣言。ROSにおける情報のやり取りをするときの特殊な変数的な何か。型は色々ある
        std_msgs::Int32MultiArray array;
//	Clear arrayのデータの初期化
        array.data.clear();
//	array.data.push_backでarrayデータの最後尾にデータを付け加える（配列の一番最後に新しく一つ付け加えてそこにデータを入れる）。（）の中身が付け加えるデータ。
//	array.data[i](iは任意)= p（pは任意）;としてもうまく行かない。とりあえず今回は9,3,4,7という４つのデータを含んだデータをMultiArrayInt32型のarrayのデータに入れる。。
        array.data.push_back(9);
        array.data.push_back(3);
        array.data.push_back(4);
        array.data.push_back(7);
//	Publish array 　　arrayをパブする。
        pub.publish(array);
//	Let the world know   ターミナル上にプリントする。ここでは「I published something!」が表示される。
        ROS_INFO("I published something!");
//	Do this.
        ros::spinOnce();
//	Added a delay so not to spam　左記の通り
        sleep(2);
    }

}

