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
//    SIFT sift;    //ʵ����SIFT��
//
//    vector < KeyPoint > key_points;    //������
//    // descriptorsΪ��������mascaraΪ�������
//    Mat descriptors, mascara;
//    Mat output_img;    //���ͼ�����
//
//    sift(img, mascara, key_points, descriptors);    //ִ��SIFT����
//    //�����ͼ���л���������
//    drawKeypoints(img,     //����ͼ��
//            key_points,      //������ʸ��
//            output_img,      //���ͼ��
//            Scalar::all(-1),      //�������������ɫ��Ϊ���
//            //��������Ϊ���Ļ�Բ��Բ�İ뾶��ʾ������Ĵ�С��ֱ�߱�ʾ������ķ���
//            DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
//
//    namedWindow("SIFT");
//    imshow("SIFT", output_img);
//    waitKey(0);
//
//    return 0;
//}
