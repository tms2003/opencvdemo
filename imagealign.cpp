//
// Created by tms on 2019/10/23.
//

#include "imagealign.h"
#include "opencv2/opencv.hpp"
#include "opencv2/xfeatures2d.hpp"
#include "opencv2/features2d.hpp"

using namespace std;
using namespace cv;
using namespace cv::xfeatures2d;

const int MAX_FEATURES = 500;
const float GOOD_MATCH_PERCENT = 0.15f;
const char *keys =
        "{ help h |                  | 请输入对齐图像. }"
        "{ input |          | Path to input image . }";

void alignImage(Mat &im1,Mat &im2,Mat &im1Reg,Mat &h){

    //Convert images to grayscale
    Mat im1Gray,im2Gray;
    cvtColor(im1,im1Gray,cv::COLOR_BGR2GRAY);
    cvtColor(im2,im2Gray,COLOR_BGR2GRAY);

    //Variables to store keypoints and descriptors
    std::vector<KeyPoint> keypoint1,keypoint2;
    Mat descriptors1,descriptor2;

    //Detect ORB features and compute descriptors
    Ptr<Feature2D> orb= ORB::create(MAX_FEATURES);
    orb->detectAndCompute(im1Gray,Mat(),keypoint1,descriptors1);
    orb->detectAndCompute(im2Gray,Mat(),keypoint2,descriptor2);

    //Match features.
    std::vector<DMatch> matches;
    Ptr<DescriptorMatcher> matcher= DescriptorMatcher::create("BruteForce-Hamming");
    matcher->match(descriptors1,descriptor2,matches,Mat());

    //sort matches by score
    std::sort(matches.begin(),matches.end());

    //Remove not so good matches
    const int numGoodMatches = matches.size() *GOOD_MATCH_PERCENT;
    matches.erase(matches.begin()+numGoodMatches,matches.end());

    //Draw top matches
    Mat imMatches;
    drawMatches(im1,keypoint1,im2,keypoint2,matches,imMatches);
    imwrite("matches.jpg",imMatches);

    //Extract location of good matches
    std::vector<Point2f> points1,points2;
    for(size_t i=0;i<matches.size();i++){
        points1.push_back(keypoint1[matches[i].queryIdx].pt);
        points2.push_back( keypoint2[ matches[i].trainIdx ].pt );

    }

    //Find homography
    h= findHomography(points1,points2,RANSAC);

    //use homography to warp image,notic:the picture's size will be cropped used img2's size
    warpPerspective(im1,im1Reg,h,im2.size());





}


int main(int argc, char **argv) {

    CommandLineParser cmd(argc, argv, keys);
    if (cmd.has("help")) {
        std::cout << "Usage: myopencv1 [options]" << std::endl;
        std::cout << "Available options:" << std::endl;
        cmd.printMessage();
        return EXIT_SUCCESS;
    }
    //read reference image
    String refFileName("form.jpg");
    cout<<"Reading reference image:"<<refFileName <<endl;
    Mat imReference =imread(refFileName);

    String fileName;
    if (!cmd.get<String>("input").empty()) {
        fileName = cmd.get<String>("input");
    } else {
        //use default file name
        fileName = "scanned-form.jpg";
    }
    cout << "Reading image to align : " << fileName << endl;
    Mat im = imread(fileName);
    if (im.empty()) {
        cout << "cannot found file ";
        return EXIT_FAILURE;
    }

    // Registered image will be resotred in imReg.
    // The estimated homography will be stored in h.
    Mat imReg, h;
    cout << "Aligning images .." << endl;
    alignImage(im, imReference, imReg, h);

    string outFileName("aligned.jpg");
    imwrite(outFileName, imReg);

    //print estimated homography
    cout << "Estimated homography :\n" << h << endl;


}