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


## 2. Algorithm Design & Programming 

### 2.1  公開介面 (Public Interface - Figure 1) 程式碼實作:

```cpp
#include <iostream>
#include <cmath> // 提供 pow() 函數給 Eval 使用
using namespace std;

class Polynomial; // 前置宣告

// ==========================================
// 定義 Term 類別
// ==========================================
class Term {
    friend class Polynomial;
    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);
private:
    float coef; // 係數
    int exp;    // 指數
};

// ==========================================
// 定義 Polynomial 類別
// ==========================================
class Polynomial {
private:
    Term* termArray;
    int capacity;
    int terms;

    // 輔助函式：用來新增項目並處理陣列擴充
    void NewTerm(float c, int e);

public:
    Polynomial();
    
    // 【重要補充】
    // 因為這份作業的 Add 和 Mult 都是用「傳值回傳 (Return by Value)」
    // 當類別內有指標動態配置時，必須補上以下三個 (Rule of Three)，否則程式結束時會發生記憶體崩潰
    ~Polynomial(); // 解構子：負責釋放記憶體
    Polynomial(const Polynomial& p); // 複製建構子
    Polynomial& operator=(const Polynomial& p); // 賦值運算子

    Polynomial Add(Polynomial poly); // 相加
    Polynomial Mult(Polynomial poly); // 相乘
    float Eval(float f); // 代入數值計算

    friend istream& operator>>(istream& is, Polynomial& p);
    friend ostream& operator<<(ostream& os, const Polynomial& p);
};

// ==========================================
// 實作 Polynomial 成員函式
// ==========================================

// 建構 p(x) = 0
Polynomial::Polynomial() : capacity(1), terms(0) {
    termArray = new Term[capacity];
}

// 解構子
Polynomial::~Polynomial() {
    delete[] termArray;
}

// 複製建構子
Polynomial::Polynomial(const Polynomial& p) : capacity(p.capacity), terms(p.terms) {
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) {
        termArray[i] = p.termArray[i];
    }
}

// 賦值運算子
Polynomial& Polynomial::operator=(const Polynomial& p) {
    if (this != &p) { // 避免自我賦值
        delete[] termArray;
        capacity = p.capacity;
        terms = p.terms;
        termArray = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            termArray[i] = p.termArray[i];
        }
    }
    return *this;
}

// 新增項目與動態擴充陣列
void Polynomial::NewTerm(float c, int e) {
    if (terms == capacity) {
        capacity *= 2; // 容量不足時翻倍
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; i++) {
            temp[i] = termArray[i]; // 搬移舊資料
        }
        delete[] termArray; // 刪除舊陣列
        termArray = temp; // 指向新陣列
    }
    termArray[terms].coef = c;
    termArray[terms].exp = e;
    terms++;
}

// 兩個多項式相加 (假設輸入的項目都已經依據指數由大到小排序)
Polynomial Polynomial::Add(Polynomial poly) {
    Polynomial result;
    int a = 0, b = 0;

    // 同時走訪兩個多項式的陣列
    while (a < terms && b < poly.terms) {
        if (termArray[a].exp == poly.termArray[b].exp) {
            float sum = termArray[a].coef + poly.termArray[b].coef;
            if (sum != 0.0) result.NewTerm(sum, termArray[a].exp);
            a++; b++;
        } else if (termArray[a].exp > poly.termArray[b].exp) {
            result.NewTerm(termArray[a].coef, termArray[a].exp);
            a++;
        } else {
            result.NewTerm(poly.termArray[b].coef, poly.termArray[b].exp);
            b++;
        }
    }
    // 把剩餘的項目補上
    while (a < terms) {
        result.NewTerm(termArray[a].coef, termArray[a].exp);
        a++;
    }
    while (b < poly.terms) {
        result.NewTerm(poly.termArray[b].coef, poly.termArray[b].exp);
        b++;
    }
    return result;
}

// 兩個多項式相乘
Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial result;
    for (int i = 0; i < terms; i++) {
        Polynomial temp; // 暫存每一項相乘的結果
        for (int j = 0; j < poly.terms; j++) {
            float c = termArray[i].coef * poly.termArray[j].coef;
            int e = termArray[i].exp + poly.termArray[j].exp;
            temp.NewTerm(c, e);
        }
        result = result.Add(temp); // 把暫存結果加進總結果中
    }
    return result;
}

// 代入數值計算
float Polynomial::Eval(float f) {
    float sum = 0.0;
    for (int i = 0; i < terms; i++) {
        sum += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return sum;
}

// ==========================================
// 實作全域輸入與輸出運算子 (第二題)
// ==========================================
istream& operator>>(istream& is, Polynomial& p) {
    p.terms = 0; // 歸零，準備讀取新資料
    int inputTerms;
    cout << "總共有幾項: ";
    is >> inputTerms;
    cout << "請「由高次方到低次方」依序輸入每項的 [係數] 與 [指數] (中間以空白隔開):\n";
    for (int i = 0; i < inputTerms; i++) {
        float c;
        int e;
        is >> c >> e;
        if (c != 0.0) {
            p.NewTerm(c, e);
        }
    }
    return is;
}

ostream& operator<<(ostream& os, const Polynomial& p) {
    if (p.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < p.terms; i++) {
        // 如果不是第一項，且係數是正的，補上加號
        if (i > 0 && p.termArray[i].coef > 0) {
            os << " + ";
        }
        os << p.termArray[i].coef; // 印出係數
        if (p.termArray[i].exp > 0) { // 如果指數大於0才印出 x^
            os << "x^" << p.termArray[i].exp;
        }
    }
    return os;
}

// ==========================================
// 主程式 Main
// ==========================================
int main() {
    Polynomial p1, p2, p3;
    float x;

    cout << "--- 建立第一個多項式 p1 ---\n";
    cin >> p1;
    
    cout << "\n--- 建立第二個多項式 p2 ---\n";
    cin >> p2;

    cout << "\n您輸入的多項式為:\n";
    cout << "p1(x) = " << p1 << endl;
    cout << "p2(x) = " << p2 << endl;

    // 測試 Add
    p3 = p1.Add(p2);
    cout << "\n【相加結果】 p1 + p2 = " << p3 << endl;

    // 測試 Mult
    p3 = p1.Mult(p2);
    cout << "【相乘結果】 p1 * p2 = " << p3 << endl;

    // 測試 Eval
    cout << "\n請輸入要代入 p1 的數值 x: ";
    cin >> x;
    cout << "p1(" << x << ") = " << p1.Eval(x) << endl;

    return 0;
}

