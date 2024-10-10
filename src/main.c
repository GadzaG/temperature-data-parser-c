#include "temp_functions.h"
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[]) {
    
    // char* str = "-20";

    // printf("%d\n", is_valid_number(str, -99, 99));
    // return 0;
    const char *file_path = NULL;
    int month = 0;

    for (int i = 1; i < argc; i++) {
        if (strcmp(argv[i], "-f") == 0 || strcmp(argv[i], "--file") == 0) {
            if (i + 1 < argc) {
                file_path = argv[++i];
            } else {
                fprintf(stderr, "Error: expected file path after -f or --file\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-m") == 0 || strcmp(argv[i], "--month") == 0) {
            if (i + 1 < argc) {
                month = atoi(argv[++i]);
                if ((month < 1) || (month > 12)) {
                    fprintf(stderr, "Error: wrong month number [1 : 12]\n");
                    return 1;
                }
            } else {
                fprintf(stderr, "Error: expected month number -m or --month\n");
                return 1;
            }
        } else if (strcmp(argv[i], "-h") == 0 || strcmp(argv[i], "--help") == 0) {
            printf("Using: %s [-f <file_path>] [-m <month>]\n", argv[0]);
            printf("  -f, --file <file_path>  Path to .csv file\n");
            printf("  -m, --month <month>     Month number (1-12)\n");
            printf("  -h, --help              Show help and exit\n");
            return 0;
        }
    }

    if (file_path == NULL) {
        printf("Using: %s [-f <file_path>] [-m <month>]\n", argv[0]);
        printf("  -f, --file <file_path>  Path to .csv file\n");
        printf("  -m, --month <month>     Month number (1-12)\n");
        printf("  -h, --help              Show help and exit\n");
        return 0;
    }

    TemperatureData* data_list = NULL;
    int data_count = 0;

    read_csv_to_list(file_path, &data_list, &data_count);

    //print_data_list(data_list, data_count);

    if (month > 0 && month <= 12) {
        calculate_temperature_stats_for_month(data_list, data_count, month);
    } else {
        calculate_temperature_stats(data_list, data_count);
    }

    free(data_list);

    return 0;
}
