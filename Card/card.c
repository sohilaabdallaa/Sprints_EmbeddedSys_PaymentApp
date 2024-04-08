#include "card.h"
#include <string.h>
#include <stdio.h>

EN_cardError_t getCardHolderName(ST_cardData_t* cardData)
{
    EN_cardError_t ThisFunction_ErrorState = CARD_OK;
    uint8_t CardHolderName[25] = { 0 };
    printf("\nEnter the card holder name: ");
    fgets(CardHolderName, sizeof(CardHolderName), stdin);
    size_t  NameLength = strlen(CardHolderName);

    if (strchr(CardHolderName, '\n') == NULL) {
        int c;
        // Consume characters until the newline or EOF
        while ((c = getchar()) != '\n' && c != EOF) {}
        ThisFunction_ErrorState = WRONG_NAME;
    }
    else {
        size_t NameLength = strlen(CardHolderName);

        if (NameLength < 20 || NameLength > 24)
        {
            ThisFunction_ErrorState = WRONG_NAME;
        }
        else
        {
            if (strcpy_s(cardData->cardHolderName, sizeof(cardData->cardHolderName), CardHolderName) != 0) {
                ThisFunction_ErrorState = WRONG_NAME;
            }
        }
    }
    return ThisFunction_ErrorState;
}
EN_cardError_t getCardExpiryDate(ST_cardData_t* cardData)
{
    EN_cardError_t ThisFunction_ErrorState = CARD_OK;

    uint8_t cardExpirationDate[6] = { 0 };

    printf("\nEnter the card expiration date (MM/YY): ");
    fgets(cardExpirationDate, sizeof(cardExpirationDate), stdin);

    size_t DateLength = strlen(cardExpirationDate);

    if ((DateLength != 5) || (cardExpirationDate[2] != '/'))
    {
        ThisFunction_ErrorState = WRONG_EXP_DATE;
    }

    else
    {
        int month = atoi(cardExpirationDate);
        int year = atoi(cardExpirationDate);
        if ((month < 1 || month > 12 || year < 0 || year > 99)) {

            ThisFunction_ErrorState = WRONG_EXP_DATE;

        }
        // Remove newline character

        else {
            cardExpirationDate[5] = '\0';

            if (strcpy_s(cardData->cardExpirationDate, sizeof(cardData->cardExpirationDate), cardExpirationDate) != 0) {
                ThisFunction_ErrorState = WRONG_EXP_DATE;
            }
        }
    }

    return ThisFunction_ErrorState;
}

EN_cardError_t getCardPAN(ST_cardData_t* cardData)
{
    EN_cardError_t ThisFunction_ErrorState = CARD_OK;

    uint8_t cardPAN[20] = { 0 };

    printf("\nEnter the card primary account number: ");
    fgets(cardPAN, sizeof(cardPAN), stdin);

    size_t PANLength = strlen(cardPAN);

    if ((PANLength < 16) || (PANLength > 19))
    {
        ThisFunction_ErrorState = WRONG_PAN;
    }
    else
    {
        // Remove newline character if present
        if (cardPAN[PANLength - 1] == '\n')
            cardPAN[PANLength - 1] = '\0';

        if (strcpy_s(cardData->primaryAccountNumber, sizeof(cardData->primaryAccountNumber), cardPAN) != 0) {
            ThisFunction_ErrorState = WRONG_PAN;
        }
    }

    return ThisFunction_ErrorState;
}
void testGetCardHolderName(void)
{
    printf("\nTester Name: mostafa mohamed \n");
    printf("Function Name: getCardHolderName\n");
    printf("\nTest Case 1 :\n");
    ST_cardData_t cardData1;
    printf("Enter card holder name: ");
    if (getCardHolderName(&cardData1) == CARD_OK) {
        printf("Expected Result: CARD_OK\nActual Result: CARD_OK\n");
        printf("Card Holder Name: %s\n", cardData1.cardHolderName);
    }
    else {
        printf("Expected Result: CARD_OK\nActual Result: WRONG_NAME\n");
    }

    printf("\nTest Case 2 (Invalid Name - greater than 24 characters or less than ):\n");
    ST_cardData_t cardData2;
    printf("Enter card holder name (greater than 20 characters): ");
    if (getCardHolderName(&cardData2) == WRONG_NAME) {
        printf("Expected Result: WRONG_NAME\nActual Result: WRONG_NAME\n");
    }
    else {
        printf("Expected Result: WRONG_NAME\nActual Result: CARD_OK\n");
    }
}

void testGetCardExpiryDate(void)
{
    printf("\nTester Name: mostafa mohamed\n");
    printf("Function Name: getCardExpiryDate\n");


    printf("\nTest Case 1 :\n");
    ST_cardData_t cardData1;
    printf("Enter card expiration date (MM/YY): ");
    if (getCardExpiryDate(&cardData1) == CARD_OK) {
        printf("Expected Result: CARD_OK\nActual Result: CARD_OK\n");
        printf("Card Expiry Date: %s\n", cardData1.cardExpirationDate);
    }
    else {
        printf("Expected Result: CARD_OK\nActual Result: WRONG_EXP_DATE\n");
    }

    printf("\nTest Case 2 (Invalid Date - not in MM/YY format):\n");
    ST_cardData_t cardData2;
    printf("Enter card expiration date (not in MM/YY format): ");
    if (getCardExpiryDate(&cardData2) == WRONG_EXP_DATE) {
        printf("Expected Result: WRONG_EXP_DATE\nActual Result: WRONG_EXP_DATE\n");
    }
    else {
        printf("Expected Result: WRONG_EXP_DATE\nActual Result: CARD_OK\n");
    }
}

void testGetCardPAN(void)
{
    printf("\nTester Name: mostafa mohamed\n");
    printf("Function Name: getCardPAN\n");


    printf("\nTest Case 1:\n");
    ST_cardData_t cardData1;
    printf("Enter card primary account number (16-19 digits): ");
    if (getCardPAN(&cardData1) == CARD_OK) {
        printf("Expected Result: CARD_OK\nActual Result: CARD_OK\n");
        printf("Card PAN: %s\n", cardData1.primaryAccountNumber);
    }
    else {
        printf("Expected Result: CARD_OK\nActual Result: WRONG_PAN\n");
    }

    printf("\nTest Case 2 (Invalid PAN - less than 16 digits):\n");
    ST_cardData_t cardData2;
    printf("Enter card primary account number (less than 16 digits): ");
    if (getCardPAN(&cardData2) == WRONG_PAN) {
        printf("Expected Result: WRONG_PAN\nActual Result: WRONG_PAN\n");
    }
    else {
        printf("Expected Result: WRONG_PAN\nActual Result: CARD_OK\n");
    }
}
