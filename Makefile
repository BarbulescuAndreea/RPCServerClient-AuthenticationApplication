# Define variables
CC = gcc
CFLAGS = -Wall -g -I/usr/include/tirpc
LIBS = -lnsl -ltirpc

# Define source files
SERVER_SRC = src/server.c resourcemanager_svc.c resourcemanager_xdr.c src/database.c
CLIENT_SRC = src/client.c resourcemanager_clnt.c resourcemanager_xdr.c headers/allocator_mem.c

# Define generated files
GENERATED_FILES = resourcemanager.h resourcemanager_svc.c resourcemanager_clnt.c resourcemanager_client.c resourcemanager_server.c resourcemanager_xdr.c

# Define executable names
SERVER_EXEC = server
CLIENT_EXEC = client

# Define targets
all: server client

server: $(SERVER_SRC) headers/database.h headers/userTokenfunctions/find.h headers/userTokenFunctions/insertToken.h headers/allocator_mem.h
	$(CC) -o $(SERVER_EXEC) $(SERVER_SRC) $(LIBS) $(CFLAGS)

client: $(CLIENT_SRC) headers/database.h headers/userTokenfunctions/find.h headers/userTokenFunctions/insertToken.h headers/allocator_mem.h
	$(CC) -o $(CLIENT_EXEC) $(CLIENT_SRC) $(LIBS) $(CFLAGS)

# # Generate RPC code
# $(GENERATED_FILES): resourcemanager.x
# 	rpcgen -C resourcemanager.x


clean:
	rm -f $(CLIENT_EXEC) $(SERVER_EXEC) $(GENERATED_FILES)
