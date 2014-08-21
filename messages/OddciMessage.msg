//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

cplusplus {{
#include "NetPkt_m.h"
#include "MessageType.h"
}};

enum Type;
packet NetPkt;
packet OddciMessage extends NetPkt
{
    int messageKind @enum(Type);    
    int instanceId;
};


