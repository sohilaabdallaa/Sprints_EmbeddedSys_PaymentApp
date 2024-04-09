#include "server.h"
// Global array of ST_transaction_t for transaction data
ST_transaction transactions[255] = { 0 }; // Initializing with zeros

// global array of ST_accountsDB_t for the valid accounts database
ST_accountsDB_t accountsDB[255] = { 
    {2000.0, RUNNING, "8989374615436851"},
    {30000.0, BLOCKED, "5807007076043875"},
    {10000.0, RUNNING, "5555666677778888"},
    {7000.0, RUNNING, "1122334455667788"},
    {500.0, BLOCKED, "9876543210987654"},
};

// This function will take all transaction data and validate its data
EN_transStat_t recieveTransactionData(ST_transaction* transData)
{
    int accountsDB_Length = sizeof(accountsDB) / sizeof(accountsDB[0]);
    // Loop until the last non-empty account
    for (int i = 0; i < accountsDB_Length && accountsDB[i].primaryAccountNumber[0] != '\0'; i++) {
        if (strcmp(transData->cardHolderData.primaryAccountNumber, accountsDB[i].primaryAccountNumber) == 0) {

            // Account found, check if the account is blocked
            if (accountsDB[i].state == BLOCKED) {
                return DECLINED_STOLEN_CARD;
            }

            // Check if the transaction amount exceeds the balance
            if (transData->terminalData.transAmount > accountsDB[i].balance) {
                return DECLINED_INSUFFECIENT_FUND;
            }

            // Update the account balance
            accountsDB[i].balance -= transData->terminalData.transAmount;

            // Return transaction approved
            return APPROVED;
        }
    }

    // Account not found, return fraud card
    return FRAUD_CARD;
}

void recieveTransactionDataTest(void)
{
    printf("Tester Name: Sohila Abdalla \n");
    printf("Function Name: recieveTransactionData\n");

    // Test Case 1: Happy Case - Transaction Approved
    printf("Test Case 1:\n");
    ST_transaction transaction = {
        .cardHolderData = {
            .primaryAccountNumber = "5555666677778888",
        },
        .terminalData = {.transAmount = 50} };
    EN_transStat_t actualResult1 = recieveTransactionData(&transaction);
    printf("Input Data: Withrwal 50 EP from Valid account for test case 1\n");
    printf("Expected Result: %d \n");
    printf("Actual Result: %d \n\n", actualResult1);


    // Test Case 2: Transaction Declined - DECLINED_STOLEN_CARD
    printf("Test Case 2:\n");
    ST_transaction transaction2 = {
        .cardHolderData = {.primaryAccountNumber = "9876543210987654"},
        .terminalData = {.transAmount = 500000} // Assuming insufficient funds for withdrawal
    };
    EN_transStat_t expectedResult2 = DECLINED_STOLEN_CARD;
    EN_transStat_t actualResult2 = recieveTransactionData(&transaction2);
    printf("Input Data: Withdrawal 500000 EP from Blocked account\n");
    printf("Expected Result: %d\n", expectedResult2);
    printf("Actual Result: %d\n\n", actualResult2);

    // Test Case 3: Transaction Declined - Account Not Found
    printf("Test Case 3:\n");
    ST_transaction transaction3 = {
        .cardHolderData = {.primaryAccountNumber = "1234567890123456"}, // Non-existent account number
        .terminalData = {.transAmount = 100} };
    EN_transStat_t expectedResult3 = ACCOUNT_NOT_FOUND;
    EN_transStat_t actualResult3 = recieveTransactionData(&transaction3);
    printf("Input Data: Withdrawal 100 EP from Non-existent account for test case 3\n");
    printf("Expected Result: %d\n", expectedResult3);
    printf("Actual Result: %d\n\n", actualResult3);
}

// Function to check if the account is valid
EN_serverError_t isValidAccount(ST_cardData_t* cardData, ST_accountsDB_t* accountRefrence)
{
    if (strcmp(cardData->primaryAccountNumber, accountRefrence->primaryAccountNumber) != 0)
    {
        accountRefrence = NULL;
        return ACCOUNT_NOT_FOUND;
    }
    return SERVER_OK;
}


