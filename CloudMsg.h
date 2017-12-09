/* Simple class to publish short messages to the cloud */

#ifndef __CLOUD_MSG_H__
#define __CLOUD_MSG_H__

#include "Particle.h"
#include "application.h"

#define MAX_STRING_LEN 255

class CloudMsg {
private:
  char msg[400];
public:
  CloudMsg() {}
  void begin();
  void cmprint(char *m);
};

#endif
