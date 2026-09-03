#include<iostream>
#include<vector>
#include<fstream>
#include<string>
#include<iomanip>
using namespace std;

const string ClientsFileName = "client.txt";
const string UsersFileName = "Users.txt";

struct sClient
{
	string AccountNumber;
	string PinCode;
	string Name;
	string Phone;
	double AccountBalance = 0;
	bool MarkForDelete = false;  
};

struct sUsers
{
	string UserName;
	string Password;
	int Permissions = 0;
	bool MarkForDelete = false;
};

enum enTransactionMenueOption
{
	eDeposit = 1, 
	eWithdraw = 2, 
	eShowTotalBalance = 3, 
	eShowMainMenue = 4 
};

enum enMainMenueOption
{
	eListClients = 1, 
	eAddNewClient = 2, 
	eDeleteClient = 3, 
	eUpdataClient = 4, 
	eFindClient = 5,
	eShowTransactionMenue = 6,
	eMangeUser=7,
	eLogout = 8
};

enum enMainMenuePermission
{
	pAll = -1,
	pListClient = 1,
	pAddClient = 2,
	pDeleteClient = 4,
	pUpdateClient = 8,
	pFindClient = 16,
	pTransactions = 32,
	pMangeUsers = 64,
};

enum enMangeUsersMenueOption
{
	eListUser = 1,
	eAddNewUser = 2,
	eDeleteUser = 3,
	eUpdateUser = 4,
	eFindUser = 5,
	eMainMenue = 6,
};

sUsers CurrentUser;

void ShowMainMenue();  // function decleration
void ShowTransactionMenue();
bool CheckAccessPermission(enMainMenuePermission Permission);
void ShowAccessDeniedMessage();
void ShowMangeUsersMenue();
void Login();

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

