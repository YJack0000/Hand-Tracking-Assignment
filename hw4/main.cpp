#include <iostream>
#include <vector>
#include <opencv2/opencv.hpp>
#include <opencv2/highgui.hpp>

#define MOVEMENT_THRESHOLD 50

using namespace std;

int x = 100, y = 100;
int width = 100, height = 100;

void addBlock(vector<cv::Rect> &blocks){
	int current_size = blocks.size();
	if(current_size < 10){
		cv::Rect rect(x + 100 + current_size * 100, y, width, height);
		blocks.push_back(rect);
	}
}

void deleteBlock(vector<cv::Rect> &blocks){
	int current_size = blocks.size();
	if(current_size > 0)
		blocks.pop_back();
}

void drawBlocks(cv::Mat &frame, vector<cv::Rect> &blocks){
	for(int i = 0; i < blocks.size(); i++){
		cv::rectangle(frame, blocks[i], cv::Scalar(255, 0, 0), -1);
	}
}

void drawAddButton(cv::Mat &frame, cv::Mat &diff, vector<cv::Rect> &blocks){
	cv::Rect rect(x + 1100, y, width, height);
	cv::rectangle(frame, rect, cv::Scalar(0, 0, 255), -1);

	cv::Mat rect_mask = cv::Mat::zeros(frame.size(), CV_8UC1);
	rect_mask(rect) = cv::Scalar(255, 255, 255);
	
	cv::Mat addBtn(frame.size().width, frame.size().height, frame.depth(), 1);
	diff.copyTo(addBtn, rect_mask);

	if(cv::countNonZero(addBtn) > 0){
		addBlock(blocks);
		cout << "Add block" << endl;
	}
	
	string text = "+";
	cv::putText(frame, text, cv::Point(x + 1120, y + 70), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 3, cv::LINE_AA);
}

void drawDeleteButton(cv::Mat &frame, cv::Mat &diff, vector<cv::Rect> &blocks){
	cv::Rect rect(x, y, width, height);
	cv::rectangle(frame, rect, cv::Scalar(0, 0, 255), -1);

	cv::Mat rect_mask = cv::Mat::zeros(frame.size(), CV_8UC1);
	rect_mask(rect) = cv::Scalar(255, 255, 255);
	
	cv::Mat deleteBtn(frame.size().width, frame.size().height, frame.depth(), 1);
	diff.copyTo(deleteBtn, rect_mask);

	if(cv::countNonZero(deleteBtn) > 0){
		deleteBlock(blocks);
		cout << "Delete block" << endl;
	}
	
	string text = "-";
	cv::putText(frame, text, cv::Point(x + 20, y + 70), cv::FONT_HERSHEY_SIMPLEX, 2, cv::Scalar(255, 255, 255), 3, cv::LINE_AA);
}

int main(int argc, char *argv[])
{
	cv::VideoCapture capture;
	bool first = true;
	cv::Mat frame, bkframe;
	cv::Mat reference_frame, diff;

	vector<cv::Rect> blocks;

	capture.open(1);
	cv::namedWindow("Webcam", cv::WindowFlags::WINDOW_AUTOSIZE);
	cv::namedWindow("FrameDiff", cv::WindowFlags::WINDOW_AUTOSIZE);

	while(true)
	{
		bool ret = capture.read(frame);
		
		if(!ret)
		{
			cout << "Error reading frame" << endl;
			break;
		}

		cv::flip(frame, frame, 1);

		if(ret && first)
		{
			first = false;
			bkframe = cv::Mat(frame.size().width, frame.size().height, frame.depth(), 1);
			cv::cvtColor(frame, bkframe, cv::COLOR_BGR2GRAY);

			reference_frame = cv::Mat(frame.size().width, frame.size().height, frame.depth(), 1);
			bkframe.copyTo(reference_frame);

			diff = cv::Mat(frame.size().width, frame.size().height, frame.depth(), 1);
		} 
		
		// else {
		// 	first = true;
		// }
		
		cv::cvtColor(frame, bkframe, cv::COLOR_BGR2GRAY);
		cv::absdiff(bkframe, reference_frame, diff);
		cv::threshold(diff, diff, MOVEMENT_THRESHOLD, 255, cv::THRESH_BINARY);

		drawDeleteButton(frame, diff, blocks);
		drawAddButton(frame, diff, blocks);

		drawBlocks(frame, blocks);


		cv::imshow("Webcam", frame);
		cv::imshow("FrameDiff", diff);

		if(cv::waitKey(1) == 'q')
			break;
	}

	capture.release();

	cv::destroyWindow("Webcam");
	cv::destroyWindow("FrameDiff");

	bkframe.release();
	reference_frame.release();
	diff.release();

	return 0;
}