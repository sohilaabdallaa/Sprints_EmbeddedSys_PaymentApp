#include "server.h"
// Global array of ST_transaction_t for transaction data
ST_transaction transactions[255] = { 0 }; // Initializing with zeros

// global array of ST_accountsDB_t for the valid accounts database
ST_accountsDB_t accountsDB[255];

void initialize_Valid_Accounts(void)
{
    // Fill in the array with sample account data
    accountsDB[0].balance = 2000.0;
    accountsDB[0].state = RUNNING;
    strcpy(accountsDB[0].primaryAccountNumber,"8989374615436851");

    accountsDB[1].balance = 30000.0;
    accountsDB[1].state = BLOCKED;
    strcpy(accountsDB[1].primaryAccountNumber, "5807007076043875");

    accountsDB[2].balance = 10000.0;
    accountsDB[2].state = RUNNING;
    strcpy(accountsDB[2].primaryAccountNumber, "5555666677778888");

    accountsDB[3].balance = 9000.0;
    accountsDB[3].state = RUNNING;
    strcpy(accountsDB[3].primaryAccountNumber, "1122334455667788");

    accountsDB[4].balance = 500.0;
    accountsDB[4].state = BLOCKED;
    strcpy(accountsDB[4].primaryAccountNumber, "9876543210987654");
}

// This function will take all transaction data and validate its data
EN_transStat_t recieveTransactionData(ST_transaction* transData)
{
    // check If the account does not exist
    int accountsDB_Length = sizeof(accountsDB)  / sizeof(accountsDB[0]);
    ST_accountsDB_t currentAccount;
    for(int i = 0; i< accountsDB_Length; i++)
    {
        if (strcmp(transData->cardHolderData.primaryAccountNumber,accountsDB[i].primaryAccountNumber) == 0)
        {
            currentAccount = accountsDB[i];
            return APPROVED;
        }
    }
    return FRAUD_CARD;

    // if the amount is not available 
    if (transData->terminalData.transAmount < currentAccount.balance )
    {
        return DECLINED_INSUFFECIENT_FUND;
    }
     
    //  if the account is blocked
    if (currentAccount.state == BLOCKED)
    {
        return DECLINED_STOLEN_CARD;
    }
    

    // if a transaction can't be saved 
    if(transData->transState != APPROVED)
    {
        return INTERNAL_SERVER_ERROR;
    }
    
    else 
        return APPROVED;
}
// Function to check if the account is valid
EN_serverError_t isValidAccount(ST_cardData_t *cardData, ST_accountsDB_t *accountRefrence) {
    if (strcmp(getCardPAN(cardData), accountRefrence->primaryAccountNumber) != 0) {
        accountRefrence = NULL;
        return ACCOUNT_NOT_FOUND;
    }
    return SERVER_OK;
}
// Function to test isValidAccount function
void isValidAccountTest(void) {
    printf("Tester Name: Your Name\n");
    printf("Function Name: isValidAccount\n\n");

    // Test case 1: Happy-case scenario (account found)
    printf("Test Case 1:\n");
    printf("Input Data:\n");
    ST_cardData_t cardData1 = {
        .cardHolderName = "John Doe",
        .primaryAccountNumber = "1234567890123456",
        .cardExpirationDate = "12/25"
    };
    ST_accountsDB_t account1 = {
        .primaryAccountNumber = "1234567890123456"
    };
    EN_serverError_t expected1 = SERVER_OK;
    EN_serverError_t actual1 = isValidAccount(&cardData1, &account1);
    printf("Expected Result: SERVER_OK\n");
    printf("Actual Result: %s\n\n", actual1 == SERVER_OK ? "SERVER_OK" : "ACCOUNT_NOT_FOUND");

    // Test case 2: Worst-case scenario (account not found)
    printf("Test Case 2:\n");
    printf("Input Data:\n");
    ST_cardData_t cardData2 = {
        .cardHolderName = "Jane Smith",
        .primaryAccountNumber = "9876543210987654",
        .cardExpirationDate = "11/24"
    };
    ST_accountsDB_t account2 = {
        .primaryAccountNumber = "1234567890123456"
    };
    EN_serverError_t expected2 = ACCOUNT_NOT_FOUND;
    EN_serverError_t actual2 = isValidAccount(&cardData2, &account2);
    printf("Expected Result: ACCOUNT_NOT_FOUND\n");
    printf("Actual Result: %s\n", actual2 == ACCOUNT_NOT_FOUND ? "ACCOUNT_NOT_FOUND" : "SERVER_OK");
}


