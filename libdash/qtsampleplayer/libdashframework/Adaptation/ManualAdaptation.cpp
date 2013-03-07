/*
 * ManualAdaptation.cpp
 *****************************************************************************
 * Copyright (C) 2013, bitmovin Softwareentwicklung OG, All Rights Reserved
 *
 * Email: libdash-dev@vicky.bitmovin.net
 *
 * This source code and its use and distribution, is subject to the terms
 * and conditions of the applicable license agreement.
 *****************************************************************************/

#include "ManualAdaptation.h"

using namespace dash::mpd;
using namespace libdash::framework::adaptation;
using namespace libdash::framework::mpd;

ManualAdaptation::ManualAdaptation          (dash::mpd::IPeriod *period, dash::mpd::IAdaptationSet *adaptationSet, dash::mpd::IMPD *mpd) :
                  AbstractAdaptationLogic   (period, adaptationSet, mpd),
                  period                    (period),
                  adaptationSet             (adaptationSet),
                  mpd                       (mpd),
                  segmentNumber             (0),
                  invokeInitSegment         (false)
{
    this->representation = this->adaptationSet->GetRepresentation().at(0);
}
ManualAdaptation::~ManualAdaptation         ()
{
}

LogicType       ManualAdaptation::GetType               ()
{
    return adaptation::Manual;
}
MediaObject*    ManualAdaptation::GetSegment            ()
{
    ISegment *seg = NULL;

    if(this->segmentNumber >= RepresentationHelper::GetSize(this->representation) + 1) 
        return NULL;

    if(this->segmentNumber == 0 || this->invokeInitSegment)
    {
        seg = RepresentationHelper::GetInitSegment(this->period, this->adaptationSet, this->representation, this->mpd);
        this->invokeInitSegment = false;
    }   
    else
    {
        seg = RepresentationHelper::GetSegment(this->period, this->adaptationSet, this->representation, this->segmentNumber - 1, this->mpd);
    }
    
    if (seg != NULL)
    {
        MediaObject *media = new MediaObject(seg, this->representation);
        this->segmentNumber++;
        return media;
    }

    return NULL;
}
uint32_t        ManualAdaptation::GetPosition           ()
{
    return this->segmentNumber;
}
void            ManualAdaptation::SetPosition           (uint32_t segmentNumber)
{
    this->segmentNumber = segmentNumber;
}
void            ManualAdaptation::SetRepresentation     (dash::mpd::IRepresentation *representation)
{
    this->representation = representation;
}
void            ManualAdaptation::InvokeInitSegment     (bool invoke)
{
    this->invokeInitSegment = invoke;
}