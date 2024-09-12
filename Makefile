.PHONY: clean

CXX = g++

ENTRANCE = ping_pong_main.cpp
CONSTANT = ping_pong_const.h
FUNCTION = ping_pong.cpp
HEADER_FILE = ping_pong.h

RELEASE_TARGET = ping_pong_game
DEMO_TARGET = ping_pong_game_demo
TEST_TARGET = ping_pong_game_test

release: $(ENTRANCE) $(CONSTANT) $(FUNCTION) $(HEADER_FILE)
	$(CXX) -DRELEASE $(ENTRANCE) $(FUNCTION) -o $(RELEASE_TARGET)
	./$(RELEASE_TARGET)

demo: $(ENTRANCE) $(CONSTANT) $(FUNCTION) $(HEADER_FILE)
	$(CXX) -DDEMO $(ENTRANCE) $(FUNCTION) -o $(DEMO_TARGET)
	./$(DEMO_TARGET)

test: $(ENTRANCE) $(CONSTANT) $(FUNCTION) $(HEADER_FILE)
	$(CXX) -DTEST $(ENTRANCE) $(FUNCTION) -o $(TEST_TARGET)
	./$(TEST_TARGET)

clean:
	rm -f $(TARGET) $(DEMO_TARGET) $(TEST_TARGET)
