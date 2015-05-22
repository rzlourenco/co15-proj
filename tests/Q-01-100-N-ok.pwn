*main(*p) { main = 1 + p; }
#pwn() = 0 {
  *p = noob;
  *q = noob;
  p = [3];
  p[0] = 3.1;
  p[1] = 2.1;
  p[2] = 6.1E1;
  q = main(p);
  q[1]!
}

