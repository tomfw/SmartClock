#include "CloudMsg.h"

void CloudMsg::begin() {
    Particle.variable("cmprint", msg, STRING);
}

void CloudMsg::cmprint(char *m) {
  uint16_t new_len = strlen(m);
  uint16_t cur_len = strlen(msg);

  if((cur_len + new_len  + 1) < MAX_STRING_LEN) {
    sprintf(msg, "%s%s", msg, m);
  } else {
    uint16_t bytes_to_delete =  new_len + 1;
    strncpy(msg, msg + bytes_to_delete, cur_len - bytes_to_delete);
    strncpy(msg + (cur_len - bytes_to_delete), m, new_len);
  }
}
