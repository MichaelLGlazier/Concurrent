CC = gcc
CFLAGS = -Wall
OBJS = prog1.o
MAIN = prog1.c
EXEC = prog1

${EXEC}: ${OBJS}
	${CC} ${CCFLAGS} -o ${EXEC} ${OBJS} -lm

main.o: ${MAIN}
	${CC} ${CCFLAGS} -c ${MAIN}

run: ${EXEC}
	./${EXEC}

clean: 
	rm -f ${EXEC} ${OBJS}
