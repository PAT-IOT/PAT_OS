#include "PAT_JsonStorage.h"
#include "PAT_DataBase.h"

//===============================================================================================================================================================================================
//                                                                                         / dbuser
//===============================================================================================================================================================================================
const char *dbuser_json = R"({
"users": [
    {
        "id": 0,
        "userId": 1,
        "username": "vendor",
        "password": "vendor",
        "role": "vendor"
    },
    {
        "id": 1,
        "userId": 2,
        "username": "supperAdmin",
        "password": "supperAdmin",
        "role": "supperAdmin"
    },
    {
        "id": 2,
        "userId": 3,
        "username": "admin",
        "password": "admin",
        "role": "admin"
    },
    {
        "id": 3,
        "userId": 4,
        "username": "user",
        "password": "user",
        "role": "user"
    }
]
})";
JsonStorage dbuser("/dbuser.json", dbuser_json, strlen(dbuser_json));
//===============================================================================================================================================================================================
//                                                                                         / dbaccount
//===============================================================================================================================================================================================
// {
//     "accounts" : [
//         {
//             "id" : 0,
//             "userId" : 1,
//             "username" : "vendor",
//             "password" : "vendor",
//             "role" : "Vendor",
//             "sessions" : [
//                 {
//                     "token" : "2WGOM;ItDDGxUVkRV>3V",
//                     "activity" : "2024-10-01 14:30:00"
//                 }
//             ]
//         },
//         {
//             "id" : 0,
//             "userId" : 1,
//             "username" : "vendor",
//             "password" : "vendor",
//             "role" : "Vendor",
//             "sessions" : [
//                 {
//                     "token" : "2WGOM;ItDDGxUVkRV>3V",
//                     "activity" : "2024-10-01 14:30:00"
//                 },
//                 {
//                     "token" : "4MJ*oQ<8vTZR6YpW2j",
//                     "activity" : "2024-10-04 18:50:00"
//                 }
//             ]
//         }
//     ]
// }
const char *dbaccount_json = R"({
 "accounts" : []
})";
JsonStorage dbaccount("/dbaccount.json", dbaccount_json, 4096);
//===============================================================================================================================================================================================
//                                                                                         / dbsetting;
//===============================================================================================================================================================================================
const char *dbsetting_json = R"({
    "settings": {
        "wifi": {
            "enable": true,
            "ssids": [
                {
                    "name": "PAT_IOT",
                    "mac": "B6:11:74:86:64:A1",
                    "rssi_db": -59,
                    "rssi_percentage": "82%",
                    "rssi_distance": 3
                },
                {
                    "name": "Farazin",
                    "mac": "7E:1C:68:6E:DA:AB",
                    "rssi_db": -89,
                    "rssi_percentage": "22%",
                    "rssi_distance": 25
                },
                {
                    "name": "B7tb-dXNlcjI5MzA3Nzg2ODc5",
                    "mac": "7E:1C:68:6E:DA:AB",
                    "rssi_db": -89,
                    "rssi_percentage": "22%",
                    "rssi_distance": 25
                }
            ],
            "connections": [
                {
                    "enable": true,
                    "name": "PAT_IOT",
                    "password": "Password_1"
                },
                {
                    "enable": false,
                    "name": "mahsa",
                    "password": "1123456789"
                }
            ]
        },
        "hotspot": {
            "enable": false,
            "name": "NAR_14285576",
            "password": "NAR_14285576",
            "status": "Disconnected",
            "ip": "192.168.80.200",
            "gw": "192.168.80.1",
            "sn": "255.255.255.0",
            "dns": "8.8.8.8"
        },
        "lan": {
            "enable": true,
            "status": "Disconnected",
            "static": {
                "enable": false,
                "mac": "DEADBEEFFE01",
                "ip": "192.168.88.220",
                "gw": "192.168.88.1",
                "sn": "255.255.255.0",
                "dns": "8.8.8.8"
            }
        },
        "inputsOutputs": {
            "number_of_channels": 12,
            "channels": [
                {
                    "channel": 1,
                    "name": "Red Light",
                    "icon": 101,
                    "signal": "relayOutput",
                    "description": "light of restroom"
                },
                {
                    "channel": 2,
                    "name": "outlet",
                    "icon": 55,
                    "signal": "relayOutput",
                    "description": "Laptop outlet room 15 first floor"
                },
                {
                    "channel": 3,
                    "name": "Red Light",
                    "icon": 101,
                    "signal": "relayOutput",
                    "description": "light of restroom"
                },
                {
                    "channel": 4,
                    "name": "outlet",
                    "icon": 55,
                    "signal": "relayOutput",
                    "description": "Laptop outlet room 15 first floor"
                },
                {
                    "channel": 5,
                    "name": "Red Light",
                    "icon": 101,
                    "signal": "relayOutput",
                    "description": "light of restroom"
                },
                {
                    "channel": 6,
                    "name": "outlet",
                    "icon": 55,
                    "signal": "relayOutput",
                    "description": "Laptop outlet room 15 first floor"
                },
                {
                    "channel": 7,
                    "name": "Red Light",
                    "icon": 101,
                    "signal": "relayOutput",
                    "description": "light of restroom"
                },
                {
                    "channel": 8,
                    "name": "outlet",
                    "icon": 55,
                    "signal": "relayOutput",
                    "description": "Laptop outlet room 15 first floor"
                },
                {
                    "channel": 9,
                    "name": "Red Light",
                    "icon": 101,
                    "signal": "relayOutput",
                    "description": "light of restroom"
                },
                {
                    "channel": 10,
                    "name": "outlet",
                    "icon": 55,
                    "signal": "relayOutput",
                    "description": "Laptop outlet room 15 first floor"
                },
                {
                    "channel": 11,
                    "name": "Red Light",
                    "icon": 101,
                    "signal": "relayOutput",
                    "description": "light of restroom"
                },
                {
                    "channel": 12,
                    "name": "outlet",
                    "icon": 55,
                    "signal": "relayOutput",
                    "description": "Laptop outlet room 15 first floor"
                }
            ]
        },
        "mqtt": {
            "enable": false,
            "broker": {
                "name": "Admin",
                "password": "1234567894",
                "ip": "192.168.1.185",
                "port": "9001"
            }
        },
        "dateTime": "xx:xx",
        "password": {
            "superAdmin": "**********",
            "admin": "**********",
            "user": "**********"
        },
        "modelName": "Novaday IOT Controller",
        "serialNumber": "NAR_xxxxxxxx",
        "lastSyncWithServer": "14xx-xx-xx xx:xx",
        "version": "1.0.0"
    }
})";

JsonStorage dbsetting("/dbsetting.json", dbsetting_json, strlen(dbsetting_json) + 1024); // Use strlen instead of sizeof
//===============================================================================================================================================================================================
//                                                                                         / db_webServer_relay
//===============================================================================================================================================================================================
const char *db_Home_json = R"({
"Relay": [0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1],
"Name": ["Light1", "Light2", "Light3", "Light4", "Light5", "Light6", "Light7", "Light8", "Light9", "Light10", "Light11", "Light12"],
"Description": ["Room1", "Room2", "Room3", "Room4", "Room5", "Room6", "Room7", "Room8", "Room9", "Room10", "Room11", "Room12"],
"Icon": ["lighting", "lighting", "lighting", "lighting", "lighting", "lighting", "lighting", "lighting", "lighting", "lighting", "lighting", "lighting"]
})";

JsonStorage db_Home("/db_Home.json", db_Home_json, strlen(db_Home_json) + 500); // Use strlen instead of sizeof
