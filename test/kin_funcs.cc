//#include "kin_funcs.h"
#include <math.h>


// This is from Byukling Kayanti Formula (6.3)
double Lambda( double x, double y, double z )
{
  return (x - y - z)*(x - y - z) - 4*y*z;
}

//From Byukling Kayanti Formula (5.14)
double T_min( double ma2, double mb2, double m1_2, double m2_2, double s)
{
  return ma2 + m1_2 - (1/(2*s))*( (s + ma2 - mb2)*(s + m1_2 - m2_2) - sqrt( Lambda(s, ma2, mb2)*Lambda(s, m1_2, m2_2) ) );
}

//From Byukling Kayanti Formula (5.14)
double T_max( double ma2, double mb2, double m1_2, double m2_2, double s)
{
  return ma2 + m1_2 - (1/(2*s))*( (s + ma2 - mb2)*(s + m1_2 - m2_2) + sqrt( Lambda(s, ma2, mb2)*Lambda(s, m1_2, m2_2) ) );
}

double Q2_min( double s, double Eb, double M )
{
  // M is the target mass;
  double me = 0.00051;
  double Eg = (s - M*M)/(2*M);
  double E_pr = Eb - Eg;
  double P0 = sqrt(Eb*Eb - me*me);
  double P_pr = sqrt(E_pr*E_pr - me*me);
  double Q2min = 2*(Eb*E_pr - P0*P_pr - me*me);

  return Q2min;
}
