#include<iostream>
#include<vector>
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
            cout<<"Name: "<<name<<endl;
            cout<<"Age: "<<age<<endl;
            cout<<"Country: "<<country<<endl;
            cout<<endl<<endl;
        }
};
vector<Delegate> delegates;

int main(){
    int ch;
    do{
        cout<<"----Delegate Management----"<<endl;
        cout<<"1. Add Delegate\n2.Display Delegates\n3.Exit\n Enter the choice: ";
        cin>>ch;
        cin.ignore();
        Delegate d;  
        switch(ch){
            case 1:
                d.input();
                delegates.push_back(d);
                break;
            case 2:
            cout<<"-----Delegates Details-----"<<endl;
                for(Delegate d:delegates){
                d.display();
                }
                break;
            
        }
    }
    while(ch<=2);
    return 0;
}