
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>
#include <time.h>

#define file 1
#define terminal 0
#define TEXT_FILE "readings.txt"
/*
-------------------------------------------------------------------------------------------------------
        FIELD               VALUE RANGE             NOTES                       MEMORY USAGE
-------------------------------------------------------------------------------------------------------
        sensor_id          0 to 255               Unique identifier                 8 bits
        temperature        -128 to 127 (°C)       Can be negative                   8 bits
        humidity           0 to 100 (%)           Percentage                        7 bits
        battery_low        0 (false) or 1 (true)  Boolean flag                      1 bit
        error_code         0 to 7                 Encoded error status              3 bits
        timestamp_seconds  0 to 4095              Seconds since last reboot         12 bits
-------------------------------------------------------------------------------------------------------
*/

typedef struct{
    uint8_t sensor_id;
    int8_t temperature;
    int8_t humidity;
    uint16_t battery_low: 1;
    uint16_t error_code: 3;
    uint16_t timestamp_seconds: 12;
}__attribute__((packed))readings;

typedef struct {
    int8_t   value;   // max value found
    uint16_t ts;      // timestamp for that max
    uint8_t  sid;     // sensor id for that max
} max_stat;

typedef struct {
    max_stat temp;
    max_stat hum;
} max;

void text_write_to_file(FILE *p_text_file, readings *r);
void print_reading(const readings *r);
void print_header(char mode, FILE *p_text_file);
void extract_data(FILE *p_text_file, readings *r);
int random_range(uint8_t min, uint8_t max);

int main(void)
{
    srand(time(NULL));

    readings r = {0};
    //int size = sizeof(r);
    //printf("Size of readings: %d\n", size);

    FILE *p_text_file = NULL;

    //print_header(terminal, p_text_file);
    print_header(file, p_text_file);

    for(uint16_t clock = 0; clock < 1000; clock += 3)
    {   
        r.sensor_id = rand() % 256;
        r.temperature = (int8_t)(rand() % 256 - 128);
        r.humidity = (int8_t)(rand() % 101);
        r.battery_low = rand() % 2;
        r.error_code = rand() % 8; 
        r.timestamp_seconds = clock; 

        //print_reading(&r);
        text_write_to_file(p_text_file, &r);
    }

    extract_data(p_text_file, &r);

    return 0;
}
void print_header(char mode, FILE *p_text_file)
{
    if(mode == terminal)
    {
        printf("\n%10s %10s %10s %10s %10s %10s\n",
        "sensor_id", "temp", "humidity", "battery", "error_code", "timestamp");
        printf("-----------------------------------------------------------------\n");
    }else{
        p_text_file = fopen (TEXT_FILE, "w");
        fprintf(p_text_file,"\n%10s %10s %10s %10s %10s %10s\n",
        "sensor_id", "temp", "humidity", "battery", "error_code", "timestamp");
        fprintf(p_text_file,"-----------------------------------------------------------------\n");
        fclose( p_text_file );
    }
}
void print_reading(const readings *r)
{
    printf("|%10hhu|%10hhd|%10hhd|%10u|%10u|%10u|\n",
        r->sensor_id,
        r->temperature,
        r->humidity,
        r->battery_low,
        r->error_code,
        r->timestamp_seconds);
}

void text_write_to_file(FILE *p_text_file, readings *r)
{
    p_text_file = fopen (TEXT_FILE, "a");
    fprintf(p_text_file,"|%10hhu|%10hhd|%10hhd|%10u|%10u|%10u|\n",
        r->sensor_id,
        r->temperature,
        r->humidity,
        r->battery_low,
        r->error_code,
        r->timestamp_seconds);
    fclose( p_text_file );
}

void extract_data(FILE *p_text_file, readings *r)
{
    char line[128];
        
    p_text_file = fopen(TEXT_FILE, "r");

    max m = {
        .temp = { .value = INT8_MIN, .ts = 0, .sid = 0 },
        .hum  = { .value = INT8_MIN, .ts = 0, .sid = 0 }
    };

    unsigned ts_u = 0;

    while (fgets(line, sizeof(line), p_text_file)) {
        if (line[0] != '|') continue;

        // overwrite *r each time
        if (sscanf(line, "|%hhu|%hhd|%hhd|%*u|%*u|%u|",
            &r->sensor_id,
            &r->temperature,
            &r->humidity,
            &ts_u) != 4) {
            continue;
        }
        r->timestamp_seconds = (uint16_t)(ts_u & 0xFFFu); 

        if (r->temperature > m.temp.value) {
            m.temp.value = r->temperature;
            m.temp.ts = r->timestamp_seconds;
            m.temp.sid = r->sensor_id;
        }

        if (r->humidity > m.hum.value) {
            m.hum.value = r->humidity;
            m.hum.ts = r->timestamp_seconds;
            m.hum.sid = r->sensor_id;
        }
    }

    printf("\n----MAX TEMPERATURE----\n|SENSOR: %hhu| TEMPERATURE: %hhd| TIMESTAMP: %u|\n",
           m.temp.sid, m.temp.value, (unsigned)m.temp.ts);

    printf("\n----MAX HUMIDITY----\n|SENSOR: %hhu| HUMIDITY: %hhd| TIMESTAMP: %u|\n",
           m.hum.sid, m.hum.value, (unsigned)m.hum.ts);
}


