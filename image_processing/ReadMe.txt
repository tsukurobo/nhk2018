キャリブレーション
http://karaage.hatenadiary.jp/entry/2015/10/29/073000
のようにカメラを有効化
http://ros-robot.blogspot.jp/2010/11/cameracalibrationusb.html
http://researchmap.jp/joumvmq5i-2001408/



rosで画像取得
http://karaage.hatenadiary.jp/entry/2015/10/29/073000

rosでOpenCVを使う
http://dailyrobottechnology.blogspot.jp/2014/11/rosopencv-c.html

cv_bridgeを使う//
http://demura.net/lecture/12480.html

https://www.google.co.jp/search?q=Ros+tutoriaru&ie=UTF-8&sa=Search&channel=fe&client=browser-ubuntu&hl=en&gws_rd=cr,ssl&ei=JcU0WeHqB8aD8wX-r5_QDQ#channel=fe&hl=en&q=cv_bridge%E3%80%80catkin_make+Cmake%E3%82%A8%E3%83%A9%E3%83%BC




単眼カメラで自己位置推定
https://blog.negativemind.com/2016/02/19/monocular-visual-odometry-using-opencv/



OpenCVのインストールと画像処理をする
http://ossyaritoori.hatenablog.com/entry/2016/10/21/032713
pythonの使い方
https://detail.chiebukuro.yahoo.co.jp/qa/question_detail/q10142258275







ーーーーーーーーーーーーーーーーーーーーーーーーーーーーーー


http://dailyrobottechnology.blogspot.jp/2014/11/rosopencv-c.html
の手順でパッケージ作成
cv_cameraでcameraの指定方法がわからなかったので、usb_camを使用。
http://karaage.hatenadiary.jp/entry/2015/10/29/073000
image_converter.cppの修正で、cv_cameraをusb_camに変更
rosrun cvTest image_converter
をコマンドする前に、
source devel/setup.bash
echo $ROS_PACKAGE_PATH
をコマンド。これをしないとcvtestを認識しない。
これでカメラから取り込んだ画像を処理できた。


エッジ検出終了＆色抽出終了
http://demura.net/lecture/12480.html
image_converterの中身を上記cv_tutorialの内容に書き換えるだけ。動作手順は上記と変わらず。なお、サブスクライブ元の書き換えを忘れないように。


以上により、
１.cvtestにより画像上に円の描写
2.エッジ検出
3.色検出
が可能となった。

//今後は、検出された色の範囲や色を検出した画素の数の情報をパブリッシュできるようにしなければならない。（その検出動作を行う範囲も指定して）でないと検出してもその情報をROSが扱えない。orz
パブリッシュ終了。











