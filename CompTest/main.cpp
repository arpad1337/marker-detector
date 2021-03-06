//
//  main.cpp
//  Interactive-Coursebook_v4
//
//  Created by Árpád Kiss on 2012.10.20..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

// CV bugfix
#define use_speed_ 0

#include <cv.h>
#include <highgui.h>
#include <features2d.hpp>
#include <objdetect.hpp>
#include <iostream>

//#include "MarkerDetector.h"
#include "MarkerTracker.h"
#include "Marker.h"

using namespace cv;

/*
int variances[4];
int _max,length,stride,labres,tresh = 10;
int i,z = 0;
uchar* ptr,* ptr_2;

void differenceEdgeDetectionWithThresh(IplImage *frame, IplImage *frame_new)
{
    z = 0;
    ptr = (uchar*)(frame->imageData);
    ptr_2 = (uchar*)(frame_new->imageData);
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
 
 */

int main(int argc, const char * argv[])
{
    CvCapture* capture = cvCaptureFromCAM( CV_CAP_ANY );
    if ( !capture ) {
        fprintf( stderr, "ERROR: capture is NULL \n" );
        getchar();
        return -1;
    }
    // Create a window in which the captured images will be presented
    cvNamedWindow( "CameraView", CV_WINDOW_AUTOSIZE );
    //cvNamedWindow( "burgonya", CV_WINDOW_AUTOSIZE );

    
    
    // Fukin blob staff
    
    /*
    vector<KeyPoint> keyPoints;
    vector< vector <Point> > contours;
    vector< vector <Point> > approxContours;
    
    SimpleBlobDetector::Params params;
    params.minThreshold = 40;
    params.maxThreshold = 60;
    params.thresholdStep = 5;
    
    params.minArea = 100;
    params.minConvexity = 0.3;
    params.minInertiaRatio = 0.01;
    
    params.maxArea = 8000;
    params.maxConvexity = 10;
    
    params.filterByColor = false;
    params.filterByCircularity = false;
    
    SimpleBlobDetector blobDetector( params );
    blobDetector.create("SimpleBlob");
     */
    
    MarkerTracker* markerTracker;
    
    //MarkerDetector* markerDetector;
    
    IplImage* frame = cvQueryFrame( capture );
    
    //MarkerDetector* markerDetector;
    
    if ( !frame ) {
        cvReleaseCapture( &capture );
        cvDestroyAllWindows();
        std::cout << "Camera is unavailable...\n";
        return 0;
    }
    else
    {
        /*markerDetector->setLength((frame->width - 1 )*(frame->height - 1));
        markerDetector->setStride(frame->width);
        markerDetector->setHeight(frame->height);*/
        markerTracker = new MarkerTracker(frame);
        //markerDetector = new MarkerDetector((frame->width - 1 )*(frame->height - 1),frame->width,frame->height);

    }
    
    //IplImage* frame_gray = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
        
    while ( 1 ) {
                
        IplImage* frame = cvQueryFrame( capture );
        
        markerTracker->ProcessFrame(frame);
        
        
        //std::cout << cvGetCaptureProperty(capture, CV_CAP_PROP_FPS) << "FPS" << std::endl;
    
        //memcpy(markersOnScene,correctedMarkers,sizeof(correctedMarkers));
    
        //
        
        //markerTracker->RenderImage(Mat(frame));
        
        //IplImage *frame_gray = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
        //IplImage *frame_binary = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
        
       /* Mat dst = Mat::zeros(frame->height, frame->width, CV_8UC3);
        Mat src = Mat::zeros(frame->height, frame->width, CV_8UC3);
        src = src < 255;*/
        
        //IplImage *frame_label = cvCreateImage(frameSize,IPL_DEPTH_8U,1);
        
        //Mat currentMat = new Mat(frame_gray);
        
       // markerDetector->setCurrentFrame(Mat(frame));
        
       // cvShowImage( "CameraViewGrayscale", frame_gray );
        
       // markerDetector->preprocessImage();
        
       // markerDetector->findPossibleMarkers();
        
        /*
        findContours( src, contours, hierarchy,CV_RETR_CCOMP, CV_CHAIN_APPROX_SIMPLE );
        
        vector<Point> approxCurve;
        
        int idx = 0;
        for( ; idx >= 0; idx = hierarchy[idx][0] )
        {
            if ( 100< contours[idx].size() && contours[idx].size()<700  )
            {
                approxPolyDP( contours[idx], approxCurve, double ( contours[idx].size() ) * 0.05 , true );

                if ( approxCurve.size() ==4 && isContourConvex ( Mat(approxCurve)) )
                {
                    Scalar color( rand()&255, rand()&255, rand()&255 );
                    drawContours( dst, contours, idx, color, CV_FILLED, 8, hierarchy );
                }
            }
        }
         
         */
        
        
        /*blobDetector.detect( frame_binary, keyPoints );
        
        std::cout << "Keypoints " << keyPoints.size() << std::endl;
        
        for (KeyPoint t : keyPoints)
        {
            std::cout << t.pt << std::endl;
        }*/
        //drawKeypoints( frame, keyPoints, frame_binary, Scalar::all(-1), DrawMatchesFlags::DEFAULT );
        
        
        //cvShowImage( "CameraView", frame );
        //cvShowImage( "CameraViewEdgeDetected",  );
        
        markerTracker->RenderImage();
        
        //cvShowImage( "CameraView", new IplImage(markerTracker->OutImage()) );
        
        imshow("CameraView",markerTracker->OutImage());
        
    //    imshow( "burgonya", markerTracker->FlowImage() );

        
        
       // imshow( "CameraViewEdgeDetected", markerDetector->PreprocessedFrame() );
       // imshow( "CameraViewSegments", markerDetector->SegmentsFrame() );
        /*if(markerDetector->currentMarker.rows > 0 && markerDetector->currentMarker.cols > 0)
        {
            imshow( "burgonya",markerDetector->currentMarker );
        }*/
        
        //cvReleaseImage(&frame_gray);

        //cvShowImage( "CameraView", frame_gray );
        
        // Do not release the frame!
        //If ESC key pressed, Key=0x10001B under OpenCV 0.9.7(linux version),
        //remove higher bits using AND operator
        
        if ( (cvWaitKey(10) & 255) == 27 ) break;
    }
    // Release the capture device housekeeping
    cvReleaseCapture( &capture );
    cvDestroyWindow( "CameraView" );
    std::cout << "Bye\n";
    return 0;
}



