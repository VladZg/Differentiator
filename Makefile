TARGET = main

CC = g++
CFLAGS = -Wall -fsanitize=address -O0 -g

APP_DIR = ./App/
SRC_DIR = ./Source/

SRC = 	$(APP_DIR)main.cpp					\
		./Stack/Stack.cpp					\
		./Stack/Log.cpp						\
		./CheckFile.cpp						\
		./Tree.cpp							\
		./TreeDump.cpp						\
		./ExpressionParams.cpp				\
		./ExpressionReader.cpp				\
		./Interactors.cpp					\
		./TexTreeTranslateFunctions.cpp		\
		./TreeSimplifyFunctions.cpp			\
		./MathFunctions.cpp					\
		./TexCreateFunctions.cpp

$(TARGET) :
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean :
	rm -f *.o $(TARGET)
