#include"application.h"
#include"card.h"
#include"terminal.h"
#include"server.h"
#include<stdio.h>

void appStart(void)
{
    EN_cardError_t errorNumCard;
    ST_cardData_t   cardData;
    ST_terminalData_t terminalData;
    ST_accountsDB_t currentAccount;

    printf("Hello in our  Payment Application\n");


    // Card Module
    errorNumCard = getCardHolderName(&cardData);
    if (errorNumCard != CARD_OK) {
        printf("Error: Failed to get card holder name\n");
        return;
    }


    errorNumCard = getCardExpiryDate(&cardData);
    if (errorNumCard != CARD_OK) {
        printf("Error: Failed to get card expiry date\n");
        return;
    }

    // Clear input buffer to remove newline
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
    errorNumCard = getCardPAN(&cardData);
    if (errorNumCard != CARD_OK) {
        printf("Error: Failed to get card PAN\n");
        return;
    }


    /*
    EN_terminalError_t cardPANError = isValidCardPAN(&cardData);
    if (cardPANError != TERMINAL_OK) {
        printf("Declined Invalid card PAN.\n");

        return;
    }*/
    


    // terminal side
    terminalData.transAmount = 0;
    terminalData.maxTransAmount = 0;
    strcpy_s(terminalData.transactionDate, sizeof(terminalData.transactionDate), "");


    EN_terminalError_t dateError = getTransactionDate(&terminalData);
    if (dateError != TERMINAL_OK) {
        printf("Invalid transaction date.\n");

        return;
    }

    EN_terminalError_t cardExpiredError = isCardExpired(cardData, terminalData);
    if (cardExpiredError == EXPIRED_CARD) {
        printf("Card has expired.\n");
        return;
    }

    EN_terminalError_t amountError = getTransactionAmount(&terminalData);
    if (amountError != TERMINAL_OK) {
        printf("Invalid transaction amount.\n");

        return;
    }


    EN_terminalError_t maxAmountError = setMaxAmount(&terminalData,10000);

	maxAmountError = isBelowMaxAmount(&terminalData);
	if (maxAmountError != TERMINAL_OK) {
		printf("Declined Amount Exceeding Limit.\n");
		return;
	}

    // Server Side
    strcpy_s(currentAccount.primaryAccountNumber ,sizeof(cardData.primaryAccountNumber), cardData.primaryAccountNumber);
    EN_serverError_t serverResponse = isValidAccount(&cardData,&currentAccount);
    if(serverResponse != SERVER_OK)
    {
        printf("Declined Invalid Account");
        return;
    }

    serverResponse = isAmountAvailable(&terminalData, &currentAccount);
	if (serverResponse != SERVER_OK)
    {
		printf("Declined Insuffecient Funds.");
		return;
	}



    ST_transaction transaction;
    transaction.cardHolderData = cardData;
    transaction.terminalData = terminalData;
  


	// Server process and Validate transaction data 
    EN_transStat_t transStats = recieveTransactionData(&transaction);
    if (transStats != APPROVED)
    {
        switch (transStats)
		{
        case DECLINED_INSUFFECIENT_FUND:
        {
			printf("Declined Insuffecient Funds.\n");
			break;
		}
        case DECLINED_STOLEN_CARD:
        {
            printf("Declined Stolen Card.\n");
            break;
        }
        case FRAUD_CARD:
        {
			printf("Declined Fraud Card.\n");
			break;
		}
		default:
            break;
        }
        return;
    }
    transaction.transState = transStats;

    EN_serverError_t saveStatus = saveTransaction(&transaction);
}