# All Targets
all: fs

# Tool invocations
# Executable "fs" depends on the files Files.o and main.o and FileSystem.o and Commands.o and Environment.o .
fs: bin/Files.o bin/main.o bin/FileSystem.o bin/Commands.o bin/Environment.o
	@echo 'Building target: fs'
	@echo 'Invoking: C++ Linker'
	g++ -o bin/fs bin/Files.o bin/main.o bin/FileSystem.o bin/Commands.o bin/Environment.o
	@echo 'Finished building target: fs'
	@echo ' '


# Depends on the source and header files
bin/main.o: src/main.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/main.o src/main.cpp

# Depends on the source and header files
bin/Files.o: src/Files.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Files.o src/Files.cpp

# Depends on the source and header files
bin/FileSystem.o: src/FileSystem.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/FileSystem.o src/FileSystem.cpp

# Depends on the source and header files
bin/Commands.o: src/Commands.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Commands.o src/Commands.cpp

# Depends on the source and header files
bin/Environment.o: src/Environment.cpp
	g++ -g -Wall -Weffc++ -std=c++11 -c -Iinclude -o bin/Environment.o src/Environment.cpp




#Clean the build directory
clean:
	rm -f bin/*



