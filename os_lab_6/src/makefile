files: server client

server: Server.cpp
	g++ -fsanitize=address Server.cpp -lzmq -o server -w

client: Client.cpp
	g++ -fsanitize=address Client.cpp -lzmq -o client -w

clean:
	rm  -rf server  client
