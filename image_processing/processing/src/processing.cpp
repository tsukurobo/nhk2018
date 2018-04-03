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
#include <vector>
static const std::string OPENCV_WINDOW = "Image window";
static int count[4];
//取りだす色の下限(h,s,v)
static const int GL[3]={65,10, 30},YL[3]={15, 5, 30},BL[3]={110, 10, 50},RL1[3]={0, 10, 30},RL2[3]={160, 10, 30}; 
//取りだす色の上限(h,s,v)
static const int GU[3]={95, 255, 255},YU[3]={50, 255, 255},BU[3]={150, 255, 255},RU1[3]={10, 255, 255},RU2[3]={180, 255, 255};

float  x[4];
float y[4];
//c コントラスト調整値           　g　ガンマ補正値
//  色くっきり←5→淡く(白黒)       暗く←1→明るく
float c=4.8f,g=0.9f;
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
    cv::Mat hsv_image, color_mask1, color_mask2, color_mask3, color_mask4, color_maskR1, color_maskR2;
    std::vector<cv::Mat> planes;

    // cv_ptrをRGB表色系からHSV表色系へ変換して、hsv_imageに格納
    cv::cvtColor(cv_ptr->image, hsv_image, CV_BGR2HSV);





    cv::Rect range(10, 10, 500, 450);
    //画像のトリミング範囲を指定
    //hsv_imageのトリミング済みのものをhsv_image_tに格納
	cv::Mat hsv=hsv_image(range);
	cv::Mat cv_image1;
	cv::Mat cv_image2;
	cv::Mat cv_image3;
	cv::Mat cv_image4;











	cv::split(hsv, planes);



 // 入力パラメータ
    // 色の補正を行うか
    bool modColor[] = {false, true, true}; // Hueはいじらない
    // コントラスト調整用（各チャンネルでのシグモイド関数のゲイン）
    //  色くっきり←5→淡く(白黒)
    float as[] = {0, c, c};
    
    // ガンマ補正用（各チャンネルでのシグマ値）
    // 暗く←1→明るく
    float gammas[] = {0, g, g};
    
    uchar lut[256];
    for (int ch = 0; ch < 3; ch++) {
        if (!modColor[ch]) continue;

        float a = as[ch];
        float gamma = gammas[ch];
        cv::Mat original = planes[ch];

        // 出力画像
        cv::Mat result;
        
        // コントラスト調整
        // ルックアップテーブル作成
        for (int i = 0; i < 256; i++){
        	lut[i] = 255.0 / (1+exp(-a*(i-128)/255));
        }
       
        cv::LUT(original, cv::Mat(cv::Size(256, 1), CV_8U, lut), result);

        
        // ガンマ補正
        if (gamma != 1) {
            double gm = 1.0 / gamma;
            // ルックアップテーブル作成
            for (int i = 0; i < 256; i++){
            	lut[i] = pow(1.0*i/255, gm) * 255;
            }
        
            cv::LUT(result, cv::Mat(cv::Size(256, 1), CV_8U, lut), result);
        }
        

        planes[ch] = result;
    }
    

    
     
    // チャンネルを合成する
    cv::Mat hsv_ch;
    cv::merge(planes, hsv_ch);



















