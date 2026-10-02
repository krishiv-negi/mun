#include <iostream>
#include "class.h"


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