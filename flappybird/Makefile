TARGET=a.out
CXX=g++ #Define o compilador
DEBUG=-g
OPT=-O0
WARN=-Wall
SFML=-lsfml-graphics -lsfml-window -lsfml-system
CXXFLAGS=$(DEBUG) $(OPT) $(WARN) $(SFML) #Define as flags de compilacao
LD=g++
OBJS= main.o flappy.o #Nome dos executaveis
all: $(OBJS)
	$(LD) -o $(TARGET) $(OBJS) $(CXXFLAGS)
	@rm *.o
	@./$(TARGET)

main.o: main.cpp
	$(CXX) -c $(CXXFLAGS) main.cpp -o main.o

flappy.o: flappy.cpp
	$(CXX) -c $(CXXFLAGS) flappy.cpp -o flappy.o