// Function to check if an account is blocked
EN_serverError_t isBlockedAccount(ST_accountsDB_t *accountRefrence) {
    if (accountRefrence->state == BLOCKED) {
        return BLOCKED_ACCOUNT;
    } else {
        return SERVER_OK;
    }
}
    
// Function to test isBlockedAccount function
void isBlockedAccountTest(void) {
    printf("Tester Name: Your Name\n");
    printf("Function Name: isBlockedAccount\n\n");

    // Test case 1: Happy-case scenario (account is running)
    printf("Test Case 1:\n");
    printf("Input Data: Account is running\n");
    ST_accountsDB_t account1 = {1000.0, RUNNING, "12345678901234567890"}; // Account is running
    EN_serverError_t expected1 = SERVER_OK;
    EN_serverError_t actual1 = isBlockedAccount(&account1);
    printf("Expected Result: SERVER_OK\n");
    printf("Actual Result: %s\n\n", actual1 == SERVER_OK ? "SERVER_OK" : "BLOCKED_ACCOUNT");

    // Test case 2: Worst-case scenario (account is blocked)
    printf("Test Case 2:\n");
    printf("Input Data: Account is blocked\n");
    ST_accountsDB_t account2 = {500.0, BLOCKED, "09876543210987654321"}; // Account is blocked
    EN_serverError_t expected2 = BLOCKED_ACCOUNT;
    EN_serverError_t actual2 = isBlockedAccount(&account2);
    printf("Expected Result: BLOCKED_ACCOUNT\n");
    printf("Actual Result: %s\n\n", actual2 == BLOCKED_ACCOUNT ? "BLOCKED_ACCOUNT" : "SERVER_OK");
}


EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
    if(termData->transAmount > accountRefrence->balance)
    {
        return LOW_BALANCE;
    }
    else
    {
        return SERVER_OK;
    }
}

// Test function for isAmountAvailable()
void isAmountAvailableTest(void)
{
    printf("Testing isAmountAvailable() function:\n");

    // Test case 1: Sufficient balance
    ST_terminalData_t termData1 = {100}; // Transaction amount
    ST_accountsDB_t accountRef1 = {200, RUNNING, "12345678901234567890"}; // Account with sufficient balance
    printf("Test case 1: Sufficient balance\n");
    printf("Input Data: Transaction amount = %f, Account balance = %f, Account number = %s\n", termData1.transAmount, accountRef1.balance, accountRef1.primaryAccountNumber);
    EN_serverError_t result1 = isAmountAvailable(&termData1, &accountRef1);
    printf("Expected Result: SERVER_OK\n");
    printf("Actual Result: %s\n", result1 == SERVER_OK ? "SERVER_OK" : "LOW_BALANCE");

    // Test case 2: Insufficient balance
    ST_terminalData_t termData2 = {300}; // Transaction amount
    ST_accountsDB_t accountRef2 = {200, RUNNING, "09876543210987654321"}; // Account with insufficient balance
    printf("Test case 2: Insufficient balance\n");
    printf("Input Data: Transaction amount = %f, Account balance = %f, Account number = %s\n", termData2.transAmount, accountRef2.balance, accountRef2.primaryAccountNumber);
    EN_serverError_t result2 = isAmountAvailable(&termData2, &accountRef2);
    printf("Expected Result: LOW_BALANCE\n");
    printf("Actual Result: %s\n", result2 == LOW_BALANCE ? "LOW_BALANCE" : "SERVER_OK");

}
EN_serverError_t saveTransaction(ST_transaction* transData)
{

}

// Function to print all transactions in the transactions DB
void listSavedTransactions(void)
{
    // Iterate over each transaction and print its details
    for (int i = 0; i < sizeof(transactions) / sizeof(transactions[0]); i++)
    {
        // Check if the transaction is empty (all fields are zeros)
        if (transactions[i].transactionSequenceNumber == 0 &&
            transactions[i].cardHolderData.primaryAccountNumber[0] == '\0')
        {
            continue; // Skip empty transactions
        }

        printf("#########################\n");
        printf("Transaction Sequence Number: %d\n", transactions[i].transactionSequenceNumber);
        printf("Transaction Date: %s\n", transactions[i].terminalData.transactionDate);
        printf("Transaction Amount: %.2f\n", transactions[i].terminalData.transAmount);
        printf("Transaction State: %d\n", transactions[i].transState);
        printf("Terminal Max Amount: %.2f\n", transactions[i].terminalData.maxTransAmount);
        printf("Cardholder Name: %s\n", transactions[i].cardHolderData.cardHolderName);
        printf("PAN: %s\n", transactions[i].cardHolderData.primaryAccountNumber);
        printf("Card Expiration Date: %s\n", transactions[i].cardHolderData.cardExpirationDate);
        printf("#########################\n");
    }
}