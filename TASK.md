# Phân chia công việc – CO3009 Two-Road Traffic Light
 
Tài liệu nội bộ của nhóm, dùng trong quá trình làm bài. Nguồn: thông báo và hướng dẫn BTL CO3009 (29/10/2026).
 
- **Hạn nộp:** 04/12/2026 (nên làm xong trước 02/12)
- **Demo:** tuần 15/16, lịch thầy sẽ báo sau
- **Phạm vi file này:** phần **code theo module**. Báo cáo PDF tạm hoãn, nhóm trưởng quyết định sau.
---
 
## 1. Nguyên tắc chia việc
 
1. **Chia theo module (file), không chia theo step.** Mỗi người sở hữu các file riêng nên ít xung đột khi merge, và commit history thể hiện rõ ai làm gì (10% điểm là tổ chức code và commit).
2. **Mỗi người tự viết và tự test phần của mình** trước, rồi mới ghép. Đề yêu cầu test từng tầng độc lập: phần cứng → scheduler → module hỗ trợ → state machine.
3. **Chốt tên hàm giữa các module từ tuần 1** để không ai phải chờ ai.
4. **State machine chiếm 30% điểm** nên cả nhóm cùng thiết kế sơ đồ. Thầy sẽ hỏi từng người, ai cũng phải giải thích được toàn hệ thống dù không phải người code phần đó.
---
 
## 2. Ba mảng công việc và nhóm trưởng
 
| Thành viên | Mảng | Nội dung tóm tắt |
|---|---|---|
| [Tên] | **A – Lõi hệ thống** | Project nền, scheduler, software timer, `fsm_automatic`, chuyển mode, tích hợp |
| [Tên] | **B – Đèn và nút** | Test đèn và nút, `traffic_light`, `button`, `fsm_manual` |
| [Tên] | **C – Cảm biến và màn hình** | Quét I2C, `dht20`, `lcd`, `fsm_tuning` |
| Nhóm trưởng | **Việc ngoài code** | Xem mục 7 |
 
> Nhóm trưởng cũng có thể nhận một mảng code. Vì còn việc ngoài code nên nên nhận mảng nhẹ hơn (B hoặc C), mảng A là nặng nhất.
 
---
 
## 3. Chi tiết từng mảng
 
### Mảng A – Lõi hệ thống
 
**File phụ trách:** `scheduler.h/.c`, `software_timer.h/.c`, `global.h/.c`, `fsm_automatic.h/.c`, phần chuyển mode, `main.c` (phần khởi tạo).
 
| Việc | Chi tiết |
|---|---|
| Project nền | Tạo project trong STM32CubeIDE từ **Board Selector → NUCLEO-F103RB**; **SYS → Debug = Serial Wire** (sai thì board chỉ nạp được một lần); cấu hình GPIO, I2C1, **TIM2 ngắt 10 ms**; push lên `dev` sớm |
| Tính timer | `T = (PSC + 1) × (ARR + 1) / f_TIM`. Xem f_TIM thật ở tab Clock Configuration rồi tính lại PSC, ARR. Ví dụ f_TIM = 8 MHz, PSC = 7999, ARR = 9 cho T = 10 ms |
| Scheduler | `SCH_Init`, `SCH_Add_Task`, `SCH_Delete_Task`, `SCH_Update`, `SCH_Dispatch_Tasks`. Danh sách sắp theo thời gian chạy, mỗi task lưu **Delay tương đối** so với task trước. `SCH_Update` **O(1)**, chỉ xử lý task đầu danh sách. Hỗ trợ **one-shot** (`Period = 0`). Chọn mảng hay linked list và ghi lại lý do |
| Software timer | `setTimer(i, duration)` và cờ `timer_flag[i]`, cập nhật bởi một task chu kỳ 10 ms |
| `fsm_automatic` | Các trạng thái kết hợp hai đường (Red1–Green2, Red1–Amber2, Green1–Red2, Amber1–Red2), chuyển trạng thái khi timer hết hạn |
| Chuyển mode | State machine chọn giữa Auto / Manual / Tuning, khởi tạo lại timer và task khi đổi mode, **hai đường không bao giờ cùng xanh** |
| Tích hợp | Ghép các nhánh vào `dev`, chạy thử toàn hệ thống |
 
