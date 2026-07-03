#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<iomanip>
using namespace std;

const string ClientsFileName = "client.txt";

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkForDelete = false;  
};

enum enTransactionMenueOption{eDeposit = 1, eWithdraw = 2, eShowTotalBalance = 3, eShowMainMenue = 4 };

enum enMainMenueOption{eListClients = 1, eAddNewClient = 2, eDeleteClient = 3, eUpdataClient = 4, eFindClient = 5, eShowTransactionMenue = 6, eExit = 7 };

void ShowMainMenue();  // function decleration
void ShowTransactionMenue();  //..

vector<string>SplitString(string s1, string delim)
{
	vector<string>vString;
	string sWord;
	short pos = 0;
	while ((pos = s1.find(delim)) != std::string::npos) {
		sWord = s1.substr(0, pos);
		if (sWord != "") {
			vString.push_back(sWord);
		}
		s1.erase(0, pos + delim.length());
	}
	if (s1 != "") {
		vString.push_back(s1);
	}
	return vString;
}

sClient ConvertLineToRecord(string s1, string delim = "#//#")
{
	vector<string>vClientData;
	vClientData = SplitString(s1, delim);
	sClient Client;

	if (vClientData.size() >= 5) {

		Client.AccountNumber = vClientData[0];
		Client.PinCode = vClientData[1];
		Client.Name = vClientData[2];
		Client.Phone = vClientData[3];
		Client.AccountBalance = stod(vClientData[4]);
	}
	return Client;
}

string ConvertRecordToLine(sClient Client, string Delim = "#//#")
{
	string stClientRecord = "";
	stClientRecord += Client.AccountNumber + Delim;
	stClientRecord += Client.PinCode + Delim;
	stClientRecord += Client.Name + Delim;
	stClientRecord += Client.Phone + Delim;
	stClientRecord += to_string(Client.AccountBalance);

	return stClientRecord;
}

vector<sClient>LoadClientsDataFromFile(string FileName)
{
	vector<sClient>vClients;
	fstream MyFile;

	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string line;
		sClient Client;

		while (getline(MyFile, line)) {
			Client = ConvertLineToRecord(line);
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	else {
		cout << "File not opined\n";
	}
	return vClients;
}

void AddDataLineToFile(string FileName,string stDataLine)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out | ios::app);

	if (MyFile.is_open())
	{
		MyFile << stDataLine << endl;
		MyFile.close();
	}
}

