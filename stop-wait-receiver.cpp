#include "packet.h"

int current;
int read_end, write_end;

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
      if ( p.seq_no == current && p.valid == true ) {
        printf("Received packet %d successfully.\n", p.seq_no);
        current += 1;
        send_ack(current);
        break;
      } else {
        printf("Received packet %d. Valid = %d.\n", p.seq_no, p.valid);
        send_ack(current);
      }
    }
  }
}
