all: receiver sender
receiver: receiver.cpp
	g++ receiver.cpp -o receiver
sender: sender.cpp
	g++ sender.cpp -o sender
clean:
	rm -f receiver sender
