
midi: LDFLAGS += -L./lib -lmidifile

midi:
	g++ -I./include -c -o main.o midiParser.cpp
	g++ -o midiParser main.o $(LDFLAGS)
	rm main.o

csound: LDFLAGS += -L./lib -lcsound64 -lcsnd6

csound:
	g++ -I./include -c -o main.o csoundPlayer.cpp
	g++ -o csoundPlayer main.o $(LDFLAGS)
	rm main.o