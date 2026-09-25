#include<iostream>
#include<memory>
#include<string>
#include<vector>
#include<mysql/jdbc.h>
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

       try {
    sql::mysql::MySQL_Driver* driver =
        sql::mysql::get_mysql_driver_instance();

    std::unique_ptr<sql::Connection> con(
        driver->connect(
            "tcp://127.0.0.1:3306",
            "root",
            "admin"
        )
    );

    con->setSchema("mun_management");
    std::unique_ptr<sql::PreparedStatement> pstmt(
        con->prepareStatement(
            "INSERT INTO delegates "
            "(name, age, country, mun_attend) "
            "VALUES (?, ?, ?, ?)"
        )
    );

    pstmt->setString(1, name);
    pstmt->setInt(2, age);
    pstmt->setString(3, country);
    pstmt->setInt(4, mun_attend);

    pstmt->executeUpdate();


    int delegate_id = 0;

    std::unique_ptr<sql::PreparedStatement> idStmt(
        con->prepareStatement(
            "SELECT delegate_id FROM delegates "
            "WHERE name = ? AND age = ? AND country = ? "
            "AND mun_attend = ? "
            "ORDER BY delegate_id DESC LIMIT 1"
        )
    );

    idStmt->setString(1, name);
    idStmt->setInt(2, age);
    idStmt->setString(3, country);
    idStmt->setInt(4, mun_attend);

    std::unique_ptr<sql::ResultSet> result(
        idStmt->executeQuery()
    );

    if (result->next()) {
        delegate_id = result->getInt("delegate_id");
        cout<<"Delegate_id:"<<delegate_id<<endl;
    }
    

    std::unique_ptr<sql::PreparedStatement> committeeStmt(
        con->prepareStatement(
            "INSERT INTO previous_committees (delegate_id, committee_name) VALUES (?, ?)"
        )
    );

    for (string committee : prev_committees) {

        committeeStmt->setInt(1, delegate_id);
        committeeStmt->setString(2, committee);
        committeeStmt->executeUpdate();

        cout<<"Committee SAved:"<<committee<<endl;
    }

    std::unique_ptr<sql::PreparedStatement> awardStmt(
        con->prepareStatement(
            "INSERT INTO award (delegate_id, award_name) VALUES (?, ?)"
        )
    );
    for (string award : awards) {
        awardStmt->setInt(1, delegate_id);
        awardStmt->setString(2, award);
        awardStmt->executeUpdate();
        cout<<"Award saved:"<<award<<endl;
    }
    cout << "Delegate saved to database successfully!" << endl;
}
catch (sql::SQLException &e) {
    cout << "Database Error: " << e.what() << endl;
}
        }
         
        void search();
        void update();
        void deleteDelegate();
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
            }else{
                cout<<"Successfully updated ! "<<endl;
            }
}
void Delegate:: deleteDelegate(){
    string s;
    int c=0,c1;
    cout<<"\nEnter Delegate Name: ";
    getline(cin,s);
    for(auto it=delegates.begin(); it!=delegates.end(); ++it){
        if(it->name==s){
            cout<<"\nName Found!\n"<<endl;
            cout<<"Are you sure you want to delete this delegate? (1 for Yes, 0 for No): ";
            cin>>c1;
            if(c1==1){
                delegates.erase(it);
                cout<<"Successfully Deleted!"<<endl;
            }c=1;
            break;

        }
    }if(c==0)
        cout<<"Name Not Found!\n";
    
}
void display(){
    try {
        sql::mysql::MySQL_Driver* driver =
            sql::mysql::get_mysql_driver_instance();

        std::unique_ptr<sql::Connection> con(
            driver->connect("tcp://127.0.0.1:3306", "root", "admin")
        );

        con->setSchema("mun_management");

        std::unique_ptr<sql::Statement> stmt(con->createStatement());
        std::unique_ptr<sql::ResultSet> res(
            stmt->executeQuery("SELECT * FROM delegates")
        );

        while (res->next()) {
            int delegate_id = res->getInt("delegate_id");
            std::string name = res->getString("name");
            int age = res->getInt("age");
            std::string country = res->getString("country");
            int mun_attend = res->getInt("mun_attend");

            std::cout << "Delegate ID: " << delegate_id << "\n";
            std::cout << "Name: " << name << "\n";
            std::cout << "Age: " << age << "\n";
            std::cout << "Country: " << country << "\n";
            std::cout << "MUN Attended: " << mun_attend << "\n";

            // Committees
            std::unique_ptr<sql::PreparedStatement> commStmt(
                con->prepareStatement("SELECT committee_name FROM previous_committees WHERE delegate_id = ?")
            );
            commStmt->setInt(1, delegate_id);
            std::unique_ptr<sql::ResultSet> commRes(commStmt->executeQuery());
            std::cout << "Previous Committees:\n";
            int i = 1;
            while (commRes->next()) {
                std::cout << i++ << ". " << commRes->getString("committee_name") << "\n";
            }

            // Awards
            std::unique_ptr<sql::PreparedStatement> awardStmt(
                con->prepareStatement("SELECT award_name FROM award WHERE delegate_id = ?")
            );
            awardStmt->setInt(1, delegate_id);
            std::unique_ptr<sql::ResultSet> awardRes(awardStmt->executeQuery());
            std::cout << "Awards:\n";
            i = 1;
            while (awardRes->next()) {
                std::cout << i++ << ". " << awardRes->getString("award_name") << "\n";
            }

            std::cout << "-----------------------------\n";
        }
    }
    catch (sql::SQLException &e) {
        std::cout << "Database Error: " << e.what() << std::endl;
    }
}

int main(){
    int ch;
    do{
        cout<<"----Delegate Management----"<<endl;
        cout<<"1. Add Delegate\n2.Display Delegate\n3.Search Delegate\n 4.Update Delegate\n5.Delete Delegate\n6.Exit\nEnter the choice: ";
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
                d.search();
                break;
            case 4:
                d.update();
                break;
            case 5:
                d.deleteDelegate();
                break;
            
        }
    }
    while(ch<6);
    return 0;
}