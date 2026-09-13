#include<iostream>
using namespace std;
class Delegate{
    string name, country;
    int age;
    public:
        void input(){
            cout<<"Enter Delegate Name: ";
            getline(cin,name);
            cout<<"Enter Age: ";
            cin>>age;
            cin.ignore();
            cout<<"Enter Country: ";
            getline(cin,country);
        }
        void display(){
            cout<<"----Delegate Details----"<<endl;
            cout<<"Name: "<<name<<endl;
            cout<<"Age: "<<age<<endl;
            cout<<"Country: "<<country<<endl;
        }
};

int main(){
   Delegate d;  

    d.input();
    d.display();
    return 0;
}