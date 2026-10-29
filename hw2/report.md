# HW2: Polynomial Class

## 解題說明

本作業的目標是實作一個 `Polynomial`（多項式）類別，並多載輸入與輸出運算子。

### 1. 題目要求 (Problems)

   <img width="1554" height="778" alt="image" src="https://github.com/user-attachments/assets/a7734b33-bfb4-491c-9443-f6148c929429" />


  
     

---

### 1-1. 多項式運算(公開介面與抽象資料型態)(Figure 1)

<img width="1098" height="749" alt="image" src="https://github.com/user-attachments/assets/eb39da1b-8f88-4c3c-9c08-8af03468588e" />



* **定義 Polynomial 類別 多項式在數學上表示為一組有序對<c,e> c代表項的係數 e表示項的次方數**
* **實作無參數建構子 Polynomial() 初始化一個p(x)=0 的多項式  實作多項式相加Add()
     相乘Mult() 以及把數值帶入多項式的結果Eval()**
---

### 1-2. 運算子實作(私有資料成員與內部實作)(Figure 2)

<img width="1422" height="1022" alt="image" src="https://github.com/user-attachments/assets/23dd2fe3-bebe-455a-9e9b-735a17644d6a" />



* **宣告Term(項) 其內部包含兩個私有成員 float coef(係數) int exp(次方)**
* **同時透過 friend 將Polynomial 類別宣告為朋友類別 允許Polynomial直接存取這些私有資料**
* **Polynomial 類別的私有成員中 須宣告一指標termArray 該指標指向由 Term 物件所構成的陣列
    用於儲存多項式中的所有非零項**

---
### 1-3.Figure 1 與 Figure 2 結合後的題目說明（對應 Problem 1 與 Problem 2）
   
* **把Figure 2的Term 類別的功能加到Figure 1 中可進行多項式運算**
---

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
```
### 2.2 運算子實作(Figure 2) 程式碼實作:
需要加進類別內的宣告:

```cpp
friend istream& operator>>(istream& is, Polynomial& p);
friend ostream& operator<<(ostream& os, const Polynomial& p);
```

2. 運算子的獨立實作 (放在 class 外面)：
這段程式碼取代了上面為了測試而寫的 Print() 和 SetTerm() 的輸入功能

```cpp
// 多載輸入運算子
istream& operator>>(istream& is, Polynomial& p) {
    p.terms = 0; // 清空原本內容
    int inputTerms;
    cout << "總共有幾項: ";
    is >> inputTerms;
    
    cout << "請依序輸入每項的 [係數] 與 [指數] (中間以空白隔開):\n";
    for (int i = 0; i < inputTerms; i++) {
        float c;
        int e;
        is >> c >> e;
        if (c != 0.0) {
            // 利用現有的 SetTerm 把資料存進陣列
            p.SetTerm(c, e); 
        }
    }
    return is;
}

// 多載輸出運算子
ostream& operator<<(ostream& os, const Polynomial& p) {
    if (p.terms == 0) {
        os << "0";
        return os;
    }
    for (int i = 0; i < p.terms; i++) {
        if (i > 0 && p.termArray[i].coef > 0) {
            os << " + ";
        }
        os << p.termArray[i].coef;
        if (p.termArray[i].exp > 0) {
            os << "x^" << p.termArray[i].exp;
        }
    }
    return os;
}
```
### 2.3 將Figure2 的實作方式加入到 Figure1 
  ```cpp
  #include<iostream>  
#include<cmath> //要在Eval() 中使用pow() 
using namespace std;

 //先宣告Polynimial 先告速編譯器 有這個類別 因為等下  定義 Term 類別 說明Polynomial 是Term 朋友
class Polynomial; 


//定義Term 類別(多項式裡的項)
class Term{   
     
     //friend 讓 Polynomial 可以自由使用 Term 的私有成員
     friend class Polynomial; 
     //在這裡使用istream ostream 是因為 如果你使用 cin>>Term type  cout<<Term type 編譯器沒看過這各類別
     friend istream& operator>>(istream& is,Polynomial& p);  
     friend ostream& operator<<(istream& os,const Polynomial& p);

   private:

      float coef; //係數 (例如 3x^2 的 3)
      int exp;  //指數(次方) (例如 3x^2 的 2)




};


//定義Polynomial 類別(多項式本身)
class Polynomial{

  
    private:
      Term* termArray; // 指向Term 動態陣列的指標
      int capacity;   //陣列的配置容量
      int terms;    //目前陣列中實際的非零項數量

  
  
  public:
     
     Polynomial(); // 建構子(初始化多項式)
     ~Polynomial(); //解構子(釋放動態記憶體)
     Polynomial(const Polynomial& p); //複製建構子
     Polynomial& operator=(const Polynomial& p); //賦值運算子 

     void SetTerm(float c,int e); //新增項數 若容量不夠會自動擴充
     Polynomial Add(Polynomial poly);  //多項式家法
     Polynomial Mult(Polynomial poly); //多項式乘法
     float Eval(float f); //數值帶入計算