sUsers ConvertUserLineToRecord(string s2, string delim = "#//#")
{
	vector<string>vUsersData;
	vUsersData = SplitString(s2, delim);
	sUsers User;
	if (vUsersData.size() >= 3)
	{
		User.UserName = vUsersData[0];
		User.Password = vUsersData[1];
		User.Permissions = stoi(vUsersData[2]);
	}
	return User;
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

string ConvertUserRecordToLine(sUsers User, string Delim = "#//#")
{
	string stUserRecord = "";
	stUserRecord += User.UserName + Delim;
	stUserRecord += User.Password + Delim;
	stUserRecord += to_string(User.Permissions);
	return stUserRecord;
}

vector <sUsers> LoadUsersDataFromFile(string FileName)
{
	fstream File;
	vector<sUsers>vUsers;
	File.open(FileName, ios::in);
	if (File.is_open())
	{
		string Line;
		sUsers Users;
		while (getline(File, Line))
		{
			Users = ConvertUserLineToRecord(Line);
			vUsers.push_back(Users);
		}
		File.close();
	}
	else {
		cout << "\nFile not opened..\n";
	}
	return vUsers;
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

vector<sClient>SaveClientsDataToFile(string FileName, vector<sClient>& vClients)
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

vector<sUsers>SaveUsersDataToFile(string FileName, vector<sUsers>& vUsers)
{
	fstream File;
	File.open(FileName, ios::out);
	if (File.is_open())
	{
		string Line;
		for (sUsers& S : vUsers)
		{
			if (S.MarkForDelete == false)
			{
				Line = ConvertUserRecordToLine(S);
				File << Line << endl;
			}
		}
		File.close();
	}
	return vUsers;
}

short ReadPermissionToSet()
{
	short Permission = 0;

	char Approve = 'Y';
	cout << "\n\nDo you want to give full access? Y/N ? ";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		return -1;
	cout << "\n\nDo you want to give access to: ";

	cout << "\n\nShow Client list? Y/N ?";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		Permission += enMainMenuePermission::pListClient;

	cout << "\nAdd New Client ? Y/N ?";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		Permission += enMainMenuePermission::pAddClient;

	cout << "\nDelete Client ? Y/N ?";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		Permission += enMainMenuePermission::pDeleteClient;

	cout << "\nUpdata Client ? Y/N ?";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		Permission += enMainMenuePermission::pUpdateClient;

	cout << "\nFind Client ? Y/N ?";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		Permission += enMainMenuePermission::pFindClient;

	cout << "\nTransaction? Y/N ?";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		Permission += enMainMenuePermission::pTransactions;

	cout << "\nMange Users? Y/N ?";
	cin >> Approve;
	if (toupper(Approve) == 'Y')
		Permission += enMainMenuePermission::pMangeUsers;

	return Permission;
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

void PrintUsersRecord(sUsers Users)
{
	cout << "| " << setw(15) << left << Users.UserName;
	cout << "| " << setw(15) << left << Users.Password;
	cout << "| " << setw(15) << left << Users.Permissions;
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

bool UsersExistByUserName(string Username)
{
	vector<sUsers>vUsers = LoadUsersDataFromFile(UsersFileName);
	for (sUsers S : vUsers)
	{
		if (S.UserName == Username)
		{
			return true;
		}
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

sUsers ReadNewUser()
{
	sUsers User;
	cout << "Enter Username: ";
	getline(cin >> ws, User.UserName);

	while (UsersExistByUserName(User.UserName))
	{
		cout << "\nUser with [" << User.UserName << "] already exists , Enter a nother Username: ";
		getline(cin >> ws, User.UserName);
	}
	cout << "Enter Password: ";
	getline(cin >> ws, User.Password);

	User.Permissions = ReadPermissionToSet();
	return User;
}

void AddNewClient()
{
	sClient Client;
	Client = ReadNewClient();
	AddDataLineToFile(ClientsFileName, ConvertRecordToLine(Client));
}

void AddNewUser()
{
	sUsers User;
	User = ReadNewUser();
	AddDataLineToFile(UsersFileName, ConvertUserRecordToLine(User));
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

void AddNewUsers()
{
	char AddMore = 'y';
	do {
		cout << "Adding New User: \n\n";
		AddNewUser();
		cout << "\nUser Added Successfully,do you want to add more users? Y/N ? ";
		cin >> AddMore;
	} while (toupper(AddMore) == 'Y');

}

void ShowAddNewClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermission::pAddClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

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

string ReadUserName()
{
	string Username;
	cout << "Please Enter Username: ";
	cin >> Username;
	return Username;
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

void PrintUserDetalis(sUsers& User)
{
	cout << "\n\nThe following are the client fetalis: ";
	cout << "\n-----------------------------------\n";
	cout << "Username  : " << User.UserName << "\n";
	cout << "Password  : " << User.Password << "\n";
	cout << "Permission: " << User.Permissions << "\n";
	cout << "-------------------------------------\n";
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

bool FindUsersByUserNameAndPassword(string UserName, string Password, sUsers& User)
{
	vector<sUsers> vUsers = LoadUsersDataFromFile(UsersFileName);
	for (sUsers U : vUsers)
	{
		if (U.UserName == UserName && U.Password == Password)
		{
			User = U;
			return true;
		}
	}
	return false;
}

bool FindUsersByUserName(string UserName, vector<sUsers>& vUsers, sUsers& User)
{
	for (sUsers S : vUsers)
	{
		if (S.UserName == UserName)
		{
			User = S;
			return true;
		}
	}
	return false;
}

bool FindUserScreen()
{
	sUsers User;
	vector<sUsers>vUsers = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	if (FindUsersByUserName(Username, vUsers, User))
	{
		PrintUserDetalis(User);
		return true;
	}
	else {
		cout << "\nUser with Username [" << Username << "] Not Found.\n";
		return false;
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

bool MarkUserForDeleteByUserName(string Username, vector<sUsers>& vUsers)
{
	for (sUsers& S : vUsers)
	{
		if (S.UserName == Username)
		{
			S.MarkForDelete = true;
			return true;
		}
	}
	return false;
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

bool DeleteUserByUserName(string Username, vector<sUsers>& vUsers)
{
	sUsers User;
	char Answer = 'Y';

	if (Username == "Admin")
	{
		cout << "\n\nYou cannot Delete This User.";
		return false;
	}

	if (FindUsersByUserName(Username, vUsers, User))
	{
		PrintUserDetalis(User);

		cout << "\nAre you sure you want delete this User? Y/N ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			MarkUserForDeleteByUserName(Username, vUsers);
			SaveUsersDataToFile(UsersFileName, vUsers);
			vUsers = LoadUsersDataFromFile(UsersFileName);

			cout << "\n\nUser Deleted successfuly.";
			return true;
		}

	}
	else {
		cout << "\nUser with Username [" << Username << "] Not Found.\n";
		return false;
	}
	return false;
}

void ShowDeleteUserScreen()
{
	cout << "-----------------------------\n";
	cout << "\tDelete User Screen";
	cout << "\n-----------------------------\n";
	vector<sUsers>vUsers = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	DeleteUserByUserName(Username, vUsers);
}

void ShowDeleteClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermission::pDeleteClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

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

sUsers ChangeUserRecord(string Username)
{
	sUsers User;
	User.UserName = Username;
	cout << "Enter Password: ";
	getline(cin >> ws, User.Password);
	User.Permissions = ReadPermissionToSet();
	return User;
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

bool UpdateUserByUsername(string Username, vector<sUsers>& vUsers)
{
	sUsers User;
	char Answer = 'Y';
	if (FindUsersByUserName(Username, vUsers, User))
	{
		PrintUserDetalis(User);

		cout << "\nAre you sure you want update this User? Y/N ? ";
		cin >> Answer;
		if (Answer == 'y' || Answer == 'Y')
		{
			for (sUsers& S : vUsers)
			{
				if (S.UserName == Username)
				{
					S = ChangeUserRecord(Username);
					break;
				}
			}
			SaveUsersDataToFile(UsersFileName, vUsers);
			cout << "\nUser Update Sucessfully..\n";
			return true;
		}

	}
	else {
		cout << "\nUser with Username [" << Username << "] Not Found.\n";
		return false;
	}
	return false;
}

void ShowUpdateUserScreen()
{
	cout << "-----------------------------\n";
	cout << "\tUpdate User Screen";
	cout << "\n-----------------------------\n";
	vector<sUsers>vUsers = LoadUsersDataFromFile(UsersFileName);
	string Username = ReadUserName();
	UpdateUserByUsername(Username, vUsers);
}

void ShowAllUsersScreen()
{
	vector<sUsers>vUsers = LoadUsersDataFromFile(UsersFileName);

	cout << "\n\t\t\t\t\tClient list (" << vUsers.size() << ") client(s).";
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;
	cout << "| " << left << setw(15) << "User Name";
	cout << "| " << left << setw(15) << "Password";
	cout << "| " << left << setw(15) << "Permissions";
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;
	for (sUsers& Users : vUsers)
	{
		PrintUsersRecord(Users);
		cout << endl;
	}
	cout << "\n-----------------------------------------------";
	cout << "-----------------------------------------------\n" << endl;
}

void ShowListUsersScreen()
{
	ShowAllUsersScreen();
}

void ShowAddNewUsersScreen()
{
	AddNewUsers();
}

void ShowUpdataClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermission::pUpdateClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

	cout << "\n-----------------------------------\n";
	cout << "\tUpdata Client Info Screen";
	cout << "\n-----------------------------------\n";

	vector<sClient>vClients = LoadClientsDataFromFile(ClientsFileName);
	string AccountNumber = ReadClientAccountNumber();
	UpdataClientByAccountNumber(AccountNumber, vClients);
}

void ShowFindClientScreen()
{
	if (!CheckAccessPermission(enMainMenuePermission::pFindClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

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

void ShowFindUserScreen()
{
	cout << "-----------------------------\n";
	cout << "\tFind User Screen";
	cout << "\n-----------------------------\n";
	FindUserScreen();
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

bool CheckAccessPermission(enMainMenuePermission Permission)
{
	if (CurrentUser.Permissions == enMainMenuePermission::pAll)
		return true;
	if ((CurrentUser.Permissions & Permission) == Permission)
		return true;
	else
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

	while (!FindClientByAccountNumber(AccountNumber, vClients, Client))  // valadite
	{
		cout << "\nClient With [" << AccountNumber << "] does not exist.\n";
		AccountNumber = ReadClientAccountNumber();
	}

	PrintClientCard(Client);

	double Amount = 0;
	cout << "\nPlease enter tWithdraw amount: ";
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

void ShowAccessDeniedMessage()
{
	system("cls");
	system("color 4f");
	cout << "\n------------------------------------\n";
	cout << "Access Denied, \nYou dont have permission to do this, \nPlease conact your admin.";
	cout << "\n------------------------------------\n";
	system("pause");
	system("color 07");
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
	if (!CheckAccessPermission(enMainMenuePermission::pTransactions))
	{
		ShowAccessDeniedMessage();
		return;
	}

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

void ShowAllClientsScreen()
{
	if (!CheckAccessPermission(enMainMenuePermission::pListClient))
	{
		ShowAccessDeniedMessage();
		return;
	}

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

short ReadMainMenueOption()
{
	cout << "Choose what do you want to do? [1 to 8]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

short ReadMangeUsersMenueOption()
{
	cout << "Choose what do you want to do? [1 to 6]? ";
	short Choice = 0;
	cin >> Choice;

	return Choice;
}

void GoBackToMainMenue()
{
	cout << "\n\nPress any key to go back to Main menue.....";
	system("pause>0");
	ShowMainMenue();
}

void GoBackToMangeUsersMenue()
{
	cout << "\n\nPress any key to go back to Main menue.....";
	system("pause>0");
	ShowMangeUsersMenue();
}

void PerformMangeUsersMenueOption(enMangeUsersMenueOption MangeUsersMenueOption)
{
	switch (MangeUsersMenueOption)
	{
	case enMangeUsersMenueOption::eListUser:
		system("cls");
		ShowListUsersScreen();
		GoBackToMangeUsersMenue();
		break;

	case enMangeUsersMenueOption::eAddNewUser:
		system("cls");
		ShowAddNewUsersScreen();
		GoBackToMangeUsersMenue();
		break;

	case enMangeUsersMenueOption::eDeleteUser:
		system("cls");
		ShowDeleteUserScreen();
		GoBackToMangeUsersMenue();
		break;

	case enMangeUsersMenueOption::eUpdateUser:
		system("cls");
		ShowUpdateUserScreen();
		GoBackToMangeUsersMenue();
		break;

	case enMangeUsersMenueOption::eFindUser:
		system("cls");
		ShowFindUserScreen();
		GoBackToMangeUsersMenue();
		break;

	case enMangeUsersMenueOption::eMainMenue:
		ShowMainMenue();
		break;
	}
}

void ShowMangeUsersMenue()
{
	if (!CheckAccessPermission(enMainMenuePermission::pMangeUsers))
	{
		ShowAccessDeniedMessage();
		return;
	}

	system("cls");
	cout << "=================================================\n";
	cout << "\t\tMange Users Menue Screen\n";
	cout << "=================================================\n";
	cout << "\t[1] List Users.\n";
	cout << "\t[2] Add New User.\n";
	cout << "\t[3] Delete User.\n";
	cout << "\t[4] Update User.\n";
	cout << "\t[5] Find User.\n";
	cout << "\t[6] Main Menue.\n";
	cout << "=====================================\n";
	PerformMangeUsersMenueOption(enMangeUsersMenueOption(ReadMangeUsersMenueOption()));
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
		ShowTransactionMenue();  
		break;

	case enMainMenueOption::eMangeUser:

		system("cls");
		ShowMangeUsersMenue();
		break;

	case::enMainMenueOption::eLogout:
		system("cls");
		Login();
	}
}

void ShowMainMenue()  
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
	cout << "\t[7] Mange Users.\n";
	cout << "\t[8] Logout.\n";
	cout << "=====================================\n";

	PerformMainMenueOption((enMainMenueOption)ReadMainMenueOption());  // casting
}

bool LoadUserInfo(string Username, string Password)
{
	if (FindUsersByUserNameAndPassword(Username, Password, CurrentUser))
		return true;
	else
		return false;
}

void Login()
{
	bool LoginFaild = false;
	string Username, Password;
	do
	{
		system("cls");
		cout << "\n-----------------------------\n";
		cout << "\tLogin Screen.";
		cout << "\n-----------------------------\n";

		if (LoginFaild)
		{
			cout << "Invalid Username/Password!\n";
		}
		cout << "Enter Username: ";
		cin >> Username;
		cout << "Enter Password: ";
		cin >> Password;

		LoginFaild = !LoadUserInfo(Username, Password);

	} while (LoginFaild);

	ShowMainMenue();
}

int main()
{
	Login();
	system("pause>0");
	return 0;
}






