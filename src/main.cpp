#include "Geometry/Geometry.h"
#include "Engine/Engine.h"
#include "Misc/Misc.h"


Scenario buildScenario() {
  ColorT backgroundColor({192,192,192});
  Camera camera({0.0, 0.0, 0.0}, {0.0, 0.0,-1.0}, {0.0, 1.0, 0.0}, {1.0, 0.7});
  Scenario scenario(camera, backgroundColor, {640, 480});

  scenario.addLight(Light({0.0, 0.0, -5.0}, {255, 255, 255}));
  scenario.addSphere(Sphere({0.0, 0.0, -10.0}, 0.250, {34, 139, 34}));
  scenario.addSphere(Sphere({1.0, 0.0, -10.0}, 0.250, {240, 128, 128}));
  scenario.addSphere(Sphere({-1.0, 0.0, -10.0}, 0.250, {138, 43, 226}));

  return scenario;
}


int main(int argc, char *argv[]) {
  auto scenario = buildScenario();
  Engine engine(scenario);
  engine.run();

  save("image.jpeg", engine.getCanvas());
  return 0;
}
