#include <ros/ros.h>
#include <image_transport/image_transport.h>
#include <cv_bridge/cv_bridge.h>
#include <sensor_msgs/image_encodings.h>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/highgui/highgui.hpp>

static const std::string OPENCV_WINDOW = "Image window";

class ImageConverter
{
  ros::NodeHandle nh_;
  image_transport::ImageTransport it_;
  image_transport::Subscriber image_sub_;
  image_transport::Publisher image_pub_;

public:
  // コンストラクタ
  //（動作時のパブサブ設定？）
ImageConverter()
    : it_(nh_)
  {
    // カラー画像をサブスクライブ                                                                
    image_sub_ = it_.subscribe("/usb_cam/image_raw", 1,
      &ImageConverter::imageCb, this);
    // 処理した画像をパブリシュ                                                                                          
    image_pub_ = it_.advertise("/image_topic", 1);
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
    cv_bridge::CvImagePtr cv_ptr, cv_ptr2, cv_ptr3;
    //cv_ptrはオリジナルに円を描写、cv_ptr3はエッジ検出したものになる
 try
    {
      // ROSからOpenCVの形式にtoCvCopy()で変換。cv_ptr->imageがcv::Matフォーマット。
      cv_ptr    = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::BGR8);
      cv_ptr3   = cv_bridge::toCvCopy(msg, sensor_msgs::image_encodings::MONO8);
    }
    catch (cv_bridge::Exception& e)
    {
      ROS_ERROR("cv_bridge exception: %s", e.what());
      return;
    }


    //（使うものの宣言？）
    cv::Mat hsv_image, color_mask, gray_image, cv_image2, cv_image3;

    // cv_ptrをRGB表色系からHSV表色系へ変換して、hsv_imageに格納
    cv::cvtColor(cv_ptr->image, hsv_image, CV_BGR2HSV);


//任意の色のビットを抽出する
    // hsv=｛色相(Hue), 彩度(Saturation), 明暗(Value, brightness)｝ 
    // 指定した範囲の色でマスク画像color_mask(CV_8U:符号なし8ビット整数)を生成  
    // マスク画像は指定した範囲の色に該当する要素は255(8ビットすべて1)、それ以外は0                                                      
    cv::inRange(hsv_image, cv::Scalar(150, 100, 50, 0) , cv::Scalar(180, 255, 255, 0), color_mask);
   //           hsv_imageに対し、（r,g,b,透明度）(180,100,50,0)から(180,255,255,0)の範囲でマスク画像color_maskを生成(cv::inRangeの作用として、マスク画像は該当する要素は255,それ以外は0)


    // ビット毎の論理積。マスク画像は指定した範囲以外は0で、指定範囲の要素は255なので、ビット毎の論理積を適用すると、指定した範囲の色に対応する要素はそのままで、他は0になる。
    cv::bitwise_and(cv_ptr->image, cv_ptr->image, cv_image2, color_mask);
    //マスク画像とcv_ptrに対し、ビットごとに論理積を適用。これをcv_image2に格納

    

//cv_ptrをグレースケールに変換し、エッジ検出。結果をcv_ptr3のimageに格納

    // グレースケールに変換
    cv::cvtColor(cv_ptr->image, gray_image, CV_BGR2GRAY);
    //cv_ptrをグレースケールに変換し、gray_imageに格納

    // エッジを検出するためにCannyアルゴリズムを適用し、cv_ptr3のimageに格納
    cv::Canny(gray_image, cv_ptr3->image, 15.0, 30.0, 3);
    //前項で作成したグレースケールにCannyアルゴリズムを適用し、cv_ptr3のimageに格納


//cv_ptrのimageに円を描写

    // cv_ptrのimageに円を描写                                               
    cv::circle(cv_ptr->image, cv::Point(100, 100), 20, CV_RGB(0,255,0));



//各画像のサイズを変更

    // 画像サイズ指定
    cv::Mat cv_half_image, cv_half_image2, cv_half_image3;
    cv::resize(cv_ptr->image, cv_half_image,cv::Size(),1.5,1.5);
    cv::resize(cv_image2, cv_half_image2,cv::Size(),1.5,1.5);
    cv::resize(cv_ptr3->image, cv_half_image3,cv::Size(),1.5,1.5);

//ウィンドウ表示

    // ウインドウ表示                                                                         
    cv::imshow("Original Image", cv_half_image);
    cv::imshow("Result Image", cv_half_image2);
    cv::imshow("Edge Image", cv_half_image3);
    cv::waitKey(3);
 
    // エッジ画像をパブリッシュ。OpenCVからROS形式にtoImageMsg()で変換。                                                            
    image_pub_.publish(cv_ptr3->toImageMsg());
  }
};

int main(int argc, char** argv)
{
  ros::init(argc, argv, "image_converter");
  ImageConverter ic;
  ros::spin();
  return 0;
}

