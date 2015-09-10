#include "packet.h"

int current;
bool outstanding;
int read_end, write_end;
int window = 4;
int total_packets;

void send_packet(int packet_no) {
  Packet p;
  p.seq_no = packet_no;
  p.valid = true;

  write(write_end, &p, sizeof(p));

  printf("Sent packet %d\n", packet_no);
}

void send_set() {
  for ( int i = 0 ; i < window ; i++ ) {
    if ( current + i < total_packets ) {
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
      if ( ack.seq_no > current && ack.valid == true ) {
        current = ack.seq_no;
        printf("Slid window to %d.\n", current);
      }
    }

    if ( current >= total_packets ) {
      break;
    }
  }
}
