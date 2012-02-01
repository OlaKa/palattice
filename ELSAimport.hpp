#ifndef __POLSIM__ELSAIMPORT_HPP_
#define __POLSIM__ELSAIMPORT_HPP_


int ELSAimport(BPM *ELSAbpms, CORR *ELSAvcorrs, magnetvec &quads, magnetvec &sexts, char *spurenFolder);

int ELSAimport_magnetstrengths(magnetvec &quads, magnetvec &sexts, char *spurenFolder);

int ELSAimport_bpms(BPM *ELSAbpms, char *spurenFolder);

int ELSAimport_vcorrs(CORR *ELSAvcorrs, char *spurenFolder);

int ELSAimport_getbpmorbit(BPM *ELSAbpms, orbitvec &bpmorbit, double t);

int ELSAimport_getvcorrs(CORR *ELSAvcorrs, magnetvec &vcorrs, double t);

int bpms_out(BPM *ELSAbpms, double t, char *filename);

int corrs_out(CORR *ELSAvcorrs, double t, char *filename);

#endif

/*__POLSIM__ELSAIMPORT_HPP_*/