// Function to test isValidAccount function
void isValidAccountTest(void) {
    printf("Tester Name:Sohila Abdalla\n");
    printf("Function Name: isValidAccount\n");

    // Test Case 1: Happy Case - Account found
    printf("Test Case 1:\n");
    ST_cardData_t card1 = { .primaryAccountNumber = "8989374615436851" };
    ST_accountsDB_t account1 = { .primaryAccountNumber = "8989374615436851" };
    EN_serverError_t expectedResult1 = SERVER_OK;
    EN_serverError_t actualResult1 = isValidAccount(&card1, &account1);
    printf("Input Data: Matching account number\n");
    printf("Expected Result: SERVER_OK %d \n",expectedResult1);
    printf("Actual Result: %d\n\n", actualResult1);

    // Test Case 2: Account not found
    printf("Test Case 2:\n");
    ST_cardData_t card2 = { .primaryAccountNumber = "1234567890123456" };
    ST_accountsDB_t account2 = { .primaryAccountNumber = "9876543210987654" };
    EN_serverError_t expectedResult2 = ACCOUNT_NOT_FOUND;
    EN_serverError_t actualResult2 = isValidAccount(&card2, &account2);
    printf("Input Data: Non-matching account number\n");
    printf("Expected Result: ACCOUNT_NOT_FOUND %d \n", expectedResult2);
    printf("Actual Result: %d\n\n", actualResult2);
}


EN_serverError_t isAmountAvailable(ST_terminalData_t* termData, ST_accountsDB_t* accountRefrence)
{
    if (termData->transAmount > accountRefrence->balance)
    {
        return SERVER_OK;
    }
    else
    {
        return LOW_BALANCE;
    }
}

void isAmountAvailableTest(void) {
    printf("Tester Name: Sohila Abdalla\n");
    printf("Function Name: isAmountAvailable\n");

    // Test Case 1: Happy Case - Sufficient balance
    printf("Test Case 1:\n");
    ST_terminalData_t termData1 = { .transAmount = 50 };
    ST_accountsDB_t account1 = { .balance = 100 };
    EN_serverError_t expectedResult1 = SERVER_OK;
    EN_serverError_t actualResult1 = isAmountAvailable(&termData1, &account1);
    printf("Input Data: Transaction amount: 50, Account balance: 100\n");
    printf("Expected Result: SERVER_OK %d \n",expectedResult1);
    printf("Actual Result: %d\n\n", actualResult1);

    // Test Case 2: Low balance
    printf("Test Case 2:\n");
    ST_terminalData_t termData2 = { .transAmount = 500 };
    ST_accountsDB_t account2 = { .balance = 200 };
    EN_serverError_t expectedResult2 = LOW_BALANCE;
    EN_serverError_t actualResult2 = isAmountAvailable(&termData2, &account2);
    printf("Input Data: Transaction amount: 500, Account balance: 200\n");
    printf("Expected Result: LOW_BALANCE %d \n", expectedResult2);
    printf("Actual Result: %d\n\n", actualResult2);
}


// Function to check if an account is blocked
EN_serverError_t isBlockedAccount(ST_accountsDB_t* accountRefrence)
{
    if (accountRefrence->state == BLOCKED)
    {
        return BLOCKED_ACCOUNT;
    }
    else
    {
        return SERVER_OK;
    }
}
void isBlockedAccountTest(void) {
    printf("Tester Name: Sohila Abdalla \n");
    printf("Function Name: isBlockedAccount\n");

    // Test Case 1: Happy Case - Account is blocked
    printf("Test Case 1:\n");
    ST_accountsDB_t account1 = { .state = BLOCKED };
    EN_serverError_t expectedResult1 = BLOCKED_ACCOUNT;
    EN_serverError_t actualResult1 = isBlockedAccount(&account1);
    printf("Input Data: Account state: BLOCKED\n");
    printf("Expected Result: BLOCKED_ACCOUNT %d \n",expectedResult1);
    printf("Actual Result: %d\n\n", actualResult1);

    // Test Case 2: Account is not blocked
    printf("Test Case 2:\n");
    ST_accountsDB_t account2 = { .state = RUNNING };
    EN_serverError_t expectedResult2 = SERVER_OK;
    EN_serverError_t actualResult2 = isBlockedAccount(&account2);
    printf("Input Data: Account state: RUNNING\n");
    printf("Expected Result: SERVER_OK %d \n", expectedResult2);
    printf("Actual Result: %d\n\n", actualResult2);
}
EN_serverError_t saveTransaction(ST_transaction* transData)
{
    // Find the last used sequence number in the transactions database
    uint32_t lastUsedSequenceNumber = 0;
    int i = 0;
    for (i = 0; i < sizeof(transactions) / sizeof(transactions[0]); i++)
    {
        if (transactions[i].transactionSequenceNumber != 0 &&
            transactions[i].transactionSequenceNumber > lastUsedSequenceNumber)
        {
            lastUsedSequenceNumber = transactions[i].transactionSequenceNumber;
        }
    }

    // Increment the last used transaction sequence number
    lastUsedSequenceNumber += 1;

    // Find the first available slot in the transactions database
    for (i = 0; i < sizeof(transactions) / sizeof(transactions[0]); i++)
    {
        if (transactions[i].transactionSequenceNumber == 0)
        {
            // Found an empty slot, store the transaction data
            transactions[i] = *transData;
            transactions[i].transactionSequenceNumber = lastUsedSequenceNumber;
            break;
        }
    }
    // List all saved transactions
    listSavedTransactions();

    return SERVER_OK; // Return success
}

