//
//  Marker.cpp
//  CompTest
//
//  Created by Árpád Kiss on 2012.12.05..
//  Copyright (c) 2012 Uni-Obuda. All rights reserved.
//

#include "Marker.h"

Marker::Marker(Point2f const newCornerPoints[4],long newHash,bool const newBinaryCode[6][6])
{
    hash = newHash;
    memcpy(cornerPoints, newCornerPoints, sizeof(cornerPoints));
    memcpy(binaryCode, newBinaryCode, sizeof(binaryCode));
}

Mat Marker::RotationMatrix() const
{
    return rotationMatrix;
}

Mat Marker::TranslationMatrix() const
{
    return translationMatrix;
}

void Marker::setRotationMatrix(Mat const newRotationMatrix)
{
    rotationMatrix = newRotationMatrix;
}

void Marker::setTranslationMatrix(Mat const newTranslationMaxtrix)
{
    translationMatrix = newTranslationMaxtrix;
}
void Marker::setCornerPointAt(int idx, Point2f const newPoint)
{
    cornerPoints[idx] = newPoint;
}

Point2f Marker::CornerPointsAt(int idx) const
{
    return cornerPoints[idx];
}

long Marker::Hash() const
{
    return hash;
}

int Marker::Age() const
{
    return age;
}

void Marker::setAge(int const newAge)
{
    age = newAge;
}