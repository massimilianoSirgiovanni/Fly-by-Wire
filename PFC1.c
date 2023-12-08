#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <math.h>

#define MAINSTRING "$GPGLL"

struct Coordinates {
    double lat_deg;
    char lat;
    double lon_deg;
    char lon;
};

double deg_to_rad(double deg) {
    return deg * M_PI / 180;
}

double calculate_distance(struct Coordinates c0, struct Coordinates c1) {
    double earth_radius = 6367770;

    double diff_lat = deg_to_rad(c1.lat_deg - c0.lat_deg);
    double diff_lon = deg_to_rad(c1.lon_deg - c0.lon_deg);

    c0.lat_deg = deg_to_rad(c0.lat_deg);
    c1.lat_deg = deg_to_rad(c1.lat_deg);

    double a = sin(diff_lat / 2) * sin(diff_lat / 2) + sin(diff_lon / 2) * sin(diff_lon / 2) * cos(c0.lat_deg) * cos(c0.lat_deg);
    double c = 2 * atan2(sqrt(a), sqrt(1-a));

    return earth_radius * c;
}

int intOf(char c) {
    return c - '0';
}

struct Coordinates parse_line(char *line) {
    struct Coordinates coord = {0, 'A', 0, 'B'};
    printf("%s", line);
    int i = 0;
    double lat_min = 0.0, lon_min = 0.0;
    // Parsing stringa
    while(line[i] != ',') {
        i++;
    }
    coord.lat_deg = intOf(line[++i]) * 10 + intOf(line[++i]);
    lat_min = intOf(line[++i]) * 10 + intOf(line[++i]);
    i++;
    lat_min += intOf(line[++i]) * 0.1 + intOf(line[++i]) * 0.01 + intOf(line[++i]) * 0.001 + intOf(line[++i]) * 0.0001;
    i++;
    coord.lat = line[++i];

    i++;
    coord.lon_deg = intOf(line[++i]) * 100 + intOf(line[++i]) * 10 + intOf(line[++i]);
    lon_min = intOf(line[++i]) * 10 + intOf(line[++i]);
    i++;
    lon_min += intOf(line[++i]) * 0.1 + intOf(line[++i]) * 0.01 + intOf(line[++i]) * 0.001 + intOf(line[++i]) * 0.0001;
    i++;
    coord.lon = line[++i];

    coord.lat_deg += lat_min / 60;
    coord.lon_deg += lon_min / 60;
    //printf("LAT %d DEG %f MIN %c\n", coord.lat_deg, coord.lat_min, coord.lat);
    //printf("LON %d DEG %f MIN %c\n", coord.lon_deg, coord.lon_min, coord.lon);

    return coord;
}

int line_contains_coordinates(char *line) {
    char string_to_compare[6];

    strncpy(string_to_compare, line, 6);
    string_to_compare[6] = 0;

    if(strcmp(string_to_compare, MAINSTRING) == 0) {
        printf("WE GOT ONE\n");
        printf("%s", line);
        return 1;
        //parse_line(line);
    }
    else {
        printf("NOT WHAT WE LOOKIN FOR; %s IS NOT %s \n", string_to_compare, MAINSTRING);
        return 0;
    }
}

FILE *create_file() {
    FILE *func_file_pointer;

    func_file_pointer = fopen("./G18.txt", "r");
    if(func_file_pointer == NULL) {
        // TODO? Codice che invia messaggio di errore o roba del genere
        exit(EXIT_FAILURE);
    }
    return func_file_pointer;
}

int main1(void) {
    FILE *file_pointer = create_file();
    // Lettura linee del file
    char *line_pointer = NULL;
    int len = 0;
    ssize_t read;

    struct Coordinates coord0;
    struct Coordinates coord1;

    int first_read = 1;

    double distance = 0;
    double speed = 0;

    while((read = getline(&line_pointer, &len, file_pointer)) != -1) {
        //printf("%s", line_pointer);
        if(line_contains_coordinates(line_pointer) == 1){
            if(first_read == 1) {
                coord1 = parse_line(line_pointer);
                first_read = 0;
            }
            else {
                coord0 = coord1;
                coord1 = parse_line(line_pointer);

                distance = calculate_distance(coord0, coord1);

                printf("LAT %f DEG %c\n", coord1.lat_deg, coord1.lat);
                printf("LON %f DEG %c\n", coord1.lon_deg, coord1.lon);

                printf("DISTANCE %f m\n", distance);
                printf("SPEED %f m/s\n", distance);
            }
            sleep(0);
        }
    }

    return 0;
}