**Cách test (theo đề):**
```c
SCH_Init();
SCH_Add_Task(toggle_PA5, 0, 50);    // 500 ms (tick 10 ms)
SCH_Add_Task(toggle_PC9, 0, 100);   // 1000 ms
SCH_Add_Task(one_shot_test, 300, 0);// chạy một lần sau 3 s
```
 
- [ ] Hai LED nháy đúng 0,5 s và 1 s khi chạy cùng lúc
- [ ] Task one-shot chạy đúng một lần rồi bị xóa khỏi danh sách
- [ ] `SCH_Delete_Task` xóa được task bất kỳ mà các task còn lại vẫn đúng giờ
- [ ] Thêm task với nhiều Delay khác nhau (chèn ở đầu, giữa, cuối danh sách)
- [ ] Không còn `HAL_Delay` trong chương trình chính
### Mảng B – Đèn và nút
 
**File phụ trách:** `traffic_light.h/.c`, `button.h/.c`, `fsm_manual.h/.c`.
 
| Việc | Chi tiết |
|---|---|
| Test đèn (Step 3.2) | Thử 4 tổ hợp `(0,0) (0,1) (1,0) (1,1)` cho từng module đèn, ghi lại tổ hợp nào là Xanh, Vàng, Đỏ, Tắt |
| Test nút (Step 3.3) | Đọc 4 nút (D7–D10) và nút USER (PC13), xác định mức logic khi nhấn là 0 hay 1 |
| Bảng map chân | Tra schematic, điền bảng chân D3–D10 sang chân STM32 (D3–D4: đèn 1, D5–D6: đèn 2, D7–D8: nút module 1, D9–D10: nút module 2) |
| `traffic_light` | Hàm đặt trạng thái từng đường, ví dụ `traffic_set(road, color)`, dùng bảng tổ hợp ở trên |
| `button` | Đọc nút mỗi 10 ms, **chống dội** (so sánh nhiều lần đọc liên tiếp), nhận diện nhấn và **nhấn giữ**, có hàm `isButtonPressed(i)` |
| `fsm_manual` | Người dùng đổi trạng thái đèn bằng nút |
 
- [ ] Một lần nhấn không bị tính nhiều lần do dội
- [ ] Mỗi đường điều khiển được qua hàm của module
- [ ] Nhấn nút liên tục không làm hỏng trạng thái
- [ ] Không có lời gọi HAL trong `fsm_manual.c`
### Mảng C – Cảm biến và màn hình
 
**File phụ trách:** `dht20.h/.c`, `lcd.h/.c`, `fsm_tuning.h/.c`.
 
| Việc | Chi tiết |
|---|---|
| Quét I2C (Step 3.4) | Dùng `HAL_I2C_IsDeviceReady` quét địa chỉ 1–127, phải tìm thấy **2 thiết bị**. Ghi lại địa chỉ DHT20 (thường là 0x38, quét để xác nhận) và địa chỉ LCD (khác nhau giữa các module). Nhớ **dịch trái 1 bit** địa chỉ 7-bit khi gọi hàm HAL |
| `dht20` | Giao tiếp I2C, đọc và quy đổi nhiệt độ, độ ẩm theo datasheet. Đọc theo chu kỳ hợp lý (ví dụ 2 s), không đọc liên tục |
| `lcd` | Khởi tạo, xóa màn hình, đặt con trỏ, in chuỗi và số |
| `fsm_tuning` | Chọn đèn cần chỉnh, tăng hoặc giảm thời lượng, xác nhận; LCD hiển thị giá trị đang chỉnh. Phải định nghĩa **ràng buộc hợp lệ** giữa Đỏ, Xanh, Vàng và cách xử lý **giá trị không hợp lệ** |
 
> Mẹo: trong task không dùng `HAL_Delay`. Với DHT20 cần chờ sau khi ra lệnh đo thì dùng một task **one-shot** để đọc kết quả sau thời gian chờ, thay vì đứng chờ.
 
- [ ] I2C quét ra đủ 2 thiết bị, đã ghi lại địa chỉ
- [ ] LCD hiện nhiệt độ và độ ẩm, cập nhật định kỳ
- [ ] Nhập giá trị thời gian không hợp lệ thì được xử lý đúng
- [ ] Không có lời gọi HAL trong `fsm_tuning.c`
---
 