//任意の色のビットを抽出する
    // hsv=｛色相(Hue), 彩度(Saturation), 明暗(Value, brightness)｝ 
    // 指定した範囲の色でマスク画像color_mask(CV_8U:符号なし8ビット整数)を生成  
    // マスク画像は指定した範囲の色に該当する要素は255(8ビットすべて1)、それ以外は0                                                    
    cv::inRange(hsv_ch, cv::Scalar(YL[0], YL[1], YL[2], 0) , cv::Scalar(YU[0], YU[1], YU[2], 0), color_mask1);
    cv::inRange(hsv_ch, cv::Scalar(GL[0], GL[1], GL[2], 0) , cv::Scalar(GU[0], GU[1], GU[2], 0), color_mask2);
    cv::inRange(hsv_ch, cv::Scalar(BL[0], BL[1], BL[2], 0) , cv::Scalar(BU[0], BU[1], BU[2], 0), color_mask3);    
    cv::inRange(hsv_ch, cv::Scalar(RL1[0], RL1[1], RL1[2], 0) , cv::Scalar(RU1[0], RU1[1], RU1[2], 0), color_maskR1); 
    cv::inRange(hsv_ch, cv::Scalar(RL2[0], RL2[1], RL2[2], 0) , cv::Scalar(RU2[0], RU2[1], RU2[2], 0), color_maskR2);
    //hsv_chに対し、（ｈ色相,ｓ彩度,ｖ明度）(150,100,50,0)から(180,255,255,0)の範囲でマスク画像color_mask1を生成(cv::inRangeの作用として、マスク画像は該当する要素は255,それ以外は0?)
	
	
	cv::bitwise_or(color_maskR1, color_maskR2, color_mask4);
	
	
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
	
	count[2] = cv::countNonZero(color_mask3);
	cv::Mat img3 = color_mask3.clone();
	cv::Moments mu3 = moments(img3,false);
	x[2]= mu3.m10/mu3.m00;
	y[2]=mu3.m01/mu3.m00;
	
	count[3] = cv::countNonZero(color_mask4);
	cv::Mat img4 = color_mask4.clone();
	cv::Moments mu4 = moments(img4,false);
	x[3]= mu4.m10/mu4.m00;
	y[3]=mu4.m01/mu4.m00;

	
    // ビット毎の論理積。マスク画像は指定した範囲以外は0で、指定範囲の要素は255なので、ビット毎の論理積を適用すると、指定した範囲の色に対応する要素はそのままで、他は0になる。
    cv::bitwise_and(hsv_ch, hsv_ch, cv_image1, color_mask1);
    cv::bitwise_and(hsv_ch, hsv_ch, cv_image2, color_mask2);
    cv::bitwise_and(hsv_ch, hsv_ch, cv_image3, color_mask3);
    cv::bitwise_and(hsv_ch, hsv_ch, cv_image4, color_mask4);
    //マスク画像とhsv_chに対し、ビットごとに論理積を適用。これをcv_image2に格納


//各画像のサイズを変更

    // 画像サイズ指定
    cv::Mat cv_half_image, hsv_half_image, cv_half_image1, cv_half_image2, cv_half_image3, cv_half_image4;
    cv::resize(cv_ptr->image, cv_half_image,cv::Size(),1.0,1.0);
    cv::resize(hsv_ch, hsv_half_image,cv::Size(),1.0,1.0);  
    cv::resize(cv_image1, cv_half_image1,cv::Size(),1.0,1.0);
    cv::resize(cv_image2, cv_half_image2,cv::Size(),1.0,1.0);
    cv::resize(cv_image3, cv_half_image3,cv::Size(),1.0,1.0);
    cv::resize(cv_image4, cv_half_image4,cv::Size(),1.0,1.0);

//ウィンドウ表示
    // ウインドウ表示                  
    cv::cvtColor(hsv_half_image, hsv_half_image, CV_HSV2BGR);                                                       
    cv::imshow("Original Image", cv_half_image);
    cv::imshow("hsv Image", hsv_half_image);
    cv::imshow("Yellow Image", cv_half_image1);
    cv::imshow("Green Image", cv_half_image2);
    cv::imshow("Brue Image", cv_half_image3);
    cv::imshow("Red Image", cv_half_image4);
    cv::waitKey(3);


	std_msgs::Int32MultiArray image_param;
	image_param.data.clear();
	for(int j=0;j<4;j++){
		image_param.data.push_back(x[j]);
		image_param.data.push_back(y[j]);
		image_param.data.push_back(count[j]);
	}
	pub.publish(image_param);
	ROS_INFO("Y_x=%f,Y_y=%f,Y_value=%d,G_x=%f,G_y=%f,G_value=%d,B_x=%f,B_y=%f,B_value=%d,R_x=%f,R_y=%f,R_value=%d",x[0],y[0],count[0],x[1],y[1],count[2],x[2],y[2],count[2],x[3],y[3],count[3]);

  }
};

 
int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}

