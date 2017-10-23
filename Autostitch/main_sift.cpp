//#include "opencv2/core/core.hpp"
//#include "highgui.h"
//#include "opencv2/imgproc/imgproc.hpp"
//#include "opencv2/features2d/features2d.hpp"
//#include "opencv2/nonfree/nonfree.hpp"
//
//using namespace cv;
////using namespace std;
//
//int main(int argc, char** argv) {
//    Mat img = imread("box_in_scene.png");
//
//    SIFT sift;    //实例化SIFT类
//
//    vector < KeyPoint > key_points;    //特征点
//    // descriptors为描述符，mascara为掩码矩阵
//    Mat descriptors, mascara;
//    Mat output_img;    //输出图像矩阵
//
//    sift(img, mascara, key_points, descriptors);    //执行SIFT运算
//    //在输出图像中绘制特征点
//    drawKeypoints(img,     //输入图像
//            key_points,      //特征点矢量
//            output_img,      //输出图像
//            Scalar::all(-1),      //绘制特征点的颜色，为随机
//            //以特征点为中心画圆，圆的半径表示特征点的大小，直线表示特征点的方向
//            DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//
//    namedWindow("SIFT");
//    imshow("SIFT", output_img);
//    waitKey(0);
//
//    return 0;
//}
