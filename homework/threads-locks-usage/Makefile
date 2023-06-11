all: counter_precise counter_approx list_st list_mt_nolocks list_mt_coarse list_mt_hoh

counter_precise: counter_precise.c
	clang -Wall -o counter_precise counter_precise.c

counter_approx: counter_approx.c
	clang -Wall -o counter_approx counter_approx.c

list_st: list_st.c
	clang -Wall -o list_st list_st.c

list_mt_nolocks: list_mt_nolocks.c
	clang -Wall -o list_mt_nolocks list_mt_nolocks.c

list_mt_coarse: list_mt_coarse.c
	clang -Wall -o list_mt_coarse list_mt_coarse.c

list_mt_hoh: list_mt_hoh.c
	clang -Wall -o list_mt_hoh list_mt_hoh.c
