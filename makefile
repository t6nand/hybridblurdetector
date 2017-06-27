program_NAME := hybridblurdetector
program_CXX_SRCS := $(wildcard src/*.cpp)

program_CXX_OBJS := ${program_CXX_SRCS:.cpp=.o}
program_OBJS := $(program_CXX_OBJS)

# Add library paths here.
LIBRARY_PATH=

# Add all paths of include files used in project and any library(for e.g. openCV) being used here.
program_INCLUDE_DIRS :=

# Add all paths to 3rd party library being used in this project here.
program_LIBRARY_DIRS :=

# This is a place holder. If you used program_LIBRARIES := boost_signals, then libboost_signals would be linked in.
program_LIBRARIES := opencv_core opencv_imgproc opencv_imgcodecs
program_CXX_SRCS := $(wildcard src/*.cpp)

CFLAGS='-O3'
CXXFLAGS='-O3'

# This adds -I$(includedir) for every include directory given in $(program_INCLUDE_DIRS)... so if you used ./include, it would expand to -I./include
# Remember that CPPFLAGS is the C preprocessor flags, so anything that compiles a C or C++ source file into an object file will use this flag.
CPPFLAGS += $(foreach includedir,$(program_INCLUDE_DIRS),-I$(includedir))

# This adds -L$(librarydir) for every library directory given in $(program_LIBRARY_DIRS)... so if you used ./lib, it would expand to -L./lib
# Since the LDFLAGS are used when linking, this will cause the appropriate flags to be passed to the linker.
LDFLAGS += $(foreach librarydir,$(program_LIBRARY_DIRS),-L$(librarydir))

# This adds -l$(library) for every library given in $(program_LIBRARIES), so if you used boost_signals, it would expand to -lboost_signals
LDFLAGS += $(foreach library,$(program_LIBRARIES),-l$(library))

.PHONY: all clean distclean

# This is first build rule in the makefile, and so executing "make" and executing "make all" are the same.
# The target simply depends on $(program_NAME), which expands to "myprogram", and that target is given below:
all: $(program_NAME)

# The program depends on the object files (which are automatically built using the predefined build rules... nothing needs to be given explicitly for them).
# The build rule $(LINK.cc) is used to link the object files and output a file with the same name as the program. Note that LINK.cc makes use of CXX,
# CXXFLAGS, LDFLAGS, etc. On my own system LINK.cc is defined as: $(CXX) $(CXXFLAGS) $(CPPFLAGS) $(LDFLAGS) $(TARGET_ARCH),
# so if CXXFLAGS, CPPFLAGS, LDFLAGS, and TARGET_ARCH are undefined, but CXX is g++, then it will expand to g++ $(program_OBJS) -o $(program_NAME).
#
# Note that the line that starts with $(LINK.cc) is indented with a single tab. This is very important! Otherwise, it will not work.
$(program_NAME): $(program_OBJS)
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) $(TARGET_ARCH) $(program_OBJS) -o $(program_NAME) $(LDFLAGS)

# This target removes the built program and the generated object files. The @ symbol indicates that the line should be run silently, and the -
# symbol indicates that errors should be ignored (i.e., if the file already doesn't exist, we don't really care, and we should continue executing subsequent commands)
clean:
	@- $(RM) $(program_NAME)
	@- $(RM) $(program_OBJS)

# The distclean target depends on the clean target (so executing distclean will cause clean to be executed), but we don't add anything else.
distclean: clean
