#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <errno.h>
#include <ctype.h>
#include <math.h>

// convert the character array entered by the user to an unsigned char array holding hex values
unsigned char * convertToHexArray(char *newValue) {

    int ii = 0;
    int jj = 0;
    size_t len;

    // check to see if '0x' is at the start, adjust variables accordingly
    if ((newValue[0] == '0') && (newValue[1] == 'x')) {
        len = strlen(newValue) - 2;
        ii = 2;
    }
    else len = strlen(newValue);

    // new unsigned char array that will hold the hex values
    unsigned char *newArray = (unsigned char *) malloc(sizeof(unsigned char) * len);

    // loop through each character in the string entered by the user
    for (; ii <= strlen(newValue); ii++) {
        /*
         * A hex byte represented as a string will be 2 characters, when these
         * two characters are converted to their hexadecimal value they will
         * now be compressed into a single 'character'.
         * To accomplish this I made a temporary array that can hold 2 values.
         * I insert the first and then second character if there is one.
         * After that I convert this temporary character array to its hex value
         * which will be held by a single unsigned char in the unsigned char array.
         * The reason I did this was to allow for writing multiple bytes in the order
         * that they are entered by the user. If the bytes are not written one at a
         * time they will be written to the file in little endian.
         */
        char temp[2] = {0};
        temp[0] = newValue[ii++];
        if (ii <= strlen(newValue)) {
            temp[1] = newValue[ii];
        }
        newArray[jj++] = (unsigned char) strtoul(temp, NULL, 16);
    }

    return newArray;
}

// converts the offset entered by the user in hexadecimal to decimal
double convertToDecimal(char *offset) {

    double decValue = 0;
    int len;
    int ii = 0;
    // check to see if '0x' was entered by the user, set variables accordingly
    if (offset[0] == '0' && offset[1] == 'x') {
        len = (int) strlen(offset) - 3;
        ii = 2;
    }
    else len = (int) strlen(offset) - 1;
    // convert the hex values to decimal
    for (; ii < strlen(offset); ii++) {
        int value = 0;
        if (offset[ii] >= '0' && offset[ii] <= '9') {
            value = offset[ii] - 48;
        }
        else if (tolower(offset[ii]) >= 'a' && tolower(offset[ii]) <= 'f') value = offset[ii] - 87;
        else { // if invalid hex value entered by user print error message and exit
            fprintf(stdout, "Incorrect offset, please only enter hexadecimal.\n");
            exit(1);
        }
        decValue += value * pow(16, len--);
    }

    return decValue;
}

int main(int argc, char *argv[]) {
    FILE *fp;
    char buf;
    size_t num_read = 0;
    long totalLen = 0;
    int offset = 0;

    // check to make sure there are 4 arguments entered
    if (argc == 4) {
        offset = (int) convertToDecimal(argv[1]); // get the offset

        // convert the string from input into an array of unsigned chars holding the hex values
        unsigned char* newWriteVal = convertToHexArray(argv[2]);

        // make sure the offset is greater than 0
        if (offset > 0) {
            // attempt to open the file
            if ((fp = fopen(argv[3], "r+b")) != NULL) {
                fseek(fp, 0, SEEK_END); // seek to the end of the file
                totalLen = ftell(fp);   // get total length of the file
                fseek(fp, 0, SEEK_SET); // seek to the beginning of the file

                // check to see if the offset is greater than the total length of the file
                if (offset > totalLen) {
                    // print an error message and exit if offset is > total length
                    fprintf(stdout, "Offset past end of file.\n");
                    exit(1);
                }
                // read each byte of the file, continuly adding the number of bytes read
                while ((num_read += fread(&buf, sizeof(char), 1, fp))) {
                    // check if the number of bytes read is equal to the offset, this is where we want to write
                    if (num_read == offset) {
                        // write each hex byte held in the array to the file
                        fprintf(stdout, "0x");
                        for (int ii = 0; ii < strlen(newWriteVal); ii++) {
                            fwrite(&newWriteVal[ii], sizeof(unsigned char), 1, fp);
                            fprintf(stdout, "%02x", newWriteVal[ii]);
                        }
                        fprintf(stdout, " successfully written at offset 0x%x\n", offset);
                        break;
                    }
                }
                fclose(fp);
            } else fprintf(stdout, "%s\n", strerror(errno));
        }
    }
    else fprintf(stdout, "./%s [Offset Address (in hex)] [New Values (in hex)] [Input File]\n", argv[0]);

    return 0;
}