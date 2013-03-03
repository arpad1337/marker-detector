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
#include "Marker.h"

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
    vector<Marker> findPossibleMarkers();
    //IplImage *CurrentFrame();
    Mat CurrentFrame() const;
    Mat CurrentFrameGray() const;
    Mat PreprocessedFrame() const;
    Mat PreprocessedFrameColor() const;
    Mat SegmentsFrame() const;
    void setCurrentFrame(Mat value);
private:
    int height;
    Mat currentFrame;
    Mat currentFrameGray;
    Mat preprocessedFrameColor;
    Mat preprocessedFrame;
    Mat segmentsFrame;
    vector<vector<Point> > contours;
    vector<Vec4i> hierarchy;
    int variances[4];
    double means[4][3];
    int minColors[3];
    int _max,_min,_dist,length,stride,labres,tresh = 10,rstride;
    int i,j,z = 0;
    uchar* ptr,* ptr_2;
    void differenceEdgeDetectionWithThresh(Mat grayFrame);
    double getEucledianDistanceByColors(int r1, int g1, int b1, int r2, int g2, int b2);
    void kuwaharaNagaoFilter(Mat colorFrame);
    double extractDataFromBinaryMarker(Mat possibleMarker, bool binaryCode[6][6]);
    long hashBinaryMatrix(bool binaryCode[6][6]);
    bool validateBinaryMatrix(bool binaryCode[6][6]);
    void init();
};

#endif /* defined(__CompTest__MarkerDetector__) */
