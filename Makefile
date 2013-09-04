OS := $(shell uname)
ARCH := $(shell uname -m)


EXECUTABLE = LeapMouse
INCLUDES:= -I include

SHAREDLIBS:= -lXtst

ifeq ($(OS), Linux)
  ifeq ($(ARCH), x86_64)
    LEAP_LIBRARY = -Wl,-rpath,../../lib/x64  -L../../lib/x64 -lLeap -L /usr/X11R6/lib -lX11
  else
    LEAP_LIBRARY = -Wl,-rpath,../../lib/x86  -L../../lib/x86 -lLeap -L /usr/X11R6/lib -lX11
  endif

endif

SOURCES:=mousemanager.cpp \
		 Leapmouse.cpp \

SHAREDLIBS:= -lXtst


LeapMouse:
ifeq ($(OS), Linux)
	$(CXX) -o $(EXECUTABLE) $(INCLUDES) $(SOURCES) $(SHAREDLIBS) $(LEAP_LIBRARY)
endif

clean:
	rm -rf $(EXECUTABLE) $(EXECUTABLE).dSYM

