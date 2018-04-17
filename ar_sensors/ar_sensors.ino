#include <Wire.h>
#include "n_line_sensor.h"
#include "LrfPair.h"
#include <ros.h>
#include <std_msgs/Float32.h>
#include <std_msgs/Int16.h>
#include <std_msgs/Int8.h>
#include "std_msgs/MultiArrayLayout.h"
#include "std_msgs/MultiArrayDimension.h"
#include "std_msgs/Int16MultiArray.h"
#include "std_msgs/Int8MultiArray.h"
#include "std_msgs/Float32MultiArray.h"
#include <VL53L0X.h>

#define LINE_THRESHOLD 800
#define LINE_SENSOR_ALL_SUM 8 // LINE_SENSOR_AVR_SUM * 2
#define SENSORGROUP_SUM 4//ラインセンサの基盤の数
#define LINE_SENSOR_THRESH 3 //ｎ以上のラインセンサが黒を検出したら参照しないようにする
#define FW_GROUP 0 //正面方向のセンサ郡
#define SD_GROUP 1 //側面方向のセンサ郡
#define PARALLAX 100 //２つのセンサー視差。mm。
#define LONG_RANGE
#define THRESHOLD 1.00 //angle 傾いているとみなす角度
#define pi 3.14159265358979
#define REQUESTS_NUM 3 //リクエストの数（req_pubの数）

#define A_LSHUT 30
#define A_RSHUT 34
///////////////
#define B_LSHUT 38
#define B_RSHUT 42

int SHUTS[4] = {A_LSHUT,A_RSHUT,B_LSHUT,B_RSHUT};

#define LRF_NUM 4
#define LRF_ERR 5000 //離れすぎるとLRFは8109?みたいな値を出す

//////////////
#define CW 1
#define CCW -1
#define STOP 0
//////////////
#define FOWARD 0
#define SIDE 1
#define TURN 2
//////////////

///////////////
#define A_L 0
#define A_R 1
#define B_L 2
#define B_R 3
///////////////

NLineSensor lineSensor[SENSORGROUP_SUM] = {NLineSensor(0x14, 0x15),NLineSensor(0x10,0x11),NLineSensor(0x12,0x13),NLineSensor(0x16,0x17)};
int sensor_group[SENSORGROUP_SUM] = {FW_GROUP,FW_GROUP,SD_GROUP,SD_GROUP};//各アドレスのラインセンサがどの向きについているか。
int line_sensor_value[SENSORGROUP_SUM][LINE_SENSOR_ALL_SUM];//[基盤単位][個々のセンサ]
int line_sensor_thresh[LINE_SENSOR_ALL_SUM];
boolean is_online[SENSORGROUP_SUM][LINE_SENSOR_ALL_SUM];//0->off 1->on
int sensors_weight[SENSORGROUP_SUM];//ロボット視点で左半分は負、右半分は正でそのセンサー郡の重みを記録する

ros::NodeHandle nh;

LrfPair* pairA = new LrfPair(A_LSHUT,A_RSHUT);
LrfPair* pairB = new LrfPair(B_LSHUT,B_RSHUT);

std_msgs::Int8MultiArray req_arr;  //{FOWARD,SIDE,TURN}
int fw_req,sd_req,turn_req; //-1->CCW  0->STOP(ON_LINE) 1->CW
ros::Publisher req_pub("req",&req_arr);

std_msgs::Int8MultiArray lines;
ros::Publisher line_pub("line_param",&lines);
std_msgs::Float32MultiArray lrfs;
ros::Publisher lrf_pub("lrf_param",&lrfs);

int weight_map[LINE_SENSOR_ALL_SUM] = {-4,-3,-2,-1,1,2,3,4};

void load_default_thresh(){
  for(int i=0;i<LINE_SENSOR_ALL_SUM;i++){
    line_sensor_thresh[i] = LINE_THRESHOLD;
  }
}

void init_vars(){//変数初期化
  for(int i=0;i<SENSORGROUP_SUM;i++){
    for(int j=0;j<LINE_SENSOR_ALL_SUM;j++){
      is_online[i][j] = 0;
    }
    sensors_weight[i] = 0;
  }
}

void set_sensor_status(){//各センサがライン上にいるか0or1で格納
  for(int i=0;i<SENSORGROUP_SUM;i++){
    for(int j=0;j<LINE_SENSOR_ALL_SUM;j++){
      if(line_sensor_value[i][j] < line_sensor_thresh[i])is_online[i][j] = false;
      else is_online[i][j] = true;
      Serial.print(line_sensor_value[i][j]);
      Serial.print(":");
    }
    Serial.println();
  }
}

void sumup_weight(){//ロボット視点で左半分は負、右半分は正でそのセンサー郡の重みを記録する
  for(int i=0;i<SENSORGROUP_SUM;i++){
    for(int j=0;j<LINE_SENSOR_ALL_SUM;j++){
      if(j<LINE_SENSOR_ALL_SUM/2)
        sensors_weight[i] -= is_online[i][j];
      else
        sensors_weight[i] += is_online[i][j];
    }
  }
}

