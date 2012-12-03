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
    MarkerDetector(int len, int str, int hei);
    ~MarkerDetector();
    int Length() const;
    int Stride() const;
    int Height() const;
    void setLength(int const value);
    void setStride(int const value);
    void setHeight(int const value);
    void preprocessImage();
    void findPossibleMarkers();
    //IplImage *CurrentFrame();
    Mat CurrentFrame() const;
    Mat CurrentFrameGray() const;
    Mat PreprocessedFrame() const;
    Mat SegmentsFrame() const;
    void setCurrentFrame(Mat const value);
    Mat currentMarker;
private:
    int height;
    Mat currentFrame;
    Mat currentFrameGray;
    Mat preprocessedFrame;
    Mat segmentsFrame;
    Mat warpMatrix;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int variances[4];
    int _max,length,stride,labres,tresh = 20;
    int i,z = 0;
    uchar* ptr,* ptr_2;
    void differenceEdgeDetectionWithThresh(Mat grayFrame);
    void init();
};

#endif /* defined(__CompTest__MarkerDetector__) */
