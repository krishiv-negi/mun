#include "addDelegate.cpp"
#include "dispDelegate.cpp"
#include "updateDelegate.cpp"
#include "searchDelegate.cpp"
#include"deleteDelegate.cpp"

vector<Delegate> delegates;

int main(){
    int ch;
    do{
        cout<<"----Delegate Management----"<<endl;
        cout<<"1. Add Delegate\n2.Display Delegate\n3.Search Delegate\n4.Update Delegate\n5.Delete Delegate\n6.Exit\nEnter the choice: ";
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
                display();
                break;
            case 3:
                search();
                break;
            case 4:
                update();
                break;
            case 5:
                deleteDelegate();
                break;
        }
    }
    while(ch<6);
    return 0;
}