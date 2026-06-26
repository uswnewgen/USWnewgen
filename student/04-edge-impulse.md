<!-- workshop-header -->
<img width="1347" height="127" alt="Coding Thailand 2026 header" src="https://github.com/user-attachments/assets/ba5cf267-f460-4fb0-b69b-c461ae061a3b" />

# ☀️ เทรน + Deploy + ทดสอบ (Edge Impulse)

บ่ายนี้เลือก **1 input** (จาก 3 ตัวที่ต่อเช้า) มาสอน AI แล้วเอาลงบอร์ดให้รันจริง
นี่คือช่วงที่แน่นที่สุดของวัน — **ทำตามทีละขั้น อย่าข้าม** ติดตรงไหนเรียกพี่เลี้ยงทันที

---

## เกริ่น: Edge Impulse ทำงานยังไง

Edge Impulse คือเว็บที่ช่วยเราเทรน AI เล็กๆ ให้ไปรันบนอุปกรณ์ได้ (ฟรี free tier)
มองงานทั้งบ่ายเป็น **4 ขั้น แล้ววนแก้**:

```
1. Collect  เก็บข้อมูลจาก input จริง (แยกตาม class)
2. Train    สอน model + ดูผลว่าแยก class ได้แค่ไหน
3. Deploy   เอา model ลง UNO Q ให้รันบนบอร์ด
4. Test     ป้อน input จริง จดผล → ถ้าพลาด กลับไปเก็บเพิ่ม (V2)
```

> 🧠 จำคำนี้ไว้: **AI ดีหรือไม่ดี อยู่ที่ "ข้อมูล" มากกว่าปุ่มเทรน** — ขั้น Collect สำคัญสุด

---

## 1. ออกแบบ class (ก่อนเก็บข้อมูล)

ทีมตัดสินใจว่าจะสอน AI ให้แยกอะไรบ้าง แล้วจดลง [afternoon/model.md](../team-template/afternoon/model.md) (ช่อง Input/Classes) ก่อน:

- มีกี่ class? (เริ่มแค่ 2–3 class พอ อย่าโลภ)
- แต่ละ class ต่างกันด้วยอะไร? edge case ที่อาจสับสนคืออะไร?
- จะเก็บกี่ตัวอย่าง/class? เก็บจากใคร ที่ไหนบ้าง?
- ตั้งชื่อ label ยังไงให้สะกดตรงกันทุกครั้ง?

> ⚠️ **ระวัง bias (กับดักอันดับ 1):** ถ้าเก็บจากคนเดียว / ฉากเดียว / ระยะเดียว AI จะจำ "คน/ฉาก/ระยะ" แทน class จริง
> เช็กง่ายๆ: *"ถ้าเปลี่ยนคนหรือเปลี่ยนห้องแล้ว ยังทายได้ไหม?"*

---

## 2. เชื่อม UNO Q เข้า Edge Impulse "ครั้งแรก"

ขั้นนี้ตั้งครั้งเดียว ผ่านแล้วใช้ยาวทั้งบ่าย **ทางเข้าต่างกันตาม input ที่เลือก**

### 2.0 สมัคร + สร้าง project (ทำบน laptop)

