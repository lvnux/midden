/*
 * main.h
 * main头文件
 */

#ifndef __MAIN_H__
#define __MAIN_H__


bool parse_conf(const char* file);  // 配置文件解析
bool init_log();  // 初始化日志记录对象
bool start_car_node_thread();
bool start_plate_recognition_thread();

#endif  // __MAIN_H__