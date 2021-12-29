#ifndef _POSIX_C_SOURCE
#define _POSIX_C_SOURCE 200809L
#endif

#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include <unistd.h>

#include <sodium/utils.h>
#include <tox/tox.h>

typedef struct DHT_node {
    const char *ip;
    uint16_t port;
    const char key_hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];
} DHT_node;

const char *savedata_filename = "savedata.tox";
const char *savedata_tmp_filename = "savedata.tox.tmp";

Tox *create_tox(void)
{
    Tox *tox;

    struct Tox_Options options;

    tox_options_default(&options);

    FILE *f = fopen(savedata_filename, "rb");

    if (f) {
        fseek(f, 0, SEEK_END);
        long fsize = ftell(f);
        fseek(f, 0, SEEK_SET);

        uint8_t *savedata = malloc(fsize);

        size_t read_size = fread(savedata, fsize, 1, f);
        fclose(f);
        if (read_size != fsize) {
            return NULL;
        }

        options.savedata_type = TOX_SAVEDATA_TYPE_TOX_SAVE;
        options.savedata_data = savedata;
        options.savedata_length = fsize;

        tox = tox_new(&options, NULL);

        free(savedata);
    } else {
        tox = tox_new(&options, NULL);
    }

    return tox;
}

void update_savedata_file(const Tox *tox)
{
    size_t size = tox_get_savedata_size(tox);
    uint8_t *savedata = malloc(size);
    tox_get_savedata(tox, savedata);

    FILE *f = fopen(savedata_tmp_filename, "wb");
    fwrite(savedata, size, 1, f);
    fclose(f);

    rename(savedata_tmp_filename, savedata_filename);

    free(savedata);
}

void bootstrap(Tox *tox)
{
    DHT_node nodes[] = {
        {"85.143.221.42",                      33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
        {"2a04:ac00:1:9f00:5054:ff:fe01:becd", 33445, "DA4E4ED4B697F2E9B000EEFE3A34B554ACD3F45F5C96EAEA2516DD7FF9AF7B43"},
        {"78.46.73.141",                       33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
        {"2a01:4f8:120:4091::3",               33445, "02807CF4F8BB8FB390CC3794BDF1E8449E9A8392C5D3F2200019DA9F1E812E46"},
        {"tox.initramfs.io",                   33445, "3F0A45A268367C1BEA652F258C85F4A66DA76BCAA667A49E770BCC4917AB6A25"},
        {"tox2.abilinski.com",                 33445, "7A6098B590BDC73F9723FC59F82B3F9085A64D1B213AAF8E610FD351930D052D"},
        {"205.185.115.131",                       53, "3091C6BEB2A993F1C6300C16549FABA67098FF3D62C6D253828B531470B53D68"},
        {"tox.kurnevsky.net",                  33445, "82EF82BA33445A1F91A7DB27189ECFC0C013E06E3DA71F588ED692BED625EC23"}
    };

    for (size_t i = 0; i < sizeof(nodes) / sizeof(DHT_node); i ++) {
        unsigned char key_bin[TOX_PUBLIC_KEY_SIZE];
        sodium_hex2bin(key_bin, sizeof(key_bin), nodes[i].key_hex, sizeof(nodes[i].key_hex) - 1,
                       NULL, NULL, NULL);
        tox_bootstrap(tox, nodes[i].ip, nodes[i].port, key_bin, NULL);
    }
}

void print_tox_id(Tox *tox)
{
    uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(tox, tox_id_bin);

    char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
    sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));

    for (size_t i = 0; i < sizeof(tox_id_hex) - 1; i ++) {
        tox_id_hex[i] = toupper(tox_id_hex[i]);
    }

    printf("Tox ID: %s\n", tox_id_hex);
}

void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length,
                       void *user_data)
{
    tox_friend_add_norequest(tox, public_key, NULL);

    update_savedata_file(tox);
}

void friend_message_cb(Tox *tox, uint32_t friend_number, Tox_Message_Type type, const uint8_t *message,
                       size_t length, void *user_data)
{
    tox_friend_send_message(tox, friend_number, type, message, length, NULL);
}

void self_connection_status_cb(Tox *tox, Tox_Connection connection_status, void *user_data)
{
    switch (connection_status) {
        case TOX_CONNECTION_NONE:
            printf("Offline\n");
            break;

        case TOX_CONNECTION_TCP:
            printf("Online, using TCP\n");
            break;

        case TOX_CONNECTION_UDP:
            printf("Online, using UDP\n");
            break;
    }
}

int main(int argc, char **argv)
{
    for (char **arg = argv; arg < argv + argc; ++arg) {
        if (strcmp(*arg, "--help") == 0 || strcmp(*arg, "-h") == 0) {
            printf("Usage: %s\n", argv[0]);
            return 0;
        }
    }

    Tox *tox = create_tox();
    if (tox == NULL) {
        fprintf(stderr, "Failed to create Tox instance\n");
        return 1;
    }

    const char *name = "Echo Bot";
    tox_self_set_name(tox, (const uint8_t *)name, strlen(name), NULL);

    const char *status_message = "Echoing your messages";
    tox_self_set_status_message(tox, (const uint8_t *)status_message, strlen(status_message), NULL);

    bootstrap(tox);

    print_tox_id(tox);

    tox_callback_friend_request(tox, friend_request_cb);
    tox_callback_friend_message(tox, friend_message_cb);

    tox_callback_self_connection_status(tox, self_connection_status_cb);

    update_savedata_file(tox);

    printf("Connecting...\n");

    while (1) {
        tox_iterate(tox, NULL);

        struct timespec pause;
        pause.tv_sec = 0;
        pause.tv_nsec = tox_iteration_interval(tox) * 1000 * 1000;
        nanosleep(&pause, NULL);
    }

    tox_kill(tox);

    return 0;
}