    //多載輸入與輸出運算子
     friend istream& operator>>(istream& is,Polynomial& p);
     friend ostream& operator<<(ostream& os,const Polynomial& p);    


}

//實作函式成員

Polynomial::Polynomial():capacity(1),terms(0){
    termArray=new Term[capacity];               //預設建構子 初始容量capacity為1 項數terms為0 
}


Polynomial::~Polynomial(){

   delete[] termArray;  //解構子 釋放動態記憶體

}

//複製建構子 配置新陣列並逐項複製資料
Polynomial::Polynomial(const Polynomial& p):capacity(p.capacity),terms(p.terms){

      termArray=new Term[capacity];
      for(int i=0;i<terms;i++){
        termArray[i]=p.termArray[i];
      }

}

// 賦值運算子：處理物件指派 (如 p2 = p1)
Polynomial& Polynomial::operator=(const Polynomial& p){
       if(this != &p){

          delete[] termArray;
          capacity=p.capacity;
          terms = p.terms;
          termArray=new Term[capacity];

            for(int i=0;i<terms;i++){
                termArray[i]=p.termArray[i];
            }

       }
    
      return *this;
}

//新增項數
void Polynomial::SetTerm(float c,int e){

    if(terms==capacity){  //一旦項數數量已經跟容量一樣 就乘2
      capacity*=2;
      Terms* temp=new Term[capacity];

            for(int  i=0;i<terms;i++){
                temp[i]=termArray[i];  //轉移原有資料
            }

          delete[] termArray;  //釋放舊陣列
          termArray=temp;  //指標指向新陣列


    }
    //寫入新資料 並增加項數
     termArray[terms].coef=c;
    termArray[terms].exp=e;
    terms++;


}

//多項式加法 (指數由大至小排列)
Polynomial Polynomial::Add(Polynomial poly){
   
    Polynomial result;

    int a=0;
    int b=0;
    

    // 依序比較兩多項式的指數大小
    while(a< terms && b<poly.terms){
        if(termArray[a].exp==poly.termArray[b].exp){
           float sum=termArray[a].coef+poly.termArray[b].coef;

           //指數相同 係數相加
           if(sum!=0.0)
             result.SetTerm(sum,termArray[a].exp);
             a++;
             b++;
        }
        else if(termArray[a].exp>poly.termArray[b].exp){
            // 當前項指數較大 直接加入結果陣列
            result.SetTerm(termArray[a].coef,termArray[a].exp);
            a++;
        }
        else{
            // 傳入項指數較大 直接加入結果陣列
            result.SetTerm(poly.termArray[b].coef,poly.termArray[b].exp);
            b++
        }
    }

    //將剩餘項加入結果陣列
        while(a<terms){
             result.SetTerm(termArray[a].coef,termArray[a].exp);
            a++;
        }
        while(b<poly.terms){
             result.SetTerm(poly.termArray[b].coef,poly.termArray[b].exp);
            b++
        }
            
    

   return result;

}

//多項式乘法
Polynomial Polynomial::Mult(Polynomial poly){
   
   Polynomial result;

  //雙層迴圈 執行分配律運算
   for(int i=0;i<terms;i++){
     Polynomial temp;   //存取單項與多項式相乘的結果
       for(int j=0;j<poly.terms;j++){
          
          float c=termArray[i].coef*poly.termArray[j].coef; //係數相乘
          int e=termArray[i].exp+poly.termArray[j].exp;     //指數相加
          temp.SetTerm(c,e);    

       }
     result=result.Add(temp);   //將暫存結果累加至最終結果


   }

  return result;

}

//帶入數值計算值
float Polynomial::Eval(float f){
    
      float sum=0.0;
      for(int i=0;i<terms;i++){
        sum+=termArray[i].coef*pow(f,termArray[i].exp);
      }
    
    return sum;

}


// 多載運算子>>  處理多項式的輸入
istream& operator>>(istream& is,Polynomial& p){

     p.terms=0;  //重至多項式項數
     int inputTerms;

     cout<<"請輸入非零像數量:";
     is>>inputTerms;

     cout<<"依序輸入 係數 與 指數(次方):\n";

     for(int i=0;i<inputTerms;i++){

        float c;
        int e;

        is>>c>>e;

        if(c!=0){  //忽視係數為 0 的無效輸入

            p.SetTerm(c,e);
        }


     }


    return is;  //回傳is 以支援連續輸入(ex:cin>>p1>>p2)

}

// 多載運算子<<  處理多項式的輸出
ostream& operator<<(ostream& os,const Polynomial& p){

       if(p.terms==0){ //若無任何項 輸出0
          os<<"0";
          return os;

       }

    for(int i=0;i<p.terms;i++){
        //格式排版 若不是首項為且係數為正數 補上加號
       if(i>0&&p.termArray[i].coef>0){
           os<<"+";
       }

      os<<p.termArray[i].coef;
      
      //指數大於0 才顯示次方曙
      if(p.termArray[i].exp>0){
        os<<"x^"<<p.termArray[i].exp;
      }      

    }


    return os;  //回傳os 以支援連續輸出(ex: cout<<p1<<p2)

}
```
---
 
