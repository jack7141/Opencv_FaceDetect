#include <QCoreApplication>
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/opencv.hpp"
#include "opencv2/videoio.hpp"
#include "opencv2/video.hpp"
#include "opencv2/videoio/videoio.hpp"



using namespace cv;
using namespace std;

void detectAndDraw( Mat& img, CascadeClassifier& cascade, double scale );
string cascadeName, nestedCascadeName;

int main( int argc, const char** argv )
{
    VideoCapture capture;
    Mat frame, image;


    CascadeClassifier cascade, nestedCascade;
    double scale=1;

    nestedCascade.load( "C:/Work/OpenCV/xml/opencv/data/haarcascades/haarcascade_eye_tree_eyeglasses.xml" ) ;
    cascade.load( "C:/Work/OpenCV/xml/opencv/data/haarcascades/haarcascade_frontalface_default.xml" ) ;


    capture.open(0);
    if( capture.isOpened() )
    {

        while(1)
        {
            capture >> frame;
            if( frame.empty() )
                break;
            Mat frame1 = frame.clone();
            detectAndDraw( frame1, cascade,scale );
            char c = (char)waitKey(10);


            if( c == 27 || c == 'q' || c == 'Q' )
                break;
        }
    }
    else
        cout<<"Could not Open Camera";
    return 0;
}

void detectAndDraw( Mat& img, CascadeClassifier& cascade,double scale)
{
    vector<Rect> faces, faces2;
    Mat gray, smallImg;

    cvtColor( img, gray, COLOR_BGR2GRAY );
    double fx = 1 / scale;


    resize( gray, smallImg, Size(), fx, fx, INTER_LINEAR );
    equalizeHist( smallImg, smallImg );


    cascade.detectMultiScale( smallImg, faces, 1.1, 2, 0|CASCADE_SCALE_IMAGE, Size(50, 50) );

    // Draw circles around the faces
    for ( size_t i = 0; i < faces.size(); i++ )
    {
        Rect r = faces[i];
        Mat smallImgROI;
        vector<Rect> nestedObjects;
        Point center;
        Scalar color = Scalar(255, 0, 0);
        int radius;

        double ratio = (double)r.width/r.height;
        if( 0.75 < ratio && ratio < 1.3 )
        {
            center.x = cvRound((r.x + r.width*0.5)*scale);
            center.y = cvRound((r.y + r.height*0.5)*scale);
            radius = cvRound((r.width + r.height)*0.25*scale);
            circle( img, center, radius, color, 3, 8, 0 );
        }
    }
    imshow( "Face Detection", img );
}
