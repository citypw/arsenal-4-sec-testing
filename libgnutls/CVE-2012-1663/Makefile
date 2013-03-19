CC = gcc
CFLAGS = -g
LIBS= -lgnutls

APP = client ex-serv-x509

all:$(APP)
	@echo "done"

ex-serv-x509: ex-serv-x509.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)

client: client.c
	$(CC) $(CFLAGS) -o $@ $^ $(LIBS)
clean:
	rm -f $(APP)
