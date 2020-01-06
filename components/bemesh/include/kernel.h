/*
 *  	LIBRARIES
 */

#pragma once
#include "gatts_table.h"




#include <stdio.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#include "sdkconfig.h"

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "nvs.h"
#include "nvs_flash.h"



#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_bt_main.h"
#include "esp_gatt_common_api.h"
#include "esp_log.h"

#include "esp_gatts_api.h"
#include "esp_bt_defs.h"

#include <stdlib.h>
#include "freertos/event_groups.h"
#include "esp_system.h"
#include "constant.hpp"


#define GATTS_CHAR_VAL_LEN_MAX 255 //was 0x40

#define TOTAL_NUMBER_LIMIT 7 // Total of incoming and outgoing edges is 7
#define CLIENTS_NUMBER_LIMIT 4 // Incoming links of clients
#define SERVERS_NUMBER_LIMIT 3 // Outgoing or incoming connections with servers
#define PROFILE_NUM      1
// Macros for the ID_TABLE

#define CLIENT 0
#define SERVER 1
#define GATTC_TAG "GATT_CLIENT"
#define GATTS_TAG "GATT_SERVER"
#define REMOTE_SERVICE_UUID        0x00FF
#define REMOTE_NOTIFY_CHAR_UUID    0xFF01
#define PROFILE_A_APP_ID 0
#define INVALID_HANDLE   0


#define SERVERS_NUM 3
#define SERVER_S1 0
#define SERVER_S2 1
#define SERVER_S3 2
#define INVALID_HANDLE   0

struct gattc_profile_inst {
    esp_gattc_cb_t gattc_cb;
    uint16_t gattc_if;
    uint16_t app_id;
    uint16_t conn_id;
    uint16_t service_start_handle;
    uint16_t service_end_handle;
    uint16_t char_handle;
    esp_bd_addr_t remote_bda;
} ;



extern uint8_t CHR_VALUES[HRS_IDX_NB][GATTS_CHAR_VAL_LEN_MAX];
extern uint16_t CHR_HANDLES[HRS_IDX_NB];
extern struct gattc_profile_inst gl_profile_tab2[PROFILE_NUM];
extern uint8_t MACS[TOTAL_NUMBER_LIMIT][MAC_ADDRESS_SIZE];


extern bool becoming_client;
extern bool becoming_server;

typedef void(*NotifyCb)(uint16_t,uint8_t,uint8_t);
typedef void(*InitCb)(uint8_t);
//This callback function pass the newly updated MAC table entry to the master object.
typedef void(*ServerUpdateCb)(uint8_t*,uint8_t);
//This callback function is triggered whenever two servers meet for the first time so that they
//can exchange their routing tables.
typedef void(*ExchangeRoutingTableCb)(uint8_t*,uint8_t*,uint16_t gattc_if,uint8_t conn_id);
typedef void(*ReceivedPacketCb)(uint8_t* packet,uint16_t len);



//
/*
 *  	FUNCTIONS DECLARATION
 */

// Internal clients for a server
void esp_gattc_internal_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);
void esp_gap_S1_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
void esp_gap_S2_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
void esp_gap_S3_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
void gattc_profile_S1_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);
void gattc_profile_S2_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);
void gattc_profile_S3_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

// Client
void esp_gap_cb(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param);
void esp_gattc_cb(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);
void gattc_profile_event_handler(esp_gattc_cb_event_t event, esp_gatt_if_t gattc_if, esp_ble_gattc_cb_param_t *param);

// Server
void gatts_profile_a_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if, esp_ble_gatts_cb_param_t *param);

// Main functions
void ble_esp_startup(); // Use this to start the entire project
void register_internal_client(uint8_t client_num); // Registration of an internal client for a server
void unregister_internal_client(uint8_t client_num);
void gatt_client_main(); // Registration of an ESP client
void gatt_server_main(); // Registration of an ESP server
void unregister_client();
void unregister_server();
void start_internal_client(uint8_t client); // internal clients are SERVER_S1, SERVER_S2, SERVER_S3. This includes the registration
void change_name(uint8_t flag, uint8_t idx); // Flag: 1 -> +, 0 -> -



//Trasferite in characteristic.h

uint8_t find_CHR(uint16_t handle); // Given an handle find the characteristic it refers to
void write_CHR(uint16_t gattc_if, uint16_t conn_id, uint8_t chr, uint8_t* array, uint8_t len);
uint8_t* read_CHR(uint16_t gattc_if, uint16_t conn_id, uint8_t chr);
uint8_t get_CHR_value_len(uint8_t chr); // Get the lenght of the last read value of a characteristic


// SERVER / CLIENT FUNCTIONS
uint8_t get_num_connections(); // Number of connected devices
uint8_t** get_connected_MACS(); // List of all connected devices, either clients or servers
uint8_t get_type_connection(uint8_t conn_id); // Returns CLIENT or SERVER depending on the link with conn_id

uint8_t* get_connid_MAC(uint8_t conn_id); // Returns the MAC of a connected device conn_id
uint8_t get_MAC_connid(uint8_t* mac_addr); // Returns the conn_id from a MAC of a connected device
uint8_t* get_my_MAC(); // Returns the MAC address of the device calling the function

uint8_t get_node_type(); // Returns CLIENT/SERVER
bool is_advertising(); // If the device is a server, returns true if it is advertising, false otherwise
bool is_scanning();	// If the device is a client, returns true if it is scanning. If the device is a server it may be scanning, looking for other servers to connect with.

uint8_t get_gatt_if(); //  Returns the interface of the device calling it, either server or client
uint8_t get_client_connid(); // Returns the conn_id that is assigned from the client to a server (if exists)

// Returns an array of 1 and 0. Position i has value 1 if i is an assigned conn_id, 0 otherwise.
// The array has length TOTAL_NUMBER_LIMIT. Use get_type_connection to know if a conn_id refers to a server or a client
uint8_t* get_server_connids(); 


// INTERNAL CLIENTS FUNCTIONS
// These functions have to be executed if the device is a server
// and the internal client of interest has been registered using register_internal_client(client_num);
// Client ids are: SERVER_S1, SERVER_S2, SERVER_S3
uint8_t get_internal_client_connid(uint8_t client_id);
uint8_t get_internal_client_gattif(uint8_t client_id);
uint8_t* get_internal_client_serverMAC(uint8_t client_id); // Returns the MAC address of the server which is connected to


// CALLBACKS
uint8_t install_NotifyCb(NotifyCb cb); // Returns 0 on succes, 1 otherwise
uint8_t install_InitCb(InitCb cb); //Same as above.
uint8_t install_ServerUpdateCb(ServerUpdateCb cb); //Same as above
uint8_t install_ExchangeRoutingTableCb(ExchangeRoutingTableCb cb); //Same as above
uint8_t install_ReceivedPacketCb(ReceivedPacketCb cb); //Same as above

bool has_ended_scanning();
