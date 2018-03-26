#include <ctype.h>
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include <unistd.h>

#include <sodium/utils.h>
#include <tox/tox.h>

typedef struct DHT_node {
    const char *ip;
    uint16_t port;
    const char key_hex[TOX_PUBLIC_KEY_SIZE * 2 + 1];
    unsigned char key_bin[TOX_PUBLIC_KEY_SIZE];
} DHT_node;

void friend_request_cb(Tox *tox, const uint8_t *public_key, const uint8_t *message, size_t length,
                       void *user_data)
{
    tox_friend_add_norequest(tox, public_key, NULL);
}

void friend_message_cb(Tox *tox, uint32_t friend_number, TOX_MESSAGE_TYPE type, const uint8_t *message,
                       size_t length, void *user_data)
{
    tox_friend_send_message(tox, friend_number, type, message, length, NULL);
}

void self_connection_status_cb(Tox *tox, TOX_CONNECTION connection_status, void *user_data)
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

    Tox *tox = tox_new(NULL, NULL);

    const char *name = "Echo Bot";
    tox_self_set_name(tox, (const uint8_t *)name, strlen(name), NULL);

    const char *status_message = "Echoing your messages";
    tox_self_set_status_message(tox, (const uint8_t *)status_message, strlen(status_message), NULL);

    DHT_node nodes[] = {
        {"178.62.250.138",             33445, "788236D34978D1D5BD822F0A5BEBD2C53C64CC31CD3149350EE27D4D9A2F9B6B", {0}},
        {"2a03:b0c0:2:d0::16:1",       33445, "788236D34978D1D5BD822F0A5BEBD2C53C64CC31CD3149350EE27D4D9A2F9B6B", {0}},
        {"tox.zodiaclabs.org",         33445, "A09162D68618E742FFBCA1C2C70385E6679604B2D80EA6E84AD0996A1AC8A074", {0}},
        {"163.172.136.118",            33445, "2C289F9F37C20D09DA83565588BF496FAB3764853FA38141817A72E3F18ACA0B", {0}},
        {"2001:bc8:4400:2100::1c:50f", 33445, "2C289F9F37C20D09DA83565588BF496FAB3764853FA38141817A72E3F18ACA0B", {0}},
        {"128.199.199.197",            33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09", {0}},
        {"2400:6180:0:d0::17a:a001",   33445, "B05C8869DBB4EDDD308F43C1A974A20A725A36EACCA123862FDE9945BF9D3E09", {0}},
        {"node.tox.biribiri.org",      33445, "F404ABAA1C99A9D37D61AB54898F56793E1DEF8BD46B1038B9D822E8460FAB67", {0}}
    };

    for (size_t i = 0; i < sizeof(nodes) / sizeof(DHT_node); i ++) {
        sodium_hex2bin(nodes[i].key_bin, sizeof(nodes[i].key_bin),
                       nodes[i].key_hex, sizeof(nodes[i].key_hex) - 1, NULL, NULL, NULL);
        tox_bootstrap(tox, nodes[i].ip, nodes[i].port, nodes[i].key_bin, NULL);
    }

    uint8_t tox_id_bin[TOX_ADDRESS_SIZE];
    tox_self_get_address(tox, tox_id_bin);

    char tox_id_hex[TOX_ADDRESS_SIZE * 2 + 1];
    sodium_bin2hex(tox_id_hex, sizeof(tox_id_hex), tox_id_bin, sizeof(tox_id_bin));

    for (size_t i = 0; i < sizeof(tox_id_hex) - 1; i ++) {
        tox_id_hex[i] = toupper(tox_id_hex[i]);
    }

    printf("Tox ID: %s\n", tox_id_hex);

    tox_callback_friend_request(tox, friend_request_cb);
    tox_callback_friend_message(tox, friend_message_cb);

    tox_callback_self_connection_status(tox, self_connection_status_cb);

    while (1) {
        tox_iterate(tox, NULL);

        usleep(tox_iteration_interval(tox) * 1000);
    }

    tox_kill(tox);

    return 0;
}
