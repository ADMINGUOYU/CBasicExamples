#include "large_uint_mul.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h> // use of strcspn()

#define MAX_INPUT_BUFFER_SIZE 100

// forward declaration
char* large_uint_mul(const char* multiplicand, const char* multiplier);
void get_input_safe(char* buff);

// main function
int large_uint_mul_main(int argc, char *argv[])
{
    // print welcome message
    fprintf(stdout, "Welcome to \"LOOOOOOG unsigned int multiplication\"!\n");
    fprintf(stdout, " -> You know sometimes even \"unsigned long long\" doesn't work...\n");
    fflush(stdout);

    // prepare buffer for multiplicand and multiplier
    char* multiplicand = (char*)malloc(MAX_INPUT_BUFFER_SIZE * sizeof(char));
    char* multiplier = (char*)malloc(MAX_INPUT_BUFFER_SIZE * sizeof(char));

    // ask user for input
    printf("Please input <multiplicand>: \n>> ");
    // fetch input from stdin
    get_input_safe(multiplicand);
    // ask user for input
    printf("Please input <multiplier>: \n>> ");
    // fetch input from stdin
    get_input_safe(multiplier);

    // check if either is empty
    if ((multiplicand[0] == '\0') || (multiplier[0] == '\0'))
    {
        // print message
        printf("Either multiplicand or multiplier is empty, try again next time!\n");
        // tidy-up
        free(multiplicand);
        free(multiplier);
        return 0;
    }

    // confirm inputs
    printf("-> Now we'll calculate %s * %s\n", multiplicand, multiplier);

    // run calculation function
    char* result = large_uint_mul(multiplicand, multiplier);

    // echo result
    printf("-> %s * %s\n = %s\n", multiplicand, multiplier, result);

    // delete allocated memory (free memory)
    free(result);
    free(multiplicand);
    free(multiplier);

    // prompt use, continue?
    printf("Great! All DONE :-)\n");
    
    return 0;
}

// definition of function
char* large_uint_mul(const char* multiplicand, const char* multiplier)
{
    // error checking
    if (!(multiplicand && multiplier)) return NULL;
    // calculate number of digit first
    unsigned int multiplicand_digit = 0;
    for ( ; multiplicand[multiplicand_digit] != '\0'; ++multiplicand_digit);
    unsigned int multiplier_digit = 0;
    for ( ; multiplier[multiplier_digit] != '\0'; ++multiplier_digit);
    // determine bit(s) we need
    unsigned int op_bits = (multiplicand_digit > multiplier_digit) ? multiplicand_digit : multiplier_digit;
    // allocate space for multiplier AND Ans
    unsigned int op_buff_last_index = (2 * op_bits) - 1;
    unsigned int op_buff_start_op_index = op_bits - 1;
    char* op_buff = (char*)malloc(2 * op_bits * sizeof(char));
    if (!op_bits) return NULL;
    for (unsigned int i = 0; i <= op_buff_last_index; ++i) op_buff[i] = 0;
    // allocate space for multiplicand
    char* multiplicand_buff = (char*)malloc(op_bits * sizeof(char));
    if (!multiplicand_buff) return NULL;
    for (unsigned int i = 0; i < op_bits; ++i) multiplicand_buff[i] = 0;
    // copy multiplier to op_buff
    for (unsigned int i = op_buff_last_index, j = multiplier_digit - 1; ; --i, --j)
    {
        op_buff[i] = multiplier[j] - '0';
        // end condition
        if (j == 0) break;
    }
    // copy multiplicand to multiplicand_buff
    for (unsigned int i = op_bits - 1, j = multiplicand_digit - 1; ; --i, --j)
    {
        multiplicand_buff[i] = multiplicand[j] - '0';
        // end condition
        if (j == 0) break;
    }
    // perform calculation
    for (unsigned int i = 0; i < op_bits; ++i)
    {
        // define carry
        char carry = 0;
        // perform addition
        for (unsigned int j = op_bits - 1, op = op_buff_start_op_index; ; --j, --op)
        {
            char result = (multiplicand_buff[j] * op_buff[op_buff_last_index]);
            result += (op_buff[op] + carry);
            carry = (char)(result / 10);
            result %= 10;
            op_buff[op] = result;
            // end condition
            if (j == 0) break;
        }
        // shift to the right
        for (unsigned int j = op_buff_last_index; j > 0; --j)
            op_buff[j] = op_buff[j - 1];
        // put carry to the first place
        op_buff[0] = carry;
    }
    // now we have result in op_buff[op_bits : END]
    // count Ans digits
    unsigned int ans_digit = 2 * op_bits;
    for (unsigned int i = 0; i <= op_buff_last_index; ++i)
    {
        if (op_buff[i] > 0)
            break;
        else
            --ans_digit;
    }
    // create return buffer
    char* ret_buff = (char*)malloc(ans_digit * sizeof(char) + 1);
    // copy buffer
    for (unsigned int i = ans_digit - 1, j = op_buff_last_index; ; --i, --j)
    {
        ret_buff[i] = op_buff[j] + '0';
        // end condition
        if (i == 0) break;
    }
    ret_buff[ans_digit] = '\0';
    // delete op_buff and multiplicand_buff
    free(op_buff);
    free(multiplicand_buff);
    // return
    return ret_buff;
}

void get_input_safe(char* buff)
{
    if (fgets(buff, MAX_INPUT_BUFFER_SIZE, stdin) != NULL) {
        // Find '\n' and replace it with '\0'
        buff[strcspn(buff, "\n")] = '\0';
    }
}