void PrintClientRecordLine(sClient Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(10) << left << Client.PinCode;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.Phone;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowAllClientsScreen()
{
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient list (" << vClients.size() << ") client(s).";
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(10) << "Pin Code";
	cout << "| " << left << setw(40) << "Name";
	cout << "| " << left << setw(12) << "Phone";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;

	if (vClients.size() == 0) {
		cout << "\t\t\tNo Clients Available In The System ...!\n";
	}
	else {
		for (sClient& Client : vClients) {

			PrintClientRecordLine(Client);
			cout << endl;
		}
	}
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;
}


bool ClientExistsByAccountNumber(string AccountNumber,string FileName)
{
	vector<sClient>vClients;
	fstream MyFile;
	MyFile.open(FileName, ios::in);
	if (MyFile.is_open()) {
		string Line;
		sClient Client;
		while(getline(MyFile, Line)){
			Client = ConvertLineToRecord(Line);

			if (Client.AccountNumber == AccountNumber) {
				MyFile.close();
				return true;
			}
			vClients.push_back(Client);
		}
		MyFile.close();
	}
	return false;
}

sClient ReadNewClient()
{
	sClient Client;
	cout << "Enter Account Number: ";
	getline(cin >> ws, Client.AccountNumber);

	while(ClientExistsByAccountNumber(Client.AccountNumber, ClientsFileName)) {
		cout << "\nClient with [" << Client.AccountNumber << "] already exists,..Enter another account: ";
		getline(cin >> ws, Client.AccountNumber);
		cout << endl;
	}

	cout << "Enter Pin Code: ";
	getline(cin >> ws, Client.PinCode);
	cout << "Enter The Name: ";
	getline(cin >> ws, Client.Name);
	cout << "Enter The Phone: ";
	getline(cin >> ws, Client.Phone);
	cout << "Enter Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;
}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewClients()
{
	char AddMore = 'Y';
	do {
		cout << "Adding New Client:\n\n";
		AddNewClient();
		cout << "\nClient Add Successfully, do you want to add more clients? Y/N: ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');
}

void ShowAddNewClientsScreen()
{
	cout << "\n---------------------------------\n";
	cout << "\tAdd New Clients Screen";
	cout << "\n---------------------------------\n";
	AddNewClients();
}

string ReadClientAccountNumber()
{
	cout << "Please Enter Account Number: ";
	string AccountNumber;
	cin >> AccountNumber;

	return AccountNumber;
}

void PrintClientCard(sClient&Client)
{
	cout << "\nThe following are the client details:\n";
	cout << "-----------------------------------";
	cout << "\nAccout Number  : " << Client.AccountNumber;
	cout << "\nPin Code       : " << Client.PinCode;
	cout << "\nName           : " << Client.Name;
	cout << "\nPhone          : " << Client.Phone;
	cout << "\nAccount Balance: " << Client.AccountBalance;
	cout << "\n-----------------------------------\n";
}

bool FindClientByAccountNumber(string AccountNumber,vector<sClient>&vClients,sClient& Client)
{
	for (sClient C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			Client = C;
			return true;
		}
	}
	return false;
}

bool MarkClientForDeleteByAccountNumber(string AccountNumber,vector<sClient>&vClients)
{
	for (sClient&C : vClients) {
		if (C.AccountNumber == AccountNumber) {
			C.MarkForDelete = true;
			return true;
		}
	}
	return false;
}

vector<sClient>SaveClientsDataToFile(string FileName,vector<sClient>&vClients)
{
	fstream MyFile;
	MyFile.open(FileName, ios::out);
	if (MyFile.is_open()) {
		string stDataLine;

		for (sClient C : vClients) {
			if (C.MarkForDelete == false) {
				stDataLine = ConvertRecordToLine(C);
				MyFile << stDataLine << endl;
			}
		}
		MyFile.close();
	}
	return vClients;
}


bool DeleteClientByAccountNumber(string AccountNumber,vector<sClient>&vClients)
{
	sClient Client;
	char Answer = 'Y';
	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

		PrintClientCard(Client);

		cout << "\n\nAre you sure you want delete this client? Y/N ? ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y') {
			MarkClientForDeleteByAccountNumber(AccountNumber, vClients);
			SaveClientsDataToFile(ClientsFileName, vClients);


			vClients = LoadClientsDataFromFile(ClientsFileName);
			cout << "\n\nClient Deleted successfuly.";
			return true;
		}
	}
	else {
		cout << "\nClient With Account Number [" << AccountNumber << "] Not Found.";
		return false;
	}
	return false;
}

void ShowDeleteClientScreen()
{
	cout << "-----------------------------\n";
	cout << "\tDelete Client Screen";
	cout << "\n-----------------------------\n";

	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	DeleteClientByAccountNumber(AccountNumber, vClients);

}


sClient ChangeClientRecord(string AccountNumber)
{
	sClient Client;
	Client.AccountNumber = AccountNumber;

	cout << "\nEnter The Pin Code: ";
	getline(cin>>ws, Client.PinCode);
	cout << "Enter The Name: ";
	getline(cin >> ws, Client.Name);
	cout << "Enter The Phone: ";
	getline(cin >> ws, Client.Phone);
	cout << "Enter the Account Balance: ";
	cin >> Client.AccountBalance;

	return Client;
}

