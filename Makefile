CC = g++
CFLAGS = -Wall
LIBS = -lglfw3 -lopengl32 -lgdi32 -lassimp -lfreetype -lpng -lbz2 -lz -lopenal32 -lsndfile
SRCDIR = src
BUILDDIR = build
EXECUTABLE = main.exe
INCDIR = include
LIBDIR = lib

SOURCES := $(wildcard $(SRCDIR)/*.cpp)
SOURCES += $(SRCDIR)/glad.c
OBJECTS := $(patsubst $(SRCDIR)/%.cpp,$(BUILDDIR)/%.o,$(SOURCES))

INCLUDES := -I$(INCDIR) -I$(INCDIR)/freetype2 -I$(INCDIR)/external

all: $(EXECUTABLE)

$(EXECUTABLE): $(OBJECTS)
	$(CC) $(CFLAGS) -o $@ $^ -L$(LIBDIR) $(LIBS)

$(BUILDDIR)/%.o: $(SRCDIR)/%.cpp | $(BUILDDIR)
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

$(BUILDDIR):
	mkdir $(BUILDDIR)

clean:
	del /Q $(BUILDDIR)\*.o
	del /Q $(EXECUTABLE)
	rmdir $(BUILDDIR)
