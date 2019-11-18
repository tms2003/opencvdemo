//
// Created by tms on 2019/10/28.
//https://blog.csdn.net/weixin_41695564/article/details/79912840
//


#include "opencv2/opencv.hpp"
#include "iostream"

#define   kernel_rows 3
using namespace cv;
using namespace std;
const char *keys =
        "{ help h |                  | 请输入demo图像. }"
        "{ input1 |          | Path to input src1 image . }";

int main(int argc, char **argv) {
    CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help")) {
        std::cout << "Usage: myopencv1 [options]" << std::endl;
        std::cout << "Available options:" << std::endl;
        cmd.printMessage();
        return EXIT_SUCCESS;
    }
    String fileName1, fileName2;
    if (!cmd.get<String>("input1").empty()
            ) {
        fileName1 = cmd.get<String>("input1");


    } else {
        //use default file name
        fileName1 = "scanned-form.jpg";

    }
    cout << "Reading image to process : " << fileName1 << "," << fileName2 << endl;


    Mat imgOrg1 = imread(fileName1);

    if (imgOrg1.empty()) {
        cout << "Couldn't open the image ,we need two images..." << endl;
        return EXIT_FAILURE;
    }


    int width = imgOrg1.size().width;
    int height = imgOrg1.size().height;
    int scaleRatio = width / 640;
    int heightNew = height / scaleRatio;

    Mat img1, img2;
    cv::resize(imgOrg1, img1, cv::Size(640, heightNew));


    Mat dst;
    dst = Mat::zeros(img1.size(), img1.type());

    float alpha = 1.5;
    float beta = 30;
    int channelNum= img1.channels();
   //write myself scaleabs

    for (int row = 0; row < width; ++row) {

        for (int col = 0; col < height; ++col) {
            if(channelNum==3){
                Vec3f curPoint=img1.at<Vec3f>(row,col);
                float b=img1.at<Vec3b>(row,col)[0];
                float g=img1.at<Vec3b>(row,col)[1];
                float r=img1.at<Vec3b>(row,col)[2];

                dst.at<Vec3b>(row,col)[0]=saturate_cast<uchar>(b*alpha+beta);
                dst.at<Vec3b>(row,col)[1]=saturate_cast<uchar>(g*alpha+beta);
                dst.at<Vec3b>(row,col)[2]=saturate_cast<uchar>(r*alpha+beta);




            }else if(channelNum==1){
                float v=img1.at<uchar >(row,col);
                dst.at<uchar>(row,col)=v*alpha+beta;

            }

        }

    }



    namedWindow("change light picture", WINDOW_AUTOSIZE);
    imshow("change light picture", dst);

    namedWindow("source图像", WINDOW_AUTOSIZE);
    imshow("source图像", img1);

    waitKey(0);

    system("PAUSE");



    //not necessary
    img1.release();

    destroyAllWindows();

}