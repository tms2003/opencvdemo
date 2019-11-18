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

    namedWindow("原始图像",WINDOW_AUTOSIZE);
    imshow("原始图像",img);

    int cols=(img.cols -1 )*img.channels();
    int rows =img.rows;
    int gap_col=img.channels();
    Mat dst=Mat::zeros(img.size(),img.type());
    for(int row=((kernel_rows-1)/2);row <=rows-((kernel_rows-1)/2);++row){
        //Mat.ptr<uchar>(int i=0)獲取像素矩陣的指针，索引i表示第几行，从0开始计行数
        //	获得当前行指针const uchar*  current = img.ptr<uchar>(row);
        //  获取当前像素点P(row, col)的像素值 p(row, col) = current[col]
        const uchar* current_row =img.ptr<uchar>(row);
        const uchar* prior_row =img.ptr<uchar>(row);
        const uchar* next_row=img.ptr<uchar>(row);

        uchar* dst_row=dst.ptr<uchar>(row);
        for(int col=0+img.channels();col<=cols;++col){
            dst_row[col]=saturate_cast<uchar>(4*current_row[col]
                    -current_row[col-gap_col]
                    -current_row[col+gap_col]
                    -prior_row[col]
                    -next_row[col]);
            //像素范围处理saturate_cast<uchar>
            //	saturate_cast<uchar>（ - 100），返回 0。
            //	saturate_cast<uchar>（288），返回255
            //	saturate_cast<uchar>（100），返回100
            //	这个函数的功能是确保RGB值得范围在0~255之间





        }

    }
    namedWindow("原始图像", cv::WINDOW_AUTOSIZE);
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