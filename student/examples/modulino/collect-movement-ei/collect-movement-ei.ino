// Collect Movement → Edge Impulse (ผ่าน data-forwarder)
// print ค่า x,y,z เป็น "ตัวเลขล้วน คั่น comma ต่อบรรทัด" ไม่มี header
// อัตราคงที่ ~100 Hz, baud 115200 (ให้ตรงกับที่ตอบ data-forwarder)
//
// ใช้คู่กับ: edge-impulse-data-forwarder บน shell ฝั่ง Linux
// ⚠️ ปิด Serial Monitor ก่อนรัน data-forwarder (ไม่งั้น port ชนกัน)

#include <Modulino.h>

ModulinoMovement movement;

void setup() {
  Serial.begin(115200);
  Modulino.begin();
  movement.begin();
}

void loop() {
  movement.update();
  Serial.print(movement.getX()); Serial.print(',');
  Serial.print(movement.getY()); Serial.print(',');
  Serial.println(movement.getZ());
  delay(10);   // ~100 Hz (คงที่ → ตอบ frequency = 100)
}
