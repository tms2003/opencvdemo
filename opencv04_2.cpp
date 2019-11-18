//
// Created by tms on 2019/10/29.
//

//
// Created by tms on 2019/10/28.
//https://blog.csdn.net/weixin_41695564/article/details/79928835
//


#include "opencv2/opencv.hpp"
#include "iostream"

#define   kernel_rows 3
using namespace cv;
using namespace std;
const char *keys =
        "{ help h |                  | 请输入demo图像. }"
        "{ input1 |          | Path to input src1 image . }";
int max_size = 21;//global varify
int element_size = 3;
Mat img1, dst;

void callBack_fun(int , void*);

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
    int scaleRatio = width / 480;
    int heightNew = height / scaleRatio;


    cv::resize(imgOrg1, img1, cv::Size(480, heightNew));



    float alpha = 1.5;
    float beta = 30;
    int channelNum = img1.channels();
    //write myself scaleabs




    namedWindow("dilate picture", WINDOW_AUTOSIZE);
    createTrackbar("结构元尺寸 ：", "dilate picture", &element_size, max_size, callBack_fun);

    callBack_fun(element_size,0);



    //namedWindow("source图像", WINDOW_AUTOSIZE);
    //imshow("source图像", img1);

    waitKey(0);

    system("PAUSE");



    //not necessary
    img1.release();

    destroyAllWindows();

}

void callBack_fun(int,void *){
    int s=element_size*2-1;
    if(s<1)
        return ;
    Mat structElement=getStructuringElement(MORPH_RECT,
            Size(s,s),
            Point(-1,-1));
    //创建结构元

    erode(img1,dst,structElement);
    imshow("dilate picture", dst);
}