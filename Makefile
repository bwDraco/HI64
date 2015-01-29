all: int16 int32 int64 float double longdouble

# Variables
ADVANCE     = 1.2589
NSAMP       = 5000
NTRIAL      = 5
PATIENCE    = 10
RUNTM       = 0.5
STOPRT      = 0.15
STOPTM      = 60

PARAMETERS  = -DADVANCE=$(ADVANCE) -DNSAMP=$(NSAMP) -DNTRIAL=$(NTRIAL) -DPATIENCE=$(PATIENCE) -DRUNTM=$(RUNTM) -DSTOPRT=$(STOPRT) -DSTOPTM=$(STOPTM)

int16:
	gcc -O1 -funroll-loops -march=native -DDSIZE=int16_t -DISIZE=uint64_t $(PARAMETERS) -o hi64-int16 hi64.c hkernel.c
int32:
	gcc -O1 -funroll-loops -march=native -DDSIZE=int32_t -DISIZE=uint64_t $(PARAMETERS) -o hi64-int32 hi64.c hkernel.c
int64:
	gcc -O1 -funroll-loops -march=native -DDSIZE=int64_t -DISIZE=uint64_t $(PARAMETERS) -o hi64-int64 hi64.c hkernel.c
float:
	gcc -O1 -funroll-loops -march=native -DDSIZE=float -DISIZE=uint64_t $(PARAMETERS) -o hi64-float hi64.c hkernel.c
double:
	gcc -O1 -funroll-loops -march=native -DDSIZE=double -DISIZE=uint64_t $(PARAMETERS) -o hi64-double hi64.c hkernel.c
longdouble:
	gcc -O1 -funroll-loops -march=native -DDSIZE="long double" -DISIZE=uint64_t $(PARAMETERS) -o hi64-longdouble hi64.c hkernel.c