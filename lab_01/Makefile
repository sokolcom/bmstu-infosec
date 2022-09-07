CC := gcc

MID := $(shell cat /etc/machine-id)
CFLAGS = -std=c99 -DMID=\"$(MID)\"

SRC_C = $(wildcard *.c)
OBJ_SRC = $(patsubst %.c, %.o, $(wildcard $(SRC_C)))

	

prog.out: $(OBJ_SRC)
	$(CC) -o $@ $^ -lcrypto -lsystemd


run: prog.out
	./$^

clean:
	$(RM) -rf *.out *.o


# get_mid:
# 	MID := @cat /etc/machine-id
# pkg-config --cflags --libs libsystemd