bool UpdataClientByAccountNumber(string AccountNumber, vector<sClient>&vClients)
{
	char Answer = 'n';
	sClient Client;

	if (FindClientByAccountNumber(AccountNumber, vClients, Client)) {

		PrintClientCard(Client);
		cout << "\n\nAre you sure you want update this client? (Y/N) : ";
		cin >> Answer;
		if (Answer == 'Y' || Answer == 'y') {

			for (sClient &C : vClients) {

				if (C.AccountNumber == AccountNumber)
				{
					C = ChangeClientRecord(AccountNumber);
					break;
				}
			}
			SaveClientsDataToFile(ClientsFileName, vClients);
			cout << "\n\nClient Update Successfuly..\n";
			return true;
		}
	}
	else {
		cout << "\n\nClient With Account Number [" << AccountNumber << "] Not Found.\n";
		return false;
	}
	return false;
}

void ShowUpdataClientScreen()
{
	cout << "\n-----------------------------------\n";
	cout << "\tUpdata Client Info Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdataClientByAccountNumber(AccountNumber, vClients);
}


void ShowFindClientScreen()
{
	cout << "\n------------------------------------\n";
	cout << "\tFind Client Screen";
	cout << "\n------------------------------------\n";

	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	sClient Client;
	if (FindClientByAccountNumber(AccountNumber, vClients,Client)) {
		PrintClientCard(Client);
	}
	else {
		cout << "\n\nClient With Account Number [" << AccountNumber << "] Is Not Found.\n";
	}

}

void ShowEndScreen()
{
	cout << "\n------------------------------\n";
	cout << "\tProgram Ends :-)";
	cout << "\n------------------------------\n";
}


bool DepositBalanceToClientByAccountNumber(string AccountNumber,double Amount, vector<sClient>&vClients)
{
	char Answer = 'Y';
	cout << "\n\nAre you sure you want perform this transaction? (Y/N) ?  ";
	cin >> Answer;
	if (Answer == 'Y' || Answer == 'y')
	{
		for (sClient &C : vClients)
		{
			if (C.AccountNumber == AccountNumber)
			{
				C.AccountBalance += Amount;
				SaveClientsDataToFile(ClientsFileName, vClients);
				cout << "\n\nDone Successfully,New balance is: " << C.AccountBalance << endl;
				return true;
			}
		}
	}
	return false;
}

void ShowDepositScreen()
{
	cout << "\n----------------------------------\n";
	cout << "\tDeposit Screen";
	cout << "\n----------------------------------\n";

	sClient Client;
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);
	double Amount = 0;
	cout << "\nPlease enter deposit amount: ";
	cin >> Amount;
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount, vClients);
}


void ShowWithdrawScreen()
{
	cout << "\n----------------------------------\n";
	cout << "\tWithdraw Screen";
	cout << "\n----------------------------------\n";

	sClient Client;
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter deposit amount: ";
	cin >> Amount;

	//Validate that the amount does not exceeds the balance 
	while (Amount > Client.AccountBalance)
	{
		cout << "\nAmount Exceeds the balance,you can withdraw up to : " << Client.AccountBalance << endl;
		cout << "Please enter another amount: ";
		cin >> Amount;
	}
	DepositBalanceToClientByAccountNumber(AccountNumber, Amount * -1, vClients);

}


void PrintClientRecordBalanceLine(sClient& Client)
{
	cout << "| " << setw(15) << left << Client.AccountNumber;
	cout << "| " << setw(40) << left << Client.Name;
	cout << "| " << setw(12) << left << Client.AccountBalance;
}

void ShowTotalBalances()
{
	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);

	cout << "\n\t\t\t\t\tClient list (" << vClients.size() << ") client(s).";
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;
	cout << "| " << left << setw(15) << "Account Number";
	cout << "| " << left << setw(40) << "Client Name";
	cout << "| " << left << setw(12) << "Balance";
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;

	double TotalBalance = 0;

	if (vClients.size() == 0) {
		cout << "\t\t\tNo Clients Available In The System ...!\n";
	}
	else {
		for (sClient& Client : vClients) {

			PrintClientRecordBalanceLine(Client);
			TotalBalance += Client.AccountBalance;
			cout << endl;
		}
	}
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;
	cout << "\t\t\t\t\tTotal Balance = " << TotalBalance << endl;
}

