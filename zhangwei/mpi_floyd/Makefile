## Author: Wei Zhang
## Author: Ali Nosrati

CC=mpicc
CFLAGS=-i_dynamic
OBJ=mpi_floyd.out
UTIL=util.c 
SRC=main.c floyd.c
EXEC=mpirun

OS=$(shell uname)
ifeq ($(OS),Darwin)
		UTIL=util_osx.c
else
		UTIL=util.c
endif

${OBJ}: ${SRC} ${UTIL} 
	${CC} -o $@ ${SRC} ${UTIL}


.PHONY: run
run:
		@${EXEC} -np ${NUM} -output-filename output/$(subst .out,,${OBJ}) ${OBJ} ${ARGS}

.PHONY: clean
clean:
	-rm -rf ${OBJ}
	-rm -rf *.btr
	-rm -rf output/

.PHONY: all
all:	clean ${OBJ} run
