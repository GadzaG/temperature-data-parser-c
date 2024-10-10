#include "temp_functions.h"
#include <stdlib.h>
#include <string.h>

char monthNames[13][10] = {
    "", "January", "February", "March", "April", "May", "June",
    "July", "August", "September", "October", "November", "December"
};


int is_valid_number(const char *str, int min, int max) {
    char *endptr;
    long val = strtol(str, &endptr, 10);

    if (*endptr != '\0' || val < min || val > max) {
        return 0;
    }
    return 1;
}

int parse_line(const char *line, TemperatureData *data) {
    char *token;
    char line_copy[MAX_LINE_LENGTH];
    strncpy(line_copy, line, MAX_LINE_LENGTH);
    line_copy[MAX_LINE_LENGTH - 1] = '\0';

    token = strtok(line_copy, ";");
    if (token == NULL || !is_valid_number(token, 1000, 9999)) return 0;
    data->year = atoi(token);

    token = strtok(NULL, ";");
    if (token == NULL || !is_valid_number(token, 1, 12)) return 0;
    data->month = atoi(token);

    token = strtok(NULL, ";");
    if (token == NULL || !is_valid_number(token, 1, 31)) return 0;
    data->day = atoi(token);

    token = strtok(NULL, ";");
    if (token == NULL || !is_valid_number(token, 0, 23)) return 0;
    data->hour = atoi(token);

    token = strtok(NULL, ";");
    if (token == NULL || !is_valid_number(token, 0, 59)) return 0;
    data->minute = atoi(token);

    token = strtok(NULL, ";");
    if (token == NULL || !is_valid_number(token, -99, 99)) return 0;
    data->temp = atof(token);

    return 1;
}

void read_csv_to_list(const char *file_path, TemperatureData **data_list, int *data_count) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "File open error: %s\n", file_path);
        return;
    }

    int line_count = 0;
    char line[MAX_LINE_LENGTH];
    while (fgets(line, sizeof(line), file)) {
        line_count++;
    }
    rewind(file);

    *data_list = (TemperatureData *)malloc(sizeof(TemperatureData) * line_count);
    if (*data_list == NULL) {
        fprintf(stderr, "Memory allocation error\n");
        fclose(file);
        return;
    }

    int line_number = 0;
    *data_count = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        TemperatureData data;
        line[strcspn(line, "\n")] = '\0';
        if (parse_line(line, &data)) {
            (*data_list)[*data_count] = data;
            (*data_count)++;
        } else {
            fprintf(stderr, "Error on line %d: %s\n", line_number, line);
        }
    }
    fclose(file);
}

void calculate_temperature_stats(TemperatureData* data_list, int data_count) {
    float **monthly_temps = (float **)malloc(sizeof(float *) * 13);
    int monthly_counts[13] = {0};
    float *all_temps = (float *)malloc(sizeof(float) * data_count);
    int all_count = 0;

    for (int i = 0; i < 13; i++) {
        monthly_temps[i] = (float *)malloc(sizeof(float) * data_count);
    }

    for (int i = 0; i < data_count; i++) {
        int month = data_list[i].month;
        monthly_temps[month][monthly_counts[month]] = data_list[i].temp;
        monthly_counts[month]++;
        all_temps[all_count] = data_list[i].temp;
        all_count++;
    }

    for (int month = 1; month <= 12; month++) {
        if (monthly_counts[month] > 0) {
            float sum = 0;
            for (int i = 0; i < monthly_counts[month]; i++) {
                sum += monthly_temps[month][i];
            }
            float avg_temp = sum / monthly_counts[month];
            float min_temp = monthly_temps[month][0];
            float max_temp = monthly_temps[month][0];
            for (int i = 1; i < monthly_counts[month]; i++) {
                if (monthly_temps[month][i] < min_temp) min_temp = monthly_temps[month][i];
                if (monthly_temps[month][i] > max_temp) max_temp = monthly_temps[month][i];
            }
            printf("\"%s\" : {\n\t\"avg\": %.2f,\n\t\"min\": %.2f,\n\t\"max\": %.2f\n},\n", monthNames[month], avg_temp, min_temp, max_temp);
        }
    }

    if (all_count > 0) {
        float sum = 0;
        for (int i = 0; i < all_count; i++) {
            sum += all_temps[i];
        }
        float avg_temp_year = sum / all_count;
        float min_temp_year = all_temps[0];
        float max_temp_year = all_temps[0];
        for (int i = 1; i < all_count; i++) {
            if (all_temps[i] < min_temp_year) min_temp_year = all_temps[i];
            if (all_temps[i] > max_temp_year) max_temp_year = all_temps[i];
        }
        printf("\"%d\" : {\n\t\"avg\": %.2f,\n\t\"min\": %.2f,\n\t\"max\": %.2f\n},\n", data_list[0].year, avg_temp_year, min_temp_year, max_temp_year);
        //printf("Year:\n\tavg = %.2f\n\tmin = %.2f\n\tmax = %.2f\n", avg_temp_year, min_temp_year, max_temp_year);
    }
}

void calculate_temperature_stats_for_month(TemperatureData* data_list, int data_count, int month) {
    float* monthly_temps = (float*)malloc(sizeof(float) * data_count);
    int monthly_count = 0;

    for (int i = 0; i < data_count; i++) {
        if (data_list[i].month == month) {
            monthly_temps[monthly_count] = data_list[i].temp;
            monthly_count++;
        }
    }

    if (monthly_count > 0) {
        float sum = 0;
        for (int i = 0; i < monthly_count; i++) {
            sum += monthly_temps[i];
        }
        float avg_temp = sum / monthly_count;
        float min_temp = monthly_temps[0];
        float max_temp = monthly_temps[0];
        for (int i = 1; i < monthly_count; i++) {
            if (monthly_temps[i] < min_temp) min_temp = monthly_temps[i];
            if (monthly_temps[i] > max_temp) max_temp = monthly_temps[i];
        }
        printf("\"%s\" : {\n\t\"avg\": %.2f,\n\t\"min\": %.2f,\n\t\"max\": %.2f\n},\n", monthNames[month], avg_temp, min_temp, max_temp);
    } else {
        printf("No data for month: %d\n", month);
    }
    free(monthly_temps);
}

void print_data_list(TemperatureData data_list[], int data_count) {
    for (int i = 0; i < data_count; i++) {
        printf("%d.%02d.%02d %02d:%02d Temp: %.2f\n",
               data_list[i].year, data_list[i].month, data_list[i].day,
               data_list[i].hour, data_list[i].minute, data_list[i].temp);
    }
}
