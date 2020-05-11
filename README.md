# LSM-tree

Run ./server in the Server directory to start the server.

Run ./client followed by the command in the DSL (e.g., p 1 2 to put the key-value pair (1,2)) in the Client directory to use the client.

Parameters are in Server/Options.h. In order to change them you will have to recompile the server code with the command g++ -std=c++11 *.cc -o server within the Server directory. Then run ./server to start the server.

The data directory is set to /tmp/data/ by default and is set in Server/Options.h. If you want to restart the server, you will need to clear the data directory as well to ensure proper function.
