CC := g++ -std=c++17

all: clean
	$(CC) -I . MainProcessor.cpp -o MainProcess.out
	$(CC) -I . MapProcessor.cpp -o MapProcess
	$(CC) -I . ReduceProcessor.cpp -o ReduceProcess

clean:
	rm -f line_* main *.out mian-process
