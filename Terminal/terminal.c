#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>
#include "../Card/card.h"

#include "terminal.h"


EN_terminalError_t getTransactionDate(ST_terminalData_t* termData)
{
    EN_terminalError_t ThisFunction_ErrorState = TERMINAL_OK;

    uint8_t transactionDate[11] = { 0 };

    printf("\n Enter the  date of transaction : ");

    gets(transactionDate);

    uint8_t TransactionDateLength = strlen(transactionDate);

    if ((NULL == transactionDate) ||
        (TransactionDateLength < 10) ||
        (TransactionDateLength > 10) ||
        (transactionDate[2] != '/' || transactionDate[5] != '/'))
    {
        ThisFunction_ErrorState = WRONG_DATE;
    }
    else
    {
        strcpy_s(termData->transactionDate, sizeof(termData->transactionDate), transactionDate);
    }

    return ThisFunction_ErrorState;
}



EN_terminalError_t isCardExpired(ST_cardData_t cardData, ST_terminalData_t termData)
{
    EN_terminalError_t ThisFunction_ErrorState = TERMINAL_OK;


    int cardExpirationMonth, cardExpirationYear;
    scanf_s(cardData.cardExpirationDate, "%2d/%2d", &cardExpirationMonth, &cardExpirationYear);


    int transactionMonth, transactionYear;

    scanf_s(termData.transactionDate, "%*2hhu/%*2hhu/%2d", &transactionYear, &transactionMonth);


    if (cardExpirationYear < transactionYear)
    {
        ThisFunction_ErrorState = EXPIRED_CARD;
    }


    else if (cardExpirationYear == transactionYear && cardExpirationMonth < transactionMonth)
    {
        ThisFunction_ErrorState = EXPIRED_CARD;
    }

    return ThisFunction_ErrorState;
}




EN_terminalError_t isValidCardPAN(ST_cardData_t* cardData)
{
    EN_terminalError_t ThisFunction_ErrorState = TERMINAL_OK;


    return ThisFunction_ErrorState;
}


EN_terminalError_t getTransactionAmount(ST_terminalData_t* termData)
{
    EN_terminalError_t ThisFunction_ErrorState = TERMINAL_OK;

    float transAmount = 0;
    printf("\n Enter transaction amount: ");
    scanf_s("%f", &transAmount);

    if ((0 >= transAmount))
    {
        ThisFunction_ErrorState = INVALID_AMOUNT;
    }
    else
    {
        termData->transAmount = transAmount;
    }
    return ThisFunction_ErrorState;
}

EN_terminalError_t isBelowMaxAmount(ST_terminalData_t* termData)
{
    EN_terminalError_t ThisFunction_ErrorState = TERMINAL_OK;

    if ((termData->transAmount > termData->maxTransAmount))
    {
        ThisFunction_ErrorState = EXCEED_MAX_AMOUNT;
    }


    return ThisFunction_ErrorState;
}

EN_terminalError_t setMaxAmount(ST_terminalData_t* termData)
{
    EN_terminalError_t ThisFunction_ErrorState = TERMINAL_OK;

    float maxTransAmount = 0.0f;

    printf("\n Enter  maximum amount of transaction : ");
    scanf_s("%f", &maxTransAmount);

    if ((0 >= maxTransAmount))
    {
        ThisFunction_ErrorState = INVALID_MAX_AMOUNT;
    }
    else
    {
        termData->maxTransAmount = maxTransAmount;
    }

    return ThisFunction_ErrorState;



}

// testcase 

void testGetTransactionDate(void)
{
    printf("\nTester Name: sara ahmed \n");
    printf("Function Name: getTransactionDate\n");
    printf("\nTest Case 1:\n");
    ST_terminalData_t termData1;
    printf("Enter the date of transaction (DD/MM/YYYY): ");
    if (getTransactionDate(&termData1) == TERMINAL_OK) {
        printf("Expected Result: TERMINAL_OK\nActual Result: TERMINAL_OK\n");
        printf("Transaction Date: %s\n", termData1.transactionDate);
    }
    else {
        printf("Expected Result: TERMINAL_OK\nActual Result: WRONG_DATE\n");
    }


 
}

void isCardExpiredTest(void)
{
    printf("\nTester Name:sara ahmed \n");
    printf("Function Name: isCardExpired\n");

    printf("\nTest Case 1:\n");
    ST_cardData_t cardData1;
    ST_terminalData_t termData1;
     
    strcpy_s(termData1.transactionDate, sizeof(termData1.transactionDate), "01/01/2022");


    printf("Enter card expiration date (MM/YY): ");
    scanf_s("%2hhu/%2hhu", &cardData1.cardExpirationDate[0], &cardData1.cardExpirationDate[1]);
    printf("Enter transaction date (DD/MM/YYYY): ");
    scanf_s("%*2hhu/%*2hhu/%*4d"); 
    strcpy_s(termData.transactionDate, "01/07/2022",sizeof(termData.transactionDate));
    if (isCardExpired(cardData1, termData1) == TERMINAL_OK) {
        printf("Expected Result: TERMINAL_OK\nActual Result: TERMINAL_OK\n");
    }
    else {
        printf("Expected Result: TERMINAL_OK\nActual Result: EXPIRED_CARD\n");
    }
}