## 4. Hợp đồng giữa các module (đề xuất, nhóm chốt trong tuần 1)
 
Ghi vào file `.h` tương ứng để người khác dùng ngay khi chưa có code bên trong. Tên dưới đây là gợi ý, nhóm đổi được miễn là thống nhất.
 
```c
/* global.h */
typedef enum { OFF, RED, GREEN, AMBER } light_color_t;
extern int status;                       // trạng thái hiện tại của hệ thống
extern int time_red, time_green, time_amber;
 
/* software_timer.h  (Mảng A) */
void setTimer(int index, int duration);  // duration tính theo tick 10 ms
extern int timer_flag[];
 
/* traffic_light.h  (Mảng B) */
void traffic_set(int road, light_color_t color);   // road = 1 hoặc 2
 
/* button.h  (Mảng B) */
void button_read(void);                  // chạy như task 10 ms
int  isButtonPressed(int index);
int  isButtonLongPressed(int index);
 
/* dht20.h  (Mảng C) */
void  dht20_read(void);
float dht20_get_temperature(void);
float dht20_get_humidity(void);
 
/* lcd.h  (Mảng C) */
void lcd_init(void);
void lcd_clear(void);
void lcd_set_cursor(int row, int col);
void lcd_print(const char *str);
```
 
**Quy tắc kiến trúc (đề bắt buộc):**
- Chỉ `main.c` và các module Step 5 (software timer, button, traffic light, DHT20, LCD) được gọi HAL.
- Các file `fsm_*.c` **không** được gọi `HAL_GPIO_...` hay `HAL_I2C_...`; mọi thao tác đi qua hàm của module.
- Biến dùng chung (trạng thái, thời lượng Đỏ/Xanh/Vàng) khai báo trong `global.h/global.c`.
- Mỗi state machine là một cặp `.h/.c` riêng, hàm chạy state machine được gọi như một task của scheduler.
---
 
## 5. Lịch làm việc
 
| Thời gian | Mảng A | Mảng B | Mảng C | Kết quả cần có |
|---|---|---|---|---|
| **Tuần 1** 03–09/11 | Tạo project nền, push lên `dev` (khoảng 05/11) | Tra chân, test đèn và nút | Quét I2C | LED nháy, bảng tổ hợp đèn, mức logic nút, địa chỉ DHT20 và LCD |
| **Tuần 2** 10–16/11 | Viết scheduler | Viết `traffic_light`, `button` | Viết `dht20`, `lcd` | Scheduler đúng; driver chạy độc lập (test tạm) |
| **Tuần 3** 17–23/11 | `software_timer`; chạy module qua scheduler | Chạy `button`, `traffic_light` thành task | Chạy `dht20`, `lcd` thành task | Mỗi module có demo nhỏ chạy trên scheduler; merge vào `dev` |
| **Tuần 4** 24–30/11 | `fsm_automatic`, chuyển mode, tích hợp | `fsm_manual` | `fsm_tuning` | Hệ thống chạy đủ 3 mode |
| **01–04/12** | Cả nhóm | Cả nhóm | Cả nhóm | Test độ ổn định và trường hợp biên; merge `main`, tag, nộp |
 
Trong **tuần 1**, B và C phải chờ project nền của A. Lúc chờ nên đọc datasheet DHT20, xem schematic để tra chân, và cùng cả nhóm vẽ sơ đồ state machine nháp.
 
**Sơ đồ state machine (cả nhóm làm, tuần 2–3).** Vẽ trước khi code, bằng draw.io hoặc giấy. Cần có 3 máy trạng thái và máy trạng thái chuyển mode. Với mỗi lần chuyển mode phải trả lời được:
1. Ngay sau khi chuyển, đèn hai đường ở trạng thái nào?
2. Timer và task được khởi tạo lại thế nào?
3. LCD hiển thị gì?
4. Làm sao đảm bảo hai đường không bao giờ xanh cùng lúc?
---
 
## 6. Quy trình Git
 
Chi tiết xem `CONTRIBUTING.md`. Tóm tắt:
 
