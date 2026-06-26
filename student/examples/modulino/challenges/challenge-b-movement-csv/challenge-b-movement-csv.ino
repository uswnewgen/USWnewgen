// Challenge B — เก็บ Movement เป็น CSV (พร้อม upload เข้า Edge Impulse)
// print: timestamp(ms),accX,accY,accZ  — รูปแบบที่ EI CSV upload ต้องการ
// ~100 Hz, baud 115200
//
// ใช้งาน:
//   1) Run -> เปิด Serial Monitor -> ทำท่าค้าง ~10 วิ
//   2) copy ข้อความทั้งหมด -> เซฟเป็น <ชื่อท่า>.csv (1 ท่า = 1 ไฟล์)
//   3) Studio -> Data acquisition -> Upload data -> ตั้ง Label
//
// ต้องมี: Modulino Movement

#include <Modulino.h>

ModulinoMovement movement;
unsigned long t0;

void setup() {
  Serial.begin(115200);
  Modulino.begin();
  movement.begin();
  t0 = millis();
  Serial.println("timestamp,accX,accY,accZ");   // header ที่ EI ต้องการ
}

void loop() {
  unsigned long t = millis() - t0;               // ms เริ่มจาก 0
  movement.update();
  Serial.print(t);               Serial.print(',');
  Serial.print(movement.getX()); Serial.print(',');
  Serial.print(movement.getY()); Serial.print(',');
  Serial.println(movement.getZ());
  delay(10);                                      // ~100 Hz
}
