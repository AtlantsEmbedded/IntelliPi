/* header file for clapack 3.2.1 */

#ifndef __CLAPACK_H
#define __CLAPACK_H

#ifdef __cplusplus 	
extern "C" {	
#endif		


integer ieeeck_(integer *ispec, real *zero, real *one);

integer ilaenv_(integer *ispec, char *name__, char *opts, integer *n1, 
	integer *n2, integer *n3, integer *n4);


logical sisnan_(real *sin__);

/* Subroutine */ int slae2_(real *a, real *b, real *c__, real *rt1, real *rt2);


/* Subroutine */ int slaebz_(integer *ijob, integer *nitmax, integer *n, 
	integer *mmax, integer *minp, integer *nbmin, real *abstol, real *
	reltol, real *pivmin, real *d__, real *e, real *e2, integer *nval, 
	real *ab, real *c__, integer *mout, integer *nab, real *work, integer 
	*iwork, integer *info);
	
	
/* Subroutine */ int slaev2_(real *a, real *b, real *c__, real *rt1, real *
	rt2, real *cs1, real *sn1);
	
logical slaisnan_(real *sin1, real *sin2);


integer slaneg_(integer *n, real *d__, real *lld, real *sigma, real *pivmin, 
	integer *r__);
	
doublereal slanst_(char *norm, integer *n, real *d__, real *e);


/* Subroutine */ int slar1v_(integer *n, integer *b1, integer *bn, real *
	lambda, real *d__, real *l, real *ld, real *lld, real *pivmin, real *
	gaptol, real *z__, logical *wantnc, integer *negcnt, real *ztz, real *
	mingma, integer *r__, integer *isuppz, real *nrminv, real *resid, 
	real *rqcorr, real *work);
	
	
/* Subroutine */ int slarnv_(integer *idist, integer *iseed, integer *n, real 
	*x);
	
	
/* Subroutine */ int slarra_(integer *n, real *d__, real *e, real *e2, real *
	spltol, real *tnrm, integer *nsplit, integer *isplit, integer *info);


/* Subroutine */ int slarrb_(integer *n, real *d__, real *lld, integer *
	ifirst, integer *ilast, real *rtol1, real *rtol2, integer *offset, 
	real *w, real *wgap, real *werr, real *work, integer *iwork, real *
	pivmin, real *spdiam, integer *twist, integer *info);

/* Subroutine */ int slarrc_(char *jobt, integer *n, real *vl, real *vu, real 
	*d__, real *e, real *pivmin, integer *eigcnt, integer *lcnt, integer *
	rcnt, integer *info);


/* Subroutine */ int slarrd_(char *range, char *order, integer *n, real *vl, 
	real *vu, integer *il, integer *iu, real *gers, real *reltol, real *
	d__, real *e, real *e2, real *pivmin, integer *nsplit, integer *
	isplit, integer *m, real *w, real *werr, real *wl, real *wu, integer *
	iblock, integer *indexw, real *work, integer *iwork, integer *info);

/* Subroutine */ int slarre_(char *range, integer *n, real *vl, real *vu, 
	integer *il, integer *iu, real *d__, real *e, real *e2, real *rtol1, 
	real *rtol2, real *spltol, integer *nsplit, integer *isplit, integer *
	m, real *w, real *werr, real *wgap, integer *iblock, integer *indexw, 
	real *gers, real *pivmin, real *work, integer *iwork, integer *info);


/* Subroutine */ int slarrf_(integer *n, real *d__, real *l, real *ld, 
	integer *clstrt, integer *clend, real *w, real *wgap, real *werr, 
	real *spdiam, real *clgapl, real *clgapr, real *pivmin, real *sigma, 
	real *dplus, real *lplus, real *work, integer *info);


/* Subroutine */ int slarrj_(integer *n, real *d__, real *e2, integer *ifirst, 
	 integer *ilast, real *rtol, integer *offset, real *w, real *werr, 
	real *work, integer *iwork, real *pivmin, real *spdiam, integer *info);


/* Subroutine */ int slarrk_(integer *n, integer *iw, real *gl, real *gu, 
	real *d__, real *e2, real *pivmin, real *reltol, real *w, real *werr, 
	integer *info);


/* Subroutine */ int slarrr_(integer *n, real *d__, real *e, integer *info);

/* Subroutine */ int slarrv_(integer *n, real *vl, real *vu, real *d__, real *
	l, real *pivmin, integer *isplit, integer *m, integer *dol, integer *
	dou, real *minrgp, real *rtol1, real *rtol2, real *w, real *werr, 
	real *wgap, integer *iblock, integer *indexw, real *gers, real *z__, 
	integer *ldz, integer *isuppz, real *work, integer *iwork, integer *
	info);

/* Subroutine */ int slaruv_(integer *iseed, integer *n, real *x);



/* Subroutine */ int slaset_(char *uplo, integer *m, integer *n, real *alpha, 
	real *beta, real *a, integer *lda);



/* Subroutine */ int slasq2_(integer *n, real *z__, integer *info);

/* Subroutine */ int slasq3_(integer *i0, integer *n0, real *z__, integer *pp, 
	 real *dmin__, real *sigma, real *desig, real *qmax, integer *nfail, 
	integer *iter, integer *ndiv, logical *ieee, integer *ttype, real *
	dmin1, real *dmin2, real *dn, real *dn1, real *dn2, real *g, real *
	tau);

/* Subroutine */ int slasq4_(integer *i0, integer *n0, real *z__, integer *pp, 
	 integer *n0in, real *dmin__, real *dmin1, real *dmin2, real *dn, 
	real *dn1, real *dn2, real *tau, integer *ttype, real *g);


/* Subroutine */ int slasq5_(integer *i0, integer *n0, real *z__, integer *pp, 
	 real *tau, real *dmin__, real *dmin1, real *dmin2, real *dn, real *
	dnm1, real *dnm2, logical *ieee);

/* Subroutine */ int slasq6_(integer *i0, integer *n0, real *z__, integer *pp, 
	 real *dmin__, real *dmin1, real *dmin2, real *dn, real *dnm1, real *
	dnm2);


/* Subroutine */ int slasrt_(char *id, integer *n, real *d__, integer *info);

/* Subroutine */ int slassq_(integer *n, real *x, integer *incx, real *scale, 
	real *sumsq);


/* Subroutine */ int sstegr_(char *jobz, char *range, integer *n, real *d__, 
	real *e, real *vl, real *vu, integer *il, integer *iu, real *abstol, 
	integer *m, real *w, real *z__, integer *ldz, integer *isuppz, real *
	work, integer *lwork, integer *iwork, integer *liwork, integer *info);



/* Subroutine */ int sstemr_(char *jobz, char *range, integer *n, real *d__, 
	real *e, real *vl, real *vu, integer *il, integer *iu, integer *m, 
	real *w, real *z__, integer *ldz, integer *nzc, integer *isuppz, 
	logical *tryrac, real *work, integer *lwork, integer *iwork, integer *
	liwork, integer *info);
	
	
/* Subroutine */ int xerbla_(char *srname, integer *info);

/* Subroutine */ int slasq4_(integer *i0, integer *n0, real *z__, integer *pp, 
	 integer *n0in, real *dmin__, real *dmin1, real *dmin2, real *dn, 
	real *dn1, real *dn2, real *tau, integer *ttype, real *g);
	
	
integer iparmq_(integer *ispec, char *name__, char *opts, integer *n, integer 
	*ilo, integer *ihi, integer *lwork);
	

#ifdef __cplusplus
}
#endif


#endif /* __CLAPACK_H */