```
nhánh feature của A ─┐
nhánh feature của B ─┼─► dev ─► main (một lần duy nhất lúc nộp, tag v1.0-final)
nhánh feature của C ─┘
```
 
| Mảng | Nhánh gợi ý |
|---|---|
| A | `feature/scheduler`, `feature/software-timer`, `feature/fsm-automatic` |
| B | `feature/traffic-light`, `feature/button`, `feature/fsm-manual` |
| C | `feature/dht20`, `feature/lcd`, `feature/fsm-tuning` |
 
- **Commit nhiều lần** trong mỗi giai đoạn, mỗi commit một thay đổi có ý nghĩa (ví dụ `button: add debouncing`), bằng **tài khoản GitHub của chính mình**.
- **Merge vào `dev`** khi phần đó chạy đúng, thường vào cuối mỗi tuần. Báo trong nhóm Zalo để mọi người `git pull origin dev`.
- Trước khi merge vào `dev`, pull `dev` về nhánh của mình để xử lý xung đột ngay trên nhánh mình.
- **Không sửa thẳng file của người khác**; thấy lỗi thì báo để họ sửa trên nhánh của họ.
- **`main.c` là file ai cũng đụng vào** nên dễ xung đột. Code test tạm để ở file riêng (như `button_test.c`) hoặc chỉ giữ trên nhánh feature, đừng merge vào `dev`. Chỉ thêm vài dòng khởi tạo và đăng ký task vào `main.c` khi ghép ở tuần 3.
---
 
## 7. Việc của nhóm trưởng (ngoài code)
 
- [ ] Add hai thành viên vào repo (**Settings → Collaborators**), nhắc chấp nhận lời mời
- [ ] Đăng ký nhóm và link GitHub trên LMS
- [ ] Chốt phân công, quy ước đặt tên, hợp đồng module ở mục 4
- [ ] Theo dõi tiến độ theo lịch ở mục 5, họp ngắn cuối mỗi tuần
- [ ] Ghép nhánh vào `dev` và chạy thử
- [ ] Cập nhật README: mục Usage (chức năng nút ở từng mode) và thông tin nộp bài
- [ ] Lúc nộp: merge `dev` vào `main`, gắn tag `v1.0-final`, nộp lên LMS
- [ ] Hỏi thầy hoặc xem nhóm Zalo các chỗ đề còn bỏ trống (thời gian chạy ổn định tối thiểu, quy định nộp trễ, lịch demo)
- [ ] Báo cáo PDF: tạm hoãn, quyết định sau
---
 
## 8. Việc chung của cả nhóm
 
- Vẽ sơ đồ state machine (mục 5).
- Test độ ổn định (chạy liên tục) và các trường hợp biên: nhấn nút liên tục, đổi mode khi đèn đang Vàng, nhập thời gian không hợp lệ.
- Kiểm tra hai đường không bao giờ cùng xanh.
- Chuẩn bị demo: mỗi người phải giải thích được toàn bộ hệ thống, không chỉ phần mình.
---
 
## 9. Rủi ro cần để ý
 
| Rủi ro | Cách phòng |
|---|---|
| Board chỉ nạp được một lần | Làm đúng Board Selector và Debug = Serial Wire ngay từ đầu; nếu hỏng, xem video khôi phục trong đề hoặc liên hệ thầy |
| Chỉ có một board, nhiều người cần dùng | Sắp lịch dùng board; scheduler và sơ đồ state machine làm được không cần board |
| Chương trình treo sau một lúc | Kiểm tra tràn mảng task, task thêm liên tục mà không xóa, còn sót `HAL_Delay` trong task |
| Một lần nhấn bị tính nhiều lần | Xem lại cơ chế chống dội và việc xóa cờ nút sau khi xử lý |
| LCD không hiện gì | Quét lại I2C xác nhận địa chỉ, nhớ dịch trái 1 bit |
| Đèn sai màu | Xem lại bảng tổ hợp tín hiệu và thứ tự chân trong `.ioc` |
| Task chạy sai chu kỳ | Kiểm tra đã gọi `HAL_TIM_Base_Start_IT`, đã bật ngắt trong NVIC, PSC/ARR đúng với f_TIM |
| Xung đột khi merge `main.c` | Xem quy tắc ở mục 6 |