void saveTransactionTest(void) {
    printf("Tester Name: Sohila Abdalla\n");
    printf("Function Name: saveTransaction\n");


    // Test Case 1: Happy case
    printf("Test Case 1:\n");
    ST_transaction transaction1 = {
        .cardHolderData = {
            .cardHolderName = "John Doe",  
            .primaryAccountNumber = "1234567890123456",
            .cardExpirationDate = "202512"
        },
        .terminalData = {
            .transAmount = 100.0,  
            .maxTransAmount = 200.0,
            .transactionDate = "2024-04-15"
        },
        .transState = APPROVED,
        .transactionSequenceNumber = 1
    };
    printf("Input Data: Transaction with sequence number %d\n", transaction1.transactionSequenceNumber);
    printf("Expected Result: Success\n");
    printf("Actual Result: \n\n");
    EN_serverError_t result1 = saveTransaction(&transaction1);
    if (result1 == SERVER_OK) {
        printf("Success\n\n");
    }
    else {
        printf("Failure\n\n");
    }

}
// Function to print all transactions in the transactions DB
void listSavedTransactions(void)
{
    // Iterate over each transaction and print its details
    for (int i = 0; i < sizeof(transactions) / sizeof(transactions[0]); i++)
    {
        // Check if the transaction is empty (all fields are zeros)
        if (transactions[i].transactionSequenceNumber == 0)
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

void listSavedTransactionsTest(void) {
    printf("Tester Name: Sohila Abdalla\n");
    printf("Function Name: listSavedTransactions\n");


    // Test Case 1: No transaction data available
    printf("Test Case 1:\n");
    // Assuming transactions have been initialized but no data has been added
    // So, all transactions are empty
    // Capture the output of the listSavedTransactions function
    printf("Expected Result: No transaction data to display\n");
    printf("Actual Result:\n");
    listSavedTransactions();
    printf("-----------------------------------------------------------------------\n");

    // Test Case 2: Happy Case - Transaction data available
    printf("Test Case 2:\n");
    // Assuming transactions have been initialized with some data
    // For demonstration, let's assume the first transaction has data
    transactions[0].transactionSequenceNumber = 1;
    strcpy(transactions[0].terminalData.transactionDate, "2024-04-01");
    transactions[0].terminalData.transAmount = 100.0;
    transactions[0].transState = APPROVED;
    transactions[0].terminalData.maxTransAmount = 500.0;
    strcpy(transactions[0].cardHolderData.cardHolderName, "John Doe");
    strcpy(transactions[0].cardHolderData.primaryAccountNumber, "1234567890123456");
    strcpy(transactions[0].cardHolderData.cardExpirationDate, "2025-12");

    // Capture the output of the listSavedTransactions function
    printf("Expected Result (Sample Transaction):\n");
    printf("#########################\n");
    printf("Transaction Sequence Number: 1\n");
    printf("Transaction Date: 2024-04-01\n");
    printf("Transaction Amount: 100.00\n");
    printf("Transaction State: APPROVED\n");
    printf("Terminal Max Amount: 500.00\n");
    printf("Cardholder Name: John Doe\n");
    printf("PAN: 1234567890123456\n");
    printf("Card Expiration Date: 2025-12\n");
    printf("#########################\n");
    printf("Actual Result:\n");
    listSavedTransactions();
    printf("\n");
}