#ifndef HOMOGRAPHY_H
#define HOMOGRAPHY_H

#include <opencv2/core/core.hpp>
#include <opencv2/imgproc/imgproc.hpp>
#include <opencv2/core/core.hpp>

class Homography
{
	cv::Mat homographyMatrix;
	cv::Mat inverseHomographyMatrix;
	std::vector<cv::Point2f> plane1Points;
	std::vector<cv::Point2f> plane2Points;

public:
	static Homography* load(std::string file){
		cv::FileStorage fs(file, cv::FileStorage::READ);
		if (!fs.isOpened())
		{
			return NULL;
		}
		cv::Mat homographyMat;
		fs["homography"] >> homographyMat;
		fs.release();
		return new Homography(homographyMat);
	}

	Homography(cv::Mat homographyMatrix){
		this->homographyMatrix = homographyMatrix;
		this->inverseHomographyMatrix = homographyMatrix.inv();
	}

	Homography(std::vector<cv::Point2f> pPlane1Points,
		std::vector<cv::Point2f> pPlane2Points){
		plane1Points = pPlane1Points;
		plane2Points = pPlane2Points;
		if (plane1Points.size() != plane2Points.size() || plane1Points.size() != 4)
			throw std::exception("Homography vectors must have 4 points each.");
		evaluateHomography();
	}

	Homography(Homography &anotherHomography){
		plane1Points = anotherHomography.plane1Points;
		plane2Points = anotherHomography.plane2Points;
		homographyMatrix = anotherHomography.homographyMatrix;
		inverseHomographyMatrix = anotherHomography.inverseHomographyMatrix;
	}

	cv::Point2f fromPlane1ToPlane2(cv::Point2f point){
		return fromPlaneToPlane(point, homographyMatrix);
	}

	cv::Point2f fromPlane2ToPlane1(cv::Point2f point){
		return fromPlaneToPlane(point, inverseHomographyMatrix);
	}

	std::string toString(){
		std::stringstream homographyInStringForm;
		homographyInStringForm << homographyMatrix;
		return std::string(homographyInStringForm.str());
	}

	cv::Mat getHomographyMatrix(){
		return homographyMatrix;
	}

	cv::Mat getInverseHomographyMatrix(){
		return inverseHomographyMatrix;
	}

	std::vector<cv::Point2f> getPlane1Points(){
		return plane1Points;
	}

	std::vector<cv::Point2f> getPlane2Points(){
		return plane2Points;
	}

	cv::Mat warp(cv::Mat image, cv::Size resultSize){
		cv::Mat warpedImage;
		cv::warpPerspective(image, warpedImage, homographyMatrix, resultSize);
		return warpedImage;
	}
private:
	void evaluateHomography(){
		homographyMatrix = cv::getPerspectiveTransform(plane1Points, plane2Points);
		inverseHomographyMatrix = homographyMatrix.inv();
	}

	cv::Point2f fromPlaneToPlane(cv::Point2f point, cv::Mat transformMatrix3x3){
		cv::Point3f homogenousPoint(point.x, point.y, 1);
		cv::Point3f transformedHomogenousPoint = transform(homogenousPoint, transformMatrix3x3);
		cv::Point2f transformedPoint;
		if (transformedHomogenousPoint.z != 0){
			transformedPoint = cv::Point2f(transformedHomogenousPoint.x / transformedHomogenousPoint.z,
				transformedHomogenousPoint.y / transformedHomogenousPoint.z);
		}
		else{
			transformedPoint = cv::Point2f(transformedHomogenousPoint.x,
				transformedHomogenousPoint.y);
		}
		return transformedPoint;
	}

	cv::Point3f transform(cv::Point3f point3x1, cv::Mat transformationMatrix3x3){
		cv::Mat pointInMatrixForm3x1(cv::Size(1, 3), CV_64F);
		pointInMatrixForm3x1.at<double>(0, 0) = point3x1.x;
		pointInMatrixForm3x1.at<double>(1, 0) = point3x1.y;
		pointInMatrixForm3x1.at<double>(2, 0) = point3x1.z;
		pointInMatrixForm3x1 = transformationMatrix3x3 * pointInMatrixForm3x1;
		return cv::Point3f(pointInMatrixForm3x1.at<double>(0, 0),
			pointInMatrixForm3x1.at<double>(1, 0),
			pointInMatrixForm3x1.at<double>(2, 0));
	}
};

#endif // HOMOGRAPHY_H