void check_weight(){
  int online_sum[SENSORGROUP_SUM] = {0,0,0,0};
  int max_weight_l[SENSORGROUP_SUM] = {0,0,0,0};
  int max_weight_r[SENSORGROUP_SUM] = {0,0,0,0};
  for(int i = 0; i<SENSORGROUP_SUM; i++){
    for(int j=0;j<LINE_SENSOR_ALL_SUM; j++){
      if(j<LINE_SENSOR_ALL_SUM/2){ 
        if(is_online[i][j] && max_weight_l[i] > weight_map[j])max_weight_l[i] = weight_map[j];
      }else{
        if(is_online[i][j] && max_weight_r[i] < weight_map[j])max_weight_r[i] = weight_map[j];
      }
      sensors_weight[i] = max_weight_l[i] + max_weight_r[i];
      if(is_online[i][j]) online_sum[i]++;
    }
  }

  for(int i=0;i<SENSORGROUP_SUM;i++){
    if(online_sum[i] >= LINE_SENSOR_THRESH)sensors_weight[i] = 0;
  }
}

/*void set_turn_req(float ang_a, float ang_b){//壁までの距離が一番近い面のみを参照して角度調整する。
  if(pairA->reliability && !(pairB->reliability)){
    if(ang_a < -THRESHOLD)turn_req = CCW;
    else if(ang_a > THRESHOLD)turn_req = CW;
    else turn_req = 0;
  }else if(!(pairA->reliability) && pairB->reliability){
    if(ang_b < -THRESHOLD)turn_req = CCW;
    else if(ang_b > THRESHOLD)turn_req = CW;
    else turn_req = 0;
    
  }else if(pairA->reliability && pairB->reliability){
    if( pairA->distance_avr >= pairB->distance_avr){
      if(ang_b < -THRESHOLD)turn_req = CCW;
      else if(ang_b > THRESHOLD)turn_req = CW;
      else turn_req = 0;
    }else if (pairA->distance_avr < pairB->distance_avr){
      if(ang_a < -THRESHOLD)turn_req = CCW;
      else if(ang_a > THRESHOLD)turn_req = CW;
      else turn_req = 0;
    }
  }
}*/

/*void set_fwsd_req(){
  int fw_weight[2] = {0,0};
  int sd_weight[2] = {0,0};
  int fwcnt = 0;
  int sdcnt = 0; 
  
  for(int i = 0;i<SENSORGROUP_SUM;i++){
    if(sensor_group[i] == FW_GROUP){
      fw_weight[fwcnt] = sensors_weight[i];
      fwcnt++;
    }else if(sensor_group[i] == SD_GROUP){
      sd_weight[sdcnt] = sensors_weight[i];
      sdcnt++;
    }
  }

  if(fw_weight[0] < 0 && fw_weight[1] < 0)sd_req = CW;//正面向きのラインセンサが踏むラインが左寄りのとき
  else if(fw_weight[0] > 0 && fw_weight[1] > 0)sd_req = CCW;

  if(sd_weight[0] < 0 && sd_weight[1] < 0)fw_req = CCW;//側向きのラインセンサが踏むラインが左寄りのとき
  else if(sd_weight[0] > 0 && sd_weight[1] > 0)fw_req = CW;
}*/

/*void set_mecanum_requests(){ 
  set_turn_req(pairA->angle,pairB->angle);
  set_fwsd_req();

  req_arr.data[FOWARD] = fw_req;
  req_arr.data[SIDE] = sd_req;
  req_arr.data[TURN] = turn_req;
}*/

void set_publish_data(){
  for(int i=0;i<SENSORGROUP_SUM;i++){
    lines.data[i] = sensors_weight[i];
  }
  lrfs.data[0] = pairA->angle;//角度正面
  lrfs.data[1] = pairB->angle;//角度側面
  lrfs.data[2] = pairA->distanceL;//距離正面L
  lrfs.data[3] = pairA->distanceR;//距離正面R
  lrfs.data[4] = pairB->distanceL;//距離側面L
  lrfs.data[5] = pairB->distanceR;//距離側面R
  
}

void setup() {
  // put your setup code here, to run once:
    
  Wire.begin();
  init_vars();
  load_default_thresh();
  req_arr.data_length=REQUESTS_NUM;
  req_arr.data=(int8_t*)malloc(sizeof(int8_t*)*REQUESTS_NUM);

  lines.data_length = SENSORGROUP_SUM;
  lines.data=(int8_t*)malloc(sizeof(int8_t*)*SENSORGROUP_SUM);
  lrfs.data_length = 6;
  lrfs.data=(float*)malloc(sizeof(float*)*6);
  
  nh.initNode();
  nh.advertise(req_pub);
  nh.advertise(lrf_pub);
  nh.advertise(line_pub);

  Serial.begin(9600);
}


void loop() {
  pairA->getDistance();
  pairB->getDistance();

  pairA->getAngle();
  pairB->getAngle();

  Serial.print(pairA->distanceL);
  Serial.print(":");
  Serial.print(pairA->distanceR);
  Serial.print("::");
  Serial.print(pairB->distanceL);
  Serial.print(":");
  Serial.println(pairB->distanceR);
  
  // put your main code here, to run repeatedly:*/
  
  for(int i=0;i<SENSORGROUP_SUM;i++){
    lineSensor[i].lineSensor(line_sensor_value[i]);
  }
  set_sensor_status();
  //sumup_weight();
  check_weight();
  set_publish_data();
  //set_mecanum_requests();
  //req_pub.publish(&req_arr);
  
  line_pub.publish(&lines);
  lrf_pub.publish(&lrfs);

  nh.spinOnce();
  delay(100);
  
}
