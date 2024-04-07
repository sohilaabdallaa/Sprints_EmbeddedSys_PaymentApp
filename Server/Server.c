#include "Server.h"
// Global array of ST_transaction_t for transaction data
ST_transaction_t transactions[255] = { 0 }; // Initializing with zeros

// global array of ST_accountsDB_t for the valid accounts database
ST_accountsDB_t accountsDB[255];