#Makefile for assignment 3 
CXX           = $(shell fltk-config --cxx)
CXXFLAGS      = $(shell fltk-config --cxxflags)
LINKFLTK_FLGS = $(shell fltk-config --ldflags --use-gl --use-images)
POSTBUILD     = fltk-config --post #build .app for osx. (does nothing on pc)

all: a3

%.o: %.cpp
	$(CXX) -c $< $(CXXFLAGS)

a3:  main.o MyGLCanvas.o Camera.o ./scene/SceneParser.o ./scene/tinyxmlparser.o ./scene/tinyxmlerror.o ./scene/tinyxml.o ./scene/tinystr.o
	$(CXX) -o $@ main.o MyGLCanvas.o Camera.o SceneParser.o tinyxmlparser.o tinyxmlerror.o tinyxml.o tinystr.o $(LINKFLTK_FLGS) && $(POSTBUILD) $@ && if test -d $@.app; then sed -i '' -e 's|<true/>|<false/>|' "$@.app/Contents/Info.plist"; fi

clean:
	@rm -rf a3.app a3 *.o *~
