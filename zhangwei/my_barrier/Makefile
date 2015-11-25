#Wei Zhang, Ali Nosrati

CC=/lustre/work/newapps/mpich-3.1.4/bin/mpicc
CFLAGS=-i_dynamic
OBJ=my_barrier.out
UTIL=util.c 
SRC=my_barrier.c 
EXEC=/lustre/work/newapps/mpich-3.1.4/bin/mpiexec

${OBJ}: ${SRC} ${UTIL} 
	${CC} ${CFLAGS} -o $@ ${SRC} ${UTIL}


.PHONY: run
run:
	@${EXEC} -n ${NUM}  ./${OBJ} ${ARGS}

.PHONY: clean
clean:
	-rm -rf ${OBJ}
	-rm -rf *.btr
	-rm -rf output/

.PHONY: all
all:	clean ${OBJ} run

.PHONY: viewrst
viewrst:	all
	-more output/*	
