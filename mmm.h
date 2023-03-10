#ifndef MMM_H_
#define MMM_H_

// globals (anything here would be shared with all threads) */
// I would declare the pointers to the matrices here (i.e., extern double **A, **B, **C),
// as well as the size of the matrices, etc.

void mmm_init(int N);
void mmm_reset();
void mmm_freeup(int N);
void mmm_seq();
void *mmm_par(int args[]);
double mmm_verify();

#endif /* MMM_H_ */
