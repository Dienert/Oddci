//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

cplusplus {{
#include "OddciMessage_m.h"
}};

packet OddciMessage;

packet WorkMsg extends OddciMessage
{
	long cycleId;
	simtime_t slotInterval;
	simtime_t firstHeartBeatInterval;
	simtime_t heartBeatInterval;
};

