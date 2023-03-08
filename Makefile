TARGET = main

CC = g++
CFLAGS = -Wall -fsanitize=address -O0 -g

APP_DIR = ./App/
SRC_DIR = ./Source/
LIBS_DIR = ./Libs/

SRC = 	$(APP_DIR)main.cpp							\
		$(LIBS_DIR)Stack/Stack.cpp					\
		$(LIBS_DIR)Stack/Log.cpp					\
		$(SRC_DIR)CheckFile.cpp						\
		$(SRC_DIR)Tree.cpp							\
		$(SRC_DIR)TreeDump.cpp						\
		$(SRC_DIR)ExpressionParams.cpp				\
		$(SRC_DIR)ExpressionReader.cpp				\
		$(SRC_DIR)Interactors.cpp					\
		$(SRC_DIR)TexTreeTranslateFunctions.cpp		\
		$(SRC_DIR)TreeSimplifyFunctions.cpp			\
		$(SRC_DIR)MathFunctions.cpp					\
		$(SRC_DIR)TexCreateFunctions.cpp

$(TARGET) :
	$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean :
	rm -f *.o $(TARGET)
