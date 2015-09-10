#include "packet.h"

int current;
bool outstanding;
int read_end, write_end;

void send_packet(int packet_no) {
  Packet p;
  p.seq_no = packet_no;
  p.valid = true;

  write(write_end, &p, sizeof(p));

  printf("Sent packet %d\n", current);
}

static void sighandler(int sig) {
  if ( outstanding ) {
    printf("Time-out. Resending packet.\n");
    send_packet(current);
    alarm(1);
  }
}

int main() {
  int total_packets;
  printf("Totally, how many packets to send? ");
  scanf("%d", &total_packets);

  write_end = open(clin, O_WRONLY);
  read_end = open(clout, O_RDONLY);

  signal(SIGALRM, sighandler);

  current = 0;

  for ( int i = 0 ; i < total_packets ; i++ ) {
    alarm(1);
    Packet ack;

    send_packet(current);
    outstanding = true;

    while ( read(read_end, &ack, sizeof(ack)) ) {
      if ( ack.seq_no == current+1 && ack.valid == true ) {
        outstanding = false;
        break;
      }
    }

    current += 1;
  }
}
