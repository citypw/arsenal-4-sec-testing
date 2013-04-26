CC = gcc
CFLAGS = -g
LIBS= -lcrypto

APP = padding_oracle_attack_poc

all:$(APP)
	@echo "done"

padding_oracle_attack_poc: padding_oracle_attack_poc.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

clean:
	rm -f $(APP)
