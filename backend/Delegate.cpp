#include<iostream>
#include<vector>
using namespace std;
class Delegate{
    string name, country;
    int age, num, mun_attend, num1;
    vector<string> prev_committees;
    vector<string>awards;
    public:
        void input(){
            string committee;
            cout<<"Enter Delegate Name: ";
            getline(cin,name);
            cout<<"Enter Age: ";
            cin>>age;
            cin.ignore();
            cout<<"Enter Country: ";
            getline(cin,country);
            cout<<"How many previous committees attends? : ";
            cin>>num;
            cin.ignore();
            cout<<"Enter Previous Committees name: ";
            for(int i=0; i<num; i++){
            getline(cin,committee);
            prev_committees.push_back(committee);
        }
        cout<<"How many MUN have you attended?: ";
        cin>>mun_attend;
        cout<<"How many awards have you received?: ";
        cin>>num1;
        cin.ignore();
        for(int i=0;i<num1; i++){
            string award;
            cout<<"Enter award: "<<i+1<<":";
            getline(cin,award);
            awards.push_back(award);
        }
        }
        void display(){
            int i=0;
            cout<<"Name: "<<name<<endl;
            cout<<"Age: "<<age<<endl;
            cout<<"Country: "<<country<<endl;
            cout<<"Previous Committee: "<<endl;
            for(string c: prev_committees){
                cout<<i+1<<". ";
            cout<< c <<" "<<endl;
            i++;
            }
            cout<<"MUN Attended: "<<mun_attend;
            cout<<endl<<endl;
            cout<<"Awards: "<<endl;
            i=0;
            for(string award:awards){
                cout<<i+1<<". "<<award<<" "<<endl;
            }
            cout<<endl;
            }     
        void search();
        void update();
        };

vector<Delegate> delegates;

void Delegate:: search(){
            string s;
            int c=0;
            cout<<"\nEnter Delegate Name: ";
            getline(cin,s);
            for(Delegate d: delegates){
                if(d.name==s){
                    cout<<"\nName Found!\n"<<endl;
                     c=1;
                     int i=0;
                cout<<"Name: "<<d.name<<endl;
                cout<<"Age: "<<d.age<<endl;
                cout<<"Country: "<<d.country<<endl;
                cout<<"Previous Committee: "<<endl;
                for(string c: d.prev_committees){
                cout<<i+1<<". ";
                 cout<< c <<" "<<endl;
                i++;
                 }
                cout<<"MUN Attended: "<<d.mun_attend;
                cout<<endl<<endl;
                cout<<"Awards: "<<endl;
                i=0;
                for(string award:d.awards){
                cout<<i+1<<". "<<award<<" "<<endl;
                }
                 
                    
                }
                
            }
            if(c==0){
                cout<<"\nName Not Found!\n"<<endl;
            }

}
void Delegate:: update(){
     string s;
     int c=0;
            cout<<"\nEnter Delegate Name: ";
            getline(cin,s);
            for(Delegate &d: delegates){
                if(d.name==s){
                    int ch,n;
                    c=1;
                    cout<<"\nName Found!\n"<<endl;
                    cout<<"What do you want to update?"<<endl<<"1.Age"<<endl
                        <<"2.Country"<<endl<<"3.Previous Committees"<<endl
                        <<"4.MUN Attended"<<endl<<"5.Awards"<<endl<<"Enter choice: ";
                        cin>>ch;
                    switch(ch){
                        case 1:
                            cout<<"Enter new Age: ";
                            cin>>d.age;
                            break;
                        case 2:
                            cout<<"Enter new Country: ";
                            cin>>d.country;
                            break;
                        case 3:
                            cout<<"How many Previous committees?: ";
                            cin>>n;
                            d.prev_committees.resize(n);
                            for(int i=0; i<n; i++){
                                cout<<"Enter Previous Committee "<<i+1<<": ";
                                cin>>d.prev_committees[i];
                            }
                            break;
                        case 4:
                            cout<<"Enter new MUN Attended: ";   
                            cin>>d.mun_attend;
                            break;
                        case 5:
                            cout<<"How many Awards?: ";
                            cin>>n;
                            d.awards.resize(n);
                            for(int i=0; i<n; i++){
                                cout<<"Enter Award "<<i+1<<": ";
                                cin>>d.awards[i];
                            }
                            break; 
                     }
                 }
            }if(c==0){
                cout<<"\nName Not Found!\n"<<endl;
            }
}
int main(){
    int ch;
    do{
        cout<<"----Delegate Management----"<<endl;
        cout<<"1. Add Delegate\n2.Display Delegate\n3.Search Delegate\n 4.Update Delegate\n5.Exit\nEnter the choice: ";
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
            case 3:
                d.search();
                break;
            case 4:
                d.update();
                break;
            
        }
    }
    while(ch<=4);
    return 0;
}