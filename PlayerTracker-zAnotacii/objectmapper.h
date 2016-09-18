#ifndef OBJECTMAPPER
#define OBJECTMAPPER

#include <string>
#include <opencv2/core/core.hpp>
#include "homography.h"

using namespace std;

class ObjectMapper{
	Homography *homography;
public:
	ObjectMapper(string file){
		homography = Homography::load(file);
	}
	~ObjectMapper(){
		delete homography;
	}

	cv::Point toMap(cv::Point imagePoint){
		if (homography){
			return homography->fromPlane1ToPlane2(imagePoint);
		}
		return cv::Point();
	}

	cv::Point toImage(cv::Point mapPoint){
		if (homography){
			return homography->fromPlane2ToPlane1(mapPoint);
		}
		return cv::Point();
	}
};

#endif // OBJECTMAPPER

