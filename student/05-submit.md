<!-- workshop-header -->
<img width="1347" height="127" alt="Coding Thailand 2026 header" src="https://github.com/user-attachments/assets/ba5cf267-f460-4fb0-b69b-c461ae061a3b" />

# 📤 Fork + ส่งงาน (Git)

วันแรกใช้ git แค่ "ส่งงาน" พอ ทำผ่านเว็บ GitHub ได้หมด ไม่ต้อง clone

> ⏰ **ลำดับเวลา:**
> - **ข้อ 1 (Fork) — ทำตั้งแต่เช้า ก่อนอย่างอื่น** เพราะ kit-checklist / hardware-check ต้องกรอกลง fork ของทีม
> - **ข้อ 2 (Commit) — ทำได้เรื่อยๆ ทั้งวัน** กรอกเสร็จช่องไหน commit ช่องนั้น ไม่ต้องรอจบ

## 1) Fork repo (ครั้งเดียวต่อทีม — ทำเช้า) ⭐

1. เข้า repo หลัก กด **Fork** (มุมขวาบน)

<img src="assets/gh-fork-button.jpg" alt="ปุ่ม Fork มุมขวาบนของ repo หลัก" width="560" />

2. ตั้งชื่อ repo → **Create fork**

<img src="assets/gh-create-fork.jpg" alt="หน้า Create a new fork: ตั้งชื่อแล้วกด Create fork" width="560" />

3. ถ้ากด Fork แล้ว error = เคย fork แล้ว → กดโลโก้ GitHub ไป dashboard หา repo เดิมที่สร้างไว้

> ⚠️ ก่อนแก้ทุกครั้ง เช็กว่าอยู่ใน repo **ของทีมเอง** ไม่ใช่ repo หลัก ไม่งั้น commit ผิดที่

## 2) แก้ + commit

1. เปิดไฟล์ที่จะกรอก (เช่น `team-template/afternoon/model.md`) → กด **ไอคอนดินสอ**

<img src="assets/gh-edit-pencil.jpg" alt="กดไอคอนดินสอเพื่อแก้ไฟล์บน GitHub" width="560" />

2. กรอกเนื้อหา → กด **Commit changes**
3. ใส่ข้อความสั้นๆ + ชื่อทีม/สมาชิก → **Propose changes**

<img src="assets/gh-commit-message.jpg" alt="กล่อง Commit message แล้วกด Propose changes" width="560" />

## 3) เช็กว่าส่งแล้ว

กดชื่อ repo ทีม → แท็บ **Commits** → เห็น commit ของทีมโผล่ = อัปเดตแล้ว

<img src="assets/gh-verify-commit.jpg" alt="แท็บ Commits เห็น commit ของทีมที่เพิ่งส่ง" width="560" />

## 4) อัปเดต fork — ดึงของใหม่ "โดยงานไม่หาย"

> งานที่ทีม commit เข้า fork ตัวเอง **ไม่หาย** อยู่แล้ว — มันคือคอมมิตของเรา

ปกติ **ไม่ต้อง sync ระหว่างวัน** — fork ครั้งเดียว กรอกงานลง fork พอ
อยากอ่านคู่มือเวอร์ชันล่าสุด? เปิดดูที่ **repo หลัก** ตรงๆ ไม่ต้องดึงเข้า fork

ถ้าพี่เลี้ยงบอกว่ามี **ไฟล์ใหม่** ต้องเอา ทำอย่างปลอดภัย:

**วิธี A (ปลอดภัยสุด — ก็อปไฟล์เดียว):** เปิดไฟล์ใหม่ใน repo หลัก → ก็อปเนื้อหา → ใน fork ทีมสร้างไฟล์ชื่อเดียวกัน → วาง → commit (ไม่ยุ่งกับงานเดิมเลย)

**วิธี B (Sync fork):** หน้า repo ทีม → ปุ่ม **Sync fork** → **Update branch**
- merge ได้ = ได้ของใหม่ + งานเดิมอยู่ครบ ✅
- ⚠️ ถ้าขึ้นปุ่ม **"Discard N commits"** = **อย่ากด!** มันลบงานทีม → ปิดไป ใช้วิธี A หรือเรียกพี่เลี้ยง

## ต้องส่งให้ครบ

- [ ] `team-template/morning/hardware-check.md` — input 3 ตัว + รูป
- [ ] `team-template/afternoon/model.md` — EI link + accuracy + F1 + confusion matrix
- [ ] `team-template/afternoon/predictions.csv` — ≥10 cases
- [ ] รูป/คลิป model รันบนบอร์ด ใน `team-template/assets/`
- [ ] `team-template/idea-canvas.md` — Lean Canvas 4 ช่องแรก
- [ ] ตอบสั้น 3 ข้อใน `team-template/README.md`
