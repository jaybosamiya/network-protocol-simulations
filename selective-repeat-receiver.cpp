#include "packet.h"

int current;
int read_end, write_end;
int window = 4;

bool received[10000];

void send_ack(int packet_no) {
  Packet p;
  p.seq_no = packet_no;
  p.valid = true;

  write(write_end, &p, sizeof(p));

  printf("Sent acknowledgement %d\n", current);
}

int main() {
  write_end = open(crin, O_WRONLY);
  read_end = open(crout, O_RDONLY);

  current = 0;

  for (;;) {
    Packet p;

    while ( true ) {
      int ret = read(read_end, &p, sizeof(p));
      if ( ret == 0 ) {
        break;
      } else if ( ret < 0 ) {
        perror("read");
        exit(-1);
      }
      printf("Received packet %d. Valid = %d.\n", p.seq_no, p.valid);
      if ( p.valid && p.seq_no >= current && p.seq_no < current + window ) {
        printf("Received packet %d successfully.\n", p.seq_no);
        received[p.seq_no] = true;
        send_ack(p.seq_no);
        while ( received[current] ) {
          current += 1;
          printf("Slid window to %d.\n", current);
        }
      } else {
        send_ack(current);
      }
    }
  }
}
