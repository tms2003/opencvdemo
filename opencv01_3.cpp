//
// Created by tms on 2019/10/23.
//
//原文链接：https://blog.csdn.net/weixin_41695564/article/details/79897821

#include "opencv2/opencv.hpp"
#include "iostream"

#define   kernel_rows 3
using namespace cv;
using namespace std;
const char *keys =
        "{ help h |                  | 请输入demo图像. }"
        "{ input |          | Path to input image . }";

int main(int argc, char **argv){
    CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help")) {
        std::cout << "Usage: myopencv1 [options]" << std::endl;
        std::cout << "Available options:" << std::endl;
        cmd.printMessage();
        return EXIT_SUCCESS;
    }
    String fileName;
    if (!cmd.get<String>("input").empty()) {
        fileName = cmd.get<String>("input");
    } else {
        //use default file name
        fileName = "scanned-form.jpg";
    }
    cout << "Reading image to process : " << fileName << endl;


    Mat img=imread(fileName);
    if(img.empty()){
        cout<<"Couldn't open the image ..."<<endl;
        return EXIT_FAILURE;
    }


    // 打印卷积核
    Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 4, -1, 0, -1, 0);   //初始化一个卷积核
    for (int i = 0; i < kernel.rows; ++i)
    {
        for (int j = 0; j < kernel.cols; ++j)
        {
            printf("%2d ", kernel.at<char>(i,j));
        }
        printf("\n");
    }

    Mat dst = Mat::zeros(img.size(),img.type());
    // 函数调用filter2D功能
    // 定义掩膜：Mat kernel = (Mat_<char>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
    // filter2D(src, dst, src.depth(), kernel);
    // 其中src与dst是Mat类型变量、src.depth表示位图深度，有32、24、8等。
    filter2D(img,dst,img.depth(),kernel);
    namedWindow("原始图像", WINDOW_AUTOSIZE);
    imshow("原始图像", img);
    waitKey(0);
    namedWindow("修改后图像", WINDOW_AUTOSIZE);
    imshow("修改后图像", dst);
    waitKey(0);
    system("PAUSE");



    //not necessary
    img.release();
    destroyAllWindows();

}