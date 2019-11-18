//
// Created by tms on 2019/10/25.
//

//
// Created by tms on 2019/10/23.
//
//原文链接：https://blog.csdn.net/weixin_41695564/article/details/79912840

#include "opencv2/opencv.hpp"
#include "iostream"

#define   kernel_rows 3
using namespace cv;
using namespace std;
const char *keys =
        "{ help h |                  | 请输入demo图像. }"
        "{ input1 |          | Path to input src1 image . }"
        "{ input2 |          | Path to input src2 image . }";

int main(int argc, char **argv) {
    CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help")) {
        std::cout << "Usage: myopencv1 [options]" << std::endl;
        std::cout << "Available options:" << std::endl;
        cmd.printMessage();
        return EXIT_SUCCESS;
    }
    String fileName1,fileName2;
    if (!cmd.get<String>("input1").empty() && !cmd.get<String>("input2").empty()) {
        fileName1 = cmd.get<String>("input1");
        fileName2 = cmd.get<String>("input2");

    } else {
        //use default file name
        fileName1 = "scanned-form.jpg";
        fileName2 = "form.jpg";
    }
    cout << "Reading image to process : " << fileName1<<","<<fileName2 << endl;


    Mat imgOrg1 = imread(fileName1);
    Mat imgOrg2 = imread(fileName2);
    if (imgOrg1.empty()&& imgOrg2.empty()) {
        cout << "Couldn't open the image ,we need two images..." << endl;
        return EXIT_FAILURE;
    }




    int width = imgOrg1.size().width;
    int height = imgOrg1.size().height;
    int scaleRatio=width/640;
    int heightNew=height/scaleRatio;

    Mat  img1,img2;
    cv::resize(imgOrg1,img1,cv::Size(640,heightNew));
    cv::resize(imgOrg2,img2,cv::Size(640,heightNew));
    Mat dst;
    if(img1.type()==img2.type()){
        addWeighted(img1,0.4,img2,(1-0.4),0,dst);
        namedWindow("合成图像", WINDOW_AUTOSIZE);
        imshow("合成图像", dst);

    }


    waitKey(0);

    system("PAUSE");



    //not necessary
    img1.release();
    img2.release();
    destroyAllWindows();

}