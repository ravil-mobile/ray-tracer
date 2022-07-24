#include "Geometry/Geometry.h"
#include "Engine/Engine.h"
#include "Misc/Misc.h"


Scenario buildScenario() {
  size_t numPixelsAlongWidth = 1920; // 320
  size_t numPixelsAlongHight = 1080; // 200
  float ratio = static_cast<float>(numPixelsAlongWidth) / static_cast<float>(numPixelsAlongHight);

  ColorT backgroundColor({192, 192, 192});
  Camera camera({0.0, 0.0, 0.0}, {0.0, 0.0,-1.0}, {0.0, 1.0, 0.0}, {1.0, 1.0f / ratio});
  Scenario scenario(camera, backgroundColor, {numPixelsAlongWidth, numPixelsAlongHight});

  scenario.addLight(Light({10.0, 10.0, 0.0}));
  scenario.addLight(Light({-10.0, 10.0, 0.0}));
  scenario.addSphere(Sphere({0.0, 0.0, -7.5}, 1.0, {34, 139, 34}, 20.0));
  scenario.addSphere(Sphere({2.0, 0.0, -10.0}, 1.0, {240, 128, 128}, 40.0));
  scenario.addSphere(Sphere({-2.0, 0.0, -10.0}, 1.0, {138, 43, 226}, 25.0));

  scenario.addPlane({0.0, -1.0, -5.0},
                    {0.0, 1.0, 0.0},
                    {0.0, 0.0, -1.0},
                    10.0, 25.0,
                    {252, 148, 3},
                    20.0);

  return scenario;
}


int main(int argc, char *argv[]) {
  auto scenario = buildScenario();
  Engine engine(scenario);
  engine.run();

  save("image.jpeg", engine.getCanvas());
  return 0;
}
