///*
// * main.cpp
// *
// *  Created on: 2017年4月26日
// *      Author: Sine
// */
//
//extern "C" {
//#include "vl/generic.h"
//#include "vl/sift.h"
//}
//#include <CImg.h>
//#include <iostream>
//
//using namespace std;
//
//typedef cimg_library::CImg<unsigned char> Img;
//
//int main() {
//
//    static const unsigned char color[] = {255};
//
//    VL_PRINT("Hello world!\n");
//
//    Img img = Img("1.bmp");
//    const int width = img.width(), height = img.height();
//    int noctaves = 4, nlevels = 2, o_min = 0;
//
//    // noctaves=(int)(log(min)/log(2));
//    //在sift.h头文件中有 typedef float vl_sift_pix 声明
//
//    vl_sift_pix *ImageData = new vl_sift_pix[width* height];
//    for (int i = 0; i < width; i++) {
//        for (int j = 0; j < height; j++) {
//            ImageData[i * height + j] = img(i, j);
//        }
//    }
//    // 定义VlSiftFilt结构体指针
//    VlSiftFilt *SiftFilt = NULL;
//    // 创建一个新的sift滤波器
//    SiftFilt = vl_sift_new(width, height, noctaves, nlevels, o_min);
//
//    int KeyPoint = 0;
//    int idx = 0;
//    if (vl_sift_process_first_octave(SiftFilt, ImageData) != VL_ERR_EOF) {
//        while (TRUE) {
//            //计算每组中的关键点
//            vl_sift_detect(SiftFilt);
//            //遍历并绘制每个点
//            KeyPoint += SiftFilt->nkeys;    //检测到的关键点的数目
//            cout << "关键点数目：" << endl << KeyPoint << endl;
//            VlSiftKeypoint *pKeyPoint = SiftFilt->keys;    //检测到的关键点
//            for (int i = 0; i < SiftFilt->nkeys; i++) {
//                VlSiftKeypoint TemptKeyPoint = *pKeyPoint;
//                pKeyPoint++;
//                img.draw_circle(TemptKeyPoint.x, TemptKeyPoint.y,
//                       TemptKeyPoint.sigma / 2, color);
//
//                idx++;
//                //计算并遍历每个点的方向
//                double angles[4];
//                int angleCount = vl_sift_calc_keypoint_orientations(
//                        SiftFilt, angles, &TemptKeyPoint);    //计算关键点的方向
//                for (int j = 0; j < angleCount; j++) {
//                    double TemptAngle = angles[j];
//                    printf("%d: %f\n", j, TemptAngle);
//                    //计算每个方向的描述
//                    float *Descriptors = new float[128];
//                    vl_sift_calc_keypoint_descriptor(SiftFilt, Descriptors,
//                                                     &TemptKeyPoint,
//                                                     TemptAngle);
//                    int k = 0;
//                    while (k < 128) {
//                        printf("%d: %f", k, Descriptors[k]);
//                        printf("; ");
//                        k++;
//                    }
//
//                    printf("\n");
//                    delete[] Descriptors;
//                    Descriptors = NULL;
//                }
//            }
//            //下一阶
//            if (vl_sift_process_next_octave(SiftFilt) == VL_ERR_EOF) {
//                break;
//            }
//            //free(pKeyPoint);
//            KeyPoint = NULL;
//        }
//    }
//    vl_sift_delete(SiftFilt);
//    delete[] ImageData;
//    ImageData = NULL;
//    img.display();
//}
