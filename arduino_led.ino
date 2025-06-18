#include <Adafruit_NeoPixel.h>

#define PIN        2
#define NUMPIXELS  30
#define SOUND_PIN  A4
#define THRESHOLD  11
#define MAX_BALLS  10   // 最多同時幾個燈

Adafruit_NeoPixel strip(NUMPIXELS, PIN, NEO_GRB + NEO_KHZ800);

struct Ball {
  int pos;
  uint32_t color;
};

Ball balls[MAX_BALLS];
int ballCount = 0;

bool lastSoundOverThreshold = false;
unsigned long lastUpdateTime = 0;
const unsigned long interval = 100; // 跑馬燈速度

void setup() {
  strip.begin();
  strip.clear();
  strip.show();
  Serial.begin(9600);
  randomSeed(analogRead(0));
}

void loop() {
  int soundValue = analogRead(SOUND_PIN);

  // 拍手時新增一顆燈
  if (!lastSoundOverThreshold && soundValue > THRESHOLD) {
    if (ballCount < MAX_BALLS) {
      balls[ballCount].pos = 0;
      balls[ballCount].color = strip.Color(random(0,256), random(0,256), random(0,256));
      ballCount++;
    }
  }
  lastSoundOverThreshold = (soundValue > THRESHOLD);

  // 更新所有燈的位置
  unsigned long now = millis();
  if (now - lastUpdateTime >= interval) {
    lastUpdateTime = now;

    // 清空燈條
    strip.clear();

    // 更新每一顆燈
    for (int i = 0; i < ballCount; i++) {
      if (balls[i].pos < NUMPIXELS) {
        strip.setPixelColor(balls[i].pos, balls[i].color);
        balls[i].pos++;
      }
    }

    // 移除已經跑完的燈
    int newCount = 0;
    for (int i = 0; i < ballCount; i++) {
      if (balls[i].pos < NUMPIXELS) {
        balls[newCount++] = balls[i];
      }
    }
    ballCount = newCount;

    strip.show();
  }

  delay(10);
}