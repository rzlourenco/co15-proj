local %f;
#pwn() = 0 {
  *p = noob;
  *q = noob;
  p = [3];
  q = [3];
  p[1] = 3;
  q[2] = 6;
  f = (q+1)[1] - p[1];
  f!
}

