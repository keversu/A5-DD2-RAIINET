# Universal makefile for single C++ program
#
# Use gcc flag -MMD (user) or -MD (user/system) to generate dependencies among source files.
# Use make default rules for commonly used file-name suffixes and make variables names.
#
# % make [ a.out ]

########## Variables ##########
# Compiler
CXX = clang++

# Compiler flags
CXXFLAGS = -std=c++20 -g -Wall -Werror=vla -MMD -I/opt/X11/include

# Linker flags for X11
LDFLAGS = -L/opt/X11/lib -Wl,-rpath,/opt/X11/lib -lX11

# Executable name
EXEC = raiinet

# Source and object files
SOURCES = $(wildcard *.cc)
OBJECTS = $(SOURCES:.cc=.o)
DEPENDS = $(OBJECTS:.o=.d)

# Linking step
$(EXEC) : $(OBJECTS)
	$(CXX) $(CXXFLAGS) $(OBJECTS) -o $(EXEC) $(LDFLAGS)

# Ensure recompilation if Makefile changes
$(OBJECTS) : Makefile

# Include dependency files
-include $(DEPENDS)

# Clean command
.PHONY : clean
clean :
	rm -f $(DEPENDS) $(OBJECTS) $(EXEC)
	rm -rf gcm.cache
	clear
