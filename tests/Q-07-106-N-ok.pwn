%main(*p) { main = p[0]; }
#pwn() = 0 {
  *p = noob;
  p = [3];
  p[0] = 3.1;
  p[1] = 2.1;
  p[2] = 6.1E1;
  main(p[2]?)!
}
