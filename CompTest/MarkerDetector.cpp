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

void MarkerDetector::init()
{
    std::cout << "Object <MarkerDetector> Created" << std::endl;
}

void MarkerDetector::differenceEdgeDetectionWithThresh(IplImage *frame, IplImage *frame_new)
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