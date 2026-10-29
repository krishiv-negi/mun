#include <iostream>
#include"committeemenu.h"
#include"Delegate.cpp"


using namespace std;

int main()
{
    int choice;

    do
    {
cout << "+--------------------------------------------------+\n";
cout << "|              MUN MANAGEMENT SYSTEM               |\n";
cout << "+--------------------------------------------------+\n";
cout << "| S.no |            OPERATION                      |\n";
cout << "+------+-------------------------------------------+\n";
cout << "|  1   |  Delegate Management                      |\n";
cout << "|  2   |  CV Information                           |\n";
cout << "|  3   |  Committee & portfolio Management         |\n";
cout << "|  4   |  Delegate Allocation                      |\n";
cout << "|  0   |  Exit                                     |\n";
cout << "+------+-------------------------------------------+\n";
        cout << "Enter your choice: ";
        cin >> choice;

        switch (choice)
        {
            case 1:
                delegateMenu();
                break;
            case 2:
                cvMenu();
                break;
            case 3:
                committeeMenu();
                break;
            case 4:
                allocationMenu();
                break;
            case 0:
                cout << "Exiting program...\n";
                break;

            default:
                cout << "Invalid choice!\n";
        }

    } while (choice != 0);

    return 0;
}