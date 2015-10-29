## Author: Wei Zhang
## Author: Ali Nosrati

CC=mpicc
CFLAGS=-i_dynamic
OBJ=mpi_floyd.out
UTIL=timeutil.c
SRC=main.c floyd.c util.c
EXEC=mpirun

OS=$(shell uname)
ifeq ($(OS),Darwin)
		UTIL=timeutil_osx.c
else
		UTIL=timeutil.c
endif

${OBJ}: ${SRC} ${UTIL} 
	${CC} -o $@ ${SRC} ${UTIL}


.PHONY: run
run:
		@${EXEC} -np ${NUM} -output-filename output/$(subst .out,,${OBJ}) ${OBJ} ${ARGS}

.PHONY: clean
clean:
	-rm -rf ${OBJ}
	-rm -rf *.out
	-rm -rf *.btr
	-rm -rf output/

.PHONY: all
all:	clean ${OBJ} run
