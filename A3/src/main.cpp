#include "GameEngine.h"

int main(int argc, char *argv[]) {
  auto gameEngine = GameEngine("file.txt");
  gameEngine.run();
  return 1;
}
