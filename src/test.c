#include <ncurses.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include <fcntl.h>
#include <math.h>
#include <time.h>
#include <stdio.h>
#include <sys/stat.h>
#include <yaml.h>



// Initialize configuration variables with default values
float DRONE_MASS = 1.0;
float DRONE_FRICTION = 1.0;
float COMMAND_FORCE_STEP = 1.0;
float REPULSIVE_CONSTANT = 10.0;
float ATTRACTIVE_CONSTANT = 0.1;
float PERCEPTION_RADIUS = 5.0;
float TIME_STEP = 0.01;

void load_config(const char *config_file);

void main()
{
    const char *config_file = "config/config.yaml";

    while (1)
    {
        load_config(config_file);
        sleep(5);
        printf("-----------------\n%f\n-----------", TIME_STEP);
    }
    
    
}


void load_config(const char *config_file) {
    FILE *file = fopen(config_file, "r");
    if (!file) {
        perror("Failed to open configuration file");
        return;
    }

    yaml_parser_t parser;
    yaml_document_t document;
    yaml_node_t *node, *key_node, *value_node;

    if (!yaml_parser_initialize(&parser)) {
        fprintf(stderr, "Failed to initialize YAML parser\n");
        fclose(file);
        return;
    }

    yaml_parser_set_input_file(&parser, file);

    if (!yaml_parser_load(&parser, &document)) {
        fprintf(stderr, "Failed to load YAML document\n");
        yaml_parser_delete(&parser);
        fclose(file);
        return;
    }

    node = yaml_document_get_root_node(&document);
    if (node && node->type == YAML_MAPPING_NODE) {
        for (yaml_node_pair_t *pair = node->data.mapping.pairs.start;
             pair < node->data.mapping.pairs.top; pair++) {
            key_node = yaml_document_get_node(&document, pair->key);
            value_node = yaml_document_get_node(&document, pair->value);
            if (key_node && value_node && key_node->type == YAML_SCALAR_NODE && value_node->type == YAML_SCALAR_NODE) {
                const char *key = (const char *)key_node->data.scalar.value;
                const char *value = (const char *)value_node->data.scalar.value;


                if (strcmp(key, "DRONE_MASS") == 0) {
                    DRONE_MASS = atof(value);
                } else if (strcmp(key, "DRONE_FRICTION") == 0) {
                    DRONE_FRICTION = atof(value);
                } else if (strcmp(key, "COMMAND_FORCE_STEP") == 0) {
                    COMMAND_FORCE_STEP = atof(value);
                } else if (strcmp(key, "REPULSIVE_CONSTANT") == 0) {
                    REPULSIVE_CONSTANT = atof(value);
                } else if (strcmp(key, "ATTRACTIVE_CONSTANT") == 0) {
                    ATTRACTIVE_CONSTANT = atof(value);
                } else if (strcmp(key, "PERCEPTION_RADIUS") == 0) {
                    PERCEPTION_RADIUS = atof(value);
                } else if (strcmp(key, "TIME_STEP") == 0) {
                    TIME_STEP = atof(value);
                }
            }
        }
    }

    yaml_document_delete(&document);
    yaml_parser_delete(&parser);
    fclose(file);
}