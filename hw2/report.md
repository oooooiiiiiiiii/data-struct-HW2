# HW2: Polynomial Class

## 解題說明

本作業的目標是實作一個 `Polynomial`（多項式）類別，並多載輸入與輸出運算子。

### 1. 題目要求 (Problems)

   <img width="1554" height="778" alt="image" src="https://github.com/user-attachments/assets/a7734b33-bfb4-491c-9443-f6148c929429" />


* **核心任務**：實作 `Polynomial` 類別，其抽象資料型別 (ADT) 與私有資料成員分別定義於 Figure 1 與 Figure 2。
* **運算子多載**：撰寫 C++ 函式來處理多項式的輸入與輸出（依照 Figure 2 的資料表示方式），必須多載 `<<` 與 `>>` 運算子。

---

### 1-1. 公開介面 (Public Interface - Figure 1)

<img width="1422" height="1022" alt="image" src="https://github.com/user-attachments/assets/2d6e09ed-ab13-4640-89e7-069be292a1a1" />



根據 Figure 1 的 ADT 定義，需要實作以下公開成員函式：
* `Polynomial()`：建構子，用來初始化一個空的多項式 p(x) = 0。
* `Polynomial Add(Polynomial poly)`：執行多項式加法，回傳 `*this` 與 `poly` 相加的結果。
* `Polynomial Mult(Polynomial poly)`：執行多項式乘法，回傳 `*this` 與 `poly` 相乘的結果。
* `float Eval(float f)`：計算多項式在代入數值 `f` 後的運算結果並回傳。

---

### 1-2. 私有成員 (Private Data Members - Figure 2)

<img width="1422" height="1022" alt="image" src="https://github.com/user-attachments/assets/23dd2fe3-bebe-455a-9e9b-735a17644d6a" />



根據 Figure 2 的定義，資料結構的設計如下：
* **`Term` 類別**：用來表示多項式中的每一個非零項。
  * `coef` (float)：係數 (coefficient)。
  * `exp` (int)：指數 (exponent)。
* **`Polynomial` 私有成員**：
  * `termArray` (Term*)：指向非零項陣列的指標。
  * `capacity` (int)：`termArray` 的最大容量。
  * `terms` (int)：目前多項式中非零項的數量。
