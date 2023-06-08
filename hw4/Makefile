CXXFLAGS = -std=c++11 -O2 -g -Wall -fmessage-length=0
INCLUDES =
LIBS =
OPENCV_INCLUDES = `pkg-config opencv4 --cflags`
OPENCV_LIBS = `pkg-config opencv4 --libs`
OBJS = main.o
TARGET = a.out

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $(TARGET) $(OBJS) $(LIBS) $(OPENCV_LIBS)

%.o: %.cpp
	$(CXX) -c -o $@ $(CXXFLAGS) $(INCLUDES) $(OPENCV_INCLUDES) $<

clean:
	rm -f $(OBJS) $(TARGET)
