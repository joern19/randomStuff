#include "RCSwitch.h"
#include <iostream>
#include <unistd.h>

void on(RCSwitch s, int group, int device) {
  std::cout << "Send on to " << group << "/" << device << std::endl;
  s.switchOn('a', group, device);
}

void off(RCSwitch s, int group, int device) {
  std::cout << "Send off to " << group << "/" << device << std::endl;
  s.switchOff('a', group, device);
}

int main(int argc, char* argv[]) {
  if (wiringPiSetup() != 0) {
    std::cerr << "Setup of wiringPi failed!";
    return 1;
  }
  RCSwitch s = RCSwitch();
  s.enableTransmit(0);

  // If you want a memory save C/C++ program, write it in rust.
  int group = atoi(argv[1]);
  int device = atoi(argv[2]);
  if (strcmp(argv[3], "on") == 0) {
    on(s, group, device);
  } else {
    off(s, group, device);
  }
  return 0;
}

