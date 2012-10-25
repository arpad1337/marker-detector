//
//  MarkerDetector.h
//  CompTest
//
//  Created by Árpád Kiss on 2012.10.24..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

#ifndef __CompTest__MarkerDetector__
#define __CompTest__MarkerDetector__

#include "cv.h"
#include <iostream>

using namespace cv;

class MarkerDetector
{
public:
    MarkerDetector();
    void preprocessImage(Mat& image);
    vector<Mat> findPossibleMarkers(Mat& image);
private:
    int variances[4];
    int _max,length,stride,labres,tresh = 10;
    int i,z = 0;
    uchar* ptr,* ptr_2;
    void differenceEdgeDetectorWithThresh(Mat& image);
    void init();
};

#endif /* defined(__CompTest__MarkerDetector__) */