void isValidCardPANTest(void)
{
    printf("\nTester Name: sara ahmed \n");
    printf("Function Name: isValidCardPAN\n");  // 1234565432567876

    printf("\nTest Case 1:\n");
    ST_cardData_t cardData1;
    printf("Enter card PAN: ");

    fgets(cardData1.primaryAccountNumber, sizeof(cardData1.primaryAccountNumber), stdin);
    if (isValidCardPAN(&cardData1) == TERMINAL_OK) {
        printf("Expected Result: TERMINAL_OK\nActual Result: TERMINAL_OK\n");
    }
    else {
        printf("Expected Result: TERMINAL_OK\nActual Result: INVALID_PAN\n");
    }
}


void getTransactionAmountTest(void)
{
    printf("\nTester Name: sara\n");
    printf("Function Name: getTransactionAmount\n");

    printf("\nTest Case 1:\n");
    ST_terminalData_t termData1;
    EN_terminalError_t result1 = getTransactionAmount(&termData1);
    printf("Input Data: N/A (Enter the transaction amount when prompted)\n");
    printf("Expected Result: TERMINAL_OK\n");
    printf("Actual Result: %s\n", result1 == TERMINAL_OK ? "TERMINAL_OK" : "INVALID_AMOUNT");
 

}


void isBelowMaxAmountTest(void)
{
    printf("\nTester Name: sara \n");
    printf("Function Name: isBelowMaxAmount\n");

    printf("\nTest Case 1:\n");
    ST_terminalData_t termData1;
    termData1.transAmount = 100.0f;   
    termData1.maxTransAmount = 500.0f;   
    EN_terminalError_t result1 = isBelowMaxAmount(&termData1);
    printf("Input Data: Transaction Amount = 100.0, Maximum Allowed Amount = 200.0\n");
    printf("Expected Result: TERMINAL_OK\n");
    printf("Actual Result: %s\n", result1 == TERMINAL_OK ? "TERMINAL_OK" : "EXCEED_MAX_AMOUNT");

     
}

void setMaxAmountTest(void)
{
    printf("\nTester Name: sara \n");
    printf("Function Name: setMaxAmount\n");
    
    termData.transAmount = 100.0f;

    result = getTransactionAmount(&termData);

    printf("\nTest Case 1 - Valid amount: %s", result == TERMINAL_OK ? "PASSED" : "FAILED");

    
    termData.transAmount = -50.0f;
    result = getTransactionAmount(&termData);

    printf("\nTest Case 2 - Invalid amount: %s", result == INVALID_AMOUNT ? "PASSED" : "FAILED");
 
}

void isBelowMaxAmountTest(void) {
    ST_terminalData_t termData;
    EN_terminalError_t result;

  
    termData.maxTransAmount = 100.0f;

  
    termData.transAmount = 50.0f;
    result = isBelowMaxAmount(&termData);
    printf("\nTest Case 1 - Below max amount: %s", result == TERMINAL_OK ? "PASSED" : "FAILED");

  
    termData.transAmount = 150.0f;
    result = isBelowMaxAmount(&termData);
    printf("\nTest Case 2 - Above max amount: %s", result == EXCEED_MAX_AMOUNT ? "PASSED" : "FAILED");

   
}

void setMaxAmountTest(void) {
    ST_terminalData_t termData;
    EN_terminalError_t result;

   
    result = setMaxAmount(&termData);

    printf("\nTest Case 1 - Valid maixiimum amount: %s", result == TERMINAL_OK ? "PASSED" : "FAILED");


    termData.maxTransAmount = -50.0f;

    result = setMaxAmount(&termData);

    printf("\nTest Case 2 - Invalid maixiimum amount: %s", result == INVALID_MAX_AMOUNT ? "PASSED" : "FAILED");

    
}

void isValidCardPANTest(void) {
    ST_cardData_t cardData;
    EN_terminalError_t result;

    
    strcpy_s(cardData.primaryAccountNumber, "1234567890123456",sizeof(cardData.primaryAccountNumber));
    result = isValidCardPAN(&cardData);
    printf("\nTest Case 1 - Valid PAN: %s", result == TERMINAL_OK ? "PASSED" : "FAILED");

 
    strcpy_s(cardData.primaryAccountNumber, "1234567890123457", sizeof(cardData.primaryAccountNumber));
    result = isValidCardPAN(&cardData);
    printf("\nTest Case 2 - Invalid PAN: %s", result == INVALID_CARD ? "PASSED" : "FAILED");
    printf("\nTest Case 1:\n");
    ST_terminalData_t termData1;
    EN_terminalError_t result1 = setMaxAmount(&termData1);
    printf("Input Data: 200.0\n");   // max 200.0
    printf("Expected Result: TERMINAL_OK\n");
    printf("Actual Result: %s\n", result1 == TERMINAL_OK ? "TERMINAL_OK" : "INVALID_MAX_AMOUNT");
}



