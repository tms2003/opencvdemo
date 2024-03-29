//
// Created by tms on 2019/10/21.
//


#include<iostream>
#include <opencv2/core/core.hpp>
#include <opencv2/opencv.hpp>
#include "opencv2/xfeatures2d.hpp"
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/features2d/features2d.hpp>
#include <opencv2/xfeatures2d/nonfree.hpp>
#include "opencv2/calib3d/calib3d.hpp"
#include "opencv2/highgui.hpp"

#include<vector>
#include<cmath>

#define PI 3.14159265358979323846264338327950288419716939937510582097
using namespace std;
using namespace cv;

int main()
{
    //读取原始基准图和待匹配图
    Mat img1_org = imread("/home/tms/data/01.jpg");      //待配准图
    Mat img2_org = imread("/home/tms/data/02.jpg");      //基准图

    Mat img1,img2;
    if(img1_org.size().width>640){
        cv::resize(img1_org,img1,cv::Size(640, 480));
        cv::resize(img2_org,img2,cv::Size(640, 480));
    }else{
        img1_org.copyTo(img1);
        img2_org.copyTo(img2);
    }
    //显示基准和待配准图
    imshow("待配准图", img1);
    imshow("基准图", img2);


    //定义SIFT特征检测类对象
    Ptr<Feature2D> f2d = xfeatures2d::SIFT::create(2000);


    //定义KeyPoint变量
    vector<KeyPoint>keyPoints1;
    vector<KeyPoint>keyPoints2;
    //特征点检测
    f2d->detect( img1, keyPoints1 );
    f2d->detect( img2, keyPoints2 );



    //绘制特征点(关键点)
    Mat feature_pic1, feature_pic2;
    drawKeypoints(img1, keyPoints1, feature_pic1, Scalar::all(-1));
    drawKeypoints(img2, keyPoints2, feature_pic2, Scalar::all(-1));

   // drawKeypoints(srcImg1, keyPoints1, feature_pic1, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);
    //drawKeypoints(srcImg2, keyPoints2, feature_pic2, Scalar(0, 255, 0), DrawMatchesFlags::DRAW_RICH_KEYPOINTS);

    //显示原图

    //显示结果
    imshow("feature1", feature_pic1);
    imshow("feature2", feature_pic2);

    //计算特征点描述符 / 特征向量提取





    Mat descriptors_1, descriptors_2;
    f2d->compute( img1, keyPoints1, descriptors_1 );
    f2d->compute( img2, keyPoints2, descriptors_2 );

    cout << keyPoints1.size() << endl;
    cout << descriptors_1.cols << endl;      //列数
    cout << descriptors_1.rows << endl;      //行数




    //进行BFMatch暴力匹配
    //BruteForceMatcher<L2<float>>matcher;    //实例化暴力匹配器
//    FlannBasedMatcher matcher;  //实例化FLANN匹配器
//    vector<DMatch>matches;   //定义匹配结果变量
//    matcher.match(descriptors_1, descriptors_2, matches);  //实现描述符之间的匹配
    BFMatcher matcher;
    std::vector< DMatch > matches;
    matcher.match( descriptors_1, descriptors_2, matches );


    //中间变量
    int i,j,k;double sum=0;double b;

    double max_dist = 0;
    double min_dist = 100;
    for(int i=0; i<matches.size(); i++)
    {
        double dist = matches[i].distance;
        if(dist < min_dist)
            min_dist = dist;
        if(dist > max_dist)
            max_dist = dist;
    }
    cout<<"最大距离："<<max_dist<<endl;
    cout<<"最小距离："<<min_dist<<endl;

    //筛选出较好的匹配点
    vector<DMatch> good_matches;
    double dThreshold = 0.5;    //匹配的阈值，越大匹配的点数越多
    for(int i=0; i<matches.size(); i++)
    {
        if(matches[i].distance < dThreshold * max_dist)
        {
            good_matches.push_back(matches[i]);
        }
    }

    //RANSAC 消除误匹配特征点 主要分为三个部分：
    //1）根据matches将特征点对齐,将坐标转换为float类型
    //2）使用求基础矩阵方法findFundamentalMat,得到RansacStatus
    //3）根据RansacStatus来将误匹配的点也即RansacStatus[i]=0的点删除

    //根据matches将特征点对齐,将坐标转换为float类型
    vector<KeyPoint> R_keypoint01,R_keypoint02;
    for (i=0;i<good_matches.size();i++)
    {
        R_keypoint01.push_back(keyPoints1[good_matches[i].queryIdx]);
        R_keypoint02.push_back(keyPoints2[good_matches[i].trainIdx]);
        // 这两句话的理解：R_keypoint1是要存储img01中能与img02匹配的特征点，
        // matches中存储了这些匹配点对的img01和img02的索引值
    }

    //坐标转换
    vector<Point2f>p01,p02;
    for (i=0;i<good_matches.size();i++)
    {
        p01.push_back(R_keypoint01[i].pt);
        p02.push_back(R_keypoint02[i].pt);
    }

    //计算基础矩阵并剔除误匹配点
    vector<uchar> RansacStatus;
    Mat Fundamental= findHomography(p01,p02,RansacStatus,RANSAC);
    Mat dst;
    warpPerspective(img1, dst, Fundamental,Size(img1.cols,img1.rows));

    imshow("配准后的图",dst );
    imwrite("dst.jpg", dst);

    //剔除误匹配的点对
    vector<KeyPoint> RR_keypoint01,RR_keypoint02;
    vector<DMatch> RR_matches;            //重新定义RR_keypoint 和RR_matches来存储新的关键点和匹配矩阵
    int index=0;
    for (i=0;i<good_matches.size();i++)
    {
        if (RansacStatus[i]!=0)
        {
            RR_keypoint01.push_back(R_keypoint01[i]);
            RR_keypoint02.push_back(R_keypoint02[i]);
            good_matches[i].queryIdx=index;
            good_matches[i].trainIdx=index;
            RR_matches.push_back(good_matches[i]);
            index++;
        }
    }
    cout<<"找到的特征点对："<<RR_matches.size()<<endl;

    //画出消除误匹配后的图
    Mat img_RR_matches;
    drawMatches(img1,RR_keypoint01,img2,RR_keypoint02,RR_matches,img_RR_matches, Scalar(0, 255, 0), Scalar::all(-1));
    imshow("消除误匹配点后",img_RR_matches);
    imwrite("匹配图.jpg", img_RR_matches);

    waitKey(0);

    return 0;
}
