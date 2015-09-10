#include "packet.h"

int main() {
  int drop_prob;

  printf("Drop Probability? ");
  scanf("%d", &drop_prob);

  int in, out;
  char *type;
  mkfifo(clin, 0777);
  mkfifo(crout, 0777);
  mkfifo(clout, 0777);
  mkfifo(crin, 0777);

  if ( fork() ) {
    in = open(clin, O_RDONLY);
    out = open(crout, O_WRONLY);
    type = "data";
  } else {
    in = open(crin, O_RDONLY);
    out = open(clout, O_WRONLY);
    type = "ACK";
  }

  Packet p;

  while ( true ) {
    int ret = read(in, &p, sizeof(p));
    if ( ret == 0 ) {
      break;
    } else if ( ret < 0 ) {
      perror(type);
      exit(-1);
    }

    printf("Received %s packet. Seq no = %d.\n", type, p.seq_no);

    if ( random() % 100 < drop_prob ) {
      p.valid = false;
      printf("Packet %d was corrupted.\n", p.seq_no);
    }

    write(out, &p, sizeof(p));

    printf("Transmitted %s packet. Seq no = %d.\n", type, p.seq_no);
  }
}

