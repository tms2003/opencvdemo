//
// Created by tms on 2019/10/21.
//

#include "siftmatch.h"
#include <iostream>
#include "opencv2/core.hpp"
#include "opencv2/opencv.hpp"

#ifdef HAVE_OPENCV_XFEATURES2D

#include "opencv2/highgui.hpp"
#include "opencv2/features2d.hpp"
#include "opencv2/xfeatures2d.hpp"

using namespace cv;
using namespace cv::xfeatures2d;
using std::cout;
using std::endl;
const char *keys =
        "{ help h |                  | Print help message. }"
        "{ input1 | box.png          | Path to input image 1. }"
        "{ input2 | box_in_scene.png | Path to input image 2. }";

int main(int argc, char *argv[]) {
    CommandLineParser parser(argc, argv, keys);
    // samples::findFile( )

    String file1 = parser.get<String>("input1");
    String file2 = parser.get<String>("input2");

    Mat img1_org = imread(file1, IMREAD_GRAYSCALE);
    Mat img2_org = imread(file2, IMREAD_GRAYSCALE);
    Mat img1, img2;
    if (img1_org.size().width > 640) {
        cv::resize(img1_org, img1, cv::Size(640, 480));
        cv::resize(img2_org, img2, cv::Size(640, 480));
    } else {
        img1_org.copyTo(img1);
        img2_org.copyTo(img2);
    }

    if (img1.empty() || img2.empty()) {
        cout << "Could not open or find the image!\n" << endl;
        parser.printMessage();
        return -1;
    }
    //-- Step 1: Detect the keypoints using SURF Detector, compute the descriptors
    int minHessian = 400;
    Ptr<SURF> detector = SURF::create(minHessian);
    std::vector<KeyPoint> keypoints1, keypoints2;
    Mat descriptors1, descriptors2;
    detector->detectAndCompute(img1, noArray(), keypoints1, descriptors1);
    detector->detectAndCompute(img2, noArray(), keypoints2, descriptors2);
    //-- Step 2: Matching descriptor vectors with a FLANN based matcher
    // Since SURF is a floating-point descriptor NORM_L2 is used
    Ptr<DescriptorMatcher> matcher = DescriptorMatcher::create(DescriptorMatcher::FLANNBASED);
    std::vector<std::vector<DMatch> > knn_matches;
    matcher->knnMatch(descriptors1, descriptors2, knn_matches, 2);
    //-- Filter matches using the Lowe's ratio test
    const float ratio_thresh = 0.7f;
    std::vector<DMatch> good_matches;
    for (size_t i = 0; i < knn_matches.size(); i++) {
        if (knn_matches[i][0].distance < ratio_thresh * knn_matches[i][1].distance) {
            good_matches.push_back(knn_matches[i][0]);
        }
    }
    //-- Draw matches
    Mat img_matches;
    drawMatches(img1, keypoints1, img2, keypoints2, good_matches, img_matches, Scalar::all(-1),
                Scalar::all(-1), std::vector<char>(), DrawMatchesFlags::NOT_DRAW_SINGLE_POINTS);
    //-- Show detected matches
    imshow("Good Matches", img_matches);
    waitKey();
    return 0;
}

#else
int main()
{
    std::cout << "This tutorial code needs the xfeatures2d contrib module to be run." << std::endl;
    return 0;
}
#endif