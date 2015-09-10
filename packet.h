#ifndef packet_h
#define packet_h

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>

#define DEBUG printf("%d\n", __LINE__)

struct Packet {
  int seq_no;
  bool valid;
};

const char * clin = "channel_l_in";
const char * clout = "channel_l_out";
const char * crin = "channel_r_in";
const char * crout = "channel_r_out";

#endif
