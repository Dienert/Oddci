/*
 * ExpInstance.h
 *
 *  Created on: Sep 7, 2012
 *      Author: root
 */

#ifndef EXPINSTANCE_H_
#define EXPINSTANCE_H_

class ExpInstance {
public:
    ExpInstance();
    virtual ~ExpInstance();
    char *getImageSize() const;
    char *getRequestedDevices() const;
    char *getRequestedSlots() const;
    char *getStartTime() const;
    void setImageSize(char *imageSize);
    void setRequestedDevices(char *requestedDevices);
    void setRequestedSlots(char *requestedSlots);
    void setStartTime(char *startTime);
    char * getId() const;
    void setId(char * id);

private:
    char * id;
    char *imageSize;
    char *requestedDevices;
    char *requestedSlots;
    char *startTime;

};

#endif /* EXPINSTANCE_H_ */
