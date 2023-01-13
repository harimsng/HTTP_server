SRC	=	syscall_test/test.cpp
OBJ	=	$(SRC:%.cpp=%.o)
DEP = 	$(OBJ:%.o=%.d)

all:	test

test: $(OBJ)
	$(CXX) $(CXXFLAGS) -o test $^ -I. -MMD

$(OBJ): %.o: %.cpp
	$(CXX) $(CXXFLAGS) -c -o $@ $< -I. -MMD
