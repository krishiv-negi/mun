#include"class.h"

void search()
{
    try
    {
        sql::mysql::MySQL_Driver* driver;
driver = sql::mysql::get_mysql_driver_instance();

con = driver->connect(
    "tcp://127.0.0.1:3306",
    "root",
    "admin"
);

con->setSchema("mun_management");
        if (con == nullptr)
        {
            cout << "\nDatabase connection is not available!\n";
            return;
        }

        int id;

        cout << "\nEnter Delegate ID: ";
        cin >> id;

        sql::PreparedStatement* pstmt =
            con->prepareStatement(
                "SELECT delegate_id, name, age, country, mun_attend "
                "FROM delegates "
                "WHERE delegate_id = ?"
            );

        pstmt->setInt(1, id);

        sql::ResultSet* res = pstmt->executeQuery();

        if (!res->next())
        {
            cout << "\nDelegate not found!\n";

            delete res;
            delete pstmt;

            return;
        }

        cout << "\n========== DELEGATE DETAILS ==========\n";
        cout << "Delegate ID: " << res->getInt("delegate_id") << endl;
        cout << "Name: " << res->getString("name") << endl;
        cout << "Age: " << res->getInt("age") << endl;
        cout << "Country: " << res->getString("country") << endl;
        cout << "MUN Attended: " << res->getInt("mun_attend") << endl;

        delete res;
        delete pstmt;

        pstmt =
            con->prepareStatement(
                "SELECT committee_name "
                "FROM previous_committees "
                "WHERE delegate_id = ?"
            );

        pstmt->setInt(1, id);

        res = pstmt->executeQuery();

        cout << "\nPrevious Committees:\n";

        int count = 1;

        while (res->next())
        {
            cout << count++ << ". "
                 << res->getString("committee_name")
                 << endl;
        }

        if (count == 1)
        {
            cout << "No previous committees.\n";
        }

        delete res;
        delete pstmt;

        pstmt =
            con->prepareStatement(
                "SELECT award_name "
                "FROM award "
                "WHERE delegate_id = ?"
            );

        pstmt->setInt(1, id);

        res = pstmt->executeQuery();

        cout << "\nAwards:\n";

        count = 1;

        while (res->next())
        {
            cout << count++ << ". "
                 << res->getString("award_name")
                 << endl;
        }

        if (count == 1)
        {
            cout << "No awards.\n";
        }

        delete res;
        delete pstmt;

        cout << "\n======================================\n";
    }

    catch (sql::SQLException& e)
    {
        cout << "\nDatabase Error: " << e.what() << endl;
        cout << "Error Code: " << e.getErrorCode() << endl;
        cout << "SQL State: " << e.getSQLState() << endl;
    }
}