void ShowTotalBalancesScreen()
{
	ShowTotalBalances();
}


void GoBackTransactionsMenue()
{
	cout << "\n\nPress any key to go back to transactions menue....";
	system("pause>0");
	ShowTransactionMenue();
}

short ReadTransactionMenueOption()
{
	short Answer;
	cout << "Choose what do you want to do? [1:4]? ";
	cin >> Answer;
	return Answer;
}

void PerformTransactionMenueOption(enTransactionMenueOption TransactionMenueOption)
{
	switch (TransactionMenueOption)
	{
	case enTransactionMenueOption::eDeposit: 

		system("cls");
		ShowDepositScreen();
		GoBackTransactionsMenue();
		break;

	case enTransactionMenueOption::eWithdraw:

		system("cls");
		ShowWithdrawScreen();
		GoBackTransactionsMenue();
		break;

	case enTransactionMenueOption::eShowTotalBalance:

		system("cls");
		ShowTotalBalancesScreen();
		GoBackTransactionsMenue();
		break;

	case enTransactionMenueOption::eShowMainMenue:

		ShowMainMenue();
	
	}
}

void ShowTransactionMenue()
{
	system("cls");
	cout << "\n===========================================\n";
	cout << "\t\tTransactions Menue Screen";
	cout << "\n===========================================\n";
	cout << "\t[1] Deposit.\n";
	cout << "\t[2] Withdraw.\n";
	cout << "\t[3] Total Balance.\n";
	cout << "\t[4] Main Menue.\n";
	cout << "=============================================\n";
	PerformTransactionMenueOption((enTransactionMenueOption)ReadTransactionMenueOption());
}


void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main menue.....";
	system("pause>0");
	ShowMainMenue();
}

short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 7]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

void PerformMainMenueOption(enMainMenueOption MainMenueOption)
{
	switch (MainMenueOption)
	{
	case::enMainMenueOption::eListClients:

		system("cls");
		ShowAllClientsScreen();
		GoBackToMainMenue();
		break;

	case::enMainMenueOption::eAddNewClient:

		system("cls");
		ShowAddNewClientsScreen();
		GoBackToMainMenue();
		break;

	case::enMainMenueOption::eDeleteClient:

		system("cls");
		ShowDeleteClientScreen();
		GoBackToMainMenue();
		break;

	case::enMainMenueOption::eUpdataClient:

		system("cls");
		ShowUpdataClientScreen();
		GoBackToMainMenue();
		break;

	case::enMainMenueOption::eFindClient:

		system("cls");
		ShowFindClientScreen();
		GoBackToMainMenue();
		break;

	case enMainMenueOption::eShowTransactionMenue:

		system("cls");
		ShowTransactionMenue();   // ?????
		break;

	case::enMainMenueOption::eExit:

		system("cls");
		ShowEndScreen();
	}
}

void ShowMainMenue()  // function definition
{
	system("cls");  
	cout << "=====================================\n";
	cout << "\t\tMain Menue Screen\n";
	cout << "=====================================\n";
	cout << "\t[1] Show Client List.\n";
	cout << "\t[2] Add New Client.\n";
	cout << "\t[3] Delete Client.\n";
	cout << "\t[4] Update Client Info.\n";
	cout << "\t[5] Find Client.\n";
	cout << "\t[6] Transactions.\n";
	cout << "\t[7] Exit.\n";
	cout << "=====================================\n";

	PerformMainMenueOption((enMainMenueOption)ReadMainMenueOption());  // casting

}

int main()
{
	ShowMainMenue();

	return 0;
}






