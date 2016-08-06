##
## when "make train", the training program will be compiled
## when "make hello", the hello-world program will be compiled
##

CC=	g++
#OPT=	-std=c++11 -static-libstdc++ -Wall -pg
OPT=	-std=c++11 -static-libstdc++
LIBS=	-lm -lpthread
SRCS_TRAIN=	mt_train.cpp
OBJS_TRAIN=	mt_train.o
SRCS_HELLO=	mt_hello.cpp
OBJS_HELLO=	mt_hello.o

EXECUTABLE_TRAIN=	mt_train
EXECUTABLE_HELLO=	mt_hello



all:
	@echo Please choose from \"make train\" and \"make hello\" to compile and link...



### compiling starts from here
train: main_train clean mv_train


$(OBJS_TRAIN): $(SRCS_TRAIN)
	$(CC) -c $*.cpp $(OPT)

main_train: $(OBJS_TRAIN)
	$(CC) -o $(EXECUTABLE_TRAIN) $(OBJS_TRAIN) $(OPT) $(LIBS)



hello: main_hello clean mv_hello


$(OBJS_HELLO): $(SRCS_HELLO)
	$(CC) -c $*.cpp $(OPT)

main_hello: $(OBJS_HELLO)
	$(CC) -o $(EXECUTABLE_HELLO) $(OBJS_HELLO) $(OPT) $(LIBS)




clean:
	-rm -f *.o



mv_train:
	@chmod 755 $(EXECUTABLE_TRAIN)

mv_hello:
	@chmod 755 $(EXECUTABLE_HELLO)


