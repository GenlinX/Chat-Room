build: server client vmclient

client: client.c head4chat.h
	arm-linux-gcc client.c threadpool.c font.c truetype.c -o client -pthread -lm

server: server.c head4chat.h
	gcc server.c threadpool.c -o server -pthread -g
vmclient: vmclient.c head4chat.h
	gcc vmclient.c threadpool.c -o vmclient -pthread -g

clean:
	rm -rf client
	rm -rf server
	rm -rf vmclient

