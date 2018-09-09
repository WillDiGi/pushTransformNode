#include "pushTransform.h"

MTypeId PushTransformNode::id(0x00000234);

MObject PushTransformNode::inCenter;
MObject PushTransformNode::inRadius;
MObject PushTransformNode::inHierarchyOutput;
MObject PushTransformNode::inMatrix;
MObject PushTransformNode::outMatrix;

PushTransformNode::PushTransformNode(){}
PushTransformNode::~PushTransformNode(){}

void* PushTransformNode::creator(){ return new PushTransformNode(); }

void PushTransformNode::postConstructor(){
    this->setExistWithoutInConnections(true);
    this->setExistWithoutOutConnections(true);
}

MStatus PushTransformNode::initialize()
{
    MStatus status = MS::kSuccess;
    MFnNumericAttribute nAttr;
    MFnMatrixAttribute mAttr;

    outMatrix = mAttr.create("result", "result", MFnMatrixAttribute::kFloat);
    mAttr.setStorable(false); mAttr.setWritable(false);
    mAttr.setArray(true); mAttr.setUsesArrayDataBuilder(true);
    addAttribute(outMatrix);

    inCenter = mAttr.create("center", "center", MFnMatrixAttribute::kFloat);
    mAttr.setStorable(true); mAttr.setWritable(true);
    mAttr.setConnectable(true); mAttr.setKeyable(true);
    addAttribute(inCenter);
    attributeAffects(inCenter, outMatrix);

    inRadius = nAttr.create("radius", "radius", MFnNumericData::kFloat, 0.0f);
    nAttr.setStorable(true); nAttr.setWritable(true); nAttr.setReadable(true);
    nAttr.setKeyable(true); nAttr.setChannelBox(true); nAttr.setConnectable(true);
    addAttribute(inRadius);
    attributeAffects(inRadius, outMatrix);

    inHierarchyOutput = nAttr.create("hierarchyOutput", "hierarchyOutput", MFnNumericData::kBoolean, false);
    nAttr.setStorable(true); nAttr.setWritable(true); nAttr.setReadable(true);
    nAttr.setKeyable(true); nAttr.setChannelBox(true); nAttr.setConnectable(true);
    addAttribute(inHierarchyOutput);
    attributeAffects(inHierarchyOutput, outMatrix);

    inMatrix = mAttr.create("inMatrix", "inMatrix", MFnMatrixAttribute::kFloat);
    mAttr.setStorable(true); mAttr.setConnectable(true);
    mAttr.setKeyable(true); mAttr.setWritable(true);
    mAttr.setArray(true); mAttr.setUsesArrayDataBuilder(true);
    addAttribute(inMatrix);
    attributeAffects(inMatrix, outMatrix);

    return status;
}

MStatus PushTransformNode::compute(const MPlug& plug, MDataBlock& data)
{
    MStatus status = MS::kUnknownParameter;
    if ( plug == outMatrix ){
        status = MS::kSuccess;
        MArrayDataHandle hdlOut = data.outputValue(outMatrix);
        MArrayDataBuilder builderOut = hdlOut.builder();

        MArrayDataHandle hdlIn = data.inputValue(inMatrix);
        float radius = data.inputValue(inRadius).asFloat();
        bool hierarchyOutput = data.inputValue(inHierarchyOutput).asBool();
        MFloatMatrix centerMtx = data.inputValue(inCenter).asFloatMatrix();
        MFloatVector centerVec = MFloatVector(centerMtx[3]);

        unsigned int incomingCount = hdlIn.elementCount();
        while ( incomingCount > builderOut.elementCount() ){
            builderOut.addLast();
        }
        while ( incomingCount < builderOut.elementCount() ){
            builderOut.removeElement( builderOut.elementCount() - 1 );
        }
        hdlOut.set(builderOut);

        for(unsigned int i=0; i<incomingCount; i++){
            hdlIn.jumpToArrayElement(i);
            hdlOut.jumpToArrayElement(i);

            MFloatMatrix sourceMtx = hdlIn.inputValue().asFloatMatrix();
            MVector sourcePos = MFloatVector(sourceMtx[3]);

            MVector tmpVec = sourcePos - centerVec;
            double length = sqrt(pow(tmpVec.x, 2.0) + pow(tmpVec.y, 2.0) + pow(tmpVec.z, 2.0));

            MFloatMatrix result = sourceMtx;
            if ( radius > length ){
                tmpVec = tmpVec * (radius - length ) / length;

                double offset[4][4];
                offset[0][0] = 1.0f; offset[0][1] = 0.0f; offset[0][2] = 0.0f; offset[0][3] = 0.0f;
                offset[1][0] = 0.0f; offset[1][1] = 1.0f; offset[1][2] = 0.0f; offset[1][3] = 0.0f;
                offset[2][0] = 0.0f; offset[2][1] = 0.0f; offset[2][2] = 1.0f; offset[2][3] = 0.0f;
                offset[3][0] = tmpVec.x; offset[3][1] = tmpVec.y; offset[3][2] = tmpVec.z; offset[3][3] = 1.0f;

                MFloatMatrix offsetMtx(offset);
                result = offsetMtx * result;
            }
            if(hierarchyOutput){result = sourceMtx.inverse() * result;}
            hdlOut.outputValue().set(result);
        }

        hdlOut.setAllClean();
    }
    return status;
}


