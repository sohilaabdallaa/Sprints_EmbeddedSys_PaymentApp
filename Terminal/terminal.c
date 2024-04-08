#include <stdio.h>
#include <stdint.h>
#include <string.h>
#include <windows.h>

#include "card.c"
#include "card.h"
 
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

// TestCases
void getTransactionDateTest(void) {
    ST_terminalData_t termData;
    EN_terminalError_t result;

 
    strcpy(termData.transactionDate, "25/06/2022");

    result = getTransactionDate(&termData);

    printf("\nTest Case 1 - Valid date: %s", result == TERMINAL_OK ? "PASSED" : "FAILED");

     strcpy(termData.transactionDate, "");

    result = getTransactionDate(&termData);

    printf("\nTest Case 2 - NULL date: %s", result == WRONG_DATE ? "PASSED" : "FAILED");

    strcpy(termData.transactionDate, "25-06-2022");

    result = getTransactionDate(&termData);

    printf("\nTest Case 3 - Invalid format: %s", result == WRONG_DATE ? "PASSED" : "FAILED");

 
}

void isCardExpiredTest(void) {
    ST_cardData_t cardData;
    ST_terminalData_t termData;
    EN_terminalError_t result;
 
    strcpy(cardData.cardExpirationDate, "06/2022");

     
    strcpy(termData.transactionDate, "01/06/2022");

    result = isCardExpired(cardData, termData);

    printf("\nTest Case 1 - Expired card: %s", result == EXPIRED_CARD ? "PASSED" : "FAILED");

    
    strcpy(termData.transactionDate, "01/07/2022");

    result = isCardExpired(cardData, termData);

    printf("\nTest Case 2 - Valid card: %s", result == TERMINAL_OK ? "PASSED" : "FAILED");
     
}

void getTransactionAmountTest(void) {
    ST_terminalData_t termData;
    EN_terminalError_t result;

    
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

    
    strcpy(cardData.primaryAccountNumber, "1234567890123456");
    result = isValidCardPAN(&cardData);
    printf("\nTest Case 1 - Valid PAN: %s", result == TERMINAL_OK ? "PASSED" : "FAILED");

 
    strcpy(cardData.primaryAccountNumber, "1234567890123457");
    result = isValidCardPAN(&cardData);
    printf("\nTest Case 2 - Invalid PAN: %s", result == INVALID_CARD ? "PASSED" : "FAILED");

    
}

 
