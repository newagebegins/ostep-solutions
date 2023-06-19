all: tcp_client tcp_server tcp_server_select file_client file_server file_server_aio file_server_aio_signal

tcp_client: tcp_client.c
	clang -Wall -Werror -o tcp_client tcp_client.c

tcp_server: tcp_server.c
	clang -Wall -Werror -o tcp_server tcp_server.c

tcp_server_select: tcp_server_select.c
	clang -Wall -Werror -o tcp_server_select tcp_server_select.c

file_client: file_client.c
	clang -Wall -Werror -o file_client file_client.c

file_server: file_server.c
	clang -Wall -Werror -o file_server file_server.c

file_server_aio: file_server_aio.c
	clang -Wall -Werror -o file_server_aio file_server_aio.c

file_server_aio_signal: file_server_aio_signal.c
	clang -Wall -Werror -o file_server_aio_signal file_server_aio_signal.c
