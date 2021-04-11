#ifndef RANDOM
  #define step_next (next = next * 1103515245L + 12345L)
  #define GetRandom(n) ( irand() % (unsigned)(n) )
  #define GetLRandom(n) ( lrand() % (unsigned long)(n) )
  #define Flip(prob)   ( irand() < ((double) 0x10000L)*(prob) )
  #define GetDRandom(x,y) (x + (y-x) * ((double) irand()) / (double) 0x10000L)

  double NormRand(void);   
  void TestRandom(void);
  void SetRandomSeed(unsigned int);
  unsigned int irand(void);
  unsigned long lrand(void);
  unsigned long lrandp(double p);
  unsigned int irandom (int num);
  void isrand(unsigned int seed);
  void irandomize (void);
  unsigned long lrandpq(double p, double q);

  int DoubleRoundToInt( double dblNum);
#define RANDOM
#endif

