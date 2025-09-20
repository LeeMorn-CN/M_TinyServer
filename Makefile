# 编译器及编译选项
CXX = g++
CXXFLAGS = -std=c++14 -Wall -g -pthread

# 源文件和目标文件
SRC_DIRS = Code/Buffer Code/HTTP Code/Pool Code/Server .
SRCS = $(wildcard $(addsuffix /*.cpp, $(SRC_DIRS)))
OBJS = $(patsubst %.cpp, %.o, $(SRCS))

# 目标可执行文件
TARGET = M_TinyServer

# 默认目标
all: $(TARGET)

# 生成可执行文件
$(TARGET): $(OBJS)
	$(CXX) $(OBJS) $(CXXFLAGS) -o $(TARGET) 

# 生成 .o 文件的规则
%.o: %.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# 清理生成的文件
clean:
	rm -f $(OBJS) $(TARGET)
