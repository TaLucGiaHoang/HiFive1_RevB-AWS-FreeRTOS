/*
 * esp32_AT.h
 *
 *  Created on: Nov 4, 2019
 *      Author: HoangSHC
 */

#ifndef ESP32_AT_H_
#define ESP32_AT_H_

#define WIFI_SSID "SHCVN02"
#define WIFI_PASS "khongduoc"


void esp32_create_tasks(void);
void esp32_reset_module(void);
void esp32_echo_off(void);
void esp32_wifi_conn(const char* ssid, const char* pssid);
void esp32_send_cmd(const char* cmd, int wait_ms);
void esp32_recv(char* msg, int len);


#endif /* ESP32_AT_H_ */
