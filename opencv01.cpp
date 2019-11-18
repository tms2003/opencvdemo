//
// Created by tms on 2019/10/23.
//

#include "opencv2/opencv.hpp"
#include "iostream"

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

    for(size_t r=0;r<img.rows;r++){
        const uchar* ptr=img.ptr<uchar >(r);
        for(size_t c=0;c<img.cols;c++){
            printf("%2d ",ptr[c]);
            //cout<<ptr[c]<<" ";
        }
        cout<<endl;
    }


    waitKey(0);


    //not necessary
    img.release();
    destroyAllWindows();

}