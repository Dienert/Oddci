package oddci.neds;

//
// This file is part of an OMNeT++/OMNEST simulation example.
//
// Copyright (C) 1992-2008 Andras Varga
//
// This file is distributed WITHOUT ANY WARRANTY. See the file
// `license' for details on this and other legal matters.
//

//
// Represents a "network cloud" between a server and its numerous clients.
// Delay modeling is very simple: constant delay.
//
simple Router
{
    parameters:
        double propDelay @unit(s) = 5ms;
        int id = default(0);
        @display("i=abstract/router");
        bool logActived = default(false);
    gates:
        inout g[];
}

