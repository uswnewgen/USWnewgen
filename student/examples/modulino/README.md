<!-- workshop-header -->
<img width="1347" height="127" alt="Coding Thailand 2026 header" src="https://github.com/user-attachments/assets/ba5cf267-f460-4fb0-b69b-c461ae061a3b" />

# 🧩 Modulino Examples — ลองครบทั้ง 7 ตัว

เช้านี้ลองให้ครบทุกตัว **เผื่อไอเดีย** บ่ายค่อยเลือกตัวที่ทีมชอบไปเทรน
ทุกตัวเสียบ Qwiic (เสียบผิดด้านเข้าไม่ได้ ปลอดภัย) และอ่าน/สั่งจากฝั่ง **MCU**

> ตัวอย่างเป็นสเก็ตช์ Arduino (C++) ใช้ไลบรารี `Modulino`
> **รันด้วย Arduino App Lab อย่างเดียว** — ไม่ต้องลง Arduino IDE แยก ไม่ต้องต่อสายอื่น

---

## ▶️ วิธีรันสเก็ตช์บน UNO Q (App Lab อย่างเดียว)

1. เปิด **Arduino App Lab** → login → เห็นบอร์ดทีม (`team-XX-q`) ในรายการ
2. กด **+ New** สร้าง App ใหม่ (หรือเปิด App เดิม) แล้วไปที่ส่วน **Sketch** (ฝั่ง Arduino/MCU)
3. ก็อปโค้ดจากไฟล์ `.ino` ในตัวอย่าง วางลงในตัวแก้ไข Sketch
   - ไลบรารี `Modulino` มีให้แล้วใน App Lab — ถ้าหาไม่เจอ เพิ่มผ่าน **Library Manager** (ค้น "Modulino")
4. กด **Run** (▶) — App Lab จะ build แล้ว upload ลงฝั่ง MCU ให้เอง (รันผ่าน Wi-Fi/USB ที่ตั้งไว้ตอน setup)
5. เปิด **Serial Monitor / Console** ใน App Lab → ตั้ง baud **9600** → ขยับ/กด/หมุน ดูค่าขยับ

> ⚠️ ทุกตัวอย่างเป็น **Sketch ฝั่ง MCU** ล้วน ยังไม่ต้องเขียนฝั่ง Python (Linux)
> ฝั่ง Python จะได้ใช้ตอนเชื่อม Edge Impulse / ทำ prototype Day 2
> รายละเอียดเพิ่ม: [docs Arduino App Lab](https://docs.arduino.cc/software/app-lab/) · [Modulino library](https://docs.arduino.cc/libraries/modulino/)

---

## 7 ตัวมีอะไรบ้าง

| # | Modulino | ทำอะไร | เป็น AI input ได้ไหม | ไอเดียลองเล่น |
|---|----------|--------|----------------------|----------------|
| 1 | **Movement** (IMU) | อ่านความเร่ง x/y/z | ✅ ดีมาก (ท่าทาง/สั่น) | จำท่ามือ · จับการล้ม · ตรวจการสั่นของมอเตอร์ |
| 2 | **Distance** (ToF) | วัดระยะเป็น มม. | ✅ ได้ | นับคนเดินผ่าน · gesture เลื่อนมือ · ตรวจของเข้าใกล้ |
| 3 | **Thermo** | อุณหภูมิ + ความชื้น | ✅ ได้ (ช้าๆ) | เฝ้าอุณหภูมิห้อง/ตู้ · แยกร้อน/ปกติ/เย็น |
| 4 | **Buttons** | ปุ่ม A/B/C | ⚠️ มักใช้ติด label | ปุ่มกำกับ class ตอนเก็บ data · เริ่ม/หยุด |
| 5 | **Knob** | หมุน + กดได้ | ⚠️ มักใช้ปรับค่า | ปรับ threshold · เลือกโหมด |
| 6 | **Pixels** | LED RGB 8 ดวง | ❌ เป็น output | แสดงสีตาม class ที่ทำนายได้ |
| 7 | **Buzzer** | ลำโพงเล่นโทน | ❌ เป็น output | เตือนเสียงเมื่อเจอเหตุการณ์ |

- **#1–3 เซนเซอร์อ่านค่า** → เหมาะเอาไปเทรนเป็น AI input ตอนบ่าย
- **#4–5 อินพุตโต้ตอบ** → ใช้สั่งงาน/กำกับ class ระหว่างเก็บ data
- **#6–7 เอาต์พุต** → ใช้ตอบสนองผลทำนาย (จะได้ใช้จริงตอนทำ prototype Day 2)

---

## ลองยังไงให้เร็ว

1. ต่อ `UNO Q → Modulino` ด้วยสาย Qwiic
2. เปิด folder ของตัวที่อยากลอง (เช่น [01-movement/](01-movement/))
3. Run แล้วเปิด Serial Monitor → ขยับ/กด/หมุน ดูค่าขยับ
4. อยากต่อหลายตัวพร้อมกัน? เสียบ daisy-chain ต่อกันเป็นสายได้เลย

> 💡 ระหว่างลอง จดไว้ว่า "ตัวไหนให้ค่าที่แยกสถานการณ์ได้ชัด" — นั่นคือตัวที่เหมาะไปเทรนบ่ายนี้

---

## โครง code ที่ทุกตัวใช้เหมือนกัน

```cpp
#include <Modulino.h>

ModulinoMovement sensor;   // เปลี่ยนชนิดตาม Modulino ที่ใช้

void setup() {
  Serial.begin(9600);
  Modulino.begin();        // เริ่ม bus Qwiic
  sensor.begin();          // เริ่มตัว Modulino
}

void loop() {
  // อ่านค่า / สั่งงาน แล้ว print
}
```

ตัวอย่างเต็มของแต่ละตัวอยู่ใน folder ย่อยด้านล่าง:

- [01-movement/](01-movement/) — ความเร่ง x/y/z
- [02-distance/](02-distance/) — ระยะ (มม.)
- [03-thermo/](03-thermo/) — อุณหภูมิ + ความชื้น
- [04-buttons/](04-buttons/) — ปุ่ม A/B/C
- [05-knob/](05-knob/) — หมุน + กด
- [06-pixels/](06-pixels/) — LED RGB 8 ดวง
- [07-buzzer/](07-buzzer/) — เล่นโทนเสียง

---

## 🏆 ลองครบแล้ว? ไปต่อที่ Challenges

โจทย์เชื่อมเช้า → บ่าย ใน [challenges/](challenges/):

- **Challenge A** — Proximity Alarm: Sensor → Buzzer (input → logic → output)
- **Challenge B** — เก็บ Movement เป็น CSV เตรียมไปเทรน Edge Impulse ตอนบ่าย

## 🤖 ส่ง Movement เข้า Edge Impulse (ตอนบ่าย)

- [collect-movement-ei/](collect-movement-ei/) — สเก็ตช์ print x,y,z สำหรับ `edge-impulse-data-forwarder` (ดูขั้นตอนเต็มใน [04-edge-impulse.md ข้อ 2B](../../04-edge-impulse.md))
