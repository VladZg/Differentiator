TARGET = main

CC = g++
CFLAGS = -O3 -g -w #-Wall #-fsanitize=address

APP_DIR = ./App/
SRC_DIR = ./Source/
LIBS_DIR = ./Libs/

SRC = 	$(APP_DIR)main.cpp							\
		$(LIBS_DIR)Stack/Source/Stack.cpp			\
		$(LIBS_DIR)Logging/Log.cpp   				\
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

all : create_dirs compile

create_dirs:
	@mkdir -p ./TexFiles ./TreeDump

compile:
	@$(CC) $(CFLAGS) $(SRC) -o $(TARGET)

clean :
	@rm -f *.o $(TARGET)
