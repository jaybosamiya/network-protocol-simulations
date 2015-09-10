#include "packet.h"

int current;
bool outstanding;
int read_end, write_end;
int window = 4;
int total_packets;

bool acknowledged[10000];

void send_packet(int packet_no) {
  Packet p;
  p.seq_no = packet_no;
  p.valid = true;

  write(write_end, &p, sizeof(p));

  printf("Sent packet %d\n", packet_no);
}

void send_set() {
  for ( int i = 0 ; i < window ; i++ ) {
    if ( !acknowledged[current + i] ) {
      send_packet(current + i);
      outstanding = true;
    }
  }
}

static void sighandler(int sig) {
  if ( outstanding && current < total_packets ) {
    printf("Time-out. Resending set.\n");
    send_set();
    alarm(1);
  }
  if ( current >= total_packets ) {
    exit(0);
  }
}

int main() {
  printf("Totally, how many packets to send? ");
  scanf("%d", &total_packets);

  write_end = open(clin, O_WRONLY);
  read_end = open(clout, O_RDONLY);

  signal(SIGALRM, sighandler);

  current = 0;

  for ( ;; ) {
    alarm(1);
    Packet ack;

    send_set();

    while ( read(read_end, &ack, sizeof(ack)) ) {
      if ( (ack.seq_no >= current + window || ack.seq_no >= total_packets) && ack.valid == true ) {
        outstanding = false;
      }
      if ( ack.seq_no >= current && ack.valid == true ) {
        printf("Received ack for %d.\n", ack.seq_no);
        acknowledged[ack.seq_no] = true;

        while ( acknowledged[current] ) {
          current += 1;
          printf("Slid window to %d.\n", current);
        }
      }
    }

    if ( current >= total_packets ) {
      break;
    }
  }
}
