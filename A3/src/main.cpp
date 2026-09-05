#include "GameEngine.h"

int main(int argc, char *argv[]) {
  auto gameEngine = GameEngine("assets/assets.txt");
  gameEngine.run();
  return 0;
}
