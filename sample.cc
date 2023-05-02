#include <opencv2/opencv.hpp>
#include <opencv2/videoio.hpp>
// Include required header files from OpenCV directory
#include <opencv2/objdetect.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <stdexcept>
#include <vector>

using namespace std;
using namespace cv;

void detectAndDraw( Mat& img, CascadeClassifier& cascade,CascadeClassifier& nestedCascade,double scale);

int main(int argc, char* argv[])
{
	std::string data_path("/Users/graceogunbowale/Documents/Classes/opencv/data/haarcascades/");

	std::cout << __PRETTY_FUNCTION__ << std::endl;
	cv::VideoCapture cap;
	cv::Mat frame;
  

	cv::CascadeClassifier cascade, nestedCascade;
    double scale=1;
 
    // Load classifiers from "opencv/data/haarcascades" directory
    nestedCascade.load( data_path + "haarcascade_eye_tree_eyeglasses.xml" ) ;
 
    // Change path before execution
    cascade.load( data_path + "haarcascade_frontalcatface.xml" ) ;

	try{
		cap.open(0);
		if( !cap.isOpened() ) 
		{
	  		throw std::runtime_error("Failed to open the camera stream");	
		}

	}
	catch(const std::runtime_error& e)
	{
		std::cout << "Caught an exception!! : " << e.what() << std::endl;
	}

	for(;;)
	{
		cap >> frame;
		// detect faces
		if(frame.empty() )
		{
			std::cout << "No frames arrived, exiting...\n" << std::endl;
			break;
			// error
		}	
		else
		{
			Mat frame1 = frame.clone();
            detectAndDraw( frame1, cascade, nestedCascade, scale );
            char c = (char)waitKey(10);
         
            // Press q to exit from window
            if( c == 27 || c == 'q' || c == 'Q' )
                break;
		}
}
	return 0;
}

/*void Detection::start_detect(Mat &image, CasacdeClassifier& faceCascade, double scale){
    std::vector<Rect> faces;
    Mat gray;
    Mat small_image;
    //coverting to GrayScale 
    cvtColor(image, gray, COLOR_BGR2GRAY);
    double fx = 1/scale;

    resize(gray, small_image, Size(), fx, fx, INTER_LINEAR);
    equalizeHist(small_image, small_image);

    //detecting faces
    faceCascade.detectMultiScale(small_image, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30));

    //drawing rectangles 
    for(size_t i = 0; i < faces.size(); i++){
        Rect r = faces[i];
        Point center;
        Scalar color = Scalar(242, 172, 185);
        int radius;

        double aspect_ratio = (double)r.width/r.height;
        if(0.75 < aspect_ratio && aspect_ratio < 1.3){
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.width + r.height)*0.25*scale);
            rectangle(image, faces, color, 2);
        }
         else
            rectangle(image, cvPoint(cvRound(r.x*scale), cvRound(r.y*scale)),
                    cvPoint(cvRound((r.x + r.width-1)*scale),
                    cvRound((r.y + r.height-1)*scale)), color, 3, 8, 0);
    }

    imshow("Face Detection", image);
}
*/

void detectAndDraw( Mat& img, CascadeClassifier& cascade,
					CascadeClassifier& nestedCascade,
					double scale)
{
	vector<Rect> faces, faces2;
	Mat gray, smallImg;

	cvtColor( img, gray, COLOR_BGR2GRAY ); // Convert to Gray Scale
	double fx = 1 / scale;

	// Resize the Grayscale Image
	resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
	equalizeHist( smallImg, smallImg );

	// Detect faces of different sizes using cascade classifier
	cascade.detectMultiScale( smallImg, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );

	// Draw circles around the faces
	for ( size_t i = 0; i < faces.size(); i++ )
	{
		Rect r = faces[i];
		Mat smallImgROI;
		vector<Rect> nestedObjects;
		Point center;
		Scalar color = Scalar(242, 172, 185);
		int radius;

		double aspect_ratio = (double)r.width/r.height;
		if( 0.75 < aspect_ratio && aspect_ratio < 1.3 )
		{
			center.x = cvRound((r.x + r.width*0.5)*scale);
			center.y = cvRound((r.y + r.height*0.5)*scale);
			radius = cvRound((r.width + r.height)*0.25*scale);
			circle( img, center, radius, color, 3, 8, 0 );
		}
		else
			
		if( nestedCascade.empty() )
			continue;
		smallImgROI = smallImg( r );
		
		// Detection of eyes in the input image
		nestedCascade.detectMultiScale( smallImgROI, nestedObjects, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(30, 30) );
		
		// Draw circles around eyes
		for ( size_t j = 0; j < nestedObjects.size(); j++ )
		{
			Rect nr = nestedObjects[j];
			center.x = cvRound((r.x + nr.x + nr.width*0.5)*scale);
			center.y = cvRound((r.y + nr.y + nr.height*0.5)*scale);
			radius = cvRound((nr.width + nr.height)*0.25*scale);
			circle( img, center, radius, color, 3, 8, 0 );
		}
	}

	// Show Processed Image with detected faces
	imshow( "Face Detection", img );
}



