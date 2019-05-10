#ifndef __PLATE_RECOGNITION_WORKER_H__
#define __PLATE_RECOGNITION_WORKER_H__

#include "base_thread.h"

#include <string>


class PlateRecognitionRequestMsg : public BaseMsg
{
public:
	int height;
	int width;
	unsigned char* img_data;
	std::string name;
};


class PlateRecognitionResponseMsg : public BaseMsg
{
public:
	int code;
	std::string desc;
	std::string plate_num;
};


class PlateRecognitionWorker
{
public:
	PlateRecognitionWorker();
	~PlateRecognitionWorker();
	
public:
	static PlateRecognitionWorker* get_instance();
	bool init(int gpu_index=0);
	void put_data(BaseMsg* msg);
	void run();

private:
    void dispose(BaseMsg* msg);
	void recognize_plate(BaseMsg* msg);

private:
	BaseQueue que_;

private:
	static PlateRecognitionWorker* inst_;
};

#endif  // __PLATE_RECOGNITION_WORKER_H__