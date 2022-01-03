CFLAGS=-O3 -Wall -g
distinctive-double-:main.o branch_and_bound.o EnumerateRelocation.o IntDequeue.o lower_bound.o support_function.o
		gcc main.o branch_and_bound.o EnumerateRelocation.o IntDequeue.o lower_bound.o support_function.o -o distinctive-double-
branch_and_bound.o:IntDequeue.h lower_bound.h branch_and_bound.h data_info.h
main.o:IntDequeue.h lower_bound.h branch_and_bound.h data_info.h
IntDequeue.o:IntDequeue.h data_info.h lower_bound.h branch_and_bound.h
lower_bound.o:IntDequeue.h lower_bound.h data_info.h branch_and_bound.h
support_function.o:IntDequeue.h lower_bound.h branch_and_bound.h data_info.h
EnumerateRelocation.o:IntDequeue.h lower_bound.h branch_and_bound.h data_info.h