1. สมัคร/login [edgeimpulse.com](https://edgeimpulse.com)
2. **Create new project** → ตั้งชื่อทีม เช่น `team-XX`
3. เปิด project ค้างไว้ที่หน้า **Devices** (เดี๋ยวบอร์ดต้องมาโผล่ตรงนี้)

### 2A. ถ้าเลือกกล้อง / ไมค์ (ฝั่ง Linux)

ทำบน **shell ของบอร์ด** (ปุ่ม `>_` ใน App Lab หรือ SSH):

```bash
# 1) ติดตั้ง Edge Impulse CLI (ตั้งครั้งแรกครั้งเดียว ~5-10 นาที)
curl -sL https://deb.nodesource.com/setup_20.x | sudo bash -
sudo apt install -y build-essential python3 nodejs sox gstreamer1.0-tools \
  gstreamer1.0-plugins-good gstreamer1.0-plugins-base gstreamer1.0-plugins-base-apps
sudo npm install edge-impulse-linux -g --unsafe-perm   # กล้อง/ไมค์ + runner
sudo npm install edge-impulse-cli -g --unsafe-perm     # data-forwarder/uploader (Modulino) — ต้องมี python3+build-essential

# 2) เช็กว่าเห็นอุปกรณ์ก่อน
ls /dev/video*   # กล้อง
arecord -l       # ไมค์

# 3) login + เลือก project
edge-impulse-linux
```

`edge-impulse-linux` จะถาม email/password → เลือก project ทีม → ตั้งชื่อ device เช่น `team-XX-q`
เห็นข้อความ **"Your device is now connected to Edge Impulse"** = ผ่าน

### 2B. ถ้าเลือก Modulino sensor (ฝั่ง MCU)

Modulino อยู่ฝั่ง MCU ส่งตรงเข้า Studio ไม่ได้ — เก็บ data ได้ **2 วิธี**:
- **วิธีหลัก (แนะนำ): เก็บเป็น CSV แล้ว Upload ผ่านเว็บ** — ไม่ต้องลง CLI อะไรเพิ่ม เวิร์กชัวร์ทุกเครื่อง
- (ขั้นสูง) live ผ่าน `edge-impulse-data-forwarder` — ต้องลง `edge-impulse-cli` + compile (มักมีปัญหาหน้างาน)

#### วิธีหลัก — เก็บ CSV แล้ว Upload ⭐

1. App Lab → Sketch → ใช้ [Challenge B](examples/modulino/challenges/) → **Run**
   - print เป็น `timestamp,accX,accY,accZ` (timestamp = ms) — **รูปแบบที่ EI ต้องการ**
     ```
     timestamp,accX,accY,accZ
     0,0.02,0.98,0.10
     10,0.05,0.97,0.12
     ```
2. เปิด **Serial Monitor** → ทำท่านั้นค้างไว้ ~10 วิ → เลือกข้อความทั้งหมด → copy → เซฟเป็นไฟล์ เช่น `circle.csv` (**1 ท่า = 1 ไฟล์**)
3. Studio → **Data acquisition** → **Upload data** → เลือกไฟล์ → ตั้ง **Label** (= ชื่อท่า) → Upload
   - เจอ **CSV wizard**: คอลัมน์ timestamp เป็น ms · ถ้าไฟล์ไม่มี timestamp เลือก *"set frequency"* = `100` Hz
4. ทำครบทุก class (จำนวนไฟล์ใกล้กัน · สลับคน/ความเร็ว กัน bias)

> 💡 ไม่ต้องแตะ shell/CLI เลย — เก็บจากเครื่องไหนก็ได้ที่เปิด Serial Monitor

#### (ขั้นสูง) live ด้วย data-forwarder

ส่งค่าสดเข้า Studio แทนการ upload ไฟล์ — แต่ต้องลง `edge-impulse-cli` ก่อน (+ `python3 build-essential`; ถ้า compile error ดู [troubleshooting.md](troubleshooting.md) ข้อ 3)

1. รัน [collect-movement-ei](examples/modulino/collect-movement-ei/) (print ตัวเลขล้วน ไม่มี header) → **ปิด Serial Monitor**
2. shell (`>_`): `edge-impulse-data-forwarder` → ตอบ: project / port (`/dev/ttyACM0`) / **frequency** `100` / **axes** `accX,accY,accZ` / device name
3. Studio → Data acquisition → ตั้ง Label → Start sampling

> ตั้งค่าผิดอยากเริ่มใหม่: `edge-impulse-data-forwarder --clean` · ไม่เจอ port → เช็กสเก็ตช์ run + ปิด Serial Monitor

### ✅ Checkpoint สำคัญสุดของข้อนี้

ใน Studio → **Devices** เห็น UNO Q ขึ้น **Connected** และลอง **Data acquisition** แล้ว data ไหลเข้าจริง

> ยังไม่เห็น? **อย่าเพิ่งเก็บข้อมูล** เปิด [troubleshooting.md](troubleshooting.md) ข้อ 3
> error บ่อย: `command not found` → ติดตั้ง CLI ใหม่ · `Permission denied` กล้อง/ไมค์ → `sudo usermod -aG video,audio $USER` แล้ว login ใหม่

---

## 3. เก็บข้อมูล (Collect) — ขั้นที่สำคัญที่สุด

ใน Studio → **Data acquisition** → เก็บตาม class ที่ออกแบบ:

- แต่ละ class **จำนวนใกล้กัน** (เช่น class ละ 30–50 ตัวอย่าง)
- **สลับ** คน / มุม / แสง / ระยะ / ความเร็ว ให้หลากหลาย
- เก็บทั้งเคสง่ายและเคสที่ใกล้ของจริง
- ปล่อยให้ Edge Impulse แบ่ง **Train / Test** (ปกติ 80/20) — อย่าเอา test ไปปนตอน train

> 💡 ของน้อยแต่สะอาด > ของเยอะแต่มั่ว · ให้คนที่ไม่ได้เก็บลองเก็บด้วย กัน bias

---

## 4. สร้าง Impulse + ดู Feature Explorer

1. **Create Impulse** → ใส่ processing block ตาม input (เสียง/ภาพ/sensor) + learning block = **Classification**
2. **Generate features** → เปิด **Feature Explorer**

Feature Explorer ตอบคำถาม: *"ข้อมูลแยก class ได้จริงหรือยัง"* (ดู**ก่อน** train)

| เห็นแบบนี้ | แปลว่า | ทำอะไรต่อ |
|---|---|---|
| จุดแต่ละ class แยกเป็นก้อน | data มี pattern ชัด | ไป train ได้เลย |
| หลาย class ทับกันเยอะ | class/data ยังสับสน | กลับไปแก้ class หรือเก็บใหม่ |
| class เดียวกระจายหลายก้อน | variation ไม่สม่ำเสมอ | เก็บเพิ่มในแบบที่ขาด |
| มีจุดหลุดไกล | มี outlier เพี้ยน | เปิดดู sample นั้น ลบหรือเก็บใหม่ |

> ถ้าจุดยังปนกัน **อย่ารีบ train** — แก้ data ก่อน คุ้มกว่าเยอะ

---

## 5. Train + อ่านผลการเทรน ⭐

กด **Start training** → เลือก model **ขนาดเล็กที่สุดที่ลง UNO Q ได้** (ดู option ตอน deploy)
เทรนเสร็จ Studio จะโชว์ตัวเลขชุดนึง — **ต้องอ่านให้เป็น ไม่ใช่ดูแค่ accuracy**

### อ่านตัวเลขแต่ละตัว

| ตัวเลข | แปลว่า | อ่านยังไง |
|---|---|---|
| **Accuracy** | ทายถูกกี่ % ของทั้งหมด | ดูภาพรวม แต่ **หลอกได้** ถ้าข้อมูลแต่ละ class ไม่เท่ากัน |
| **Precision** (ราย class) | ที่ทายว่าเป็น A จริงเป็น A กี่ % | ต่ำ = ชอบทายมั่วว่าเป็น A |
| **Recall** (ราย class) | ของ A จริงทั้งหมด จับเจอกี่ % | ต่ำ = ของ A หลุดบ่อย |
| **F1 score** (ราย class) | สมดุลของ Precision กับ Recall (0–1) | **ใกล้ 1 = ดี** · เลขนี้บอกสุขภาพ "รายตัว" ดีกว่า accuracy รวม |

> 🎯 วิธีใช้ F1 จริง: ไล่ดู F1 **ทุก class** → class ไหน F1 ต่ำกว่าเพื่อน = class นั้นมีปัญหา
> แล้วไปดู **Confusion Matrix** ว่ามันไปสับสนกับ class ไหน

### อ่าน Confusion Matrix

ตารางบอกว่า model สับสน class ไหนกับ class ไหน:

| เห็นแบบนี้ | ความหมาย | วิธีแก้ |
|---|---|---|
| เส้นทแยงมุม (diagonal) สูง | ทายถูกเยอะ | ไป deploy ต่อได้ |
| นอกเส้นทแยงสูงระหว่าง 2 class | model สับสนคู่นั้น | เก็บ data ที่ทำให้ 2 class นี้ต่างกันชัดขึ้น |
| class นึงผิดบ่อยมาก | data น้อย/นิยามไม่ชัด | เพิ่ม sample หรือแก้นิยาม class |
| accuracy สูงมาก แต่ test จริงพัง | overfitting | เพิ่ม hard case + variation |

### สัญญาณว่า V1 ยังไม่พร้อม

- ทายถูกเฉพาะคนที่เก็บ data / แสงเดิม / ห้องเดิม
- confidence ต่ำแทบทุก class
- F1 ของบาง class ต่ำกว่าตัวอื่นชัดเจน

> ✅ **ผ่านเมื่อ:** มี model V1 + อ่าน accuracy, F1 ราย class และ confusion matrix ของตัวเองออก

---

## 6. Deploy ลง UNO Q ให้รันจริง ⭐

> 🧠 วิธี deploy **ต่างกันตาม input** (เหมือนตอนเชื่อม ข้อ 2):
> - 📷🎤 **กล้อง/ไมค์ = ฝั่ง Linux** → รันได้เลยบน shell หรือ App Lab
> - ✋ **Modulino = ฝั่ง MCU** → ต้อง build เป็น Arduino library ใส่ในสเก็ตช์

> ⚠️ **เข้าใจก่อน:** App Lab มี AI bricks (Video Object Detection / Audio Classification / Camera Code Detection) — แต่พวกนั้นคือ **model สำเร็จรูปของ Arduino เอง**
> **model ที่ทีมเทรนบน Edge Impulse ยังไม่มี brick ให้เสียบตรงๆ** → รันผ่าน **shell** ด้วย `edge-impulse-linux-runner`

### 6.1 รัน — กล้อง/ไมค์ (ฝั่ง Linux) = วิธีหลัก

บน shell ของบอร์ด (ปุ่ม `>_`):
```bash
edge-impulse-linux-runner
```
- ครั้งแรกถาม login + เลือก project → มัน**ดึง model ล่าสุดของทีมมารันให้เลย** (ไม่ต้อง build/download เอง)
- เปิดเบราว์เซอร์ไปที่ `http://<IP ของบอร์ด>:4912` → เห็นภาพ/เสียง + ผลทำนายสด
- IP ดูได้จาก **แถบล่างของ App Lab** (เช่น `IP: 192.168.x.x`) หรือสั่ง `ip addr`

> ✅ checkpoint: เห็น live classification ที่ `:4912` แล้วป้อน input จริงเปลี่ยน prediction ตาม

**(เสริม) อยากลอง model สำเร็จรูปใน App Lab:**
- Examples → เลือก (เช่น *Detect Objects on Camera*) → **Create your own app** (example เป็น read-only แก้ไม่ได้)
- ตัวอย่างกล้องต้องรันแบบ **Network Mode** (บอร์ดอยู่ Wi-Fi + USB-C hub + กล้อง)
- กด **Run** (มุมขวาบน) → ดูผลที่ Web UI · log ขึ้นที่แถบล่าง

### 6.2 รัน — Modulino (ฝั่ง MCU)

Studio → **Deployment** → **Arduino library** → Build → ได้ `.zip`:
1. App Lab → Sketch → import library (.zip)
2. ใช้ตัวอย่าง `..._inferencing` ของ library → เรียก `run_classifier()` อ่านผล
3. เอาผล (label + value) ไป `Serial.print` หรือสั่ง output

> เส้นนี้เขียน code เยอะกว่ากล้อง/ไมค์ — ติดตรงไหนเรียกพี่เลี้ยง

### 6.3 Quick Test — อ่านผล runtime ให้เป็น

ป้อน input จริง 3–5 ครั้ง แล้วดูเลขพวกนี้ (จาก Web UI `:4912` หรือ Serial):

| ค่า | ดูอะไร |
|---|---|
| **predicted class** | เปลี่ยน input แล้วเปลี่ยนตามไหม |
| **confidence** (0–1) | สูง/ต่ำแค่ไหน นิ่งหรือแกว่ง |
| **inferencing time (ms)** | = response time เร็วพอใช้จริงไหม |
| **anomaly score** (ถ้ามี) | สูง = เจอของแปลกที่ไม่เหมือน training |

เช็กเพิ่ม: ค้าง / หลุด / ทายแกว่งไหม

> ✅ **ผ่านเมื่อ:** ป้อน input จริงแล้ว model ตอบบนบอร์ดได้ และ prediction เปลี่ยนตาม input

### 6.4 Deploy มีปัญหา?

| อาการ | วิธีแก้ |
|---|---|
| หา "import EI model" ใน App Lab ไม่เจอ | ปกติ — model ทีมรันผ่าน `edge-impulse-linux-runner` ไม่ใช่ brick |
| `runner` error เรื่อง 32-bit | ใช้ `edge-impulse-linux-runner` (จัดการ arch ให้) อย่าโหลด `.eim` แบบเลือก arch ผิดมาเอง |
| กล้อง/ไมค์ไม่ขึ้น / app ค้าง | ต้องรัน **Network Mode** (บอร์ดบน Wi-Fi + hub) → [troubleshooting.md](troubleshooting.md) ข้อ 2 |
| memory / build error (Modulino) | model ใหญ่ไป → ลด model size / ลด class / ลด window |
| ทาย class เดิมตลอด | input ไม่เปลี่ยนจริง / data ไม่ balance |

### 6.5 (ต่อยอด) Prototype — ให้ตอบเป็นไฟ/เสียง

อยากให้ผลทำนายไปสั่ง Pixels/Buzzer? เพิ่ม logic:

```
1. อ่าน predicted class + confidence
2. ตั้ง threshold เช่น confidence >= 0.80
3. map class -> output:
     "ปลอดภัย" -> Pixels เขียว
     "เตือน"   -> Pixels แดง + Buzzer
4. ถ้า confidence ต่ำ -> ไม่ตัดสิน / ขึ้น unknown / ให้ลองใหม่
```

> 💡 นี่คือจุดที่ "ของที่รันได้" กลายเป็น "นวัตกรรมที่ใช้งานได้" — เก็บไว้ต่อ Day 2

---

## 7. ทดสอบกับกล้องสด — Live classification vs runner

ทดสอบ model กับกล้อง/ไมค์ที่ต่อ UNO Q ได้ **2 ที่** ใช้คนละจังหวะ:

| | **Live classification** (ใน Studio) | **`edge-impulse-linux-runner`** (บนบอร์ด) |
|---|---|---|
| อยู่ที่ | เมนูซ้ายของ Studio | shell + เว็บ `:4912` |
| ทำงานแบบ | ถ่ายทีละช็อต แล้วทาย | สด ต่อเนื่อง real-time |
| ต้องมี | `edge-impulse-linux` connect device ค้างไว้ | รันตรงบนบอร์ด (ดูข้อ 6.1) |
| เหมาะกับ | ลองทีละรูป เทียบของจริง | ทดสอบ 10 cases + เดโม่ |

**Live classification ทำยังไง:**
1. shell (`>_`): `edge-impulse-linux` → login → เลือก project → บอร์ดขึ้น **เขียว (Connected)** ใน Devices
2. Studio → **Live classification** → เลือก **Device = UNO Q**, **Sensor = Camera** → **Start sampling**
3. ถ่าย 1 รูปจากกล้องจริง → ทาย → เห็นรูป + กรอบ (bounding box) + confidence (เลื่อนดูตาราง **Detected objects** ใต้รูป)

> ⚠️ อย่าเปิด `edge-impulse-linux` กับ `runner` **พร้อมกัน** — แย่งกล้อง/port กัน

**กดแล้ว "ไม่เห็นค่า"? เช็กตามนี้:**
- **เห็นรูป แต่ไม่มีกรอบ/ค่า** = model **ตรวจไม่เจอ object** (ไม่ใช่ระบบพัง) → ลด slider **threshold**, เอา object เข้าใกล้แบบตอนเก็บ data, หรือข้อมูลน้อยไป เก็บเพิ่ม+เทรนใหม่
- **ไม่เห็นรูปเลย / ค้าง** = device ไม่เขียว, เลือก sensor ผิด, หรือกล้องไม่ขึ้น (`ls /dev/video*`)
- อยากดู**ภาพรวม**ว่าโมเดลแม่นแค่ไหน → เมนู **Model testing** (ทดสอบกับ test set ที่เก็บไว้ ไม่ใช่กล้องสด)

---

## 8. ทดสอบ 10 cases + จดผล

จดลง `team-template/afternoon/predictions.csv` ทุกครั้ง:

| ชุด | กี่เคส | ตัวอย่าง |
|---|---|---|
| Normal | 5 | input แบบที่ train มา |
| ท้าทาย | 5 | ก้ำกึ่งระหว่าง class / เร็วผิดปกติ / คนใหม่ที่ไม่ได้เก็บ / ใกล้เคียงแต่ไม่ใช่ |

แต่ละเคสบันทึก: input · predicted+confidence · actual · ถูก/ผิด · response time · อาการ (ค้าง/แกว่ง)

> ✅ **ผ่านเมื่อ:** มี prediction log ≥10 cases ที่มีทั้งถูกและผิด พร้อมเหตุผลที่น่าจะพลาด

---

## 9. ถ้าผลไม่ดี → V2 (ถ้าเหลือเวลา)

```
Feature Explorer ปนกัน?          -> แก้ class definition / เก็บ data เพิ่ม
Confusion Matrix สับสนคู่เดิมซ้ำ?  -> เพิ่ม sample เฉพาะคู่นั้น + boundary case
Studio ดี แต่ test จริงพัง?        -> environment ต่างกัน เก็บ variation จริงเพิ่ม
confidence ต่ำแต่ output ยังตัดสิน? -> เพิ่ม threshold / ใส่ unknown state
model ใหญ่ deploy ไม่ได้?          -> ลด model size / target = Arduino UNO Q
```

เก็บข้อมูลเพิ่มในจุดที่ผิด → train V2 → เทียบกับ V1 ว่าดีขึ้นตรงไหน

---

ต่อไป → [05-submit.md](05-submit.md)
