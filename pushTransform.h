#pragma once

#include <maya/MPxNode.h>
#include <maya/MTypeId.h>
#include <maya/MStatus.h>

#include <maya/MFnNumericAttribute.h>
#include <maya/MFnNumericData.h>
#include <maya/MFnUnitAttribute.h>
#include <maya/MFnEnumAttribute.h>
#include <maya/MFnMatrixAttribute.h>

#include <maya/MPlug.h>
#include <maya/MDataBlock.h>
#include <maya/MDataHandle.h>
#include <maya/MArrayDataBuilder.h>
#include <maya/MArrayDataHandle.h>
#include <maya/MFloatMatrix.h>
#include <maya/MMatrix.h>
#include <maya/MTransformationMatrix.h>
#include <maya/MEulerRotation.h>
#include <maya/MFloatVector.h>
#include <maya/MVector.h>

#include <cmath>


class PushTransformNode: public MPxNode
{
public:
    PushTransformNode();
    ~PushTransformNode();

    static void* creator();
    static MStatus initialize();

    virtual void postConstructor();
    virtual MStatus compute(const MPlug&, MDataBlock&);
    virtual SchedulingType schedulingType()const{ return kParallel; }

    static MTypeId id;

    static MObject inCenter;
    static MObject inRadius;
    static MObject inHierarchyOutput;
    static MObject inMatrix;

    static MObject outMatrix;

};