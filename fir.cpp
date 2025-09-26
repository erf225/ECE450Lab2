#define N 16

#include "ap_int.h"

//typedef int coef_t;
typedef ap_int<4> coef_t;
typedef ap_int<9> data_t;
//typedef int data_t;

//typedef int acc_t;

typedef ap_int<16> acc_t;

void fir(acc_t *y, data_t x) {

#pragma HLS INTERFACE axis port=x
#pragma HLS INTERFACE axis port=y
#pragma HLS INTERFACE ap_ctrl_none port=return
	coef_t c[N] = { 1, 2, 0, -3, 0, 4, -5, 0, 1, -2, 0, -3, 0, 4, -5, 0 };
#pragma HLS ARRAY_PARTITION variable=c complete dim=1


	static data_t shift_reg[N];
#pragma HLS ARRAY_PARTITION variable=shift_reg complete dim=1

	acc_t acc;
	int i;

	acc = 0;
	Shift_Accum_Loop: for (i = N - 1; i >= 0; i--) {
#pragma HLS UNROLL
		if (i == 0) {
			acc += x * c[0];
			shift_reg[0] = x;
		} else {
			shift_reg[i] = shift_reg[i - 1];
			acc += shift_reg[i] * c[i];
		}
	}
	*y = acc;
}
