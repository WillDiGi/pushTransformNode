#include <maya/MFnPlugin.h>
#include <maya/MString.h>
#include "pushTransform.h"


MStatus initializePlugin(MObject obj) {
    MStatus status;

    MFnPlugin plugin(obj, "WEi HE", "1.0", "Any");

    status = plugin.registerNode("AL_rig_push_transform",
                                 PushTransformNode::id,
                                 PushTransformNode::creator,
                                 PushTransformNode::initialize);

    CHECK_MSTATUS_AND_RETURN_IT( status );

    return MS::kSuccess;
}

MStatus uninitializePlugin( MObject obj )
{
    MStatus status;

    MFnPlugin plugin( obj );

    status = plugin.deregisterNode( PushTransformNode::id );
    CHECK_MSTATUS_AND_RETURN_IT( status );

    return MS::kSuccess;
}