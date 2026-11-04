#include<iostream>  
#include<cstdlib> 
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
   friend ostream& operator<<(ostream& os,const Polynomial& p);

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


};

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
    Term* temp=new Term[capacity];

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
          b++;
      }
  }

  //將剩餘項加入結果陣列
      while(a<terms){
           result.SetTerm(termArray[a].coef,termArray[a].exp);
          a++;
      }
      while(b<poly.terms){
           result.SetTerm(poly.termArray[b].coef,poly.termArray[b].exp);
          b++;
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



int main(void){

   system("chcp 65001"); //設定編碼為UTF-8

   Polynomial p1,p2,p3;
   float x;

   
  cout << "--- 設定多項式 p1 ---\n";
  cin >> p1;
  
  cout << "\n--- 設定多項式 p2 ---\n";
  cin >> p2;

  cout << "\n[輸入確認]\n";
  cout << "p1(x) = " << p1 << "\n"; 
  cout << "p2(x) = " << p2 << "\n";

  p3 = p1.Add(p2);
  cout << "\n p1 + p2 = " << p3 << "\n";

  p3 = p1.Mult(p2);
  cout << "p1 * p2 = " << p3 << "\n";

  cout << "\n輸入要代入 p1 的數值 x: ";
  cin >> x;
  cout << "p1(" << x << ") = " << p1.Eval(x) << "\n";

  return 0;

}
