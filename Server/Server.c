#include "server.h"
// Global array of ST_transaction_t for transaction data
ST_transaction_t transactions[255] = { 0 }; // Initializing with zeros

// global array of ST_accountsDB_t for the valid accounts database
ST_accountsDB_t accountsDB[255];

public void initialize_Valid_Accounts()
{
    // Fill in the array with sample account data
    accountsDB[0].balance = 2000.0;
    accountsDB[0].status = RUNNING;
    strcpy(accountsDB[0].cardNumber, "8989374615436851");

    accountsDB[1].balance = 30000.0;
    accountsDB[1].status = BLOCKED;
    strcpy(accountsDB[1].cardNumber, "5807007076043875");

    accountsDB[2].balance = 10000.0;
    accountsDB[2].status = RUNNING;
    strcpy(accountsDB[2].cardNumber, "5555666677778888");

    accountsDB[3].balance = 9000.0;
    accountsDB[3].status = RUNNING;
    strcpy(accountsDB[3].cardNumber, "1122334455667788");

    accountsDB[4].balance = 500.0;
    accountsDB[4].status = BLOCKED;
    strcpy(accountsDB[4].cardNumber, "9876543210987654");
}

// This function will take all transaction data and validate its data
EN_transState_t recieveTransactionData(ST_transaction_t* transData)
{
    // check If the account does not exist
    // return FRAUD_CARD;


    // if the amount is not available 
    // return DECLINED_INSUFFECIENT_FUND;
    
     
    //  if the account is blocked
    //  return DECLINED_STOLEN_CARD;
    

    // if a transaction can't be saved
    // return INTERNAL_SERVER_ERROR;
    
    
    //else returns APPROVED;
}