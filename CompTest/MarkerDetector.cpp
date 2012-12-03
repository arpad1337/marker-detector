//
//  MarkerDetector.cpp
//  CompTest
//
//  Created by Árpád Kiss on 2012.10.24..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

#include "MarkerDetector.h"

MarkerDetector::MarkerDetector()
{
    MarkerDetector::init();
}

MarkerDetector::MarkerDetector(int len, int str, int hei)
{
    length = len;
    stride = str;
    height = hei;
    MarkerDetector::init();
}

void MarkerDetector::preprocessImage()
{
    preprocessedFrame.release();
    preprocessedFrame = Mat::zeros(height, stride, CV_8UC1);
    cvtColor(currentFrame,currentFrameGray,CV_RGB2GRAY);
    differenceEdgeDetectionWithThresh(currentFrameGray);
}

void MarkerDetector::differenceEdgeDetectionWithThresh(Mat from)
{
    z = 0;
    ptr = (uchar*)(from.data);
    ptr_2 = (uchar*)(preprocessedFrame.data);
    i = stride + 1;
    for(; i < length; i++)
    {
        _max = 0;
        variances[0] = abs(ptr[i-1 -stride] - ptr[i+1 + stride]);
        variances[1] = abs(ptr[i - stride] - ptr[i + stride]);
        variances[2] = abs(ptr[i+1 - stride] - ptr[i - 1 + stride]);
        variances[3] = abs(ptr[i - 1] - ptr[i + 1]);
        
        for(z = 0; z<4; z++)
        {
            if(variances[z] > variances[_max])
            {
                _max = z;
            }
        }
        ptr_2[i] = (variances[_max] > tresh)?255:0;
    }
}

void MarkerDetector::findPossibleMarkers()
{
    segmentsFrame.release();
    segmentsFrame = Mat::zeros(height, stride, CV_8UC3);
    
    findContours( preprocessedFrame, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
    
    vector<Point> approxCurve;
    
    for(int idx = 0; idx >= 0; idx = hierarchy[idx][0] )
    {
    //    Scalar color( rand()&255, rand()&255, rand()&255 );
        
      //  drawContours( segmentsFrame, contours, idx, color, CV_FILLED, 8, hierarchy );
        
        if ( 80< contours[idx].size() && contours[idx].size()<1000  )
        {
            approxPolyDP( contours[idx], approxCurve, double ( contours[idx].size() ) * 0.2 , true );
            
            if ( approxCurve.size() ==4 && isContourConvex ( Mat(approxCurve)) )
            {
                Point2f p[4];
                Point2f q[4];
                
                p[0].x= approxCurve[0].x;
                p[0].y= approxCurve[0].y;
                p[1].x= approxCurve[1].x;
                p[1].y= approxCurve[1].y;
                
                p[2].x= approxCurve[2].x;
                p[2].y= approxCurve[2].y;
                p[3].x= approxCurve[3].x;
                p[3].y= approxCurve[3].y;
                
                // távolságok vizsgálata
                
                if( (abs(approxCurve[0].x-approxCurve[1].x) + abs(approxCurve[0].y-approxCurve[1].y)) > 50 &&
                    (abs(approxCurve[1].x-approxCurve[2].x) + abs(approxCurve[1].y-approxCurve[2].y)) > 50 &&
                    (abs(approxCurve[2].x-approxCurve[3].x) + abs(approxCurve[2].y-approxCurve[3].y)) > 50 &&
                    (abs(approxCurve[3].x-approxCurve[4].x) + abs(approxCurve[3].y-approxCurve[4].y)) > 50
                    )
                {
                
                q[0].x= (float) 0;
                q[0].y= (float) 0;
                q[1].x= (float)300;//(float) stride-1;
                q[1].y= (float) 0;
                
                q[2].x= (float)300;//(float) stride-1;
                q[2].y= (float)300;//(float) height-1;
                q[3].x= (float) 0;
                q[3].y= (float)300;//(float) height-1;
                
                //Set of destination points to calculate Perspective matrix
                p[0].x= approxCurve[0].x;
                p[0].y= approxCurve[0].y;
                p[1].x= approxCurve[1].x;
                p[1].y= approxCurve[1].y;
                
                p[2].x= approxCurve[2].x;
                p[2].y= approxCurve[2].y;
                p[3].x= approxCurve[3].x;
                p[3].y= approxCurve[3].y;
                
                //Calculate Perspective matrix
                warpMatrix = getPerspectiveTransform(p,q);
                
                //Rect boundRect = boundingRect(approxCurve);
                
                //Mat cuttedImage = Mat(currentFrame,boundRect);
                
                currentMarker = Mat();
                Mat temp = Mat();
                
                warpPerspective(currentFrameGray, temp, warpMatrix, Size(300,300),INTER_NEAREST);
                    
                threshold(temp, currentMarker, 0, 255, THRESH_OTSU);
                    
                    
                    //currentMarker = temp;
                
                //currentMarker = cuttedImage;
                
                line( currentFrame, p[0], p[1], CV_RGB(255,0,0),2);
                line( currentFrame, p[1], p[2], CV_RGB(0,255,0),2);
                line( currentFrame, p[2], p[3], CV_RGB(0,0,255),2);
                line( currentFrame, p[3], p[0], CV_RGB(255,255,0),2);
                
                }
                
              //  drawContours( currentFrame, contours, idx, color, CV_FILLED, 8, hierarchy );
            }
        }
    }
}



/*IplImage* MarkerDetector::CurrentFrame()
{
    return new IplImage(currentFrame);
}*/

Mat MarkerDetector::CurrentFrame() const
{
    return currentFrame;
}

Mat MarkerDetector::CurrentFrameGray() const
{
    return currentFrameGray;
}

Mat MarkerDetector::PreprocessedFrame() const
{
    return preprocessedFrame;
}
Mat MarkerDetector::SegmentsFrame() const
{
    return segmentsFrame;
}

void MarkerDetector::setCurrentFrame(Mat value)
{
    currentFrame.release();
    currentFrame = Mat(value);
}

//void MarkerDetector::set

void MarkerDetector::init()
{
    Mat warpMatrix = Mat(3,3,CV_32FC1);
    std::cout << "Object <MarkerDetector> Created" << std::endl;
}

int MarkerDetector::Length() const
{
    return length;
}

int MarkerDetector::Stride() const
{
    return stride;
}

void MarkerDetector::setLength(const int value)
{
    length = value;
}

void MarkerDetector::setStride(const int value)
{
    stride = value;
}

int MarkerDetector::Height() const
{
    return height;
}

void MarkerDetector::setHeight(const int value)
{
    height = value;
}
