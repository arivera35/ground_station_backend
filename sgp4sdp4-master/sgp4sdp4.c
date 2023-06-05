/*
 *  Unit SGP4SDP4
 *           Author:  Dr TS Kelso
 * Original Version:  1991 Oct 30
 * Current Revision:  1992 Sep 03
 *          Version:  1.50
 *        Copyright:  1991-1992, All Rights Reserved
 *
 *   Ported to C by:  Neoklis Kyriazis  April 10  2001
 */

#define SGP4SDP4_CONSTANTS
#include "sgp4sdp4.h"

/* SGP4 */
/* This function is used to calculate the position and velocity */
/* of near-earth (period < 225 minutes) satellites. tsince is   */
/* time since epoch in minutes, tle is a pointer to a tle_t     */
/* structure with Keplerian orbital elements and pos and vel    */
/* are vector_t structures returning ECI satellite position and */
/* velocity. Use Convert_Sat_State() to convert to km and km/s. */
  void
SGP4(double tsince, tle_t *tle, vector_t *pos, vector_t *vel)
{
  static double
	aodp,aycof,c1,c4,c5,cosio,d2,d3,d4,delmo,omgcof,
	eta,omgdot,sinio,xnodp,sinmo,t2cof,t3cof,t4cof,t5cof,
	x1mth2,x3thm1,x7thm1,xmcof,xmdot,xnodcf,xnodot,xlcof;

  double
	cosuk,sinuk,rfdotk,vx,vy,vz,ux,uy,uz,xmy,xmx,
	cosnok,sinnok,cosik,sinik,rdotk,xinck,xnodek,uk,
	rk,cos2u,sin2u,u,sinu,cosu,betal,rfdot,rdot,r,pl,
	elsq,esine,ecose,epw,cosepw,x1m5th,xhdot1,tfour,
	sinepw,capu,ayn,xlt,aynl,xll,axn,xn,beta,xl,e,a,
	tcube,delm,delomg,templ,tempe,tempa,xnode,tsq,xmp,
	omega,xnoddf,omgadf,xmdf,a1,a3ovk2,ao,betao,betao2,
	c1sq,c2,c3,coef,coef1,del1,delo,eeta,eosq,etasq,
	perige,pinvsq,psisq,qoms24,s4,temp,temp1,temp2,
	temp3,temp4,temp5,temp6,theta2,theta4,tsi;

  int i;

  /* Initialization */
  if (isFlagClear(SGP4_INITIALIZED_FLAG))
  {
	SetFlag(SGP4_INITIALIZED_FLAG);

	/* Recover original mean motion (xnodp) and   */
	/* semimajor axis (aodp) from input elements. */
	a1 = pow(xke/tle->xno,tothrd);
	cosio = cos(tle->xincl);
	theta2 = cosio*cosio;
	x3thm1 = 3*theta2-1.0;
	eosq = tle->eo*tle->eo;
	betao2 = 1-eosq;
	betao = sqrt(betao2);
	del1 = 1.5*ck2*x3thm1/(a1*a1*betao*betao2);
	ao = a1*(1-del1*(0.5*tothrd+del1*(1+134/81*del1)));
	delo = 1.5*ck2*x3thm1/(ao*ao*betao*betao2);
	xnodp = tle->xno/(1+delo);
	aodp = ao/(1-delo);

	/* For perigee less than 220 kilometers, the "simple" flag is set */
	/* and the equations are truncated to linear variation in sqrt a  */
	/* and quadratic variation in mean anomaly.  Also, the c3 term,   */
	/* the delta omega term, and the delta m term are dropped.        */
	if((aodp*(1-tle->eo)/ae) < (220/xkmper+ae))
	  SetFlag(SIMPLE_FLAG);
	else
	  ClearFlag(SIMPLE_FLAG);

	/* For perigee below 156 km, the       */
	/* values of s and qoms2t are altered. */
	s4 = s;
	qoms24 = qoms2t;
	perige = (aodp*(1-tle->eo)-ae)*xkmper;
	if(perige < 156)
	{
	  if(perige <= 98)
		s4 = 20;
	  else
		s4 = perige-78;
	  qoms24 = pow((120-s4)*ae/xkmper,4);
	  s4 = s4/xkmper+ae;
	}; /* End of if(perige <= 98) */

	pinvsq = 1/(aodp*aodp*betao2*betao2);
	tsi = 1/(aodp-s4);
	eta = aodp*tle->eo*tsi;
	etasq = eta*eta;
	eeta = tle->eo*eta;
	psisq = fabs(1-etasq);
	coef = qoms24*pow(tsi,4);
	coef1 = coef/pow(psisq,3.5);
	c2 = coef1*xnodp*(aodp*(1+1.5*etasq+eeta*(4+etasq))+
		0.75*ck2*tsi/psisq*x3thm1*(8+3*etasq*(8+etasq)));
	c1 = tle->bstar*c2;
	sinio = sin(tle->xincl);
	a3ovk2 = -xj3/ck2*pow(ae,3);
	c3 = coef*tsi*a3ovk2*xnodp*ae*sinio/tle->eo;
	x1mth2 = 1-theta2;
	c4 = 2*xnodp*coef1*aodp*betao2*(eta*(2+0.5*etasq)+
		tle->eo*(0.5+2*etasq)-2*ck2*tsi/(aodp*psisq)*
		(-3*x3thm1*(1-2*eeta+etasq*(1.5-0.5*eeta))+0.75*
		 x1mth2*(2*etasq-eeta*(1+etasq))*cos(2*tle->omegao)));
	c5 = 2*coef1*aodp*betao2*(1+2.75*(etasq+eeta)+eeta*etasq);
	theta4 = theta2*theta2;
	temp1 = 3*ck2*pinvsq*xnodp;
	temp2 = temp1*ck2*pinvsq;
	temp3 = 1.25*ck4*pinvsq*pinvsq*xnodp;
	xmdot = xnodp+0.5*temp1*betao*x3thm1+
	  0.0625*temp2*betao*(13-78*theta2+137*theta4);
	x1m5th = 1-5*theta2;
	omgdot = -0.5*temp1*x1m5th+0.0625*temp2*(7-114*theta2+
		395*theta4)+temp3*(3-36*theta2+49*theta4);
	xhdot1 = -temp1*cosio;
	xnodot = xhdot1+(0.5*temp2*(4-19*theta2)+
		2*temp3*(3-7*theta2))*cosio;
	omgcof = tle->bstar*c3*cos(tle->omegao);
	xmcof = -tothrd*coef*tle->bstar*ae/eeta;
	xnodcf = 3.5*betao2*xhdot1*c1;
	t2cof = 1.5*c1;
	xlcof = 0.125*a3ovk2*sinio*(3+5*cosio)/(1+cosio);
	aycof = 0.25*a3ovk2*sinio;
	delmo = pow(1+eta*cos(tle->xmo),3);
	sinmo = sin(tle->xmo);
	x7thm1 = 7*theta2-1;
	if (isFlagClear(SIMPLE_FLAG))
	{
	  c1sq = c1*c1;
	  d2 = 4*aodp*tsi*c1sq;
	  temp = d2*tsi*c1/3;
	  d3 = (17*aodp+s4)*temp;
	  d4 = 0.5*temp*aodp*tsi*(221*aodp+31*s4)*c1;
	  t3cof = d2+2*c1sq;
	  t4cof = 0.25*(3*d3+c1*(12*d2+10*c1sq));
	  t5cof = 0.2*(3*d4+12*c1*d3+6*d2*d2+15*c1sq*(2*d2+c1sq));
	}; /* End of if (isFlagClear(SIMPLE_FLAG)) */
  }; /* End of SGP4() initialization */

  /* Update for secular gravity and atmospheric drag. */
  xmdf = tle->xmo+xmdot*tsince;
  omgadf = tle->omegao+omgdot*tsince;
  xnoddf = tle->xnodeo+xnodot*tsince;
  omega = omgadf;
  xmp = xmdf;
  tsq = tsince*tsince;
  xnode = xnoddf+xnodcf*tsq;
  tempa = 1-c1*tsince;
  tempe = tle->bstar*c4*tsince;
  templ = t2cof*tsq;
  if (isFlagClear(SIMPLE_FLAG))
  {
	delomg = omgcof*tsince;
	delm = xmcof*(pow(1+eta*cos(xmdf),3)-delmo);
	temp = delomg+delm;
	xmp = xmdf+temp;
	omega = omgadf-temp;
	tcube = tsq*tsince;
	tfour = tsince*tcube;
	tempa = tempa-d2*tsq-d3*tcube-d4*tfour;
	tempe = tempe+tle->bstar*c5*(sin(xmp)-sinmo);
	templ = templ+t3cof*tcube+tfour*(t4cof+tsince*t5cof);
  }; /* End of if (isFlagClear(SIMPLE_FLAG)) */

  a = aodp*pow(tempa,2);
  e = tle->eo-tempe;
  xl = xmp+omega+xnode+xnodp*templ;
  beta = sqrt(1-e*e);
  xn = xke/pow(a,1.5);

  /* Long period periodics */
  axn = e*cos(omega);
  temp = 1/(a*beta*beta);
  xll = temp*xlcof*axn;
  aynl = temp*aycof;
  xlt = xl+xll;
  ayn = e*sin(omega)+aynl;

  /* Solve Kepler's' Equation */
  capu = FMod2p(xlt-xnode);
  temp2 = capu;

  i = 0;
  do
  {
	sinepw = sin(temp2);
	cosepw = cos(temp2);
	temp3 = axn*sinepw;
	temp4 = ayn*cosepw;
	temp5 = axn*cosepw;
	temp6 = ayn*sinepw;
	epw = (capu-temp4+temp3-temp2)/(1-temp5-temp6)+temp2;
	if(fabs(epw-temp2) <= e6a)
	  break;
	temp2 = epw;
  }
  while( i++ < 10 );

  /* Short period preliminary quantities */
  ecose = temp5+temp6;
  esine = temp3-temp4;
  elsq = axn*axn+ayn*ayn;
  temp = 1-elsq;
  pl = a*temp;
  r = a*(1-ecose);
  temp1 = 1/r;
  rdot = xke*sqrt(a)*esine*temp1;
  rfdot = xke*sqrt(pl)*temp1;
  temp2 = a*temp1;
  betal = sqrt(temp);
  temp3 = 1/(1+betal);
  cosu = temp2*(cosepw-axn+ayn*esine*temp3);
  sinu = temp2*(sinepw-ayn-axn*esine*temp3);
  u = AcTan(sinu, cosu);
  sin2u = 2*sinu*cosu;
  cos2u = 2*cosu*cosu-1;
  temp = 1/pl;
  temp1 = ck2*temp;
  temp2 = temp1*temp;

  /* Update for short periodics */
  rk = r*(1-1.5*temp2*betal*x3thm1)+0.5*temp1*x1mth2*cos2u;
  uk = u-0.25*temp2*x7thm1*sin2u;
  xnodek = xnode+1.5*temp2*cosio*sin2u;
  xinck = tle->xincl+1.5*temp2*cosio*sinio*cos2u;
  rdotk = rdot-xn*temp1*x1mth2*sin2u;
  rfdotk = rfdot+xn*temp1*(x1mth2*cos2u+1.5*x3thm1);

  /* Orientation vectors */
  sinuk = sin(uk);
  cosuk = cos(uk);
  sinik = sin(xinck);
  cosik = cos(xinck);
  sinnok = sin(xnodek);
  cosnok = cos(xnodek);
  xmx = -sinnok*cosik;
  xmy = cosnok*cosik;
  ux = xmx*sinuk+cosnok*cosuk;
  uy = xmy*sinuk+sinnok*cosuk;
  uz = sinik*sinuk;
  vx = xmx*cosuk-cosnok*sinuk;
  vy = xmy*cosuk-sinnok*sinuk;
  vz = sinik*cosuk;

  /* Position and velocity */
  pos->x = rk*ux;
  pos->y = rk*uy;
  pos->z = rk*uz;
  vel->x = rdotk*ux+rfdotk*vx;
  vel->y = rdotk*uy+rfdotk*vy;
  vel->z = rdotk*uz+rfdotk*vz;

} /*SGP4*/

/* Functions for testing and setting/clearing flags */

/* An int variable holding the single-bit flags */
static int Flags = 0;

  int
isFlagSet(int flag)
{
  return (Flags & flag);
}

  int
isFlagClear(int flag)
{
  return (~Flags & flag);
}

  void
SetFlag(int flag)
{
  Flags |= flag;
}

  void
ClearFlag(int flag)
{
  Flags &= ~flag;
}

/*------------------------------------------------------------------*/
