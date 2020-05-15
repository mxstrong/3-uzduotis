CC = clang++
CFLAGS = -O0
Main:
	$(CC) -std=c++17 -o Vector $(CFLAGS) Vector.cpp
clean:
	rm *.o Vector
