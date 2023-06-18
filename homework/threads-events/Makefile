all: tcp_client tcp_server tcp_server_select file_client file_server

tcp_client: tcp_client.c
	clang -Wall -o tcp_client tcp_client.c

tcp_server: tcp_server.c
	clang -Wall -o tcp_server tcp_server.c

tcp_server_select: tcp_server_select.c
	clang -Wall -o tcp_server_select tcp_server_select.c

file_client: file_client.c
	clang -Wall -o file_client file_client.c

file_server: file_server.c
	clang -Wall -o file_server file_server.c
