#include <deepstate/DeepState.hpp>
#include "BankAccount.cpp" // Include the header file containing the Bank and BankAccount classes

using namespace deepstate;

TEST(BankingSystem, SingleAccount)
{
    cout << "----------------------------------------------------------------" << endl;
    Bank bank;

    // Generate random account number
    int accNumber = DeepState_IntInRange(100000, 999999);
    int invalid_accNumber = accNumber + 1; // This account number will not exist so we can test the error messages.

    // Generate random initial values
    double initialBalance = DeepState_Int();
    float overdraftLimit = DeepState_IntInRange(0, 500);
    enum AccountType accType = (DeepState_Bool()) ? AccountType::SAVINGS : AccountType::CHECKING;

    // initialBalance shjould be positive
    initialBalance = (initialBalance < 0) ? -initialBalance : initialBalance;
    initialBalance = (initialBalance == 0) ? 1 : initialBalance;

    // Create account
    bank.createAccount(to_string(accNumber), "John Doe", accType, initialBalance, (overdraftLimit < 0.0) ? overdraftLimit : -overdraftLimit);

    // Check balance after account creation
    ASSERT_TRUE(bank.checkBalance(to_string(accNumber)) == "Balance for account " + to_string(accNumber) + " is: $" + to_string(initialBalance)) << "Can't catch initial balance correctly.";

    // Check balance for invalid account number
    ASSERT_TRUE(bank.checkBalance(to_string(invalid_accNumber)) == "Account not found.") << "Can't catch invalid account number";

    // Generate random deposit amount
    double currentBalance = initialBalance;
    double depositAmount = DeepState_Int();

    // cout << "Deposit amount: " << depositAmount << endl;

    // Deposit funds
    string depositMsg = bank.deposit(to_string(accNumber), depositAmount);
    if (depositMsg == "Invalid amount for deposit.")
    {
        // Check balance without deposit if deposit failed
        ASSERT_TRUE(bank.checkBalance(to_string(accNumber)) == "Balance for account " + to_string(accNumber) + " is: $" + to_string(initialBalance))
            << "The invalid deposit changed the balance"; // This should be the same as the initial balance

        // as the deposit is negative, deposit the -depositAmount
        depositAmount = (depositAmount < 0) ? -depositAmount : depositAmount;
        depositAmount = (depositAmount == 0) ? 1 : depositAmount;
        // Update the current balance that we keep track of
        currentBalance = currentBalance + depositAmount;

        depositMsg = bank.deposit(to_string(accNumber), depositAmount);
        ASSERT_TRUE(depositMsg == "Deposit of $" + to_string(depositAmount) + " successful.") << "Can't catch the deposit message correctly.";

        // Check balance after successful deposit
        ASSERT_TRUE(bank.checkBalance(to_string(accNumber)) == "Balance for account " + to_string(accNumber) + " is: $" + to_string(currentBalance))
            << "incorrect balance after deposit"; // This should be the initial balance plus the deposit amount
    }
    else
    {
        // Update the current balance that we keep track of
        currentBalance = currentBalance + depositAmount;

        ASSERT_TRUE(depositMsg == "Deposit of $" + to_string(depositAmount) + " successful.") << "Can't catch the deposit message correctly.";
        // Check balance after successful deposit
        ASSERT_TRUE(bank.checkBalance(to_string(accNumber)) == "Balance for account " + to_string(accNumber) + " is: $" + to_string(currentBalance))
            << "incorrect balance after deposit"; // This should be the initial balance plus the deposit amount
    }

    // Check if invalid account can deposit
    ASSERT_TRUE(bank.deposit(to_string(invalid_accNumber), depositAmount) == "Account not found.") << "Can't catch invalid account number in deposit";

    // Generate random withdrawal amount
    float withdrawalAmount = DeepState_Int();

    cout << "Balance Before Withdrawal: " << currentBalance << endl;
    // Withdraw funds
    string withdrawalMsg = bank.withdraw(to_string(accNumber), withdrawalAmount);
    if (withdrawalMsg == "Invalid amount for withdrawal.")
    {
        // Check balance without withdrawal if withdrawal failed
        ASSERT_TRUE(bank.checkBalance(to_string(accNumber)) == "Balance for account " + to_string(accNumber) + " is: $" + to_string(currentBalance));

        // as the withdrawal is negative, withdraw the -withdrawalAmount
        withdrawalAmount = (withdrawalAmount < 0) ? -withdrawalAmount : withdrawalAmount;
        withdrawalAmount = (withdrawalAmount == 0) ? 1 : withdrawalAmount;

        // Update the current balance that we keep track of
        currentBalance = currentBalance - withdrawalAmount;

        cout << "Withdraw amount: " << withdrawalAmount << endl;
        cout << "Balance After Withdrawal: " << currentBalance << endl;
        cout << "Overdraft: " << -overdraftLimit << endl;
        cout << "Account Type: " << bank.accountType(to_string(accNumber)) << endl;

        withdrawalMsg = bank.withdraw(to_string(accNumber), withdrawalAmount);
        if (withdrawalMsg == "Withdrawal exceeds overdraft limit.")
        {
            ASSERT_TRUE(bank.accountType(to_string(accNumber)) == AccountType::CHECKING) << "Incorrect account type for overdraft limit.";
            ASSERT_TRUE(currentBalance < -overdraftLimit) << "Exceeded overdraft limit.";
        }
        if (withdrawalMsg == "Withdrawal of $" + to_string(withdrawalAmount) + " successful.")
        {
            // Check balance after successful withdrawal
            ASSERT_TRUE(bank.checkBalance(to_string(accNumber)) == "Balance for account " + to_string(accNumber) + " is: $" + to_string(currentBalance));
        }
        if (withdrawalMsg == "Insufficient balance.")
        {
            ASSERT_TRUE(bank.accountType(to_string(accNumber)) == AccountType::SAVINGS) << "Incorrect account type for insufficient balance.";
            ASSERT_TRUE(currentBalance < withdrawalAmount) << "Insufficient balance.";
        }
    }
    else
    {
        // Update the current balance that we keep track of
        currentBalance = currentBalance - withdrawalAmount;

        cout << "Withdraw amount: " << withdrawalAmount << endl;
        cout << "Balance After Withdrawal: " << currentBalance << endl;
        cout << "Overdraft: " << -overdraftLimit << endl;
        cout << "Account Type: " << bank.accountType(to_string(accNumber)) << endl;

        // withdrawalMsg = bank.withdraw(to_string(accNumber), withdrawalAmount);
        if (withdrawalMsg == "Withdrawal exceeds overdraft limit.")
        {
            ASSERT_TRUE(bank.accountType(to_string(accNumber)) == AccountType::CHECKING) << "Incorrect account type for overdraft limit.";
            ASSERT_TRUE(currentBalance < -overdraftLimit) << "Withdrawal exceeds overdraft limit.";
        }
        if (withdrawalMsg == "Withdrawal of $" + to_string(withdrawalAmount) + " successful.")
        {
            // Check balance after successful withdrawal
            ASSERT_TRUE(bank.checkBalance(to_string(accNumber)) == "Balance for account " + to_string(accNumber) + " is: $" + to_string(currentBalance)) << "wrong Balance after withdrawal";
        }
        if (withdrawalMsg == "Insufficient balance.")
        {
            ASSERT_TRUE(bank.accountType(to_string(accNumber)) == AccountType::SAVINGS) << "Incorrect account type for insufficient balance.";
            ASSERT_TRUE(currentBalance < withdrawalAmount) << "Insufficient balance.";
        }
    }

    // Display transaction history
    ASSERT_TRUE(bank.displayTransactionHistory(to_string(accNumber)).find("Transaction History for Account " + to_string(accNumber) + ":") != string::npos);

    // Close account
    ASSERT_TRUE(bank.closeAccount(to_string(accNumber)) == "Account closed successfully.");
}
