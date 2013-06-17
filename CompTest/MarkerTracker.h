//
//  MarkerTracker.h
//  CompTest
//
//  Created by Árpád Kiss on 2012.12.07..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

#ifndef __CompTest__MarkerTracker__
#define __CompTest__MarkerTracker__

#include "cv.h"
#include <iostream>
#include "Marker.h"
#include "MarkerDetector.h"

using namespace cv;

class MarkerTracker
{
public:
    MarkerTracker(IplImage *firstFrame);
    vector<Marker> RecognizedMarkers() const;
    Mat OutImage() const;
    void ProcessFrame(IplImage *newFrame);
    void RenderImage();
    void DrawMarkerLines(Marker m);
private:
    vector<Marker> recognizedMarkers;
    Mat outImage;
    Mat currentFrame;
    Mat previousFrame;
    Mat flowImage;
    MarkerDetector* markerDetector;
    vector<Point2f> lastMarkerCornerPoints;
    vector<Point2f> recognizedMarkerCornerPoints;
    Mat trackSuccess;
    Mat trackError;
    void drawOptFlowMap(const cv::Mat& flow,
                                       cv::Mat& cflowmap,
                                       int step,
                                       const cv::Scalar& color
                        );
};

#endif /* defined(__CompTest__MarkerTracker__) */
