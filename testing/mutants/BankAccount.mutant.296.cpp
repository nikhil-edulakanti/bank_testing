#include <iostream>
#include <vector>
#include <string>
#include <ctime>

using namespace std;

// Enum for different account types
enum AccountType
{
    SAVINGS,
    CHECKING,
    INVALID
};

// Transaction struct to store transaction details
struct Transaction
{
    string timestamp;
    string type; // "deposit" or "withdrawal"
    double amount;
};

class BankAccount
{
private:
    string accountNumber;
    string accountHolderName;
    AccountType type;
    double balance;
    double overdraftLimit; // Only applicable for CHECKING accounts
    bool isOpen;
    vector<Transaction> transactionHistory;

public:
    BankAccount(string accNumber, string accHolderName, AccountType accType, double initialBalance, float overdraft = 0)
    {
        accountNumber = accNumber;
        accountHolderName = accHolderName;
        type = accType;
        balance = initialBalance;
        overdraftLimit = overdraft;
        isOpen = true;
    }

    string deposit(double amount)
    {
        if (!isOpen)
        {
            return "Account is closed.";
        }
        if (amount <= 0)
        {
            return "Invalid amount for deposit.";
        }
        balance += amount;
        recordTransaction("deposit", amount);
        return "Deposit of $" + to_string(amount) + " successful.";
    }

    string withdraw(double amount)
    {
        if (!isOpen)
        {
            return "Account is closed.";
        }
        if (amount <= 0)
        {
            return "Invalid amount for withdrawal.";
        }
        if (type == CHECKING && balance - amount < -overdraftLimit)
        {
            return "Withdrawal exceeds overdraft limit.";
        }
        if (balance >= amount)
        {
            balance -= amount;
            recordTransaction("withdrawal", amount);
            return "Withdrawal of $" + to_string(amount) + " successful.";
        }
        else
        {
            return "Insufficient balance.";
        }
    }

    double getBalance()
    {
        return balance;
    }

    string displayAccountInfo()
    {
        string info = "Account Number: " + accountNumber + "\n";
        info += "Account Holder Name: " + accountHolderName + "\n";
        info += "Account Type: " + (type == SAVINGS ? string("Savings") : string("Checking")) + "\n";
        info += "Balance: $" + to_string(balance) + "\n";
        info += "Account Status: " + (isOpen ? string("Open") : string("Closed")) + "\n";
        if (type == CHECKING)
        {
            info += "Overdraft Limit: $" + to_string(overdraftLimit) + "\n";
        }
        return info;
    }

    string displayTransactionHistory()
    {
        string history = "Transaction History for Account " + accountNumber + ":\n";
        for (const auto &transaction : transactionHistory)
        {
            history += transaction.timestamp + " - " + transaction.type + " $" + to_string(transaction.amount) + "\n";
        }
        return history;
    }

    void recordTransaction(string transactionType, double amount)
    {
        time_t now = time(0);
        string timestamp = ctime(&now);
        Transaction newTransaction = {timestamp, transactionType, amount};
        transactionHistory.push_back(newTransaction);
    }

    void closeAccount()
    {
        isOpen = false;
        balance = 0;
        transactionHistory.clear();
    }

    string getAccountNumber()
    {
        return accountNumber;
    }

    AccountType getAccountType() const
    {
        return type;
    }
};
    
class Bank
{
private:
    vector<BankAccount> accounts;

public:
    string createAccount(string accNumber, string accHolderName, AccountType accType, double initialBalance, double overdraft = 0)
    {
        BankAccount newAccount(accNumber, accHolderName, accType, initialBalance, overdraft);
        accounts.push_back(newAccount);
        return "Account created successfully.";
    }

    AccountType accountType(string accNumber)
    {
        for (auto &account : accounts)
        {
            if (account.getAccountNumber() == accNumber)
            {
                return account.getAccountType();
            }
        }
        return AccountType::INVALID;
    }

    string deposit(string accNumber, double amount)
    {
        for (auto &account : accounts)
        {
            if (account.getAccountNumber() == accNumber)
            {
                return account.deposit(amount);
            }
        }
        return "Account not found.";
    }

    string withdraw(string accNumber, double amount)
    {
        for (auto &account : accounts)
        {
            if (account.getAccountNumber() == accNumber)
            {
                return account.withdraw(amount);
            }
        }
        return "Account not found.";
    }

    string checkBalance(string accNumber)
    {
        for (auto &account : accounts)
        {
            if (account.getAccountNumber() == accNumber)
            {
                return "Balance for account " + accNumber + " is: $" + to_string(account.getBalance());
                cout<<account.getBalance();
            }
        }
        return "Account not found.";
    }

    string displayTransactionHistory(string accNumber)
    {
        for (auto &account : accounts)
        {
            if (account.getAccountNumber() == accNumber)
            {
                /*return account.displayTransactionHistory();*/
            }
        }
        return "Account not found.";
    }

    string closeAccount(string accNumber)
    {
        for (auto it = accounts.begin(); it != accounts.end(); ++it)
        {
            if (it->getAccountNumber() == accNumber)
            {
                it->closeAccount();
                accounts.erase(it);
                return "Account closed successfully.";
            }
        }
        return "Account not found.";
    }

    string transferFunds(string fromAccNumber, string toAccNumber, double amount)
    {
        string withdrawalMsg = withdraw(fromAccNumber, amount);
        if (withdrawalMsg != "Withdrawal of $" + to_string(amount) + " successful.")
        {
            return withdrawalMsg;
        }
        string depositMsg = deposit(toAccNumber, amount);
        if (depositMsg != "Deposit of $" + to_string(amount) + " successful.")
        {
            // If deposit to recipient account fails, refund amount to sender account
            deposit(fromAccNumber, amount);
            return "Error transferring funds: " + depositMsg;
        }
        return "Funds transferred successfully.";
    }
};

// Example usage of the extended Bank and BankAccount classes
// int main()
// {
//     Bank bank;
//     cout << bank.createAccount("123456", "John Doe", SAVINGS, 1000) << endl;
//     cout << bank.createAccount("789012", "Jane Smith", CHECKING, 2000, 500) << endl;

//     cout << bank.deposit("123456", 500) << endl;
//     cout << bank.withdraw("789012", 300) << endl;
//     cout << bank.checkBalance("123456") << endl;
//     cout << bank.checkBalance("789012") << endl;

//     cout << bank.displayTransactionHistory("123456") << endl;

//     cout << bank.transferFunds("123456", "789012", 200) << endl;

//     cout << bank.closeAccount("123456") << endl;

//     return 0;
// }
