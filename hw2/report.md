# HW2: Polynomial Class

## 解題說明

本作業的目標是實作一個 `Polynomial`（多項式）類別，並多載輸入與輸出運算子。

### 1. 題目要求 (Problems)

   <img width="1554" height="778" alt="image" src="https://github.com/user-attachments/assets/a7734b33-bfb4-491c-9443-f6148c929429" />


* **核心任務**：實作 `Polynomial` 類別，其抽象資料型別 (ADT) 與私有資料成員分別定義於 Figure 1 與 Figure 2。
* **運算子多載**：撰寫 C++ 函式來處理多項式的輸入與輸出（依照 Figure 2 的資料表示方式），必須多載 `<<` 與 `>>` 運算子。

---

### 1-1. 多項式運算(Figure 1)

<img width="1422" height="1022" alt="image" src="https://github.com/user-attachments/assets/2d6e09ed-ab13-4640-89e7-069be292a1a1" />



根據 Figure 1 的 ADT 定義，需要實作以下公開成員函式：
* `Polynomial()`：建構子，用來初始化一個空的多項式 p(x) = 0。
* `Polynomial Add(Polynomial poly)`：執行多項式加法，回傳 `*this` 與 `poly` 相加的結果。
* `Polynomial Mult(Polynomial poly)`：執行多項式乘法，回傳 `*this` 與 `poly` 相乘的結果。
* `float Eval(float f)`：計算多項式在代入數值 `f` 後的運算結果並回傳。

---

### 1-2. 運算子實作 (Figure 2)

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

### 2.1   多項式運算(Figure 1) 程式碼實作:

```cpp
#include <iostream>
#include <cmath> // 給 Eval 的 pow() 使用
using namespace std;

class Polynomial; // 前置宣告

// 依照 Figure 2 實作 Term 類別
class Term {
    friend class Polynomial;
private:
    float coef; // 係數
    int exp;    // 指數
};

// 依照 Figure 1 與 Figure 2 實作 Polynomial 類別
class Polynomial {
private:
    Term* termArray; // 非零項陣列
    int capacity;    // 容量
    int terms;       // 非零項數量

public:
    Polynomial(); // 建構子
    ~Polynomial(); // 解構子 (釋放動態記憶體)
    Polynomial(const Polynomial& p); // 複製建構子
    Polynomial& operator=(const Polynomial& p); // 賦值運算子

    Polynomial Add(Polynomial poly); // 相加
    Polynomial Mult(Polynomial poly); // 相乘
    float Eval(float f); // 代入運算

    // 為了在沒有 cin>> 的情況下測試第一題，新增一個手動加入項目的函式
    void SetTerm(float c, int e);
    // 為了在沒有 cout<< 的情況下測試第一題，新增一個陽春版印出函式
    void Print() const;
};

// --- 成員函式實作 ---

Polynomial::Polynomial() : capacity(1), terms(0) {
    termArray = new Term[capacity];
}

Polynomial::~Polynomial() {
    delete[] termArray;
}

Polynomial::Polynomial(const Polynomial& p) : capacity(p.capacity), terms(p.terms) {
    termArray = new Term[capacity];
    for (int i = 0; i < terms; i++) {
        termArray[i] = p.termArray[i];
    }
}

Polynomial& Polynomial::operator=(const Polynomial& p) {
    if (this != &p) {
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

void Polynomial::SetTerm(float c, int e) {
    if (terms == capacity) {
        capacity *= 2;
        Term* temp = new Term[capacity];
        for (int i = 0; i < terms; i++) temp[i] = termArray[i];
        delete[] termArray;
        termArray = temp;
    }
    termArray[terms].coef = c;
    termArray[terms].exp = e;
    terms++;
}

Polynomial Polynomial::Add(Polynomial poly) {
    Polynomial result;
    int a = 0, b = 0;
    while (a < terms && b < poly.terms) {
        if (termArray[a].exp == poly.termArray[b].exp) {
            float sum = termArray[a].coef + poly.termArray[b].coef;
            if (sum != 0.0) result.SetTerm(sum, termArray[a].exp);
            a++; b++;
        } else if (termArray[a].exp > poly.termArray[b].exp) {
            result.SetTerm(termArray[a].coef, termArray[a].exp);
            a++;
        } else {
            result.SetTerm(poly.termArray[b].coef, poly.termArray[b].exp);
            b++;
        }
    }
    while (a < terms) {
        result.SetTerm(termArray[a].coef, termArray[a].exp);
        a++;
    }
    while (b < poly.terms) {
        result.SetTerm(poly.termArray[b].coef, poly.termArray[b].exp);
        b++;
    }
    return result;
}

Polynomial Polynomial::Mult(Polynomial poly) {
    Polynomial result;
    for (int i = 0; i < terms; i++) {
        Polynomial temp;
        for (int j = 0; j < poly.terms; j++) {
            float c = termArray[i].coef * poly.termArray[j].coef;
            int e = termArray[i].exp + poly.termArray[j].exp;
            temp.SetTerm(c, e);
        }
        result = result.Add(temp);
    }
    return result;
}

float Polynomial::Eval(float f) {
    float sum = 0.0;
    for (int i = 0; i < terms; i++) {
        sum += termArray[i].coef * pow(f, termArray[i].exp);
    }
    return sum;
}

void Polynomial::Print() const {
    if (terms == 0) { cout << "0\n"; return; }
    for (int i = 0; i < terms; i++) {
        cout << termArray[i].coef << "x^" << termArray[i].exp;
        if (i < terms - 1) cout << " + ";
    }
    cout << endl;
}

// 第一題測試 main
int main() {
    Polynomial p1, p2, p3;
    
    // 手動建構 p1 = 3x^2 + 2x^1
    p1.SetTerm(3, 2);
    p1.SetTerm(2, 1);
    
    // 手動建構 p2 = 1x^1 + 5x^0
    p2.SetTerm(1, 1);
    p2.SetTerm(5, 0);
    
    cout << "p1 = "; p1.Print();
    cout << "p2 = "; p2.Print();
    
    p3 = p1.Add(p2);
    cout << "p1 + p2 = "; p3.Print();
    
    p3 = p1.Mult(p2);
    cout << "p1 * p2 = "; p3.Print();
    
    cout << "p1 代入 x=2 的結果: " << p1.Eval(2) << endl;

    return 0;
}

### 2.2  運算子實作(Figure 2) 程式碼實作:
       需要加進類別內的宣告:
              ```cpp
              friend istream& operator>>(istream& is, Polynomial& p);
              friend ostream& operator<<(ostream& os, const Polynomial& p);
      將這兩行放入 Term 與 Polynomial 的 class 宣告裡面（因為它們需要存取私有變數）
     
