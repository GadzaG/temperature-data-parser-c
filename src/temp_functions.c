#include "temp_functions.h"
#include <stdlib.h>
#include <string.h>

void read_csv_to_list(const char *file_path, TemperatureData data_list[], int *data_count) {
    FILE *file = fopen(file_path, "r");
    if (file == NULL) {
        fprintf(stderr, "File open error: %s\n", file_path);
        return;
    }

    char line[MAX_LINE_LENGTH];
    int line_number = 0;

    while (fgets(line, sizeof(line), file)) {
        line_number++;
        char *token;
        char *rest = line;
        int field_count = 0;
        TemperatureData data;

        while ((token = strtok_r(rest, ";", &rest)) != NULL) {
            field_count++;
            switch (field_count) {
                case 1: data.year   = atoi(token); break;
                case 2: data.month  = atoi(token); break;
                case 3: data.day    = atoi(token); break;
                case 4: data.hour   = atoi(token); break;
                case 5: data.minute = atoi(token); break;
                case 6: data.temp   = atof(token); break;
                default: break;
            }
        }

        if (field_count != 6) {
            fprintf(stderr, "Error on line: %d: Wrong arguments count: %s", line_number, line);
            continue;
        }

        if (data.month < 1 || data.month > 12 || data.day < 1 || data.day > 31 ||
            data.hour < 0 || data.hour > 23 || data.minute < 0 || data.minute > 59 ||
            data.temp < -99 || data.temp > 99) {
            fprintf(stderr, "Error on linе %d: Wrong data: %s", line_number, line);
            continue;
        }

        data_list[*data_count] = data;
        (*data_count)++;
    }

    fclose(file);
}

void calculate_temperature_stats(TemperatureData data_list[], int data_count) {
    float monthly_temps[13][MAX_DATA_LENGTH];
    int monthly_counts[13] = {0};
    float all_temps[MAX_DATA_LENGTH];
    int all_count = 0;

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
            printf("Month [%d]:\n\tavg = %.2f\n\tmin = %.2f\n\tmax = %.2f\n", month, avg_temp, min_temp, max_temp);
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
        printf("Year:\n\tavg = %.2f\n\tmin = %.2f\n\tmax = %.2f\n", avg_temp_year, min_temp_year, max_temp_year);
    }
}

void calculate_temperature_stats_for_month(TemperatureData data_list[], int data_count, int month) {
    float monthly_temps[MAX_DATA_LENGTH];
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
        printf("Month [%d]:\n\tavg = %.2f\n\tmin = %.2f\n\tmax = %.2f\n", month, avg_temp, min_temp, max_temp);
    } else {
        printf("No data for month: %d\n", month);
    }
}

void print_data_list(TemperatureData data_list[], int data_count) {
    for (int i = 0; i < data_count; i++) {
        printf("%d.%d.%d %d:%d\ttemp : %.2f\n", data_list[i].year, data_list[i].month, data_list[i].day, data_list[i].hour, data_list[i].minute, data_list[i].temp);
    }
}