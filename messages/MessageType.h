#ifndef MESSAGETYPE_H
#define MESSAGETYPE_H

enum Type {
    ODDCI_WAKEUP = 0,
    ODDCI_WORK = 1,
    ODDCI_DIE = 2,
    ODDCI_HEARTBEAT = 3,
    ODDCI_WAITING = 4,
    ODDCI_IDLE_PROBE = 5,
    ODDCI_KILL = 6
};

#endif
