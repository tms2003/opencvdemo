//
// Created by tms on 2019/10/25.
//

//
// Created by tms on 2019/10/23.
//
//原文链接：https://blog.csdn.net/weixin_41695564/article/details/79903666

#include "opencv2/opencv.hpp"
#include "iostream"

#define   kernel_rows 3
using namespace cv;
using namespace std;
const char *keys =
        "{ help h |                  | 请输入demo图像. }"
        "{ input |          | Path to input image . }";

int main(int argc, char **argv) {
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


    Mat img = imread(fileName);
    if (img.empty()) {
        cout << "Couldn't open the image ..." << endl;
        return EXIT_FAILURE;
    }

    Mat dst(img.size(), img.type());// 创建一个与img同类型和大小的Mat对象

    //img.copyTo(dst);//也可以使用Copyto方法来生成一个
    int width = img.size().width;
    int height = img.size().height;

    for (int i = 0; i < height; ++i) {
        for (int j = 0; j < width; ++j) {
            dst.at<Vec3b>(i, j)[0] = 255 - img.at<Vec3b>(i, j)[0];
            dst.at<Vec3b>(i, j)[1] = 255 - img.at<Vec3b>(i, j)[1];
            dst.at<Vec3b>(i, j)[2] = 255 - img.at<Vec3b>(i, j)[2];

            cout << img.at<Vec3b>(i, j) << ",";

        }
        cout << endl;

    }


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