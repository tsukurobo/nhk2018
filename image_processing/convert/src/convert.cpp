#include <ros/ros.h>
#include <stdio.h>
#include "opencv/cv.h"
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "opencv/highgui.h"
#include "std_msgs/Int32.h"
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int32MultiArray.h"
#include <stdlib.h>
static const std::string OPENCV_WINDOW = "Image window";
static int count[2];
float  x[2];
float y[2];
class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  ros::Publisher pub = nh_.advertise<std_msgs::Int32MultiArray>("image_param", 100);
  

public:
  // コンストラクタ
  //（動作時のパブサブ設定？）
ImageConverter()
    : it_(nh_)
  {
    // カラー画像をサブスクライブ                                                                
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1,
      &ImageConverter::imageCb, this);
 }
 

  // デストラクタ
  //（終了時の処理？）
  ~ImageConverter()
  {
    cv::destroyWindow(OPENCV_WINDOW);
  }


  // コールバック関数
  void imageCb(const sensor_msgs::ImageConstPtr& msg)
  {


    cv_bridge::CvImagePtr cv_ptr, cv_ptr2;
    //cv_ptrはオリジナル
 try
    {
      // ROSからOpenCVの形式にtoCvCopy()で変換。cv_ptr->imageがcv::Matフォーマット。
      cv_ptr    = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }

    //（使うものの宣言？）
    cv::Mat hsv_image, color_mask1, color_mask2;

    // cv_ptrをRGB表色系からHSV表色系へ変換して、hsv_imageに格納
    cv::cvtColor(cv_ptr->image, hsv_image, CV_BGR2HSV);






    cv::Rect range(10, 10, 500, 450);
    //画像のトリミング範囲を指定
    //hsv_imageのトリミング済みのものをhsv_image_tに格納
	cv::Mat hsv_ch=hsv_image(range);
	cv::Mat cv_image1;
	cv::Mat cv_image2;



//任意の色のビットを抽出する
    // hsv=｛色相(Hue), 彩度(Saturation), 明暗(Value, brightness)｝ 
    // 指定した範囲の色でマスク画像color_mask(CV_8U:符号なし8ビット整数)を生成  
    // マスク画像は指定した範囲の色に該当する要素は255(8ビットすべて1)、それ以外は0                                                    
    cv::inRange(hsv_ch, cv::Scalar(10, 0, 130, 0) , cv::Scalar(40, 255, 255, 0), color_mask1);
    cv::inRange(hsv_ch, cv::Scalar(95,45, 40, 100) , cv::Scalar(125, 255, 255, 0), color_mask2);
    //hsv_chに対し、（ｈ色相,ｓ彩度,ｖ明度）(150,100,50,0)から(180,255,255,0)の範囲でマスク画像color_mask1を生成(cv::inRangeの作用として、マスク画像は該当する要素は255,それ以外は0?)
	
	count[0] = cv::countNonZero(color_mask1);
	
	cv::Mat img1 = color_mask1.clone();
	cv::Moments mu1 = moments(img1,false);
	x[0]= mu1.m10/mu1.m00;
	y[0]=mu1.m01/mu1.m00;
	
	count[1] = cv::countNonZero(color_mask2);
	
	cv::Mat img2 = color_mask2.clone();
	cv::Moments mu2 = moments(img2,false);
	x[1]= mu2.m10/mu2.m00;
	y[1]=mu2.m01/mu2.m00;

	
    // ビット毎の論理積。マスク画像は指定した範囲以外は0で、指定範囲の要素は255なので、ビット毎の論理積を適用すると、指定した範囲の色に対応する要素はそのままで、他は0になる。
    cv::bitwise_and(hsv_ch, hsv_ch, cv_image1, color_mask1);
    cv::bitwise_and(hsv_ch, hsv_ch, cv_image2, color_mask2);
    //マスク画像とcv_ptrに対し、ビットごとに論理積を適用。これをcv_image2に格納


//各画像のサイズを変更

    // 画像サイズ指定
    cv::Mat cv_half_image, cv_half_image1, cv_half_image2;
    cv::resize(cv_ptr->image, cv_half_image,cv::Size(),1.5,1.5);  
    cv::resize(cv_image1, cv_half_image1,cv::Size(),1.5,1.5);
    cv::resize(cv_image2, cv_half_image2,cv::Size(),1.5,1.5);

//ウィンドウ表示

    // ウインドウ表示                                                                         
    cv::imshow("Original Image", cv_half_image);
    cv::imshow("Result Image1", cv_half_image1);
    cv::imshow("Result Image2", cv_half_image2);
    cv::waitKey(3);


	std_msgs::Int32MultiArray image_param;
	image_param.data.clear();
	for(int j=0;j<2;j++){
		image_param.data.push_back(x[j]);
		image_param.data.push_back(y[j]);
		image_param.data.push_back(count[j]);
	}
	pub.publish(image_param);
	ROS_INFO("x=%f,y=%f,value=%d,x=%f,y=%f,value=%d",x[0],y[0],count[0],x[1],y[1],count[1]);

  }
};

 
int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}

